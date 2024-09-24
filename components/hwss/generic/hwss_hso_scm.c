#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "hwss_hso_scm.h"
#include "hwss_event.h"

static const char *TAG = "hwss_hso_scm";

static esp_err_t hwss_hso_scm_sock_event_post(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t sintr){
    esp_err_t ret=ESP_OK;
    hwss_hso_event_arg_t hso_event_arg={
        .hso=hso_scm->hso,
        .id=id
    };

    if(sintr&HWSS_HSO_INTR_SEND_OK)
        ESP_GOTO_ON_ERROR(esp_event_post_to(hso_scm->elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSO_SEND_OK,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_TIMEOUT)
        ESP_GOTO_ON_ERROR(esp_event_post_to(hso_scm->elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSO_TIMEOUT,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_RECV)
        ESP_GOTO_ON_ERROR(esp_event_post_to(hso_scm->elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSO_RECV,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_DISCONN)
        ESP_GOTO_ON_ERROR(esp_event_post_to(hso_scm->elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSO_DISCONN,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_CONNECT)
        ESP_GOTO_ON_ERROR(esp_event_post_to(hso_scm->elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSO_CONNECT,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

err:
    return ret;
}

static void hwss_hso_scm_ir_handler(void* event_handler_arg,
                                    esp_event_base_t event_base,
                                    int32_t event_id,
                                    void* event_data){
    hwss_hso_scm_t *hso_scm=(hwss_hso_scm_t *)event_handler_arg;
    uint8_t gintr=0,sintr=0;

    if(hso_scm->drv.get_sock_global_intr(hso_scm,&gintr)!=ESP_OK){
        ESP_LOGE(TAG,"cannot get socket global interrupt");
        return;
    }

    for(hwss_sockid_t id=0;id<hso_scm->en_socknum;id++){
        if(gintr&0x01 && hso_scm->sockact_sta_list[id]==HWSS_HSO_SOCKACT_IDLE){
            if(hso_scm->drv.get_sock_intr(hso_scm,id,&sintr)!=ESP_OK){
                ESP_LOGE(TAG,"cannot read sock%u status",id);
                continue;
            }

            if(hwss_hso_scm_sock_event_post(hso_scm,id,sintr)!=ESP_OK)
                continue;

            if(hso_scm->drv.clear_sock_intr(hso_scm,id)!=ESP_OK)
                continue;

            if(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,false)!=ESP_OK){
                ESP_LOGE(TAG,"fail to mask sock%u global interrupt",id);
                continue;
            }

            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_ACTIVE;
            if(hso_scm->active_sock_num==0)
                if(esp_timer_start_periodic(hso_scm->sock_polling_timer,hso_scm->sock_polling_period_ms*1000)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot start sock polling timer");
                    continue;
                }
            hso_scm->active_sock_num++;
            ESP_LOGD(TAG,"SOCK%u Active",id);

            if(esp_timer_start_once(hso_scm->socktimer_list[id],hso_scm->sock_active_threshold_ms*1000)!=ESP_OK){
                ESP_LOGE(TAG,"fail to start sock%u timer",id);
                continue;
            }
        }
        gintr>>=1;
    }
}

static void hwss_hso_scm_sock_polling_timer_cb(void *args){
    hwss_hso_scm_t *hso_scm=(hwss_hso_scm_t *)args;
    uint8_t gintr=0,sintr=0;

    if(hso_scm->drv.get_sock_global_intr(hso_scm,&gintr)!=ESP_OK){
        ESP_LOGE(TAG,"cannot get socket global interrupt");
        return;
    }

    for(hwss_sockid_t id=0;id<hso_scm->en_socknum;id++){
        if(gintr&0x01&&hso_scm->sockact_sta_list[id]==HWSS_HSO_SOCKACT_ACTIVE){
            if(hso_scm->drv.get_sock_intr(hso_scm,id,&sintr)!=ESP_OK){
                ESP_LOGE(TAG,"cannot get socket%u interrupt",id);
                return;
            }

            if(hwss_hso_scm_sock_event_post(hso_scm,id,sintr)!=ESP_OK)
                return;

            if(hso_scm->drv.clear_sock_intr(hso_scm,id)!=ESP_OK){
                ESP_LOGE(TAG,"fail to clear sock%u interrupt",id);
                return;
            }

            if(esp_timer_restart(hso_scm->socktimer_list[id],hso_scm->sock_active_threshold_ms*1000)!=ESP_OK){
                ESP_LOGE(TAG,"fail to restart sock%u timer",id);
                return;
            }
        }
        gintr>>=1;
    }
}

static void hwss_hso_scm_sockact_timer_cb(void *args){
    hwss_hso_scm_timer_arg_t *hso_scm_arg=(hwss_hso_scm_timer_arg_t *)args;
    hwss_hso_scm_t *hso_scm=hso_scm_arg->hso_scm;
    hwss_sockid_t id=hso_scm_arg->id;

    hso_scm->active_sock_num--;
    if(hso_scm->active_sock_num==0)
        if(esp_timer_stop(hso_scm->sock_polling_timer)!=ESP_OK){
            ESP_LOGE(TAG,"failt to stop sock polling timer");
            return;
        }
    hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_IDLE;
    ESP_LOGD(TAG,"SOCK%u Idle",id);

    if(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,true)!=ESP_OK){
        ESP_LOGE(TAG,"fail to enable sock%u global interrupt",id);
        return;
    }
}

static esp_err_t hwss_hso_scm_init(hwss_hso_scm_t *hso_scm){
    esp_err_t ret=ESP_OK;
    return ret;
}

static esp_err_t hwss_hso_scm_deinit(hwss_hso_scm_t *hso_scm){
    esp_err_t ret=ESP_OK;
    if(hso_scm->is_started){
        ESP_GOTO_ON_ERROR(hso_scm->stop(hso_scm),err,TAG,"cannot stop hso_scm");
        hso_scm->is_started=false;
    }

err:
    return ret;    
}

static esp_err_t hwss_hso_scm_start(hwss_hso_scm_t *hso_scm){
    esp_err_t ret=ESP_OK;

    if(hso_scm->is_started)
        return ret;
    
    if(hso_scm->hir){
        ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_global_intr_enable_all(hso_scm,false),err,TAG,"fail to disable global socket intr");
        for(hwss_sockid_t id=0;id<hso_scm->en_socknum;id++){
            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_IDLE;
            ESP_GOTO_ON_ERROR(hso_scm->drv.clear_sock_intr(hso_scm,id),err,TAG,"cannot clear sock%u intr",id);
            ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_intr_enable(hso_scm,id,true),err,TAG,"cannot enable sock%u intr",id);
        };
        hso_scm->active_sock_num=0;
        ESP_GOTO_ON_ERROR(hso_scm->hir->register_handler(hso_scm->hir,hwss_hso_scm_ir_handler,hso_scm),err,TAG,"fail to register handler");
        ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_global_intr_enable_all(hso_scm,true),err,TAG,"fail to enable global socket intr");
    }
    else{
        for(hwss_sockid_t id=0;id<hso_scm->en_socknum;id++){
            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_GENERIC;
            ESP_GOTO_ON_ERROR(hso_scm->drv.clear_sock_intr(hso_scm,id),err,TAG,"cannot clear sock%u intr",id);
            ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_intr_enable(hso_scm,id,true),err,TAG,"cannot enable sock%u intr",id);
        };
        hso_scm->active_sock_num=0;

        ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_scm->sock_polling_timer,hso_scm->sock_polling_period_ms*1000),err,TAG,
                            "cannot start sock polling timer");
    }
    hso_scm->is_started=true;
