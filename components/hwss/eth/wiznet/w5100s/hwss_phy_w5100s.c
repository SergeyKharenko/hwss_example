#include <stdatomic.h>
#include <sys/cdefs.h>
#include <machine/endian.h>
#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "drv_w5100s.h"
#include "hwss_eth_event.h"
#include "hwss_phy_wiznet.h"

static const char *TAG = "w5100s.hwss_phy";

typedef struct{
    hwss_phy_t super;

    hwss_io_t *io;
    esp_event_loop_handle_t elp_hdl;

    _Atomic hwss_link_t link;
    _Atomic hwss_speed_t speed;
    _Atomic hwss_duplex_t duplex;

    uint32_t check_period_ms;
    esp_timer_handle_t check_timer;

    uint32_t reset_timeout_ms;
    uint32_t autonego_timeout_ms;
}hwss_phy_w5100s_t;

typedef union{
    #if BYTE_ORDER == LITTLE_ENDIAN
    struct{
        uint8_t reset :1;
        uint8_t reserve1 :4;
        uint8_t pwdown :1;
        uint8_t reserve2 :1;
        uint8_t wol :1;
    };
    #else
    struct{
        uint8_t wol :1;
        uint8_t reserve2 :1;
        uint8_t pwdown :1;
        uint8_t reserve1 :4;
        uint8_t reset :1;
    };
    #endif
    uint8_t val;
}phycr1_reg_t;

static void hwss_phy_w5100s_check_timer_cb(void *args){
    hwss_phy_w5100s_t *phy_w5100s=(hwss_phy_w5100s_t *)args;
    uint8_t stat=0;

    if(W5100S_getPHYSR(phy_w5100s->io,&stat) !=ESP_OK){
        ESP_LOGE(TAG,"fail to read PHYSR");
        return;
    }

    // ESP_LOGI(TAG,"PHYSR:%X",stat);

    hwss_link_t link_p=atomic_load(&(phy_w5100s->link));
    hwss_link_t link;

    if(stat&W5100S_PHYSR_LNK)
        link=HWSS_LINK_UP;
    else
        link=HWSS_LINK_DOWN;
    atomic_store(&(phy_w5100s->link),link);

    if(stat&W5100S_PHYSR_SPD)
        atomic_store(&(phy_w5100s->speed),HWSS_SPEED_10M);
    else
        atomic_store(&(phy_w5100s->speed),HWSS_SPEED_100M);

    if(stat&W5100S_PHYSR_DUP)
        atomic_store(&(phy_w5100s->duplex),HWSS_DUPLEX_HALF);
    else
        atomic_store(&(phy_w5100s->duplex),HWSS_DUPLEX_FULL);

    if(link!=link_p){
        if(link==HWSS_LINK_UP){
            if(esp_event_post_to(phy_w5100s->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_CONNECT,NULL,0,0)!= ESP_OK)
                ESP_LOGE(TAG,"fail to post event");
            ESP_LOGD(TAG,"Connected");
        }
        else{
            if(esp_event_post_to(phy_w5100s->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_DISCONN,NULL,0,0)!= ESP_OK)
                ESP_LOGE(TAG,"fail to post event");
            ESP_LOGD(TAG,"Disconnected");
        }
    }
}

static esp_err_t hwss_phy_w5100s_reset(hwss_phy_t *phy){
    esp_err_t ret=ESP_OK;
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    phycr1_reg_t phycr1;

    atomic_store(&(phy_w5100s->link),HWSS_LINK_DOWN);

    ESP_GOTO_ON_ERROR(W5100S_getPHYCR1(phy_w5100s->io,&phycr1.val),err,TAG,"fail to read PHYCR1");
    ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,false),err,TAG,"fail to unlock PHY");
    phycr1.reset=1;
    ESP_GOTO_ON_ERROR(W5100S_setPHYCR1(phy_w5100s->io,&phycr1.val),err,TAG,"fail to write PHYCR1");
    vTaskDelay(pdMS_TO_TICKS(1));
    phycr1.reset=0;
    ESP_GOTO_ON_ERROR(W5100S_setPHYCR1(phy_w5100s->io,&phycr1.val),err,TAG,"fail to write PHYCR1");
    vTaskDelay(pdMS_TO_TICKS(phy_w5100s->reset_timeout_ms));

    ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,true),err,TAG,"fail to lock PHY");

err:
    return ret;
}

static esp_err_t hwss_phy_w5100s_init(hwss_phy_t *phy){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    
    atomic_store(&(phy_w5100s->link),HWSS_LINK_DOWN);
    return ESP_OK;
}

static esp_err_t hwss_phy_w5100s_deinit(hwss_phy_t *phy){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    if(!phy->is_started)
        return ESP_OK;
    phy->is_started=false;
    return esp_timer_stop(phy_w5100s->check_timer);
}

static esp_err_t hwss_phy_w5100s_start(hwss_phy_t *phy){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    if(phy->is_started)
        return ESP_OK;
    phy->is_started=true;
    return esp_timer_start_periodic(phy_w5100s->check_timer, phy_w5100s->check_period_ms*1000);
}

