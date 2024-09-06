#pragma once
#include "esp_err.h"
#include "hwss_type.h"
#include "hwss_io.h"

typedef struct{
    uint32_t reset_timeout_ms;    /*!< Reset timeout value (Unit: ms) */
    uint32_t autonego_timeout_ms; /*!< Auto-negotiation timeout value (Unit: ms) */
}hwss_phy_config_t;

typedef struct{
    hwss_io_t *io;
    
    esp_err_t (*reset)(void *phy);
    esp_err_t (*init)(void *phy);
    esp_err_t (*deinit)(void *phy);
    esp_err_t (*autonego_ctrl)(void *phy, hwss_phy_autoneg_cmd_t cmd, bool *autonego_en_stat);
    esp_err_t (*set_link)(void *phy, hwss_link_t link);
    esp_err_t (*get_link)(void *phy, hwss_link_t *link);
    esp_err_t (*set_speed)(void *phy, hwss_speed_t speed);
    esp_err_t (*get_speed)(void *phy, hwss_speed_t *speed);
    esp_err_t (*set_duplex)(void *phy, hwss_duplex_t duplex);
    esp_err_t (*get_duplex)(void *phy, hwss_duplex_t *duplex);
}hwss_phy_t;

