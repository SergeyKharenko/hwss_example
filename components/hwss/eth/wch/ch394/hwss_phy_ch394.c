#include <stdatomic.h>
#include <machine/endian.h>
#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "drv_ch394.h"
#include "hwss_eth_event.h"
#include "hwss_phy_wch.h"

static const char *TAG = "ch394.hwss_phy";

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
}hwss_phy_ch394_t;

typedef union {
    #if BYTE_ORDER == LITTLE_ENDIAN
    struct {
        uint8_t link: 1;   /*!< Link status */
        uint8_t speed: 1;  /*!< Speed status */
        uint8_t duplex: 1; /*!< Duplex status */
        uint8_t opmode: 3; /*!< Operation mode */
        uint8_t opsel: 1;  /*!< Operation select */
        uint8_t reset: 1;  /*!< Reset, when this bit is '0', PHY will get reset */
    };
    #else
    struct {
        uint8_t speed: 1;  /*!< Speed status */
        uint8_t duplex: 1; /*!< Duplex status */
        uint8_t opmode: 3; /*!< Operation mode */
        uint8_t opsel: 1;  /*!< Operation select */
        uint8_t reset: 1;  /*!< Reset, when this bit is '0', PHY will get reset */
        uint8_t link: 1;   /*!< Link status */
    };
    #endif
    uint8_t val;
} phycfg_reg_t;

typedef enum {
    CH394_OP_MODE_10BT_HALF_AUTO_DIS,
    CH394_OP_MODE_10BT_FULL_AUTO_DIS,
    CH394_OP_MODE_100BT_HALF_AUTO_DIS,
    CH394_OP_MODE_100BT_FULL_AUTO_DIS,
    CH394_OP_MODE_100BT_HALF_AUTO_EN,
    CH394_OP_MODE_NOT_USED,
    CH394_OP_MODE_PWR_DOWN,
    CH394_OP_MODE_ALL_CAPABLE,
} phy_ch394_op_mode_t;

static void hwss_phy_ch394_check_timer_cb(void *args){
    hwss_phy_ch394_t *phy_ch394=(hwss_phy_ch394_t *)args;
    uint8_t stat=0;

    if(CH394_GetPHY_CFG(phy_ch394->io,&stat) !=ESP_OK){
        ESP_LOGE(TAG,"fail to read PHY_CFG");
        return;
    }

    hwss_link_t link_p=atomic_load(&(phy_ch394->link));
    hwss_link_t link;

    if(stat&CH394_PHY_CFG_LNK_ON)
        link=HWSS_LINK_UP;
    else
        link=HWSS_LINK_DOWN;
    atomic_store(&(phy_ch394->link),link);

    if(stat&CH394_PHY_CFG_SPD_100M)
        atomic_store(&(phy_ch394->speed),HWSS_SPEED_100M);
    else
        atomic_store(&(phy_ch394->speed),HWSS_SPEED_10M);

    if(stat&CH394_PHY_CFG_DPX_FULL)
        atomic_store(&(phy_ch394->duplex),HWSS_DUPLEX_FULL);
    else
        atomic_store(&(phy_ch394->duplex),HWSS_DUPLEX_HALF);

    if(link!=link_p){
        if(link==HWSS_LINK_UP){
            if(esp_event_post_to(phy_ch394->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_CONNECT,NULL,0,0)!= ESP_OK)
                ESP_LOGE(TAG,"fail to post event");
            ESP_LOGD(TAG,"Connected");
        }
        else{
            if(esp_event_post_to(phy_ch394->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_DISCONN,NULL,0,0)!= ESP_OK)
                ESP_LOGE(TAG,"fail to post event");
            ESP_LOGD(TAG,"Disconnected");
        }
    }
}