err:
    return ret;
}

static esp_err_t hwss_hso_scm_stop(hwss_hso_scm_t *hso_scm){
    esp_err_t ret=ESP_OK;
    if(!hso_scm->is_started)
        return ret;

    if(hso_scm->hir){
        ESP_GOTO_ON_ERROR(hso_scm->hir->unregister_handler(hso_scm->hir,hwss_hso_scm_ir_handler),err,TAG,"fail to unregister handler");
        if(esp_timer_is_active(hso_scm->sock_polling_timer))
            ESP_GOTO_ON_ERROR(esp_timer_stop(hso_scm->sock_polling_timer),err,TAG,"fail to stop sock polling timer");
    }
    else
        ESP_GOTO_ON_ERROR(esp_timer_stop(hso_scm->sock_polling_timer),err,TAG,"fail to stop sock polling timer");
    hso_scm->is_started=false;
err:
    return ret;
}

static esp_err_t hwss_hso_scm_set_sock_state(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, const hwss_hso_sockact_sta_t *state){
    esp_err_t ret=ESP_OK;
    
    if(hso_scm->hir==NULL || *state==HWSS_HSO_SOCKACT_GENERIC)
        return ESP_ERR_NOT_SUPPORTED;

    if(*state==hso_scm->sockact_sta_list[id])
        return ESP_OK;
    
    if(*state == HWSS_HSO_SOCKACT_ACTIVE){
        uint8_t sintr=0;
        ESP_GOTO_ON_ERROR(hso_scm->drv.get_sock_intr(hso_scm,id,&sintr),err,TAG,"cannot get socket%u interrupt",id);
        ESP_GOTO_ON_ERROR(hwss_hso_scm_sock_event_post(hso_scm,id,sintr),err,TAG,"cannot handler sock%u interrupt",id);
        if(hso_scm->active_sock_num==0)
            ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_scm->sock_polling_timer,hso_scm->sock_polling_period_ms*1000),
                            err,TAG,"cannot start sock polling timer");
        hso_scm->active_sock_num++;

        ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,false),err,TAG,"fail to mask sock%u global interrupt",id);
        ESP_GOTO_ON_ERROR(esp_timer_start_once(hso_scm->socktimer_list[id],hso_scm->sock_active_threshold_ms*1000),
                            err,TAG,"fail to start sock%u timer",id);
    }
    else{
        hso_scm->active_sock_num--;
        if(hso_scm->active_sock_num==0)
            ESP_GOTO_ON_ERROR(esp_timer_stop(hso_scm->sock_polling_timer),err,TAG,"failt to stop sock polling timer");

        ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,true),err,TAG,"fail to enable sock%u global interrupt",id);
    }

    hso_scm->sockact_sta_list[id]=*state;

