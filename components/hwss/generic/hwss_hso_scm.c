#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "hwss_hso_scm.h"
#include "hwss_event.h"

static const char *TAG = "hwss_hso_scm";

static esp_err_t hwss_hso_scm_sock_intr_handler(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t sintr){
    esp_err_t ret=ESP_OK;
    hwss_hso_event_arg_t hso_event_arg={
        .hso=hso_scm->hso,
        .id=id
    };

    if(sintr&HWSS_HSO_INTR_SEND_OK)
        ESP_GOTO_ON_ERROR(hwss_event_post(HWSS_INTER_SOCK_EVENT,HWSS_INTER_SOCK_EVENT_SEND_OK,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_TIMEOUT)
        ESP_GOTO_ON_ERROR(hwss_event_post(HWSS_INTER_SOCK_EVENT,HWSS_INTER_SOCK_EVENT_TIMEOUT,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_RECV)
        ESP_GOTO_ON_ERROR(hwss_event_post(HWSS_INTER_SOCK_EVENT,HWSS_INTER_SOCK_EVENT_RECV,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_DISCONN)
        ESP_GOTO_ON_ERROR(hwss_event_post(HWSS_INTER_SOCK_EVENT,HWSS_INTER_SOCK_EVENT_DISCONN,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

    if(sintr&HWSS_HSO_INTR_CONNECT)
        ESP_GOTO_ON_ERROR(hwss_event_post(HWSS_INTER_SOCK_EVENT,HWSS_INTER_SOCK_EVENT_CONNECT,
                    (void *)&hso_event_arg,sizeof(hwss_hso_event_arg_t),0),err,TAG,"fail to post event");

err:
    return ret;
}

IRAM_ATTR static void hwss_hso_scm_isr_handler(void *args){
    hwss_hso_scm_t *hso_scm=(hwss_hso_scm_t *)args;
    BaseType_t high_task_wakeup=pdTRUE;

    vTaskNotifyGiveFromISR(hso_scm->irq_handler,&high_task_wakeup);
    if(high_task_wakeup != pdTRUE)
        portYIELD_FROM_ISR();
}

static void hwss_hso_scm_irq_handler_task(void *args){
    hwss_hso_scm_t *hso_scm=(hwss_hso_scm_t *)args;
    
    uint8_t gintr=0;
    uint8_t sintr=0;
    while(1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        if(hso_scm->drv.get_sock_global_intr(hso_scm,&gintr)!=ESP_OK){
            ESP_LOGE(TAG,"cannot get socket global interrupt");
            continue;
        }

        for(hwss_sockid_t id=0;id<hso_scm->en_sock_num;id++){
            if(gintr&0x01 && hso_scm->sockact_sta_list[id]==HWSS_HSO_SOCKACT_IDLE){
                if(hso_scm->drv.get_sock_intr(hso_scm,id,&sintr)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot read sock%u status",id);
                    continue;
                }

                if(hwss_hso_scm_sock_intr_handler(hso_scm,id,sintr)!=ESP_OK)
                    continue;

                hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_ACTIVE;
                if(hso_scm->active_sock_num==0)
                    if(esp_timer_start_periodic(hso_scm->sock_polling_timer,hso_scm->sock_polling_period_ms*1000)!=ESP_OK){
                        ESP_LOGE(TAG,"cannot start sock polling timer");
                        continue;
                    }
                hso_scm->active_sock_num++;

                if(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,false)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to mask sock%u global interrupt",id);
                    continue;
                }

                if(esp_timer_start_once(hso_scm->socktimer_list[id],hso_scm->sock_active_threshold_ms*1000)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to start sock%u timer",id);
                    continue;
                }
            }
            gintr>>=1;
        }
    }
}

static void hwss_hso_scm_sock_polling_timer_cb(void *args){
    hwss_hso_scm_t *hso_scm=(hwss_hso_scm_t *)args;
    uint8_t gintr;
    uint8_t sintr;

    if(hso_scm->drv.get_sock_global_intr(hso_scm,&gintr)!=ESP_OK){
        ESP_LOGE(TAG,"cannot get socket global interrupt");
        return;
    }

    for(hwss_sockid_t id=0;id<hso_scm->en_sock_num;id++){
        if(gintr&0x01&&hso_scm->sockact_sta_list[id]==HWSS_HSO_SOCKACT_ACTIVE){
            if(hso_scm->drv.get_sock_intr(hso_scm,id,&sintr)!=ESP_OK){
                ESP_LOGE(TAG,"cannot get socket%u interrupt",id);
                return;
            }

            if(hwss_hso_scm_sock_intr_handler(hso_scm,id,sintr)!=ESP_OK)
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

    if(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,true)!=ESP_OK){
        ESP_LOGE(TAG,"fail to enable sock%u global interrupt",id);
        return;
    }
}

esp_err_t hwss_hso_scm_init(hwss_hso_scm_t *hso_scm){
    esp_err_t ret=ESP_OK;
    if(hso_scm->irq_gpio_num>=0){
        ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_global_intr_enable_all(hso_scm,false),err,TAG,"fail to disable global socket intr");
        for(hwss_sockid_t id=0;id<hso_scm->en_sock_num;id++){
            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_IDLE;
            ESP_GOTO_ON_ERROR(hso_scm->drv.clear_sock_intr(hso_scm,id),err,TAG,"cannot clear sock%u intr",id);
            ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_intr_enable(hso_scm,id,true),err,TAG,"cannot enable sock%u intr",id);
        };
        hso_scm->active_sock_num=0;

        BaseType_t xReturned = xTaskCreatePinnedToCore(hwss_hso_scm_irq_handler_task, "hso_scm_irq_handle_tsk", 
                                hso_scm->irq_task_stack_size, hso_scm, hso_scm->irq_task_prio, &hso_scm->irq_handler, tskNO_AFFINITY);
        ESP_GOTO_ON_FALSE(xReturned == pdPASS, ESP_FAIL, err, TAG,"cannot create hso_w5500 recv task");

        gpio_config_t io_cfg={
            .pin_bit_mask=1ull<<hso_scm->irq_gpio_num,
            .mode=GPIO_MODE_INPUT,
            .pull_up_en=1,
            .intr_type=GPIO_INTR_NEGEDGE,
        };

        ESP_GOTO_ON_ERROR(gpio_config(&io_cfg),err,TAG,"cannot setup irq pin");
        ESP_GOTO_ON_ERROR(gpio_intr_enable(hso_scm->irq_gpio_num),err,TAG,"cannot enable irq");
        ESP_GOTO_ON_ERROR(gpio_isr_handler_add(hso_scm->irq_gpio_num,hwss_hso_scm_isr_handler,(void *)hso_scm),
                        err,TAG,"cannot enable irq");
        ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_global_intr_enable_all(hso_scm,true),err,TAG,"fail to enable global socket intr");
    }
    else{
        for(hwss_sockid_t id=0;id<hso_scm->en_sock_num;id++){
            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_GENERIC;
            ESP_GOTO_ON_ERROR(hso_scm->drv.clear_sock_intr(hso_scm,id),err,TAG,"cannot clear sock%u intr",id);
            ESP_GOTO_ON_ERROR(hso_scm->drv.set_sock_intr_enable(hso_scm,id,true),err,TAG,"cannot enable sock%u intr",id);
        };
        hso_scm->active_sock_num=0;

        ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_scm->sock_polling_timer,hso_scm->sock_polling_period_ms*1000),err,TAG,
                            "cannot start sock polling timer");

    }

err:
    return ret;
}