static esp_err_t hwss_phy_ch394_reset(hwss_phy_t *phy){
    esp_err_t ret=ESP_OK;
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    uint8_t cfg=0;

    atomic_store(&(phy_ch394->link),HWSS_LINK_DOWN);

    ESP_GOTO_ON_ERROR(CH394_GetPHY_CFG(phy_ch394->io,&cfg),err,TAG,"fail to read PHY_CFG");
    cfg &= CH394_PHY_CFG_RST;
    ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&cfg),err,TAG,"fail to write PHY_CFG");
    
    vTaskDelay(pdMS_TO_TICKS(phy_ch394->reset_timeout_ms));

    ESP_GOTO_ON_ERROR(CH394_GetPHY_CFG(phy_ch394->io,&cfg),err,TAG,"fail to read PHY_CFG");
    cfg |= ~CH394_PHY_CFG_RST;
    ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&cfg),err,TAG,"fail to write PHY_CFG");

err:
    return ret;
}

static esp_err_t hwss_phy_ch394_init(hwss_phy_t *phy){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    
    atomic_store(&(phy_ch394->link),HWSS_LINK_DOWN);
    return ESP_OK;
}

static esp_err_t hwss_phy_ch394_deinit(hwss_phy_t *phy){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    if(!phy->is_started)
        return ESP_OK;
    phy->is_started=false;
    return esp_timer_stop(phy_ch394->check_timer);
}

static esp_err_t hwss_phy_ch394_start(hwss_phy_t *phy){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    if(phy->is_started)
        return ESP_OK;
    phy->is_started=true;
    return esp_timer_start_periodic(phy_ch394->check_timer, phy_ch394->check_period_ms*1000);
}

static esp_err_t hwss_phy_ch394_stop(hwss_phy_t *phy){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    if(!phy->is_started)
        return ESP_OK;
    phy->is_started=false;
    return esp_timer_stop(phy_ch394->check_timer);
}

static esp_err_t hwss_phy_ch394_autonego_ctrl(hwss_phy_t *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat){
    esp_err_t ret = ESP_OK;
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    phycfg_reg_t stat;

    ESP_GOTO_ON_ERROR(CH394_GetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to read PHY_CFG");

    switch (cmd) {
    case HWSS_PHY_AUTONEGO_RESTART:
        ESP_GOTO_ON_FALSE(stat.opmode == CH394_OP_MODE_ALL_CAPABLE || stat.opmode == CH394_OP_MODE_100BT_HALF_AUTO_EN,
                            ESP_ERR_INVALID_STATE, err, TAG, "auto negotiation is disabled");
        /* in case any link status has changed, let's assume we're in link down status */
        atomic_store(&(phy_ch394->link),HWSS_LINK_DOWN);

        stat.val&=CH394_PHY_CFG_RST;
        ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to write PHY_CFG");

        vTaskDelay(pdMS_TO_TICKS(phy_ch394->reset_timeout_ms));

        stat.val|=~CH394_PHY_CFG_RST;
        ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to write PHY_CFG");

        *autonego_en_stat = stat.opmode == CH394_OP_MODE_ALL_CAPABLE || stat.opmode == CH394_OP_MODE_100BT_HALF_AUTO_EN;
        break;

    case HWSS_PHY_AUTONEGO_DIS:
        /* CH394 autonegotiation cannot be separately disabled, only specific speed/duplex mode needs to be configured. Hence set the
        last used configuration */
        if (stat.val&CH394_PHY_CFG_DPX_FULL) { // Full duplex
            if (stat.val&CH394_PHY_CFG_SPD_100M) { // 100 Mbps speed
                stat.opmode = CH394_OP_MODE_100BT_FULL_AUTO_DIS;
            } else {
                stat.opmode = CH394_OP_MODE_10BT_FULL_AUTO_DIS;
            }
        } else {
            if (stat.val&CH394_PHY_CFG_SPD_100M) { // 100 Mbps speed
                stat.opmode = CH394_OP_MODE_100BT_HALF_AUTO_DIS;
            } else {
                stat.opmode = CH394_OP_MODE_10BT_HALF_AUTO_DIS;
            }
        }
        stat.val|=CH394_PHY_CFG_OPMD;
        stat.val&=CH394_PHY_CFG_RST;
        ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)), err, TAG, "write PHY_CFG failed");

        vTaskDelay(pdMS_TO_TICKS(phy_ch394->reset_timeout_ms));

        stat.val|=~CH394_PHY_CFG_RST;
        ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)), err, TAG, "write PHY_CFG failed");

        *autonego_en_stat = false;
        break;

    case HWSS_PHY_AUTONEGO_EN:
        stat.val|=CH394_PHY_CFG_OPMD;
        stat.val&=CH394_PHY_CFG_RST;
        stat.opmode = CH394_OP_MODE_ALL_CAPABLE; // all capable, auto-negotiation enabled
        ESP_GOTO_ON_ERROR(CH394_setPHYCFGR(phy_ch394->io,&(stat.val)), err, TAG, "write PHY_CFG failed");

        vTaskDelay(pdMS_TO_TICKS(phy_ch394->reset_timeout_ms));

        stat.val|=~CH394_PHY_CFG_RST;
        ESP_GOTO_ON_ERROR(CH394_setPHYCFGR(phy_ch394->io,&(stat.val)), err, TAG, "write PHY_CFG failed");

        *autonego_en_stat = true;
        break;

    case HWSS_PHY_AUTONEGO_G_STAT:
        *autonego_en_stat = stat.opmode == CH394_OP_MODE_ALL_CAPABLE || stat.opmode == CH394_OP_MODE_100BT_HALF_AUTO_EN;
        break;

    default:
        return ESP_ERR_INVALID_ARG;
    }

