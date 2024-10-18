#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_ch394.h"
#include "hwss_mac_wch.h"

static const char *TAG = "ch394.hwss_mac";

typedef struct{
    hwss_mac_t super;

    hwss_io_t *io;
    hwss_eth_mac_addr_t addr;
}hwss_mac_ch394_t;

static esp_err_t hwss_mac_ch394_set_addr(hwss_mac_t *mac, const hwss_eth_mac_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_mac_ch394_t *mac_ch394=__containerof(mac,hwss_mac_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_SetMAC(mac_ch394->io,addr),err,TAG,"cannot write MAC");
    memcpy(mac_ch394->addr,addr,HWSS_ETH_MAC_ADDR_LEN);
    
err:
    return ret;
}

static esp_err_t hwss_mac_ch394_get_addr(hwss_mac_t *mac, hwss_eth_mac_addr_t addr){
    esp_err_t ret=ESP_OK;
    hwss_mac_ch394_t *mac_ch394=__containerof(mac,hwss_mac_ch394_t,super);
    ESP_GOTO_ON_ERROR(CH394_GetMAC(mac_ch394->io,addr),err,TAG,"cannot read MAC");
err:
    return ret;
}

static esp_err_t hwss_mac_ch394_init(hwss_mac_t *mac){
    esp_err_t ret=ESP_OK;
    hwss_mac_ch394_t *mac_ch394=__containerof(mac,hwss_mac_ch394_t,super);
    if(mac_ch394->addr!=NULL)
        ESP_GOTO_ON_ERROR(hwss_mac_ch394_set_addr(mac,mac_ch394->addr),err,TAG,"cannot set MAC address");
err:
    return ret;
}

static esp_err_t hwss_mac_ch394_deinit(hwss_mac_t *mac){
    return ESP_OK;
}

hwss_mac_t *hwss_mac_new_ch394(hwss_io_t *io, const hwss_mac_config_t *config){
    hwss_mac_t *ret=NULL;
    hwss_mac_ch394_t *mac=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");


    if(config->use_burnin_addr){
        /**
         * @warning CH394 has a burn-in MAC Address. DO NOT change it unless necessary.
         *  If you really want to change, uncomment codes below:
         */
        // mac=calloc(1,sizeof(hwss_mac_ch394_t));
        // ESP_GOTO_ON_FALSE(mac,NULL,err,TAG,"calloc mac failed!");
        // memcpy(mac->addr,config->addr,HWSS_ETH_MAC_ADDR_LEN);

        ESP_LOGW(TAG,"CH394 has a burn-in MAC Address. DO NOT change it unless necessary.");
        ESP_LOGW(TAG,"If you really need, look at %s(%d)" ,__FUNCTION__, __LINE__);
    }

    mac->io = io;
    mac->super.set_addr=hwss_mac_ch394_set_addr;
    mac->super.get_addr=hwss_mac_ch394_get_addr;

    mac->super.init=hwss_mac_ch394_init;
    mac->super.deinit=hwss_mac_ch394_deinit;

    return &mac->super;

err:
    if(mac)
        free(mac);
    return ret;
}
