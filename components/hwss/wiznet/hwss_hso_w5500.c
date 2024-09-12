#include <cstddef>
#include <freertos/task.h>
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5500.h"
#include "hwss_event.h"
#include "hwss_hso_wiznet.h"

#define W5500_SOCK_TOTAL_NUM                                8

static const uint8_t    w5500_sock_mode_default=            Sn_MR_CLOSE;
static const uint16_t   w5500_sock_port_defualt=            0x0000;
static const uint8_t    w5500_sock_dest_mac_addr_default[]= {0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8_t    w5500_sock_dest_addr_default[]=     {0x00,0x00,0x00,0x00};
static const uint16_t   w5500_sock_dest_port_default=       0x0000;

static const char *TAG = "w5500.hwss_hso";

typedef struct{
    hwss_hso_t super;
    
    uint32_t            check_state_period_ms;
    uint32_t            sock0_check_state_fast_period_ms;
    uint32_t            sock0_check_state_slow_period_ms;

    hwss_hso_socksta_t  sock0_sta;
    uint8_t             en_sock_num;

    uint16_t            irq_inter_tick;
    gpio_num_t          irq_gpio_num;

    uint8_t             txbuf_kb;
    uint8_t             rxbuf_kb;

    TaskHandle_t        recv_handler;

    esp_timer_handle_t  sock0_check_state_timer;
    esp_timer_handle_t  sock_check_state_timer;
}hwss_hso_w5500_t;

IRAM_ATTR static void hwss_hso_w5500_isr_handler(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_w5500_t *)args;
    BaseType_t high_task_wakeup=pdTRUE;

    vTaskNotifyGiveFromISR(hso_w5500->recv_handler,&high_task_wakeup);
    if(high_task_wakeup != pdTRUE)
        portYIELD_FROM_ISR();
}

static void hwss_hso_w5500_recv_handler_task(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_t *)args;
    const uint8_t snir_recvclr=Sn_IR_RECV;

    uint8_t sir=0;
    uint8_t snir=0;
    while(1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        if(getSIR(hso_w5500->super.io,&sir)!=ESP_OK){
            ESP_LOGE(TAG,"cannot read SIR");
            continue;
        }

        for(hwss_sockid_t id=0;id<hso_w5500->en_sock_num;id++){
            if(sir&0x01){
                if(getSn_IR(hso_w5500->super.io,id,&snir)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot read Sn_IR");
                    continue;
                }
                
                if(snir&Sn_IR_RECV){
                    if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_RECV,
                                (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                        ESP_LOGE(TAG,"fail to post event");
                        continue;
                    }

                    if(setSn_IR(hso_w5500->super.io,id,&snir_recvclr)!=ESP_OK){
                        ESP_LOGE(TAG,"cannot write Sn_IR");
                        continue;
                    }
                }
            }
            sir>>=1;
        }
    }
}

