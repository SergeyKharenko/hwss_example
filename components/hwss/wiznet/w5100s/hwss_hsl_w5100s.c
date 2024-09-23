#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5100s.h"
#include "hwss_event.h"
#include "hwss_hsl_wiznet.h"

static const char *TAG = "w5100s.hwss_hsl";

typedef struct{
    hwss_hsl_t  super;

    uint16_t    retry_time_tick;
    uint8_t     retry_cnt;

    uint32_t    check_state_period_ms;
    esp_timer_handle_t check_state_timer;
}hwss_hsl_w5100_t;

static void hwss_hsl_w5100s_check_state_timer_cb(void *args){
    hwss_hsl_w5100_t* hsl_w5100s=(hwss_hsl_w5100_t *)args;

    uint8_t slir;
    if(W5100S_getSLIR(hsl_w5100s->super.io,&slir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot read SLIR");
        return;
    }

    if(slir&W5100S_SLIR_ARP){
        if(esp_event_post_to(hsl_w5100s->super.elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSL_ARP,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
            return;
        }
    }

    if(slir&W5100S_SLIR_TIMEOUT){
        if(esp_event_post_to(hsl_w5100s->super.elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSL_TIMEOUT,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
            return;
        }
    }

    if(slir&W5100S_SLIR_PING){
        if(esp_event_post_to(hsl_w5100s->super.elp_hdl,HWSS_INTER_EVENT,HWSS_INTER_EVENT_HSL_PING,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
            return;
        }
    }

    if(W5100S_setSLIR(hsl_w5100s->super.io,&slir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot write SLIR");
        return;
    }
}

static esp_err_t hwss_hsl_w5100s_init(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setSLRTR(hsl->io,&hsl_w5100s->retry_time_tick),err,TAG,"cannot write SLRTR");
    ESP_GOTO_ON_ERROR(W5100S_setSLRCR(hsl->io,&hsl_w5100s->retry_cnt),err,TAG,"cannot write SLRCR");

    uint8_t slir;
    slir=W5100S_SLIR_ARP|W5100S_SLIR_TIMEOUT|W5100S_SLIR_PING;
    ESP_GOTO_ON_ERROR(W5100S_setSLIR(hsl->io,&slir),err,TAG,"cannot write SLIR");
    
    uint8_t slimr;
    slimr=0x00;
    ESP_GOTO_ON_ERROR(W5100S_setSLIMR(hsl->io,&slimr),err,TAG,"cannot write SLIMR");

    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hsl_w5100s->check_state_timer,hsl_w5100s->check_state_period_ms*1000),
                        err,TAG,"cannot start timer");

err:
    return ret;
}

static esp_err_t hwss_hsl_w5100s_deinit(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);
    ESP_GOTO_ON_ERROR(esp_timer_stop(hsl_w5100s->check_state_timer),err,TAG,"cannot stop timer");

err:
    return ret;
}

static esp_err_t hwss_hsl_w5100s_set_peer_addr(hwss_hsl_t *hsl, const hwss_ip_addr_t addr){
    return W5100S_setSLPIPR(hsl->io,addr);
}

static esp_err_t hwss_hsl_w5100s_get_peer_addr(hwss_hsl_t *hsl, hwss_ip_addr_t addr){
    return W5100S_getSLPIPR(hsl->io,addr);
}

static esp_err_t hwss_hsl_w5100s_get_peer_mac_addr(hwss_hsl_t *hsl, hwss_mac_addr_t addr){
    return W5100S_getSLPHAR(hsl->io,addr);
}

static esp_err_t hwss_hsl_w5100s_set_ping_seqnum(hwss_hsl_t *hsl, const uint16_t *num){
    return W5100S_setPINGSEQR(hsl->io,num);
}

static esp_err_t hwss_hsl_w5100s_get_ping_seqnum(hwss_hsl_t *hsl, uint16_t *num){
    return W5100S_getPINGSEQR(hsl->io,num);
}

static esp_err_t hwss_hsl_w5100s_set_ping_id(hwss_hsl_t *hsl, const uint16_t *id){
    return W5100S_setPINGIDR(hsl->io,id);
}

static esp_err_t hwss_hsl_w5100s_get_ping_id(hwss_hsl_t *hsl, uint16_t *id){
    return W5100S_getPINGIDR(hsl->io,id);
}

static esp_err_t hwss_hsl_w5100s_send_ping(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;

    uint8_t slcr=0;
    while(1){
        ESP_GOTO_ON_ERROR(W5100S_getSLCR(hsl->io,&slcr),err,TAG,"cannot read SLCR");
        if(slcr&(W5100S_SLCMD_ARP|W5100S_SLCMD_PING))
            continue;
        else
            break;
    }

    slcr=W5100S_SLCMD_PING;
    ESP_GOTO_ON_ERROR(W5100S_setSLCR(hsl->io,&slcr),err,TAG,"cannot write SLCR");

err:
    return ret;
}

static esp_err_t hwss_hsl_w5100s_send_arp(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;

    uint8_t slcr=0;
    while(1){
        ESP_GOTO_ON_ERROR(W5100S_getSLCR(hsl->io,&slcr),err,TAG,"cannot read SLCR");
        if(slcr&(W5100S_SLCMD_ARP|W5100S_SLCMD_PING))
            continue;
        else
            break;
    }

    slcr=W5100S_SLCMD_ARP;
    ESP_GOTO_ON_ERROR(W5100S_setSLCR(hsl->io,&slcr),err,TAG,"cannot write SLCR");

err:
    return ret;
}


hwss_hsl_t *hwss_hsl_new_w5100s(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hsl_config_t *config){
    hwss_hsl_t *ret=NULL;
    hwss_hsl_w5100_t *hsl=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");

    hsl=calloc(1,sizeof(hwss_hsl_w5100_t));
    ESP_GOTO_ON_FALSE(hsl,NULL,err,TAG,"calloc hsl failed!");

    hsl->super.io=io;
    hsl->super.elp_hdl=elp_hdl;
    hsl->super.init=hwss_hsl_w5100s_init;
    hsl->super.deinit=hwss_hsl_w5100s_deinit;
    hsl->super.set_peer_addr=hwss_hsl_w5100s_set_peer_addr;
    hsl->super.get_peer_addr=hwss_hsl_w5100s_get_peer_addr;
    hsl->super.get_peer_mac_addr=hwss_hsl_w5100s_get_peer_mac_addr;
    hsl->super.set_ping_seqnum=hwss_hsl_w5100s_set_ping_seqnum;
    hsl->super.get_ping_seqnum=hwss_hsl_w5100s_get_ping_seqnum;
    hsl->super.set_ping_id=hwss_hsl_w5100s_set_ping_id;
    hsl->super.get_ping_id=hwss_hsl_w5100s_get_ping_id;
    hsl->super.send_ping=hwss_hsl_w5100s_send_ping;
    hsl->super.send_arp=hwss_hsl_w5100s_send_arp;

    hsl->retry_time_tick=config->retry_time_ms*10;
    hsl->retry_cnt=config->retry_cnt;
    hsl->check_state_period_ms=config->check_state_period_ms;

    esp_timer_create_args_t timer_arg={
        .name="hwss_hsl_w5500_check_state_timer",
        .callback=hwss_hsl_w5100s_check_state_timer_cb,
        .arg=hsl,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&hsl->check_state_timer)==ESP_OK,NULL,err,TAG,"create check state timer failed");

    return &hsl->super;
err:
    if(hsl)
        free(hsl);
    return ret;
}
