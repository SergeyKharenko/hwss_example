#pragma once
#include "esp_event.h"
#include "hwss_cvr.h"
#include "hwss_eth_config.h"

typedef enum{
    HWSS_ETH_STA_RUNNING,
    HWSS_ETH_STA_READY,
    HWSS_ETH_STA_SLEEP
}hwss_eth_state_t;

typedef struct hwss_eth_s hwss_eth_t;

struct hwss_eth_s{
    hwss_eth_sku_t  sku;
    char name[HWSS_ETH_NAMELEN_MAX];
    hwss_eth_state_t state;
    esp_event_loop_handle_t elp_hdl;

    hwss_io_t   *io;
    hwss_hir_t  *hir;
    hwss_sir_t  *sir;

    hwss_phy_t  *phy;
    hwss_mac_t  *mac;
    hwss_hnet_t *hnet;
    hwss_hso_t  *hso;
    hwss_sscm_t *sscm;

    hwss_lpw_t  *lpw;
    hwss_cvr_t  *cvr;
    hwss_cctl_t *cctl;

    struct{
        hwss_hppe_t *hppe;
        hwss_hsl_t  *hsl;
        hwss_hdc_t  *hdc;
    }opt;
};

hwss_eth_t *hwss_eth_new(const hwss_eth_config_t *config);

esp_err_t hwss_eth_init(hwss_eth_t *eth);
esp_err_t hwss_eth_deinit(hwss_eth_t *eth);

esp_err_t hwss_eth_start(hwss_eth_t *eth);
esp_err_t hwss_eth_stop(hwss_eth_t *eth);

esp_err_t hwss_eth_print_info(const hwss_eth_t *eth);