err:
    return ret;
}

static esp_err_t hwss_phy_ch394_set_link(hwss_phy_t *phy, hwss_link_t link){
    esp_err_t ret=ESP_OK;
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);

    hwss_link_t link_p=atomic_load(&(phy_ch394->link));
    if(link!=link_p){
        atomic_store(&(phy_ch394->link),link);
        if(link==HWSS_LINK_UP)
            ESP_GOTO_ON_ERROR(esp_event_post_to(phy_ch394->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_CONNECT,NULL,0,0),
                            err,TAG,"fail to post event");
        else
            ESP_GOTO_ON_ERROR(esp_event_post_to(phy_ch394->elp_hdl,HWSS_PHY_EVENT,HWSS_PHY_EVENT_DISCONN,NULL,0,0),
                            err,TAG,"fail to post event");
    }

err:    
    return ret;
}

static esp_err_t hwss_phy_ch394_get_link(hwss_phy_t *phy, hwss_link_t *link){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);

    *link=atomic_load(&(phy_ch394->link));
    return ESP_OK;
}

static esp_err_t hwss_phy_ch394_set_speed(hwss_phy_t *phy, hwss_speed_t speed){
    esp_err_t ret = ESP_OK;
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    phycfg_reg_t stat;

    atomic_store(&(phy_ch394->link),HWSS_LINK_DOWN);

    ESP_GOTO_ON_ERROR(CH394_GetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to read PHY_CFG");
    if(stat.val&CH394_PHY_CFG_DPX_FULL){
        if(speed==HWSS_SPEED_100M)
            stat.opmode=CH394_OP_MODE_100BT_FULL_AUTO_DIS;
        else
            stat.opmode=CH394_OP_MODE_10BT_FULL_AUTO_DIS;
    }
    else{
        if(speed==HWSS_SPEED_100M)
            stat.opmode=CH394_OP_MODE_100BT_HALF_AUTO_DIS;
        else
            stat.opmode=CH394_OP_MODE_10BT_HALF_AUTO_DIS;
    }

    stat.val|=CH394_PHY_CFG_OPMD;
    stat.val&=CH394_PHY_CFG_RST;
    ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to write PHY_CFG");
    vTaskDelay(pdMS_TO_TICKS(phy_ch394->reset_timeout_ms));

    stat.val|=~CH394_PHYCFGR_RST;
    ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to write PHY_CFG");

    atomic_store(&(phy_ch394->speed),speed);
err:
    return ret;
}

static esp_err_t hwss_phy_ch394_get_speed(hwss_phy_t *phy, hwss_speed_t *speed){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);

    *speed=atomic_load(&(phy_ch394->speed));
    return ESP_OK;
}

