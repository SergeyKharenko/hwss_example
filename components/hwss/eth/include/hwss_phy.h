#pragma once
#include "esp_err.h"
#include "hwss_eth_sku.h"
#include "hwss_eth_type.h"
#include "hwss_event.h"
#include "hwss_io.h"

typedef struct{
    uint32_t reset_timeout_ms;    /*!< Reset timeout value (Unit: ms) */
    uint32_t check_period_ms;
} hwss_phy_config_t;

typedef struct hwss_phy_s hwss_phy_t;

struct hwss_phy_s {
    bool is_started;
    
    esp_err_t (*reset)(hwss_phy_t *phy);
    esp_err_t (*init)(hwss_phy_t *phy);
    esp_err_t (*deinit)(hwss_phy_t *phy);
    esp_err_t (*start)(hwss_phy_t *phy);
    esp_err_t (*stop)(hwss_phy_t *phy);
    esp_err_t (*autonego_ctrl)(hwss_phy_t *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat);
    esp_err_t (*set_link)(hwss_phy_t *phy, hwss_link_t link);
    esp_err_t (*get_link)(hwss_phy_t *phy, hwss_link_t *link);
    esp_err_t (*set_speed)(hwss_phy_t *phy, hwss_speed_t speed);
    esp_err_t (*get_speed)(hwss_phy_t *phy, hwss_speed_t *speed);
    esp_err_t (*set_duplex)(hwss_phy_t *phy, hwss_duplex_t duplex);
    esp_err_t (*get_duplex)(hwss_phy_t *phy, hwss_duplex_t *duplex);
};

static inline esp_err_t hwss_phy_reset(hwss_phy_t *phy){
    return phy->reset(phy);
}

static inline esp_err_t hwss_phy_init(hwss_phy_t *phy){
    return phy->init(phy);
}

static inline esp_err_t hwss_phy_deinit(hwss_phy_t *phy){
    return phy->deinit(phy);
}

static inline esp_err_t hwss_phy_start(hwss_phy_t *phy){
    return phy->start(phy);
}

static inline esp_err_t hwss_phy_stop(hwss_phy_t *phy){
    return phy->stop(phy);
}

static inline esp_err_t hwss_phy_autonego_ctrl(hwss_phy_t *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat){
    return phy->autonego_ctrl(phy,cmd,autonego_en_stat);
}

static inline esp_err_t hwss_phy_set_link(hwss_phy_t *phy, hwss_link_t link){
    return phy->set_link(phy,link);
}

static inline esp_err_t hwss_phy_get_link(hwss_phy_t *phy, hwss_link_t *link){
    return phy->get_link(phy,link);
}

static inline esp_err_t hwss_phy_set_speed(hwss_phy_t *phy, hwss_speed_t speed){
    return phy->set_speed(phy,speed);
}

static inline esp_err_t hwss_phy_get_speed(hwss_phy_t *phy, hwss_speed_t *speed){
    return phy->get_speed(phy,speed);
}

static inline esp_err_t hwss_phy_set_duplex(hwss_phy_t *phy, hwss_duplex_t duplex){
    return phy->set_duplex(phy,duplex);
}

static inline esp_err_t hwss_phy_get_duplex(hwss_phy_t *phy, hwss_duplex_t *duplex){
    return phy->get_duplex(phy,duplex);
}

hwss_phy_t *hwss_phy_new(hwss_eth_sku_t sku, esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_phy_config_t *phy_config);
