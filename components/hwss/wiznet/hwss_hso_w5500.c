#include <cstddef>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5500.h"
#include "hwss_hso_wiznet.h"

#define W5500_SOCK_TOTAL_NUM                                8

static const uint8_t    w5500_sock_mode_default=            Sn_MR_CLOSE;
static const uint16_t   w5500_sock_port_defualt=            0x0000;
static const uint8_t    w5500_sock_dest_mac_addr_default[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static const uint8_t    w5500_sock_dest_addr_default[]=     {0x00,0x00,0x00,0x00};
static const uint16_t   w5500_sock_dest_port_default=       0x0000;
static const uint16_t   w5500_sock_mss_default=             0x0000;
static const uint8_t    w5500_sock_keepalive_tick_default=  0x00;
static const uint16_t   w5500_sock_frag_default=            0x4000;
static const uint8_t    w5500_sock_irq_mask_default=        Sn_IR_SENDOK|Sn_IR_TIMEOUT|Sn_IR_RECV|Sn_IR_CON|Sn_IR_DISCON;

static const char *TAG = "w5500.hwss_hso";

typedef union{
    struct{
        uint8_t protocol :4;
        uint8_t unicastb_mip6b :1;
        uint8_t nd_mc_mmb :1;
        uint8_t bcastb :1;
        uint8_t multi_mfen :1;
    };
    uint8_t val;
}sn_mr_reg_t;

typedef struct{
    hwss_hso_t super;
    
    uint32_t            sock_check_state_period_ms;

    uint8_t             en_sock_num;
    uint8_t             txbuf_size_kb[W5500_SOCK_TOTAL_NUM];
    uint8_t             rxbuf_size_kb[W5500_SOCK_TOTAL_NUM];

    uint16_t            irq_inter_tick;
}hwss_hso_w5500_t;



static void hwss_hso_w5500_irq_handler_task(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_t *)args;

    uint8_t sir=0;
    uint8_t snir=0;
    uint8_t simr=0;
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

                if(snir&Sn_IR_TIMEOUT){

                }

                if(snir&Sn_IR_RECV){

                }

                if(snir&Sn_IR_DISCON){

                }

                if(snir&Sn_IR_CON){

                }

                if(setSn_IR(hso_w5500->super.io,id,&snir)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot write Sn_IR");
                    return;
                }

                if(getSIMR(hso_w5500->super.io,&simr)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot read SIMR");
                    return;
                }

                simr&=~(1<<id);

                if(setSIMR(hso_w5500->super.io,&simr)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot write SIMR");
                    return;
                }

                hso_w5500->socksta_list[id]=HWSS_HSO_SOCK_ACTIVE;
                if(esp_timer_start_once(hso_w5500->socktimer_list[id],hso_w5500->sock_active_threshold_ms*1000)!=ESP_OK){
                    ESP_LOGE(TAG,"cannot start active timer");
                    return;
                }
            }
            sir>>=1;
        }
    }
}

static void hwss_hso_w5500_check_state_timer_cb(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_w5500_t *)args;

}



