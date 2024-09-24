#pragma once

#include "hwss_opt.h"

#include "hwss_io.h"
#include "hwss_hir.h"
#include "hwss_lpw.h"
#include "hwss_phy.h"
#include "hwss_mac.h"
#include "hwss_hnet.h"
#include "hwss_hso.h"
#include "hwss_hsl.h"

typedef enum{
    HWSS_ETH_STA_RUNNING,
    HWSS_ETH_STA_READY,
    HWSS_ETH_STA_SLEEP
}hwss_eth_state_t;

typedef struct{

}hwss_eth_config_t;

typedef struct hwss_eth_s hwss_eth_t;

struct hwss_eth_s{
    char name[HWSS_ETH_NAMELEN_MAX];
    hwss_eth_state_t state;

    hwss_io_t   *io;
    hwss_hir_t  *hir;

    hwss_phy_t  *phy;
    hwss_mac_t  *mac;
    hwss_hnet_t *hnet;
    hwss_hso_t  *hso;

    hwss_hsl_t  *hsl;
};

esp_err_t hwss_eth_init(hwss_eth_t *eth);
esp_err_t hwss_eth_deinit(hwss_eth_t *eth);

