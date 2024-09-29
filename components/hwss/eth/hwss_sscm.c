#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "hwss_eth_type.h"
#include "hwss_eth_event.h"
#include "hwss_sscm.h"

static const char *TAG = "hwss_sscm";

typedef struct hwss_sscm_pro_s hwss_sscm_pro_t;

typedef struct{
    hwss_sscm_pro_t *sscm_pro;
    hwss_eth_sockid_t   id;
}hwss_sscm_timer_arg_t;

struct hwss_sscm_pro_s{
    hwss_sscm_t         super;

    hwss_sscm_policy_t  policy;
    hwss_sscm_drv_t     *drv;

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;

    uint8_t     en_socknum;
    uint8_t     active_sock_num;

    esp_event_loop_handle_t elp_hdl;

    hwss_sockact_sta_t      *sockact_sta_list;
    
    esp_timer_handle_t      *socktimer_list;
    esp_timer_handle_t      sock_polling_timer;

    hwss_sscm_timer_arg_t   *socktimer_args;
};

static inline esp_err_t hwss_sscm_sock_event_post(hwss_sscm_pro_t *sscm_pro, hwss_eth_sockid_t id, uint8_t sintr){
    esp_err_t ret=ESP_OK;

    if(sintr&HWSS_SOCK_INTR_SEND_OK)
        ESP_GOTO_ON_ERROR(esp_event_post_to(sscm_pro->elp_hdl,HWSS_SSCM_EVENT,HWSS_SSCM_EVENT_SEND_OK,
                    (void *)&id,sizeof(hwss_eth_sockid_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_SOCK_INTR_TIMEOUT)
        ESP_GOTO_ON_ERROR(esp_event_post_to(sscm_pro->elp_hdl,HWSS_SSCM_EVENT,HWSS_SSCM_EVENT_TIMEOUT,
                    (void *)&id,sizeof(hwss_eth_sockid_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_SOCK_INTR_RECV)
        ESP_GOTO_ON_ERROR(esp_event_post_to(sscm_pro->elp_hdl,HWSS_SSCM_EVENT,HWSS_SSCM_EVENT_RECV,
                    (void *)&id,sizeof(hwss_eth_sockid_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_SOCK_INTR_DISCONN)
        ESP_GOTO_ON_ERROR(esp_event_post_to(sscm_pro->elp_hdl,HWSS_SSCM_EVENT,HWSS_SSCM_EVENT_DISCONN,
                    (void *)&id,sizeof(hwss_eth_sockid_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_SOCK_INTR_CONNECT)
        ESP_GOTO_ON_ERROR(esp_event_post_to(sscm_pro->elp_hdl,HWSS_SSCM_EVENT,HWSS_SSCM_EVENT_CONNECT,
                    (void *)&id,sizeof(hwss_eth_sockid_t),0),err,TAG,"fail to post event");

err:
    return ret;
}

static void IRAM_ATTR hwss_sscm_sock_polling_timer_cb(void *args){
    hwss_sscm_pro_t *sscm_pro=(hwss_sscm_pro_t *)args;
    hwss_sscm_drv_t *sscm_drv=sscm_pro->drv;
    uint16_t gintr=0;
    uint8_t sintr=0;

    if(sscm_drv->get_sock_global_intr_bits(sscm_drv,&gintr)!=ESP_OK){
        ESP_LOGE(TAG,"cannot get socket global interrupt");
        return;
    }

    for(hwss_eth_sockid_t id=0;id<sscm_pro->en_socknum;id++){
        if(!(gintr&0x01))
            continue;
        if(sscm_pro->sockact_sta_list[id]==HWSS_SOCKACT_IDLE)
            continue;

        if(sscm_drv->get_sock_intr(sscm_drv,id,&sintr)!=ESP_OK){
            ESP_LOGE(TAG,"cannot get socket%u interrupt",id);
            return;
        }

        if(hwss_sscm_sock_event_post(sscm_pro,id,sintr)!=ESP_OK)
            return;

        if(sscm_drv->clear_sock_intr(sscm_drv,id)!=ESP_OK){
            ESP_LOGE(TAG,"fail to clear sock%u interrupt",id);
            return;
        }

        if(sscm_pro->sockact_sta_list[id]==HWSS_SOCKACT_ACTIVE){
            if(esp_timer_restart(sscm_pro->socktimer_list[id],sscm_pro->sock_active_threshold_ms*1000)!=ESP_OK){
                ESP_LOGE(TAG,"fail to restart sock%u timer",id);
                return;
            }
        }
        gintr>>=1;
    }
}

static void hwss_sscm_sockact_timer_cb(void *args){
    hwss_sscm_timer_arg_t *timer_arg=(hwss_sscm_timer_arg_t *)args;
    hwss_sscm_pro_t *sscm_pro=timer_arg->sscm_pro;
    hwss_sscm_drv_t *sscm_drv=sscm_pro->drv;
    hwss_eth_sockid_t id=timer_arg->id;

    sscm_pro->active_sock_num--;
    if(sscm_pro->active_sock_num==0)
        if(esp_timer_stop(sscm_pro->sock_polling_timer)!=ESP_OK){
            ESP_LOGE(TAG,"failt to stop sock polling timer");
            return;
        }
    sscm_pro->sockact_sta_list[id]=HWSS_SOCKACT_IDLE;
    ESP_LOGD(TAG,"SOCK%u Idle",id);

    if(sscm_drv->set_sock_global_intr_enable(sscm_drv,id,true)!=ESP_OK){
        ESP_LOGE(TAG,"fail to enable sock%u global interrupt",id);
        return;
    }
}

esp_err_t hwss_sscm_init(hwss_sscm_t *sscm){
    esp_err_t ret=ESP_OK;
    return ret;
}

esp_err_t hwss_sscm_deinit(hwss_sscm_t *sscm){
    esp_err_t ret=ESP_OK;
    if(sscm->is_started){
        ESP_GOTO_ON_ERROR(hwss_sscm_stop(sscm),err,TAG,"cannot stop sscm");
        sscm->is_started=false;
    }

err:
    return ret;    
}

esp_err_t hwss_sscm_start(hwss_sscm_t *sscm){
    esp_err_t ret=ESP_OK;
    hwss_sscm_pro_t *sscm_pro=__containerof(sscm,hwss_sscm_pro_t,super);
    hwss_sscm_drv_t *sscm_drv=sscm_pro->drv;
    if(sscm->is_started)
        return ret;
    
    switch(sscm_pro->policy){
        case HWSS_SSCM_POLICY_NO_INTR_POLLING:
            for(hwss_eth_sockid_t id=0;id<sscm_pro->en_socknum;id++){
                sscm_pro->sockact_sta_list[id]=HWSS_SOCKACT_GENERIC;
                ESP_GOTO_ON_ERROR(sscm_drv->clear_sock_intr(sscm_drv,id),err,TAG,"cannot clear sock%u intr",id);
                ESP_GOTO_ON_ERROR(sscm_drv->set_sock_intr_enable(sscm_drv,id,true),err,TAG,"cannot enable sock%u intr",id);
            };
            sscm_pro->active_sock_num=0;

            ESP_GOTO_ON_ERROR(esp_timer_start_periodic(sscm_pro->sock_polling_timer,sscm_pro->sock_polling_period_ms*1000),err,TAG,
                                "cannot start sock polling timer");
        break;

        case HWSS_SSCM_POLICY_INTR_WAKEUP_POLLING:
            ESP_GOTO_ON_ERROR(sscm_drv->set_sock_global_intr_enable_all(sscm_drv,false),err,TAG,"fail to disable global socket intr");
            for(hwss_eth_sockid_t id=0;id<sscm_pro->en_socknum;id++){
                sscm_pro->sockact_sta_list[id]=HWSS_SOCKACT_IDLE;
                ESP_GOTO_ON_ERROR(sscm_drv->clear_sock_intr(sscm_drv,id),err,TAG,"cannot clear sock%u intr",id);
                ESP_GOTO_ON_ERROR(sscm_drv->set_sock_intr_enable(sscm_drv,id,true),err,TAG,"cannot enable sock%u intr",id);
            };
            sscm_pro->active_sock_num=0;
            ESP_GOTO_ON_ERROR(sscm_drv->set_sock_global_intr_enable_all(sscm_drv,true),err,TAG,"fail to enable global socket intr");
        break;

        default: return ESP_ERR_NOT_SUPPORTED;
    }

    sscm->is_started=true;
err:
    return ret;
}

esp_err_t hwss_sscm_stop(hwss_sscm_t *sscm){
    esp_err_t ret=ESP_OK;
    hwss_sscm_pro_t *sscm_pro=__containerof(sscm,hwss_sscm_pro_t,super);
    if(!sscm->is_started)
        return ret;

    switch(sscm_pro->policy){
        case HWSS_SSCM_POLICY_NO_INTR_POLLING:
            ESP_GOTO_ON_ERROR(esp_timer_stop(sscm_pro->sock_polling_timer),err,TAG,"fail to stop sock polling timer");
        break;

        case HWSS_SSCM_POLICY_INTR_WAKEUP_POLLING:
            if(esp_timer_is_active(sscm_pro->sock_polling_timer))
                ESP_GOTO_ON_ERROR(esp_timer_stop(sscm_pro->sock_polling_timer),err,TAG,"fail to stop sock polling timer");
            
            if(sscm_pro->active_sock_num!=0){
                for(hwss_eth_sockid_t id=0;id<sscm_pro->en_socknum;id++){
                    if(sscm_pro->sockact_sta_list[id]==HWSS_SOCKACT_ACTIVE){
                        if(esp_timer_is_active(sscm_pro->socktimer_list[id]))
                            ESP_GOTO_ON_ERROR(esp_timer_stop(sscm_pro->socktimer_list[id]),
                                                err,TAG,"fail to stop sock active timer");
                    }
                }
            }
        break;

        default: return ESP_ERR_NOT_SUPPORTED;
    }
        
    sscm->is_started=false;
err:
    return ret;
}

esp_err_t hwss_sscm_set_sock_state(hwss_sscm_t *sscm, hwss_eth_sockid_t id, const hwss_sockact_sta_t *state){
    esp_err_t ret=ESP_OK;
    hwss_sscm_pro_t *sscm_pro=__containerof(sscm,hwss_sscm_pro_t,super);
    hwss_sscm_drv_t *sscm_drv=sscm_pro->drv;

    if(sscm_pro->policy==HWSS_SSCM_POLICY_NO_INTR_POLLING || *state==HWSS_SOCKACT_GENERIC)
        return ESP_ERR_NOT_SUPPORTED;

    if(*state==sscm_pro->sockact_sta_list[id])
        return ESP_OK;
    
    if(*state == HWSS_SOCKACT_ACTIVE){
        uint8_t sintr=0;
        ESP_GOTO_ON_ERROR(sscm_drv->get_sock_intr(sscm_drv,id,&sintr),err,TAG,"cannot get socket%u interrupt",id);
        ESP_GOTO_ON_ERROR(hwss_sscm_sock_event_post(sscm_pro,id,sintr),err,TAG,"cannot handler sock%u interrupt",id);
        if(sscm_pro->active_sock_num==0)
            ESP_GOTO_ON_ERROR(esp_timer_start_periodic(sscm_pro->sock_polling_timer,sscm_pro->sock_polling_period_ms*1000),
                            err,TAG,"cannot start sock polling timer");
        sscm_pro->active_sock_num++;

        ESP_GOTO_ON_ERROR(sscm_drv->set_sock_global_intr_enable(sscm_drv,id,false),err,TAG,"fail to mask sock%u global interrupt",id);
        ESP_GOTO_ON_ERROR(esp_timer_start_once(sscm_pro->socktimer_list[id],sscm_pro->sock_active_threshold_ms*1000),
                            err,TAG,"fail to start sock%u timer",id);
    }
    else{
        sscm_pro->active_sock_num--;
        if(sscm_pro->active_sock_num==0)
            ESP_GOTO_ON_ERROR(esp_timer_stop(sscm_pro->sock_polling_timer),err,TAG,"failt to stop sock polling timer");

        ESP_GOTO_ON_ERROR(sscm_drv->set_sock_global_intr_enable(sscm_drv,id,true),err,TAG,"fail to enable sock%u global interrupt",id);
    }

    sscm_pro->sockact_sta_list[id]=*state;

err:
    return ret;    
}

esp_err_t hwss_sscm_get_sock_state(hwss_sscm_t *sscm, hwss_eth_sockid_t id, hwss_sockact_sta_t *state){
    hwss_sscm_pro_t *sscm_pro=__containerof(sscm,hwss_sscm_pro_t,super);
    *state=sscm_pro->sockact_sta_list[id];
    return ESP_OK;
}

esp_err_t hwss_sscm_intr_process(hwss_sscm_t *sscm){
    esp_err_t ret=ESP_OK;
    hwss_sscm_pro_t *sscm_pro=__containerof(sscm,hwss_sscm_pro_t,super);
    hwss_sscm_drv_t *sscm_drv=sscm_pro->drv;
    uint16_t gintr=0;
    uint8_t sintr=0;

    ESP_GOTO_ON_ERROR(sscm_drv->get_sock_global_intr_bits(sscm_drv,&gintr),err,TAG,"cannot get socket global interrupt");

    for(hwss_eth_sockid_t id=0;id<sscm_pro->en_socknum;id++){
        if(gintr&0x01 && sscm_pro->sockact_sta_list[id]==HWSS_SOCKACT_IDLE){
            ESP_GOTO_ON_ERROR(sscm_drv->get_sock_intr(sscm_drv,id,&sintr),err,TAG,"cannot read sock%u status",id);

            ESP_GOTO_ON_ERROR(hwss_sscm_sock_event_post(sscm_pro,id,sintr),err,TAG,"fail to post event");

            ESP_GOTO_ON_ERROR(sscm_drv->clear_sock_intr(sscm_drv,id),err,TAG,"fail to clear intr");

            ESP_GOTO_ON_ERROR(sscm_drv->set_sock_global_intr_enable(sscm_drv,id,false),err,TAG,"fail to disable sock global intr");
        
            sscm_pro->sockact_sta_list[id]=HWSS_SOCKACT_ACTIVE;
            if(sscm_pro->active_sock_num==0)
                ESP_GOTO_ON_ERROR(esp_timer_start_periodic(sscm_pro->sock_polling_timer,sscm_pro->sock_polling_period_ms*1000),
                                err,TAG,"cannot start sock polling timer");
            
            sscm_pro->active_sock_num++;
            ESP_LOGD(TAG,"SOCK%u Active",id);

            ESP_GOTO_ON_ERROR(esp_timer_start_once(sscm_pro->socktimer_list[id],sscm_pro->sock_active_threshold_ms*1000),err,TAG,
                                "fail to start sock%u timer",id);
        }
        gintr>>=1;
    }

err:
    return ret;
}

hwss_sscm_t *hwss_sscm_new(esp_event_loop_handle_t elp_hdl,hwss_sscm_drv_t *drv, const hwss_sscm_config_t *config){
    hwss_sscm_t *ret=NULL;
    hwss_sscm_pro_t *sscm=NULL;
    sscm=calloc(1,sizeof(hwss_sscm_pro_t));
    ESP_GOTO_ON_FALSE(sscm,NULL,err,TAG,"fail to calloc sscm_pro");

    sscm->policy=config->policy;
    sscm->drv=drv;
    sscm->sockact_sta_list=calloc(config->en_socknum,sizeof(hwss_sockact_sta_t));
    ESP_GOTO_ON_FALSE(sscm->sockact_sta_list,NULL,err,TAG,"fail to calloc sockact_sta_list");

    sscm->en_socknum=config->en_socknum;
    sscm->elp_hdl=elp_hdl;

    sscm->sock_polling_period_ms=config->sock_polling_period_ms;
    sscm->sock_active_threshold_ms=config->sock_active_threshold_ms;

    sscm->super.is_started=false;

    if(sscm->policy==HWSS_SSCM_POLICY_INTR_WAKEUP_POLLING){
        sscm->socktimer_list=calloc(config->en_socknum,sizeof(esp_timer_handle_t));
        ESP_GOTO_ON_FALSE(sscm->socktimer_list,NULL,err,TAG,"fail to calloc socktimer_list");

        sscm->socktimer_args=calloc(config->en_socknum,sizeof(hwss_sscm_timer_arg_t));
        ESP_GOTO_ON_FALSE(sscm->socktimer_args,NULL,err,TAG,"fail to calloc socktimer_args");

        esp_timer_create_args_t timer_arg={
            .name="hwss_sscm_sockact_timer",
            .callback=hwss_sscm_sockact_timer_cb,
            .skip_unhandled_events=true
        };

        for(hwss_eth_sockid_t id=0;id<config->en_socknum;id++){
            sscm->socktimer_args[id].sscm_pro=sscm;
            sscm->socktimer_args[id].id=id;
            timer_arg.arg=sscm->socktimer_args+id;
            ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,sscm->socktimer_list+id)==ESP_OK,NULL,err,TAG,"cannot create sock%u act timer",id);
        }
    }

    esp_timer_create_args_t timer_arg={
        .arg=sscm,
        .name="hwss_sscm_polling_timer",
        .callback=hwss_sscm_sock_polling_timer_cb,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&(sscm->sock_polling_timer))==ESP_OK,NULL,err,TAG,"cannot create polling timer");

    return &sscm->super;
err:
    return ret;
}
