#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5500.h"
#include "hwss_event.h"
#include "hwss_hnet_wiznet.h"

#define CP_REQ_MS2TICK(ms)              ms/25
#define CP_REQ_TICK2MS(tick)            tick*25

static const char *TAG = "w5500.hwss_hnet";

typedef struct{
    hwss_hnet_t super;
    uint16_t    retry_tick;
    uint8_t     retry_cnt;
    uint8_t     ppp_cp_request_tick;
    uint8_t     ppp_cp_magic_num;

    uint32_t    check_state_period_ms;
    esp_timer_handle_t check_state_timer;
}hwss_hnet_w5500_t;

static void hwss_hnet_w5500_check_state_timer_cb(void *args){
    hwss_hnet_w5500_t* hnet_w5500=(hwss_hnet_w5500_t *)args;

    uint8_t ir;
    if(W5500_getIR(hnet_w5500->super.io,&ir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot read IR");
        return;
    }

    if(ir&W5500_IR_CONFLICT){        
        if(hwss_event_post(HWSS_INTER_NET_EVENT,HWSS_INTER_NET_EVENT_IP_CONFLICT,(void *)&hnet_w5500->super,
                        sizeof(hwss_hnet_t *),0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(ir&W5500_IR_UNREACH){        
        if(hwss_event_post(HWSS_INTER_NET_EVENT,HWSS_INTER_NET_EVENT_DEST_UNREACH,(void *)&hnet_w5500->super,
                        sizeof(hwss_hnet_t *),0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(ir&W5500_IR_PPPoE){        
        if(hwss_event_post(HWSS_INTER_NET_EVENT,HWSS_INTER_NET_EVENT_PPPOE_CLOSE,(void *)&hnet_w5500->super,
                        sizeof(hwss_hnet_t *),0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(ir&W5500_IR_MP){        
        if(hwss_event_post(HWSS_INTER_NET_EVENT,HWSS_INTER_NET_EVENT_MAGIC_PACK,(void *)&hnet_w5500->super,
                        sizeof(hwss_hnet_t *),0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(W5500_setIR(hnet_w5500->super.io,&ir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot write IR");
        return;
    }

}

static esp_err_t hwss_hnet_w5500_init(hwss_hnet_t *hnet){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_setRTR(hnet->io, &(hnet_w5500->retry_tick)),err,TAG,"cannot write RTR");
    ESP_GOTO_ON_ERROR(W5500_setRCR(hnet->io, &(hnet_w5500->retry_cnt)),err,TAG,"cannot write RCR");
    ESP_GOTO_ON_ERROR(W5500_setPTIMER(hnet->io,&(hnet_w5500->ppp_cp_request_tick)),err,TAG,"cannot write PTIMER");
    ESP_GOTO_ON_ERROR(W5500_setPMAGIC(hnet->io,&(hnet_w5500->ppp_cp_magic_num)),err,TAG,"cannot write PMAGIC");

    uint8_t ir=W5500_IR_CONFLICT|W5500_IR_UNREACH|W5500_IR_PPPoE|W5500_IR_MP;
    ESP_GOTO_ON_ERROR(W5500_setIR(hnet->io,&ir),err,TAG,"cannot write IR");

    uint8_t imr=W5500_IM_IR7|W5500_IM_IR6|W5500_IM_IR5|W5500_IM_IR4;
    ESP_GOTO_ON_ERROR(W5500_setIMR(hnet->io,&imr),err,TAG,"cannot write IMR");

    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hnet_w5500->check_state_timer,hnet_w5500->check_state_period_ms*1000),
                        err,TAG,"cannot start timer");

err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_deinit(hwss_hnet_t *hnet){
    return ESP_OK;
}

static esp_err_t hwss_hnet_w5500_set_gateway_addr(hwss_hnet_t *hnet, const hwss_ip_addr_t addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_setGAR(hnet->io, addr),err,TAG,"cannot write GAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_gateway_addr(hwss_hnet_t *hnet, hwss_ip_addr_t addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getGAR(hnet->io, addr),err,TAG,"cannot read GAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_subnet_mask(hwss_hnet_t *hnet, const hwss_ip_addr_t mask){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_setSUBR(hnet->io, mask),err,TAG,"cannot write SUBR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_subnet_mask(hwss_hnet_t *hnet, hwss_ip_addr_t mask){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getSUBR(hnet->io, mask),err,TAG,"cannot read SUBR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_source_addr(hwss_hnet_t *hnet, const hwss_ip_addr_t addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_setSIPR(hnet->io, addr),err,TAG,"cannot write SIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_source_addr(hwss_hnet_t *hnet, hwss_ip_addr_t addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getSIPR(hnet->io, addr),err,TAG,"cannot read SIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_retry_time(hwss_hnet_t *hnet, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    uint16_t tick=*ms*10;

    ESP_GOTO_ON_ERROR(W5500_setRTR(hnet->io, &tick),err,TAG,"cannot write RTR");
    hnet_w5500->retry_tick=tick;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_retry_time(hwss_hnet_t *hnet, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    uint16_t tick=0;

    ESP_GOTO_ON_ERROR(W5500_getRTR(hnet->io, &tick),err,TAG,"cannot read RTR");
    *ms=tick/10;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_retry_cnt(hwss_hnet_t *hnet, const uint8_t *cnt){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_setRCR(hnet->io, cnt),err,TAG,"cannot write RCR");
    hnet_w5500->retry_cnt=*cnt;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_retry_cnt(hwss_hnet_t *hnet, uint8_t *cnt){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(W5500_getRCR(hnet->io, cnt),err,TAG,"cannot read RCR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_unreachable_addr(hwss_hnet_t *hnet, hwss_ip_addr_t addr){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(W5500_getUIPR(hnet->io, addr),err,TAG,"cannot read UIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_unreachable_port(hwss_hnet_t *hnet, hwss_port_t *port){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(W5500_getUPORTR(hnet->io, port),err,TAG,"cannot read UPORTR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_ppp_link_cp_request_time(hwss_hnet_t *hnet, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(*ms)<=0xFF,ESP_ERR_INVALID_ARG,err,TAG,"control protocol request time too long");

    uint8_t tick=(uint8_t) CP_REQ_MS2TICK(*ms);
    ESP_GOTO_ON_ERROR(W5500_setPTIMER(hnet->io,&tick),err,TAG,"cannot write PTIMER");
    hnet_w5500->ppp_cp_request_tick=tick;
err:
    return ret;    
}

static esp_err_t hwss_hnet_w5500_get_ppp_link_cp_request_time(hwss_hnet_t *hnet, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    uint8_t tick=0;
    ESP_GOTO_ON_ERROR(W5500_getPTIMER(hnet->io,&tick),err,TAG,"cannot read PTIMER");
    *ms=(uint16_t) CP_REQ_TICK2MS(tick);
err:
    return ret;    
}

static esp_err_t hwss_hnet_w5500_set_ppp_link_cp_magic_num(hwss_hnet_t *hnet, const uint8_t *num){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_setPMAGIC(hnet->io,num),err,TAG,"cannot write PMAGIC");
    hnet_w5500->ppp_cp_magic_num=*num;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_ppp_link_cp_magic_num(hwss_hnet_t *hnet, uint8_t *num){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getPMAGIC(hnet->io,num),err,TAG,"cannot read PMAGIC");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_ppp_dest_mac_addr(hwss_hnet_t *hnet, const hwss_mac_addr_t mac_addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_setPHAR(hnet->io,mac_addr),err,TAG,"cannot write PHAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_ppp_dest_mac_addr(hwss_hnet_t *hnet, hwss_mac_addr_t mac_addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getPHAR(hnet->io,mac_addr),err,TAG,"cannot read PHAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_ppp_sess_id(hwss_hnet_t *hnet, const uint16_t *id){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_setPSID(hnet->io,id),err,TAG,"cannot write PSID");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_ppp_sess_id(hwss_hnet_t *hnet, uint16_t *id){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getPSID(hnet->io,id),err,TAG,"cannot read PSID");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_ppp_max_recv_unit(hwss_hnet_t *hnet, const uint16_t *unit){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_setPMRU(hnet->io,unit),err,TAG,"cannot write PMRU");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_ppp_max_recv_unit(hwss_hnet_t *hnet, uint16_t *unit){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5500_getPMRU(hnet->io,unit),err,TAG,"cannot read PMRU");
err:
    return ret;
}

hwss_hnet_t *hwss_hnet_new_w5500(hwss_io_t *io, hwss_hnet_config_t *config){
    hwss_hnet_t *ret=NULL;
    hwss_hnet_w5500_t* hnet=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");
    if(config->check_state_period_ms<5)
        ESP_LOGW(TAG,"check period too short might SERIOUSLY affects the execution of other tasks!");

    hnet=calloc(1,sizeof(hwss_hnet_w5500_t));
    ESP_GOTO_ON_FALSE(hnet,NULL,err,TAG,"calloc hnet failed!");

    hnet->super.io=io;
    hnet->super.init=hwss_hnet_w5500_init;
    hnet->super.deinit=hwss_hnet_w5500_deinit;
    hnet->super.set_gateway_addr=hwss_hnet_w5500_set_gateway_addr;
    hnet->super.get_gateway_addr=hwss_hnet_w5500_get_gateway_addr;
    hnet->super.set_subnet_mask=hwss_hnet_w5500_set_subnet_mask;
    hnet->super.get_subnet_mask=hwss_hnet_w5500_get_subnet_mask;
    hnet->super.set_source_addr=hwss_hnet_w5500_set_source_addr;
    hnet->super.get_source_addr=hwss_hnet_w5500_get_source_addr;
    hnet->super.set_retry_time=hwss_hnet_w5500_set_retry_time;
    hnet->super.get_retry_time=hwss_hnet_w5500_get_retry_time;
    hnet->super.set_retry_cnt=hwss_hnet_w5500_set_retry_cnt;
    hnet->super.get_retry_cnt=hwss_hnet_w5500_get_retry_cnt;
    hnet->super.get_unreachable_addr=hwss_hnet_w5500_get_unreachable_addr;
    hnet->super.get_unreachable_port=hwss_hnet_w5500_get_unreachable_port;
    hnet->super.set_ppp_link_cp_request_time=hwss_hnet_w5500_set_ppp_link_cp_request_time;
    hnet->super.get_ppp_link_cp_request_time=hwss_hnet_w5500_get_ppp_link_cp_request_time;
    hnet->super.set_ppp_link_cp_magic_num=hwss_hnet_w5500_set_ppp_link_cp_magic_num;
    hnet->super.get_ppp_link_cp_magic_num=hwss_hnet_w5500_get_ppp_link_cp_magic_num;
    hnet->super.set_ppp_dest_mac_addr=hwss_hnet_w5500_set_ppp_dest_mac_addr;
    hnet->super.get_ppp_dest_mac_addr=hwss_hnet_w5500_get_ppp_dest_mac_addr;
    hnet->super.set_ppp_sess_id=hwss_hnet_w5500_set_ppp_sess_id;
    hnet->super.get_ppp_sess_id=hwss_hnet_w5500_get_ppp_sess_id;
    hnet->super.set_ppp_max_recv_unit=hwss_hnet_w5500_set_ppp_max_recv_unit;
    hnet->super.get_ppp_max_recv_unit=hwss_hnet_w5500_get_ppp_max_recv_unit;

    hnet->retry_tick=config->retry_time_ms*10;
    hnet->retry_cnt=config->retry_cnt;
    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(config->ppp_cp_request_time_ms)<=0xFF,NULL,
                    err,TAG,"control protocol request time too long");
    hnet->ppp_cp_request_tick=(uint8_t) CP_REQ_MS2TICK(config->ppp_cp_request_time_ms);
    hnet->ppp_cp_magic_num=config->ppp_cp_magic_num;
    hnet->check_state_period_ms=config->check_state_period_ms;

    esp_timer_create_args_t timer_arg={
        .name="hwss_hnet_w5500_check_state_timer",
        .callback=hwss_hnet_w5500_check_state_timer_cb,
        .arg=hnet,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&hnet->check_state_timer)==ESP_OK,NULL,err,TAG,"create check state timer failed");

    return &hnet->super;
err:
    if(hnet)
        free(hnet);
    return ret;
}

