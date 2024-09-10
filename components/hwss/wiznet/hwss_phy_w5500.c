#include <stdatomic.h>
#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "hwss_phy_wiznet.h"

static const char *TAG = "w5500.hwss_phy";

typedef struct{
    hwss_phy_t super;
    _Atomic hwss_link_t link;
    _Atomic hwss_speed_t speed;
    _Atomic hwss_duplex_t duplex;

    uint32_t check_period_ms;
    esp_timer_handle_t check_timer;
}hwss_phy_w5500_t;

static inline esp_t 

esp_err_t hwss_phy_w5500_reset(void *phy){
    hwss_phy_t *hwss_phy=(hwss_phy_t *)phy;



}

esp_err_t hwss_phy_w5500_init(void *phy){

}

esp_err_t hwss_phy_w5500_deinit(void *phy){

}

esp_err_t hwss_phy_w5500_autonego_ctrl)(void *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat);
esp_err_t hwss_phy_w5500_set_link)(void *phy, hwss_link_t link);
esp_err_t hwss_phy_w5500_get_link)(void *phy, hwss_link_t *link);
esp_err_t hwss_phy_w5500_set_speed)(void *phy, hwss_speed_t speed);
esp_err_t hwss_phy_w5500_get_speed)(void *phy, hwss_speed_t *speed);
esp_err_t hwss_phy_w5500_set_duplex)(void *phy, hwss_duplex_t duplex);
esp_err_t hwss_phy_w5500_get_duplex)(void *phy, hwss_duplex_t *duplex);




hwss_phy_t *hwss_phy_new_w5500(const hwss_io_t *io, const hwss_phy_config_t *phy_config){
    hwss_phy_t *ret=NULL;
    hwss_phy_w5500_t* phy=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(phy_config,NULL,err,TAG,"cannot set config to null");

    phy=calloc(1,sizeof(hwss_phy_w5500_t));
    ESP_GOTO_ON_FALSE(phy,NULL,err,TAG,"calloc phy failed!");

    ESP_GOTO_ON_FALSE(phy_config->autonego_timeout_ms>0 && phy_config->reset_timeout_ms>0);

    phy->super.io=io;
    


err:
    if(phy)
        free(phy);
    return ret;
}