#pragma once
#include "hwss_sku.h"
#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    bool use_burnin_addr;
    hwss_mac_addr_t addr;
}hwss_mac_config_t;

typedef struct hwss_mac_s hwss_mac_t;

struct hwss_mac_s{
    esp_err_t (*set_addr)(hwss_mac_t *mac, const hwss_mac_addr_t addr);
    esp_err_t (*get_addr)(hwss_mac_t *mac, hwss_mac_addr_t addr);

    esp_err_t (*init)(hwss_mac_t *mac);
    esp_err_t (*deinit)(hwss_mac_t *mac);
};

hwss_mac_t *hwss_mac_new(hwss_sku_t sku, hwss_io_t *io, const hwss_mac_config_t *config);