static esp_err_t hwss_phy_w5100s_stop(hwss_phy_t *phy){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    if(!phy->is_started)
        return ESP_OK;
    phy->is_started=false;
    return esp_timer_stop(phy_w5100s->check_timer);
}

static esp_err_t hwss_phy_w5100s_autonego_ctrl(hwss_phy_t *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    uint8_t stat;
    uint8_t mode;

    ESP_GOTO_ON_ERROR(W5100S_getPHYSR(phy_w5100s->io,&stat),err,TAG,"fail to read PHYSR");

    switch (cmd) {
    case HWSS_PHY_AUTONEGO_RESTART:
        ESP_GOTO_ON_FALSE(stat & W5100S_PHYSR_CAUTO, ESP_ERR_INVALID_STATE, err, TAG, 
                        "auto negotiation is disabled");
        /* in case any link status has changed, let's assume we're in link down status */
        phycr1_reg_t phycr1;

        atomic_store(&(phy_w5100s->link),HWSS_LINK_DOWN);

        ESP_GOTO_ON_ERROR(W5100S_getPHYCR1(phy_w5100s->io,&phycr1.val),err,TAG,"fail to read PHYCR1");
        ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,false),err,TAG,"fail to unlock PHY");
        phycr1.reset=1;
        ESP_GOTO_ON_ERROR(W5100S_setPHYCR1(phy_w5100s->io,&phycr1.val),err,TAG,"fail to write PHYCR1");
        vTaskDelay(pdMS_TO_TICKS(1));
        phycr1.reset=0;
        ESP_GOTO_ON_ERROR(W5100S_setPHYCR1(phy_w5100s->io,&phycr1.val),err,TAG,"fail to write PHYCR1");
        vTaskDelay(pdMS_TO_TICKS(phy_w5100s->reset_timeout_ms));

        ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,true),err,TAG,"fail to lock PHY");
        *autonego_en_stat = true;
        break;

    case HWSS_PHY_AUTONEGO_DIS:
        mode=W5100S_PHYCR_AUTONEGO_DISABLE;

        if (stat&W5100S_PHYSR_DUP) // Half duplex
            mode|=W5100S_PHYCR_HALF_DUP;
        else
            mode|=W5100S_PHYCR_FULL_DUP;

        if(stat&W5100S_PHYSR_SPD) // 10 Mbps speed
            mode|=W5100S_PHYCR_SPD_10;
        else
            mode|=W5100S_PHYCR_SPD_100;

        ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,false),err,TAG,"fail to unlock PHY");
        ESP_GOTO_ON_ERROR(W5100S_setPHYCR0(phy_w5100s->io,&mode),err,TAG,"fail to write PHYCR0");
        ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,true),err,TAG,"fail to lock PHY");
        *autonego_en_stat = false;
        break;

    case HWSS_PHY_AUTONEGO_EN:
        mode=W5100S_PHYCR_AUTONEGO_ENABLE;
        ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,false),err,TAG,"fail to unlock PHY");
        ESP_GOTO_ON_ERROR(W5100S_setPHYCR0(phy_w5100s->io,&mode),err,TAG,"fail to write PHYCR0");
        ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,true),err,TAG,"fail to lock PHY");
        *autonego_en_stat = true;
        break;

    case HWSS_PHY_AUTONEGO_G_STAT:
        *autonego_en_stat = stat&W5100S_PHYSR_CAUTO;
        break;

    default:
        return ESP_ERR_INVALID_ARG;
    }

err:
    return ret;
}

static esp_err_t hwss_phy_w5100s_set_link(hwss_phy_t *phy, hwss_link_t link){
    esp_err_t ret=ESP_OK;
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);

    hwss_link_t link_p=atomic_load(&(phy_w5100s->link));
    if(link!=link_p){
        atomic_store(&(phy_w5100s->link),link);
        if(link==HWSS_LINK_UP)
            ESP_GOTO_ON_ERROR(esp_event_post_to(phy_w5100s->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_CONNECT,NULL,0,0),
                            err,TAG,"fail to post event");
        else
            ESP_GOTO_ON_ERROR(esp_event_post_to(phy_w5100s->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_DISCONN,NULL,0,0),
                            err,TAG,"fail to post event");
    }

err:    
    return ret;
}

static esp_err_t hwss_phy_w5100s_get_link(hwss_phy_t *phy, hwss_link_t *link){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);

    *link=atomic_load(&(phy_w5100s->link));
    return ESP_OK;
}

