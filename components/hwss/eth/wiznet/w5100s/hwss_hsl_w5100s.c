#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5100s.h"
#include "hwss_eth_event.h"
#include "hwss_hsl_wiznet.h"

static const char *TAG = "w5100s.hwss_hsl";

typedef struct{
    hwss_hsl_t  super;

    hwss_io_t   *io;

    uint16_t    retry_time_tick;
    uint8_t     retry_cnt;
}hwss_hsl_w5100_t;

static esp_err_t hwss_hsl_w5100s_init(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setSLRTR(hsl_w5100s->io,&hsl_w5100s->retry_time_tick),err,TAG,"cannot write SLRTR");
    ESP_GOTO_ON_ERROR(W5100S_setSLRCR(hsl_w5100s->io,&hsl_w5100s->retry_cnt),err,TAG,"cannot write SLRCR");

err:
    return ret;
}

static esp_err_t hwss_hsl_w5100s_deinit(hwss_hsl_t *hsl){
    return ESP_OK;
}

static esp_err_t hwss_hsl_w5100s_set_peer_addr(hwss_hsl_t *hsl, const hwss_eth_ip4_addr_t addr){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    return W5100S_setSLPIPR(hsl_w5100s->io,addr);
}

static esp_err_t hwss_hsl_w5100s_get_peer_addr(hwss_hsl_t *hsl, hwss_eth_ip4_addr_t addr){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);
    
    return W5100S_getSLPIPR(hsl_w5100s->io,addr);
}

static esp_err_t hwss_hsl_w5100s_get_peer_mac_addr(hwss_hsl_t *hsl, hwss_eth_mac_addr_t addr){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    return W5100S_getSLPHAR(hsl_w5100s->io,addr);
}

static esp_err_t hwss_hsl_w5100s_set_ping_seqnum(hwss_hsl_t *hsl, const uint16_t *num){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    return W5100S_setPINGSEQR(hsl_w5100s->io,num);
}

static esp_err_t hwss_hsl_w5100s_get_ping_seqnum(hwss_hsl_t *hsl, uint16_t *num){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    return W5100S_getPINGSEQR(hsl_w5100s->io,num);
}

static esp_err_t hwss_hsl_w5100s_set_ping_id(hwss_hsl_t *hsl, const uint16_t *id){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    return W5100S_setPINGIDR(hsl_w5100s->io,id);
}

static esp_err_t hwss_hsl_w5100s_get_ping_id(hwss_hsl_t *hsl, uint16_t *id){
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);

    return W5100S_getPINGIDR(hsl_w5100s->io,id);
}

static esp_err_t hwss_hsl_w5100s_send_ping(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);
    uint8_t slcr=0;

    while(1){
        ESP_GOTO_ON_ERROR(W5100S_getSLCR(hsl_w5100s->io,&slcr),err,TAG,"cannot read SLCR");
        if(slcr&(W5100S_SLCMD_ARP|W5100S_SLCMD_PING))
            continue;
        else
            break;
    }

    slcr=W5100S_SLCMD_PING;
    ESP_GOTO_ON_ERROR(W5100S_setSLCR(hsl_w5100s->io,&slcr),err,TAG,"cannot write SLCR");

err:
    return ret;
}

static esp_err_t hwss_hsl_w5100s_send_arp(hwss_hsl_t *hsl){
    esp_err_t ret=ESP_OK;
    hwss_hsl_w5100_t* hsl_w5100s=__containerof(hsl,hwss_hsl_w5100_t,super);
    uint8_t slcr=0;

    while(1){
        ESP_GOTO_ON_ERROR(W5100S_getSLCR(hsl_w5100s->io,&slcr),err,TAG,"cannot read SLCR");
        if(slcr&(W5100S_SLCMD_ARP|W5100S_SLCMD_PING))
            continue;
        else
            break;
    }

    slcr=W5100S_SLCMD_ARP;
    ESP_GOTO_ON_ERROR(W5100S_setSLCR(hsl_w5100s->io,&slcr),err,TAG,"cannot write SLCR");

err:
    return ret;
}


hwss_hsl_t *hwss_hsl_new_w5100s(hwss_io_t *io, const hwss_hsl_config_t *config){
    hwss_hsl_t *ret=NULL;
    hwss_hsl_w5100_t *hsl=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");

    hsl=calloc(1,sizeof(hwss_hsl_w5100_t));
    ESP_GOTO_ON_FALSE(hsl,NULL,err,TAG,"calloc hsl failed!");

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

    hsl->io=io;
    hsl->retry_time_tick=config->retry_time_ms*10;
    hsl->retry_cnt=config->retry_cnt;

    return &hsl->super;
err:
    if(hsl)
        free(hsl);
    return ret;
}
