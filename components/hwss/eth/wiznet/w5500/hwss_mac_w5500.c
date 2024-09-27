#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_mac_wiznet.h"

static const char *TAG = "w5500.hwss_mac";

typedef struct{
    hwss_mac_t super;

    hwss_io_t *io;
    hwss_eth_mac_addr_t addr;
}hwss_mac_w5500_t;

static esp_err_t hwss_mac_w5500_set_addr(hwss_mac_t *mac, const hwss_eth_mac_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_mac_w5500_t *mac_w5500=__containerof(mac,hwss_mac_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_setSHAR(mac_w5500->io,addr),err,TAG,"cannot write SHAR");
    memcpy(mac_w5500->addr,addr,HWSS_ETH_MAC_ADDR_LEN);
err:
    return ret;
}

static esp_err_t hwss_mac_w5500_get_addr(hwss_mac_t *mac, hwss_eth_mac_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_mac_w5500_t *mac_w5500=__containerof(mac,hwss_mac_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_getSHAR(mac_w5500->io,addr),err,TAG,"cannot read SHAR");
err:
    return ret;
}

static esp_err_t hwss_mac_w5500_init(hwss_mac_t *mac){
    esp_err_t ret=ESP_OK;
    hwss_mac_w5500_t *mac_w5500=__containerof(mac,hwss_mac_w5500_t,super);
    ESP_GOTO_ON_ERROR(hwss_mac_w5500_set_addr(mac,mac_w5500->addr),err,TAG,"cannot set MAC address");
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
    ESP_GOTO_ON_FALSE(!config->use_burnin_addr,NULL,err,TAG,"cannot W5500 have NO burn in MAC address");
    
    mac=calloc(1,sizeof(hwss_mac_w5500_t));
    ESP_GOTO_ON_FALSE(mac,NULL,err,TAG,"calloc mac failed!");

    memcpy(mac->addr,config->addr,HWSS_ETH_MAC_ADDR_LEN);

    mac->io = io;
    mac->super.set_addr=hwss_mac_w5500_set_addr;
    mac->super.get_addr=hwss_mac_w5500_get_addr;

    mac->super.init=hwss_mac_w5500_init;
    mac->super.deinit=hwss_mac_w5500_deinit;

    return &mac->super;

err:
    if(mac)
        free(mac);
    return ret;
}