static esp_err_t hwss_phy_w5100s_set_speed(hwss_phy_t *phy, hwss_speed_t speed){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    uint8_t stat;
    uint8_t mode=W5100S_PHYCR_AUTONEGO_DISABLE;

    atomic_store(&(phy_w5100s->link),HWSS_LINK_DOWN);

    ESP_GOTO_ON_ERROR(W5100S_getPHYSR(phy_w5100s->io,&stat),err,TAG,"fail to read PHYSR");
    
    if(speed==HWSS_SPEED_10M)
        mode|=W5100S_PHYCR_SPD_10;
    else
        mode|=W5100S_PHYCR_SPD_100;

    if(stat&W5100S_PHYSR_DUP)
        mode|=W5100S_PHYCR_HALF_DUP;
    else
        mode|=W5100S_PHYCR_FULL_DUP;

    ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,false),err,TAG,"fail to unlock PHY");
    ESP_GOTO_ON_ERROR(W5100S_setPHYCR0(phy_w5100s->io,&mode),err,TAG,"fail to write PHYCR0");
    ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,true),err,TAG,"fail to lock PHY");

    atomic_store(&(phy_w5100s->speed),speed);
err:
    return ret;
}

static esp_err_t hwss_phy_w5100s_get_speed(hwss_phy_t *phy, hwss_speed_t *speed){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);

    *speed=atomic_load(&(phy_w5100s->speed));
    return ESP_OK;
}

static esp_err_t hwss_phy_w5100s_set_duplex(hwss_phy_t *phy, hwss_duplex_t duplex){
    esp_err_t ret = ESP_OK;
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);
    uint8_t stat;
    uint8_t mode=W5100S_PHYCR_AUTONEGO_DISABLE;

    atomic_store(&(phy_w5100s->link),HWSS_LINK_DOWN);

    ESP_GOTO_ON_ERROR(W5100S_getPHYSR(phy_w5100s->io,&stat),err,TAG,"fail to read PHYSR");
    
    if(duplex==HWSS_DUPLEX_FULL)
        mode|=W5100S_PHYCR_FULL_DUP;
    else
        mode|=W5100S_PHYCR_HALF_DUP;

    if(stat&W5100S_PHYSR_SPD)
        mode|=W5100S_PHYCR_SPD_10;
    else
        mode|=W5100S_PHYCR_SPD_100;

    ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,false),err,TAG,"fail to unlock PHY");
    ESP_GOTO_ON_ERROR(W5100S_setPHYCR0(phy_w5100s->io,&mode),err,TAG,"fail to write PHYCR0");
    ESP_GOTO_ON_ERROR(W5100S_setPHYLOCK(phy_w5100s->io,true),err,TAG,"fail to lock PHY");

    atomic_store(&(phy_w5100s->duplex),duplex);
err:
    return ret;
}


static esp_err_t hwss_phy_w5100s_get_duplex(hwss_phy_t *phy, hwss_duplex_t *duplex){
    hwss_phy_w5100s_t *phy_w5100s=__containerof(phy,hwss_phy_w5100s_t,super);

    *duplex=atomic_load(&(phy_w5100s->duplex));
    return ESP_OK;
}

hwss_phy_t *hwss_phy_new_w5100s(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_phy_config_t *phy_config){
    hwss_phy_t *ret=NULL;
    hwss_phy_w5100s_t* phy=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(phy_config,NULL,err,TAG,"cannot set config to null");

    phy=calloc(1,sizeof(hwss_phy_w5100s_t));
    ESP_GOTO_ON_FALSE(phy,NULL,err,TAG,"calloc phy failed!");

    ESP_GOTO_ON_FALSE(phy_config->reset_timeout_ms>0 && phy_config->check_period_ms >0,
                        NULL,err,TAG,"invalid config args");
    if(phy_config->check_period_ms<5)
        ESP_LOGW(TAG,"check period too short might SERIOUSLY affects the execution of other tasks!");
    
    phy->super.is_started=false;
    phy->super.reset=hwss_phy_w5100s_reset;
    phy->super.init=hwss_phy_w5100s_init;
    phy->super.deinit=hwss_phy_w5100s_deinit;
    phy->super.start=hwss_phy_w5100s_start;
    phy->super.stop=hwss_phy_w5100s_stop;
    phy->super.autonego_ctrl=hwss_phy_w5100s_autonego_ctrl;
    phy->super.set_link=hwss_phy_w5100s_set_link;
    phy->super.get_link=hwss_phy_w5100s_get_link;
    phy->super.set_speed=hwss_phy_w5100s_set_speed;
    phy->super.get_speed=hwss_phy_w5100s_get_speed;
    phy->super.set_duplex=hwss_phy_w5100s_set_duplex;
    phy->super.get_duplex=hwss_phy_w5100s_get_duplex;

    phy->io=io;
    phy->elp_hdl=elp_hdl;
    phy->check_period_ms=phy_config->check_period_ms;
    phy->reset_timeout_ms=phy_config->reset_timeout_ms;

    esp_timer_create_args_t timer_arg={
        .name="hwss_phy_w5100s_check_timer",
        .callback=hwss_phy_w5100s_check_timer_cb,
        .arg=phy,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&phy->check_timer)==ESP_OK,NULL,err,TAG,"create check timer failed");

    return &phy->super;
err:
    if(phy)
        free(phy);
    return ret;
}