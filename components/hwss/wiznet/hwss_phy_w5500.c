#include <stdatomic.h>
#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "drv_w5500.h"
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


static void hwss_phy_check_timer_cb(void *args){
    hwss_phy_w5500_t *phy_w5500=(hwss_phy_w5500_t *)args;
    uint8_t stat=0;

    getPHYCFGR(phy_w5500->super.io,&stat);

    hwss_link_t link_p=atomic_load(&(phy_w5500->link));
    hwss_link_t link;

    if(stat&PHYCFGR_LNK_ON)
        link=HWSS_LINK_UP;
    else
        link=HWSS_LINK_DOWN;
    atomic_store(&(phy_w5500->link),link);

    if(link!=link_p){
        if(link==HWSS_LINK_UP)
            esp_event_post(HWSS_EVENT,HWSS_EVENT_CONNECTED,(void *)&phy_w5500->super,sizeof(hwss_phy_t),0);
        else
            esp_event_post(HWSS_EVENT,HWSS_EVENT_DISCONNECTED,(void *)&phy_w5500->super,sizeof(hwss_phy_t),0);
    }

    if(stat&PHYCFGR_SPD_100)
        atomic_store(&(phy_w5500->speed),HWSS_SPEED_100M);
    else
        atomic_store(&(phy_w5500->speed),HWSS_SPEED_10M);

    if(stat&PHYCFGR_DPX_FULL)
        atomic_store(&(phy_w5500->duplex),HWSS_DUPLEX_FULL);
    else
        atomic_store(&(phy_w5500->duplex),HWSS_DUPLEX_HALF);
}

esp_err_t hwss_phy_w5500_reset(hwss_phy_t *phy){
    esp_err_t ret=ESP_OK;
    uint8_t cfg=0;

    ESP_GOTO_ON_ERROR(getPHYCFGR(phy->io,&cfg),err,TAG,"fail to read PHYCFGR");
    cfg &= PHYCFGR_RST;
    ESP_GOTO_ON_ERROR(setPHYCFGR(phy->io,&cfg),err,TAG,"fail to write PHYCFGR");
    
    ESP_GOTO_ON_ERROR(getPHYCFGR(phy->io,&cfg),err,TAG,"fail to read PHYCFGR");
    cfg |= PHYCFGR_RST;
    ESP_GOTO_ON_ERROR(setPHYCFGR(phy->io,&cfg),err,TAG,"fail to write PHYCFGR");

err:
    return ret;
}

esp_err_t hwss_phy_w5500_init(hwss_phy_t *phy){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);

    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(phy_w5500->check_timer, phy_w5500->check_period_ms*1000),
                        err,TAG,"cannot start timer");
    uint8_t cfg=0;
    ESP_GOTO_ON_ERROR(getPHYCFGR(phy->io,&cfg),err,TAG,"fail to read PHYCFGR");
    cfg |= PHYCFGR_OPMD;
    ESP_GOTO_ON_ERROR(setPHYCFGR(phy->io,&cfg),err,TAG,"fail to write PHYCFGR");

err:
    return ret;
}

esp_err_t hwss_phy_w5500_deinit(hwss_phy_t *phy){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);
    ESP_GOTO_ON_ERROR(esp_timer_stop(phy_w5500->check_timer),err,TAG,"cannot stop timer");

err:
    return ret;
}

esp_err_t hwss_phy_w5500_autonego_ctrl(hwss_phy_t *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat){
    esp_err_t ret = ESP_OK;


err:
    return ret;
}

esp_err_t hwss_phy_w5500_set_link(hwss_phy_t *phy, hwss_link_t link){
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);

    hwss_link_t link_p=atomic_load(&(phy_w5500->link));
    if(link!=link_p){
        atomic_store(&(phy_w5500->link),link);
        if(link==HWSS_LINK_UP)
            esp_event_post(HWSS_EVENT,HWSS_EVENT_CONNECTED,(void *)&phy_w5500->super,sizeof(hwss_phy_t),0);
        else
            esp_event_post(HWSS_EVENT,HWSS_EVENT_DISCONNECTED,(void *)&phy_w5500->super,sizeof(hwss_phy_t),0);
    }
    return ESP_OK;
}

esp_err_t hwss_phy_w5500_get_link(hwss_phy_t *phy, hwss_link_t *link){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);

    *link=atomic_load(&(phy_w5500->link));
    return ESP_OK;
}

esp_err_t hwss_phy_w5500_set_speed(hwss_phy_t *phy, hwss_speed_t speed){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);

    atomic_store(&(phy_w5500->link),HWSS_LINK_DOWN);

    uint8_t stat=0;
    getPHYCFGR(phy->io,&stat);
    if(stat&PHYCFGR_DPX_FULL){
        if(speed==HWSS_SPEED_100M)
            stat
    }
    else{

    }

}

esp_err_t hwss_phy_w5500_get_speed(hwss_phy_t *phy, hwss_speed_t *speed){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);

    *speed=atomic_load(&(phy_w5500->speed));
    return ESP_OK;
}

esp_err_t hwss_phy_w5500_set_duplex)(hwss_phy_t *phy, hwss_duplex_t duplex);


esp_err_t hwss_phy_w5500_get_duplex(hwss_phy_t *phy, hwss_duplex_t *duplex){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5500_t *phy_w5500=__containerof(phy,hwss_phy_w5500_t,super);

    *duplex=atomic_load(&(phy_w5500->duplex));
    return ESP_OK;
}

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