static esp_err_t hwss_phy_ch394_set_duplex(hwss_phy_t *phy, hwss_duplex_t duplex){
    esp_err_t ret = ESP_OK;
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);
    phycfg_reg_t stat;

    atomic_store(&(phy_ch394->link),HWSS_LINK_DOWN);

    ESP_GOTO_ON_ERROR(CH394_GetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to read PHY_CFG");
    if(stat.val&CH394_PHY_CFG_SPD_100M){
        if(duplex==HWSS_DUPLEX_FULL)
            stat.opmode=CH394_OP_MODE_100BT_FULL_AUTO_DIS;
        else
            stat.opmode=CH394_OP_MODE_100BT_HALF_AUTO_DIS;
    }
    else{
        if(duplex==HWSS_DUPLEX_FULL)
            stat.opmode=CH394_OP_MODE_10BT_FULL_AUTO_DIS;
        else
            stat.opmode=CH394_OP_MODE_10BT_HALF_AUTO_DIS;
    }

    stat.val|=CH394_PHY_CFG_OPMD;
    stat.val&=CH394_PHY_CFG_RST;
    ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to write PHY_CFG");
    vTaskDelay(pdMS_TO_TICKS(phy_ch394->reset_timeout_ms));

    stat.val|=~CH394_PHY_CFG_RST;
    ESP_GOTO_ON_ERROR(CH394_SetPHY_CFG(phy_ch394->io,&(stat.val)),err,TAG,"fail to write PHY_CFG");

    atomic_store(&(phy_ch394->duplex),duplex);
err:
    return ret;
}


static esp_err_t hwss_phy_ch394_get_duplex(hwss_phy_t *phy, hwss_duplex_t *duplex){
    hwss_phy_ch394_t *phy_ch394=__containerof(phy,hwss_phy_ch394_t,super);

    *duplex=atomic_load(&(phy_ch394->duplex));
    return ESP_OK;
}

hwss_phy_t *hwss_phy_new_ch394(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_phy_config_t *phy_config){
    hwss_phy_t *ret=NULL;
    hwss_phy_ch394_t* phy=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(phy_config,NULL,err,TAG,"cannot set config to null");

    phy=calloc(1,sizeof(hwss_phy_ch394_t));
    ESP_GOTO_ON_FALSE(phy,NULL,err,TAG,"calloc phy failed!");

    ESP_GOTO_ON_FALSE(phy_config->reset_timeout_ms>0 && phy_config->check_period_ms >0,
                        NULL,err,TAG,"invalid config args");
    if(phy_config->check_period_ms<5)
        ESP_LOGW(TAG,"check period too short might SERIOUSLY affects the execution of other tasks!");
    
    phy->super.is_started=false;
    phy->super.reset=hwss_phy_ch394_reset;
    phy->super.init=hwss_phy_ch394_init;
    phy->super.deinit=hwss_phy_ch394_deinit;
    phy->super.start=hwss_phy_ch394_start;
    phy->super.stop=hwss_phy_ch394_stop;
    phy->super.autonego_ctrl=hwss_phy_ch394_autonego_ctrl;
    phy->super.set_link=hwss_phy_ch394_set_link;
    phy->super.get_link=hwss_phy_ch394_get_link;
    phy->super.set_speed=hwss_phy_ch394_set_speed;
    phy->super.get_speed=hwss_phy_ch394_get_speed;
    phy->super.set_duplex=hwss_phy_ch394_set_duplex;
    phy->super.get_duplex=hwss_phy_ch394_get_duplex;

    phy->io=io;
    phy->elp_hdl=elp_hdl;
    phy->check_period_ms=phy_config->check_period_ms;
    phy->reset_timeout_ms=phy_config->reset_timeout_ms;

    esp_timer_create_args_t timer_arg={
        .name="hwss_phy_ch394_check_timer",
        .callback=hwss_phy_ch394_check_timer_cb,
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