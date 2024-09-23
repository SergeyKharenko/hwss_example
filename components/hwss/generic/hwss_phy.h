#pragma once
#include "esp_err.h"
#include "hwss_type.h"
#include "hwss_event.h"
#include "hwss_io.h"

typedef struct{
    uint32_t reset_timeout_ms;    /*!< Reset timeout value (Unit: ms) */
    uint32_t check_period_ms;
} hwss_phy_config_t;

typedef struct hwss_phy_s hwss_phy_t;

struct hwss_phy_s {
    hwss_io_t *io;
    esp_event_loop_handle_t elp_hdl;
    
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