err:
    return ret;    
}

static esp_err_t hwss_hso_scm_get_sock_state(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, hwss_hso_sockact_sta_t *state){
    *state=hso_scm->sockact_sta_list[id];
    return ESP_OK;
}

hwss_hso_scm_t *hwss_hso_scm_new(esp_event_loop_handle_t elp_hdl, hwss_hso_t *hso, hwss_hir_t  *hir, const hwss_hso_scm_config_t *config){
    hwss_hso_scm_t *ret=NULL;

    ret=calloc(1,sizeof(hwss_hso_scm_t));
    ESP_GOTO_ON_FALSE(ret,NULL,err,TAG,"fail to calloc hso_scm");

    ret->hso=hso;
    ret->hir=hir;

    ret->sockact_sta_list=calloc(config->sock_total_num,sizeof(hwss_hso_sockact_sta_t));
    ESP_GOTO_ON_FALSE(ret->sockact_sta_list,NULL,err,TAG,"fail to calloc sockact_sta_list");

    ret->en_socknum=config->en_socknum;
    ret->elp_hdl=elp_hdl;

    ret->sock_polling_period_ms=config->sock_polling_period_ms;
    ret->sock_active_threshold_ms=config->sock_active_threshold_ms;

    ret->is_started=false;

    ret->init=hwss_hso_scm_init;
    ret->deinit=hwss_hso_scm_deinit;
    ret->start=hwss_hso_scm_start;
    ret->stop=hwss_hso_scm_stop;
    ret->set_sock_state=hwss_hso_scm_set_sock_state;
    ret->get_sock_state=hwss_hso_scm_get_sock_state;

    if(ret->hir){
        ret->socktimer_list=calloc(config->sock_total_num,sizeof(esp_timer_handle_t));
        ESP_GOTO_ON_FALSE(ret->socktimer_list,NULL,err,TAG,"fail to calloc socktimer_list");

        ret->socktimer_args=calloc(config->sock_total_num,sizeof(hwss_hso_scm_timer_arg_t));
        ESP_GOTO_ON_FALSE(ret->socktimer_args,NULL,err,TAG,"fail to calloc socktimer_args");

        esp_timer_create_args_t timer_arg={
            .name="hwss_hso_scm_sockact_timer",
            .callback=hwss_hso_scm_sockact_timer_cb,
            .skip_unhandled_events=true
        };

        for(hwss_devid_t id=0;id<config->sock_total_num;id++){
            ret->socktimer_args[id].hso_scm=ret;
            ret->socktimer_args[id].id=id;
            timer_arg.arg=ret->socktimer_args+id;
            ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,ret->socktimer_list+id)==ESP_OK,NULL,err,TAG,"cannot create sock%u act timer",id);
        }
    }

    esp_timer_create_args_t timer_arg={
        .arg=ret,
        .name="hwss_hso_scm_polling_timer",
        .callback=hwss_hso_scm_sock_polling_timer_cb,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&ret->sock_polling_timer)==ESP_OK,NULL,err,TAG,"cannot create polling timer");

err:
    return ret;
}
