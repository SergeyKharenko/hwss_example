#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5100s.h"
#include "hwss_hnet_wiznet.h"

#define CP_REQ_MS2TICK(ms)              ms/25
#define CP_REQ_TICK2MS(tick)            tick*25

static const char *TAG = "w5100s.hwss_hnet";

typedef struct{
    hwss_hnet_t super;

    hwss_io_t   *io;

    uint16_t    retry_tick;
    uint8_t     retry_cnt;
}hwss_hnet_w5100s_t;

static esp_err_t hwss_hnet_w5100s_init(hwss_hnet_t *hnet){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setRTR(hnet_w5100s->io, &(hnet_w5100s->retry_tick)),err,TAG,"cannot write RTR");
    ESP_GOTO_ON_ERROR(W5100S_setRCR(hnet_w5100s->io, &(hnet_w5100s->retry_cnt)),err,TAG,"cannot write RCR");

err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_deinit(hwss_hnet_t *hnet){
    return ESP_OK;
}

static esp_err_t hwss_hnet_w5100s_set_gateway_addr(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(hnet_w5100s->io,false),err,TAG,"cannot unlock NET");
    ESP_GOTO_ON_ERROR(W5100S_setGAR(hnet_w5100s->io, addr),err,TAG,"cannot write GAR");
    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(hnet_w5100s->io,true),err,TAG,"cannot lock NET");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_gateway_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getGAR(hnet_w5100s->io, addr),err,TAG,"cannot read GAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_set_subnet_mask(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t mask){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(hnet_w5100s->io,false),err,TAG,"cannot unlock NET");
    ESP_GOTO_ON_ERROR(W5100S_setSUBR(hnet_w5100s->io, mask),err,TAG,"cannot write SUBR");
    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(hnet_w5100s->io,true),err,TAG,"cannot lock NET");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_subnet_mask(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t mask){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getSUBR(hnet_w5100s->io, mask),err,TAG,"cannot read SUBR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_set_source_addr(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(hnet_w5100s->io,false),err,TAG,"cannot unlock NET");
    ESP_GOTO_ON_ERROR(W5100S_setSIPR(hnet_w5100s->io, addr),err,TAG,"cannot write SIPR");
    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(hnet_w5100s->io,true),err,TAG,"cannot lock NET");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_source_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getSIPR(hnet_w5100s->io, addr),err,TAG,"cannot read SIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_set_retry_time(hwss_hnet_t *hnet, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);
    uint16_t tick=*ms*10;

    ESP_GOTO_ON_ERROR(W5100S_setRTR(hnet_w5100s->io, &tick),err,TAG,"cannot write RTR");
    hnet_w5100s->retry_tick=tick;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_retry_time(hwss_hnet_t *hnet, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);
    uint16_t tick=0;

    ESP_GOTO_ON_ERROR(W5100S_getRTR(hnet_w5100s->io, &tick),err,TAG,"cannot read RTR");
    *ms=tick/10;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_set_retry_cnt(hwss_hnet_t *hnet, const uint8_t *cnt){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setRCR(hnet_w5100s->io, cnt),err,TAG,"cannot write RCR");
    hnet_w5100s->retry_cnt=*cnt;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_retry_cnt(hwss_hnet_t *hnet, uint8_t *cnt){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getRCR(hnet_w5100s->io, cnt),err,TAG,"cannot read RCR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_unreachable_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getUIPR(hnet_w5100s->io, addr),err,TAG,"cannot read UIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5100s_get_unreachable_port(hwss_hnet_t *hnet, hwss_eth_port_t *port){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5100s_t *hnet_w5100s=__containerof(hnet,hwss_hnet_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getUPORTR(hnet_w5100s->io, port),err,TAG,"cannot read UPORTR");
err:
    return ret;
}

hwss_hnet_t *hwss_hnet_new_w5100s(hwss_io_t *io, const hwss_hnet_config_t *config){
    hwss_hnet_t *ret=NULL;
    hwss_hnet_w5100s_t* hnet=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");

    hnet=calloc(1,sizeof(hwss_hnet_w5100s_t));
    ESP_GOTO_ON_FALSE(hnet,NULL,err,TAG,"calloc hnet failed!");

    hnet->super.init=hwss_hnet_w5100s_init;
    hnet->super.deinit=hwss_hnet_w5100s_deinit;
    hnet->super.set_gateway_addr=hwss_hnet_w5100s_set_gateway_addr;
    hnet->super.get_gateway_addr=hwss_hnet_w5100s_get_gateway_addr;
    hnet->super.set_subnet_mask=hwss_hnet_w5100s_set_subnet_mask;
    hnet->super.get_subnet_mask=hwss_hnet_w5100s_get_subnet_mask;
    hnet->super.set_source_addr=hwss_hnet_w5100s_set_source_addr;
    hnet->super.get_source_addr=hwss_hnet_w5100s_get_source_addr;
    hnet->super.set_retry_time=hwss_hnet_w5100s_set_retry_time;
    hnet->super.get_retry_time=hwss_hnet_w5100s_get_retry_time;
    hnet->super.set_retry_cnt=hwss_hnet_w5100s_set_retry_cnt;
    hnet->super.get_retry_cnt=hwss_hnet_w5100s_get_retry_cnt;
    hnet->super.get_unreachable_addr=hwss_hnet_w5100s_get_unreachable_addr;
    hnet->super.get_unreachable_port=hwss_hnet_w5100s_get_unreachable_port;

    hnet->io=io;
    hnet->retry_tick=config->retry_time_ms*10;
    hnet->retry_cnt=config->retry_cnt;

    return &hnet->super;
err:
    if(hnet)
        free(hnet);
    return ret;
}

