#include <cstddef>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_mac_wiznet.h"

static const char *TAG = "w5500.hwss_mac";

typedef struct{
    hwss_mac_t super;
    uint8_t *addr;
}hwss_mac_w5500_t;

static esp_err_t hwss_mac_w5500_set_addr(hwss_mac_t *mac, const uint8_t *addr){
    esp_err_t ret=ESP_OK;
    hwss_mac_w5500_t *mac_w5500=__containerof(mac,hwss_mac_w5500_t,super);

    ESP_GOTO_ON_ERROR(setSHAR(mac->io,addr),err,TAG,"cannot write SHAR");
    memcpy(mac_w5500->addr,addr,HWSS_MAC_ADDR_LEN);
err:
    return ret;
}

static esp_err_t hwss_mac_w5500_get_addr(hwss_mac_t *mac, uint8_t *addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getSHAR(mac->io,addr),err,TAG,"cannot read SHAR");
err:
    return ret;
}

static esp_err_t hwss_mac_w5500_init(hwss_mac_t *mac){
    esp_err_t ret=ESP_OK;
    if(mac->addr)
        ESP_GOTO_ON_ERROR(hwss_mac_w5500_set_addr(mac,mac->addr),err,TAG,"cannot set MAC address");
err:
    return ret;
}

static esp_err_t hwss_mac_w5500_deinit(hwss_mac_t *mac){
    return ESP_OK;
}

hwss_mac_t *hwss_mac_new_w5500(hwss_io_t *io, const hwss_mac_config_t *config){
    hwss_mac_t *ret=NULL;
    hwss_mac_w5500_t *mac=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");
    ESP_GOTO_ON_FALSE(config->addr,NULL,err,TAG,"cannot initialized without mac addresss");
    
    mac=calloc(1,sizeof(hwss_mac_w5500_t));
    ESP_GOTO_ON_FALSE(mac,NULL,err,TAG,"calloc mac failed!");

    
    mac->addr=malloc(HWSS_MAC_ADDR_LEN);
    ESP_GOTO_ON_FALSE(mac->addr,NULL,err,TAG,"malloc mac addr failed!");
    memcpy(mac->addr,config->addr,HWSS_MAC_ADDR_LEN);

    mac->super.io = io;
    mac->super.set_addr=hwss_mac_w5500_set_addr;
    mac->super.get_addr=hwss_mac_w5500_get_addr;

    mac->super.init=hwss_mac_w5500_init;
    mac->super.deinit=hwss_mac_w5500_deinit;

err:
    if(mac){
        if(mac->addr)
            free(mac->addr);
        free(mac);
    }
    return ret;
}