static esp_err_t hwss_hso_w5500_init(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);

    ESP_GOTO_ON_ERROR(setINTLEVEL(hso->io,&(hso_w5500->irq_inter_tick)),err,TAG,"cannot write INTLEVEL");

    uint8_t snir_clr=Sn_IR_CON|Sn_IR_DISCON|Sn_IR_RECV|Sn_IR_SENDOK|Sn_IR_TIMEOUT;
    for(hwss_sockid_t id=0;id<W5500_SOCK_TOTAL_NUM;id++){
        ESP_GOTO_ON_ERROR(setSn_MR(hso->io,id,&w5500_sock_mode_default),err,TAG,"cannot write Sn_MR");
        ESP_GOTO_ON_ERROR(setSn_IR(hso->io,id,&snir_clr),err,TAG,"cannot write Sn_IR");
        ESP_GOTO_ON_ERROR(setSn_PORT(hso->io,id,&w5500_sock_port_defualt),err,TAG,"cannot write Sn_PORT");
        ESP_GOTO_ON_ERROR(setSn_DHAR(hso->io,id,w5500_sock_dest_mac_addr_default),err,TAG,"cannot write Sn_DHAR");
        ESP_GOTO_ON_ERROR(setSn_DIPR(hso->io,id,w5500_sock_dest_addr_default),err,TAG,"cannot write Sn_DIPR");
        ESP_GOTO_ON_ERROR(setSn_DPORT(hso->io,id,&w5500_sock_dest_port_default),err,TAG,"cannot write Sn_DPORT");
        ESP_GOTO_ON_ERROR(setSn_MSSR(hso->io,id,&w5500_sock_mss_default),err,TAG,"cannot write Sn_MSSR");
        ESP_GOTO_ON_ERROR(setSn_RXBUF_SIZE(hso->io,id,&(hso_w5500->rxbuf_size_kb[id])),err,TAG,"cannot write Sn_RXBUF_SIZE");
        ESP_GOTO_ON_ERROR(setSn_TXBUF_SIZE(hso->io,id,&(hso_w5500->txbuf_size_kb[id])),err,TAG,"cannot write Sn_TXBUF_SIZE");
        ESP_GOTO_ON_ERROR(setSn_IMR(hso->io,id,&w5500_sock_irq_mask_default),err,TAG,"cannot write Sn_IMR");
        ESP_GOTO_ON_ERROR(setSn_FRAG(hso->io,id,&w5500_sock_frag_default),err,TAG,"cannot write Sn_FRAG");
        ESP_GOTO_ON_ERROR(setSn_KPALVTR(hso->io,id,&w5500_sock_keepalive_tick_default),err,TAG,"cannot write Sn_KPALVTR");
    }

    hso_w5500->sock0_sta=HWSS_HSO_SOCK_IDLE;
    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_w5500->sock_check_state_timer,hso_w5500->check_state_period_ms*1000),
                        err,TAG,"fail to start sock timer");
    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_w5500->sock0_check_state_timer,hso_w5500->sock0_check_state_slow_period_ms*1000),
                        err,TAG,"fail to start sock0 timer");



err:
    return ret;
}

static esp_err_t hwss_hso_w5500_deinit(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);



    ESP_GOTO_ON_ERROR(esp_timer_stop(hso_w5500->sock_check_state_timer),err,TAG,"fail to stop sock timer");
    ESP_GOTO_ON_ERROR(esp_timer_stop(hso_w5500->sock0_check_state_timer),err,TAG,"fail to stop sock0 timer");

err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sock_proto(hwss_hso_t *hso, hwss_sockid_t id, const hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    
    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");
    
    switch (*proto)
    {
    case HWSS_PROTO_CLOSE: snmr.protocol=Sn_MR_CLOSE; break;
    case HWSS_PROTO_MACRAW: snmr.protocol=Sn_MR_MACRAW; break;
    case HWSS_PROTO_TCP: snmr.protocol=Sn_MR_TCP; break;
    case HWSS_PROTO_UDP: snmr.protocol=Sn_MR_UDP; break;
    default: return ESP_ERR_NOT_SUPPORTED;
    }

    ESP_GOTO_ON_ERROR(setSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_proto(hwss_hso_t *hso, hwss_sockid_t id, hwss_proto_t *proto){

}

static esp_err_t hwss_hso_w5500_set_sock_source_port(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port){

}

static esp_err_t hwss_hso_w5500_get_sock_source_port(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port){

}

static esp_err_t hwss_hso_w5500_set_sock_dest_port(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port){

}

static esp_err_t hwss_hso_w5500_get_sock_dest_port(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port){

}

static esp_err_t hwss_hso_w5500_set_sock_dest_mac_addr(hwss_hso_t *hso, hwss_sockid_t id, const hwss_mac_addr_t addr){

}

static esp_err_t hwss_hso_w5500_get_sock_dest_mac_addr(hwss_hso_t *hso, hwss_sockid_t id, hwss_mac_addr_t addr){

}

static esp_err_t hwss_hso_w5500_set_sock_keepalive_tick(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *tick){

}

static esp_err_t hwss_hso_w5500_get_sock_keepalive_tick(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *tick){

}

hwss_hso_t *hwss_hso_new_w5500(hwss_io_t *io, const hwss_hso_config_t *config){

}
