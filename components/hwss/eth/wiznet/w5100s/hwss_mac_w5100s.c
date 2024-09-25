#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5100s.h"
#include "hwss_mac_wiznet.h"

static const char *TAG = "w5100s.hwss_mac";

typedef struct{
    hwss_mac_t super;
    hwss_mac_addr_t addr;
}hwss_mac_w5100s_t;

static esp_err_t hwss_mac_w5100s_set_addr(hwss_mac_t *mac, const hwss_mac_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_mac_w5100s_t *mac_w5100s=__containerof(mac,hwss_mac_w5100s_t,super);
    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(mac->io,false),err,TAG,"cannot unlock NET");
    ESP_GOTO_ON_ERROR(W5100S_setSHAR(mac->io,addr),err,TAG,"cannot write SHAR");
    ESP_GOTO_ON_ERROR(W5100S_setNETLOCK(mac->io,true),err,TAG,"cannot lock NET");
    memcpy(mac_w5100s->addr,addr,HWSS_MAC_ADDR_LEN);
err:
    return ret;
}

static esp_err_t hwss_mac_w5100s_get_addr(hwss_mac_t *mac, hwss_mac_addr_t addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(W5100S_getSHAR(mac->io,addr),err,TAG,"cannot read SHAR");
err:
    return ret;
}

static esp_err_t hwss_mac_w5100s_init(hwss_mac_t *mac){
    esp_err_t ret=ESP_OK;
    hwss_mac_w5100s_t *mac_w5100s=__containerof(mac,hwss_mac_w5100s_t,super);
    ESP_GOTO_ON_ERROR(hwss_mac_w5100s_set_addr(mac,mac_w5100s->addr),err,TAG,"cannot set MAC address");
err:
    return ret;
}

static esp_err_t hwss_mac_w5100s_deinit(hwss_mac_t *mac){
    return ESP_OK;
}

hwss_mac_t *hwss_mac_new_w5100s(hwss_io_t *io, const hwss_mac_config_t *config){
    hwss_mac_t *ret=NULL;
    hwss_mac_w5100s_t *mac=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");
    ESP_GOTO_ON_FALSE(!config->use_burnin_addr,NULL,err,TAG,"cannot W5500 have NO burn in MAC address");
    
    mac=calloc(1,sizeof(hwss_mac_w5100s_t));
    ESP_GOTO_ON_FALSE(mac,NULL,err,TAG,"calloc mac failed!");

    memcpy(mac->addr,config->addr,HWSS_MAC_ADDR_LEN);

    mac->super.io = io;
    mac->super.set_addr=hwss_mac_w5100s_set_addr;
    mac->super.get_addr=hwss_mac_w5100s_get_addr;

    mac->super.init=hwss_mac_w5100s_init;
    mac->super.deinit=hwss_mac_w5100s_deinit;

    return &mac->super;

err:
    if(mac)
        free(mac);
    return ret;
}