esp_err_t hwss_hso_scm_deinit(hwss_hso_scm_t *hso_scm){
    esp_err_t ret=ESP_OK;
    if(hso_scm->irq_gpio_num>=0){
        ESP_GOTO_ON_ERROR(gpio_isr_handler_remove(hso_scm->irq_gpio_num),err,TAG,"cannot disable irq");
        ESP_GOTO_ON_ERROR(gpio_intr_disable(hso_scm->irq_gpio_num),err,TAG,"cannot disable irq");
        ESP_GOTO_ON_ERROR(gpio_reset_pin(hso_scm->irq_gpio_num),err,TAG,"cannot reset irq pin");

        if(esp_timer_is_active(hso_scm->sock_polling_timer))
            ESP_GOTO_ON_ERROR(esp_timer_stop(hso_scm->sock_polling_timer),err,TAG,"fail to stop sock polling timer");
        
        vTaskDelete(hso_scm->irq_handler);
    }
    else
        ESP_GOTO_ON_ERROR(esp_timer_stop(hso_scm->sock_polling_timer),err,TAG,"fail to stop sock polling timer");

err:
    return ret;    
}

esp_err_t hwss_hso_scm_set_sock_state(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, const hwss_hso_sockact_sta_t *state){
    esp_err_t ret=ESP_OK;
    
    if(hso_scm->irq_gpio_num<0 || *state==HWSS_HSO_SOCKACT_GENERIC)
        return ESP_ERR_NOT_SUPPORTED;

    if(*state==hso_scm->sockact_sta_list[id])
        return ESP_OK;
    
    if(*state == HWSS_HSO_SOCKACT_ACTIVE){
        uint8_t sintr=0;
        ESP_GOTO_ON_ERROR(hso_scm->drv.get_sock_intr(hso_scm,id,&sintr),err,TAG,"cannot get socket%u interrupt",id);
        ESP_GOTO_ON_ERROR(hwss_hso_scm_sock_intr_handler(hso_scm,id,sintr),err,TAG,"cannot handler sock%u interrupt",id);
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

esp_err_t hwss_hso_scm_get_sock_state(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, hwss_hso_sockact_sta_t *state){
    *state=hso_scm->sockact_sta_list[id];
    return ESP_OK;
}

hwss_hso_scm_t *hwss_hso_scm_new(hwss_hso_t *hso, const hwss_hso_scm_config_t *config){
    hwss_hso_scm_t *ret=NULL;

    ret=calloc(1,sizeof(hwss_hso_scm_t));
    ESP_GOTO_ON_FALSE(ret,NULL,err,TAG,"fail to calloc hso_scm");

    ret->hso=hso;

    ret->sockact_sta_list=calloc(config->sock_total_num,sizeof(hwss_hso_sockact_sta_t));
    ESP_GOTO_ON_FALSE(ret->sockact_sta_list,NULL,err,TAG,"fail to calloc sockact_sta_list");

    ret->en_sock_num=config->en_sock_num;

    ret->sock_polling_period_ms=config->sock_polling_period_ms;
    ret->sock_active_threshold_ms=config->sock_active_threshold_ms;

    ret->irq_gpio_num=config->irq_gpio_num;

    if(ret->irq_gpio_num>=0){
        ret->socktimer_list=calloc(config->sock_total_num,sizeof(esp_timer_handle_t));
        ESP_GOTO_ON_FALSE(ret->socktimer_list,NULL,err,TAG,"fail to calloc socktimer_list");

        ret->socktimer_args=calloc(config->sock_total_num,sizeof(hwss_hso_scm_timer_arg_t));
        ESP_GOTO_ON_FALSE(ret->socktimer_args,NULL,err,TAG,"fail to calloc socktimer_args");

        ret->irq_task_prio=config->irq_handler_task_prio;
        ret->irq_task_stack_size=config->irq_handler_task_stack_size;

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

