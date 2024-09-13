#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "hwss_hso_scm.h"
#include "hwss_event.h"

static const char *TAG = "hwss_hso_scm";

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
                    ESP_LOGE(TAG,"cannot get socket%u interrupt",id);
                    continue;
                }

                if(sintr&HWSS_HSO_INTR_SEND_OK){
                    if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_SEND_OK,
                                (void *)hso_scm->hso,sizeof(hwss_hso_t *),0)!=ESP_OK){
                        ESP_LOGE(TAG,"fail to post event");
                        continue;
                    }
                }

                if(sintr&HWSS_HSO_INTR_TIMEOUT){
                    if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_TIMEOUT,
                                (void *)hso_scm->hso,sizeof(hwss_hso_t *),0)!=ESP_OK){
                        ESP_LOGE(TAG,"fail to post event");
                        continue;
                    }
                }

                if(sintr&HWSS_HSO_INTR_RECV){
                    if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_RECV,
                                (void *)hso_scm->hso,sizeof(hwss_hso_t *),0)!=ESP_OK){
                        ESP_LOGE(TAG,"fail to post event");
                        continue;
                    }
                }

                if(sintr&HWSS_HSO_INTR_DISCONN){
                    if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_DISCONN,
                                (void *)hso_scm->hso,sizeof(hwss_hso_t *),0)!=ESP_OK){
                        ESP_LOGE(TAG,"fail to post event");
                        continue;
                    }
                }

                if(sintr&HWSS_HSO_INTR_CONNECT){
                    if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_CONNECT,
                                (void *)hso_scm->hso,sizeof(hwss_hso_t *),0)!=ESP_OK){
                        ESP_LOGE(TAG,"fail to post event");
                        continue;
                    }
                }

                hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_ACTIVE;
                if(hso_scm->drv.set_sock_global_intr_enable(hso_scm,id,false)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to mask sock%u global interrupt",id);
                    continue;
                }

                if(hso_scm->drv.clear_sock_intr(hso_scm,id)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to clear sock%u interrupt",id);
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
    
    while(1){
        if(hso_scm->active_sock_num==0)
            vTaskSuspend(NULL);
        else{

        }
    }
}

static void hwss_hso_scm_sockact_timer_cb(void *args){
    hwss_hso_scm_t *hso_scm=(hwss_hso_scm_t *)args;

}

esp_err_t hwss_hso_scm_init(hwss_hso_scm_t *hso_scm){
    if(hso_scm->irq_gpio_num>=0){
        BaseType_t xReturned = xTaskCreatePinnedToCore(hwss_hso_w5500_recv_handler_task, "hso_w5500_tsk", 
                                hso_scm->recv_task_stack_size, hso_scm, hso_scm->recv_task_prio, &hso_scm->recv_handler, tskNO_AFFINITY);

        ESP_GOTO_ON_FALSE(xReturned == pdPASS, ESP_FAIL, err, TAG,"cannot create hso_w5500 recv task");

        gpio_config_t io_cfg={
            .pin_bit_mask=1ull<<hso_scm->irq_gpio_num,
            .mode=GPIO_MODE_INPUT,
            .pull_up_en=1,
            .intr_type=GPIO_INTR_NEGEDGE,
        };

        ESP_GOTO_ON_ERROR(gpio_config(&io_cfg),err,TAG,"cannot setup irq pin");
        ESP_GOTO_ON_ERROR(gpio_intr_enable(hso_w5500->irq_gpio_num),err,TAG,"cannot enable irq");
        ESP_GOTO_ON_ERROR(gpio_isr_handler_add(hso_scm->irq_gpio_num,hwss_hso_w5500_isr_handler,(void *)hso_scm),
                        err,TAG,"cannot enable irq");
    }
}

esp_err_t hwss_hso_scm_deinit(hwss_hso_scm_t *hso_scm){
    if(hso_scm->irq_gpio_num>=0){
        ESP_GOTO_ON_ERROR(gpio_isr_handler_remove(hso_w5500->irq_gpio_num),err,TAG,"cannot disable irq");
        ESP_GOTO_ON_ERROR(gpio_intr_disable(hso_w5500->irq_gpio_num),err,TAG,"cannot disable irq");
        ESP_GOTO_ON_ERROR(gpio_reset_pin(hso_w5500->irq_gpio_num),err,TAG,"cannot reset irq pin");

        vTaskDelete(hso_w5500->recv_handler);
    }
}

esp_err_t hwss_hso_scm_set_sock_state(hwss_hso_scm_t *hso_scm, const hwss_hso_sockact_sta_t *state){

}

esp_err_t hwss_hso_scm_get_sock_state(hwss_hso_scm_t *hso_scm, hwss_hso_sockact_sta_t *state){

}

esp_err_t hwss_hso_scm_initialize(hwss_hso_scm_t *hso_scm, const hwss_hso_scm_config_t *config){
    esp_err_t ret=ESP_OK;

    hso_scm->sockact_sta_list=calloc(config->sock_total_num,sizeof(hwss_hso_sockact_sta_t));
    ESP_GOTO_ON_FALSE(hso_scm->sockact_sta_list,ESP_FAIL,err,TAG,"fail to calloc sockact_sta_list");

    hso_scm->en_sock_num=config->en_sock_num;

    hso_scm->sock_polling_period_ms=config->sock_polling_period_ms;
    hso_scm->sock_active_threshold_ms=config->sock_active_threshold_ms;

    hso_scm->irq_gpio_num=config->irq_gpio_num;
    hso_scm->polling_task_stack_size=config->polling_task_stack_size;
    hso_scm->polling_task_prio=config->polling_task_prio;

    if(hso_scm->irq_gpio_num<0){
        for(hwss_sockid_t id=0;id<config->sock_total_num;id++)
            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_GENERIC;
    }
    else{
        for(hwss_sockid_t id=0;id<config->sock_total_num;id++)
            hso_scm->sockact_sta_list[id]=HWSS_HSO_SOCKACT_IDLE;

        hso_scm->socktimer_list=calloc(config->sock_total_num,sizeof(esp_timer_handle_t));
        ESP_GOTO_ON_FALSE(hso_scm->socktimer_list,ESP_FAIL,err,TAG,"fail to calloc socktimer_list");
    }



err:
    return ret;
}

