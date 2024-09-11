#include <cstddef>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_hip_wiznet.h"

static const char *TAG = "w5500.hwss_hip";

typedef struct{
    hwss_hip_t super;
    uint16_t retry_tick;
    uint8_t retry_cnt;
}hwss_hip_w5500_t;

static esp_err_t hwss_hip_w5500_init(hwss_hip_t *hip){
    esp_err_t ret=ESP_OK;
    hwss_hip_w5500_t *hip_w5500=__containerof(hip,hwss_hip_w5500_t,super);

    ESP_GOTO_ON_ERROR(setRTR(hip->io, &(hip_w5500->retry_tick)),err,TAG,"cannot write RTR");
    ESP_GOTO_ON_ERROR(setRCR(hip->io, &(hip_w5500->retry_cnt)),err,TAG,"cannot write RCR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_deinit(hwss_hip_t *hip){
    return ESP_OK;
}

static esp_err_t hwss_hip_w5500_set_gateway_addr(hwss_hip_t *hip, const uint8_t *addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(setGAR(hip->io, addr),err,TAG,"cannot write GAR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_gateway_addr(hwss_hip_t *hip, uint8_t *addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getGAR(hip->io, addr),err,TAG,"cannot read GAR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_set_subnet_mask(hwss_hip_t *hip, const uint8_t *mask){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(setSUBR(hip->io, mask),err,TAG,"cannot write SUBR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_subnet_mask(hwss_hip_t *hip, uint8_t *mask){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getSUBR(hip->io, mask),err,TAG,"cannot read SUBR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_set_source_addr(hwss_hip_t *hip, const uint8_t *addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(setSIPR(hip->io, addr),err,TAG,"cannot write SIPR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_source_addr(hwss_hip_t *hip, uint8_t *addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getSIPR(hip->io, addr),err,TAG,"cannot read SIPR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_set_retry_time(hwss_hip_t *hip, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hip_w5500_t *hip_w5500=__containerof(hip,hwss_hip_w5500_t,super);
    uint16_t tick=*ms*10;

    ESP_GOTO_ON_ERROR(setRTR(hip->io, &tick),err,TAG,"cannot write RTR");
    hip_w5500->retry_tick=tick;
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_retry_time(hwss_hip_t *hip, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    uint16_t tick=0;

    ESP_GOTO_ON_ERROR(getRTR(hip->io, &tick),err,TAG,"cannot read RTR");
    *ms=tick/10;
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_set_retry_cnt(hwss_hip_t *hip, const uint8_t *cnt){
    esp_err_t ret=ESP_OK;
    hwss_hip_w5500_t *hip_w5500=__containerof(hip,hwss_hip_w5500_t,super);

    ESP_GOTO_ON_ERROR(setRCR(hip->io, cnt),err,TAG,"cannot write RCR");
    hip_w5500->retry_cnt=*cnt;
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_retry_cnt(hwss_hip_t *hip, uint8_t *cnt){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(getRCR(hip->io, cnt),err,TAG,"cannot read RCR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_unreachable_addr(hwss_hip_t *hip, uint8_t *addr){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(getUIPR(hip->io, addr),err,TAG,"cannot read UIPR");
err:
    return ret;
}

static esp_err_t hwss_hip_w5500_get_unreachable_port(hwss_hip_t *hip, uint16_t *port){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(getUPORTR(hip->io, port),err,TAG,"cannot read UPORTR");
err:
    return ret;
}

hwss_hip_t *hwss_hip_new_w5500(hwss_io_t *io, hwss_hip_config_t *config){
    hwss_hip_t *ret=NULL;
    hwss_hip_w5500_t* hip=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");

    hip=calloc(1,sizeof(hwss_hip_w5500_t));
    ESP_GOTO_ON_FALSE(hip,NULL,err,TAG,"calloc hip failed!");

    hip->super.io=io;
    hip->super.init=hwss_hip_w5500_init;
    hip->super.deinit=hwss_hip_w5500_deinit;
    hip->super.set_gateway_addr=hwss_hip_w5500_set_gateway_addr;
    hip->super.get_gateway_addr=hwss_hip_w5500_get_gateway_addr;
    hip->super.set_subnet_mask=hwss_hip_w5500_set_subnet_mask;
    hip->super.get_subnet_mask=hwss_hip_w5500_get_subnet_mask;
    hip->super.set_source_addr=hwss_hip_w5500_set_source_addr;
    hip->super.get_source_addr=hwss_hip_w5500_get_source_addr;
    hip->super.set_retry_time=hwss_hip_w5500_set_retry_time;
    hip->super.get_retry_time=hwss_hip_w5500_get_retry_time;
    hip->super.set_retry_cnt=hwss_hip_w5500_set_retry_cnt;
    hip->super.get_retry_cnt=hwss_hip_w5500_get_retry_cnt;
    hip->super.get_unreachable_addr=hwss_hip_w5500_get_unreachable_addr;
    hip->super.get_unreachable_port=hwss_hip_w5500_get_unreachable_port;

    hip->retry_tick=config->retry_time_ms*10;
    hip->retry_cnt=config->retry_cnt;
    return hip->super;
err:
    if(hip)
        free(hip);
    return ret;
}