static void hwss_hso_w5500_check_state_timer_cb(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_w5500_t *)args;
    uint8_t sir=0;
    uint8_t snir=0;
    if(getSIR(hso_w5500->super.io,&sir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot read SIR");
        return;
    }

    sir>>=1;
    for(hwss_sockid_t id=1;id<hso_w5500->en_sock_num;id++){
        if(sir&0x01){
            if(getSn_IR(hso_w5500->super.io,id,&snir)!=ESP_OK){
                ESP_LOGE(TAG,"cannot read Sn_IR");
                return;
            }

            if(snir&Sn_IR_SENDOK){
                if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_SEND_OK,
                            (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to post event");
                    return;
                }
            }

            if(snir&Sn_IR_TIMEOUT){
                if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_TIMEOUT,
                            (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to post event");
                    return;
                }
            }

            if(snir&Sn_IR_RECV){
                if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_RECV,
                            (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to post event");
                    return;
                }
            }

            if(snir&Sn_IR_DISCON){
                if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_DISCONN,
                            (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to post event");
                    return;
                }
            }

            if(snir&Sn_IR_CON){
                if(hwss_event_post(HWSS_EVENT_BASE_SOCKET_LIST[id],HWSS_INTER_SOCK_EVENT_CONNECT,
                            (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                    ESP_LOGE(TAG,"fail to post event");
                    return;
                }
            }

            if(setSn_IR(hso_w5500->super.io,id,&snir)!=ESP_OK){
                ESP_LOGE(TAG,"cannot write Sn_IR");
                return;
            }
        }
        sir>>=1;
    }
}

static void hwss_hso_w5500_sock0_check_state_timer_cb(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_w5500_t *)args;
    
    uint8_t sir=0;
    if(getSIR(hso_w5500->super.io,&sir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot read SIR");
        return;
    }

    if(sir&0x01){
        uint8_t snir=0;
        if(getSn_IR(hso_w5500->super.io,0,&snir)!=ESP_OK){
            ESP_LOGE(TAG,"cannot read Sn_IR");
            return;
        }

        if(snir&Sn_IR_SENDOK){
            if(hwss_event_post(HWSS_INTER_SOCK0_EVENT,HWSS_INTER_SOCK_EVENT_SEND_OK,
                        (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                ESP_LOGE(TAG,"fail to post event");
                return;
            }
        }

        if(snir&Sn_IR_TIMEOUT){
            if(hwss_event_post(HWSS_INTER_SOCK0_EVENT,HWSS_INTER_SOCK_EVENT_TIMEOUT,
                        (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                ESP_LOGE(TAG,"fail to post event");
                return;
            }
        }

        if(snir&Sn_IR_RECV){
            if(hwss_event_post(HWSS_INTER_SOCK0_EVENT,HWSS_INTER_SOCK_EVENT_RECV,
                        (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                ESP_LOGE(TAG,"fail to post event");
                return;
            }
        }

        if(snir&Sn_IR_DISCON){
            if(hwss_event_post(HWSS_INTER_SOCK0_EVENT,HWSS_INTER_SOCK_EVENT_DISCONN,
                        (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                ESP_LOGE(TAG,"fail to post event");
                return;
            }
        }

        if(snir&Sn_IR_CON){
            if(hwss_event_post(HWSS_INTER_SOCK0_EVENT,HWSS_INTER_SOCK_EVENT_CONNECT,
                        (void *)&hso_w5500->super,sizeof(hwss_hso_w5500_t),0)!=ESP_OK){
                ESP_LOGE(TAG,"fail to post event");
                return;
            }
        }

        if(setSn_IR(hso_w5500->super.io,0,&snir)!=ESP_OK){
            ESP_LOGE(TAG,"cannot write Sn_IR");
            return;
        }
    }
}

static esp_err_t hwss_hso_w5500_init(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);

    ESP_GOTO_ON_ERROR(setINTLEVEL(hso->io,&(hso_w5500->irq_inter_tick)),err,TAG,"cannot write INTLEVEL");

    for(hwss_sockid_t id=0;id<W5500_SOCK_TOTAL_NUM;id++){
        ESP_GOTO_ON_ERROR((hso->io,&(hso_w5500->irq_inter_tick)),err,TAG,"cannot write INTLEVEL");
    }

    if(hso_w5500->irq_gpio_num>=0){
        gpio_config_t io_cfg={
            .pin_bit_mask=1ull<<hso_w5500->irq_gpio_num,
            .mode=GPIO_MODE_INPUT,
            .pull_up_en=1,
            .intr_type=GPIO_INTR_NEGEDGE,
        };

        ESP_GOTO_ON_ERROR(gpio_config(&io_cfg),err,TAG,"cannot setup irq pin");
        ESP_GOTO_ON_ERROR(gpio_intr_enable(hso_w5500->irq_gpio_num),err,TAG,"cannot enable irq");
        ESP_GOTO_ON_ERROR(gpio_isr_handler_add(hso_w5500->irq_gpio_num,hwss_hso_w5500_isr_handler,(void *)hso_w5500),
                        err,TAG,"cannot enable irq");
    }

    hso_w5500->sock0_sta=HWSS_HSO_SOCK_IDLE;
    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_w5500->sock_check_state_timer,hso_w5500->check_state_period_ms*1000));
    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_w5500->sock0_check_state_timer,hso_w5500->sock0_check_state_slow_period_ms*1000));

err:
    return ret;
}

static esp_err_t hwss_hso_w5500_deinit(hwss_hso_t *hso){

}

esp_err_t (*set_sock_proto)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_proto_t *proto);
esp_err_t (*get_sock_proto)(hwss_hso_t *hso, hwss_sockid_t id, hwss_proto_t *proto);

esp_err_t (*set_sock_source_port)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port);
esp_err_t (*get_sock_source_port)(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port);

esp_err_t (*set_sock_dest_port)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port);
esp_err_t (*get_sock_dest_port)(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port);

esp_err_t (*set_sock_dest_mac_addr)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_mac_addr_t addr);
esp_err_t (*get_sock_dest_mac_addr)(hwss_hso_t *hso, hwss_sockid_t id, hwss_mac_addr_t addr);

esp_err_t (*set_sock_keepalive_tick)(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *tick);
esp_err_t (*get_sock_keepalive_tick)(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *tick);

esp_err_t (*sock_send)(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *data, uint16_t len);
esp_err_t (*sock_recv)(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *data, uint16_t *len);
esp_err_t (*sock_drop)(hwss_hso_t *hso, hwss_sockid_t id, uint16_t len);
