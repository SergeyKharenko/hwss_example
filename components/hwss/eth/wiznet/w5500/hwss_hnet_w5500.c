#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_hnet_wiznet.h"

#define CP_REQ_MS2TICK(ms)              ms/25
#define CP_REQ_TICK2MS(tick)            tick*25

static const char *TAG = "w5500.hwss_hnet";

typedef struct{
    hwss_hnet_t super;

    hwss_io_t   *io;

    uint16_t    retry_tick;
    uint8_t     retry_cnt;
}hwss_hnet_w5500_t;

static esp_err_t hwss_hnet_w5500_init(hwss_hnet_t *hnet){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_setRTR(hnet_w5500->io, &(hnet_w5500->retry_tick)),err,TAG,"cannot write RTR");
    ESP_GOTO_ON_ERROR(W5500_setRCR(hnet_w5500->io, &(hnet_w5500->retry_cnt)),err,TAG,"cannot write RCR");

err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_deinit(hwss_hnet_t *hnet){
    return ESP_OK;
}

static esp_err_t hwss_hnet_w5500_set_gateway_addr(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_setGAR(hnet_w5500->io, addr),err,TAG,"cannot write GAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_gateway_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_getGAR(hnet_w5500->io, addr),err,TAG,"cannot read GAR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_subnet_mask(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t mask){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_setSUBR(hnet_w5500->io, mask),err,TAG,"cannot write SUBR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_subnet_mask(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t mask){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_getSUBR(hnet_w5500->io, mask),err,TAG,"cannot read SUBR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_source_addr(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_setSIPR(hnet_w5500->io, addr),err,TAG,"cannot write SIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_source_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_getSIPR(hnet_w5500->io, addr),err,TAG,"cannot read SIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_retry_time(hwss_hnet_t *hnet, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    uint16_t tick=*ms*10;
    hnet_w5500->retry_tick=tick;

    ESP_GOTO_ON_ERROR(W5500_setRTR(hnet_w5500->io, &tick),err,TAG,"cannot write RTR");
    
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_retry_time(hwss_hnet_t *hnet, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);
    uint16_t tick=0;
    ESP_GOTO_ON_ERROR(W5500_getRTR(hnet_w5500->io, &tick),err,TAG,"cannot read RTR");
    *ms=tick/10;
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_set_retry_cnt(hwss_hnet_t *hnet, const uint8_t *cnt){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    hnet_w5500->retry_cnt=*cnt;
    ESP_GOTO_ON_ERROR(W5500_setRCR(hnet_w5500->io, cnt),err,TAG,"cannot write RCR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_retry_cnt(hwss_hnet_t *hnet, uint8_t *cnt){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_getRCR(hnet_w5500->io, cnt),err,TAG,"cannot read RCR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_unreachable_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_getUIPR(hnet_w5500->io, addr),err,TAG,"cannot read UIPR");
err:
    return ret;
}

static esp_err_t hwss_hnet_w5500_get_unreachable_port(hwss_hnet_t *hnet, hwss_eth_port_t *port){
    esp_err_t ret=ESP_OK;
    hwss_hnet_w5500_t *hnet_w5500=__containerof(hnet,hwss_hnet_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_getUPORTR(hnet_w5500->io, port),err,TAG,"cannot read UPORTR");
err:
    return ret;
}

hwss_hnet_t *hwss_hnet_new_w5500(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hnet_config_t *config){
    hwss_hnet_t *ret=NULL;
    hwss_hnet_w5500_t* hnet=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");

    hnet=calloc(1,sizeof(hwss_hnet_w5500_t));
    ESP_GOTO_ON_FALSE(hnet,NULL,err,TAG,"calloc hnet failed!");

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

    hnet->io=io;
    hnet->retry_tick=config->retry_time_ms*10;
    hnet->retry_cnt=config->retry_cnt;

    return &hnet->super;
err:
    if(hnet)
        free(hnet);
    return ret;
}

