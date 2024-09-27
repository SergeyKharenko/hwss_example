#pragma once
#include "hwss_eth_sku.h"
#include "hwss_io.h"
#include "hwss_eth_type.h"

typedef struct{
    bool use_burnin_addr;
    hwss_eth_mac_addr_t addr;
}hwss_mac_config_t;

typedef struct hwss_mac_s hwss_mac_t;

struct hwss_mac_s{
    esp_err_t (*init)(hwss_mac_t *mac);
    esp_err_t (*deinit)(hwss_mac_t *mac);

    esp_err_t (*set_addr)(hwss_mac_t *mac, const hwss_eth_mac_addr_t addr);
    esp_err_t (*get_addr)(hwss_mac_t *mac, hwss_eth_mac_addr_t addr);
};

static inline esp_err_t hwss_mac_init(hwss_mac_t *mac){
    return mac->init(mac);
}

static inline esp_err_t hwss_mac_deinit(hwss_mac_t *mac){
    return mac->deinit(mac);
}

static inline esp_err_t hwss_mac_set_addr(hwss_mac_t *mac, const hwss_eth_mac_addr_t addr){
    return mac->set_addr(mac,addr);
}

static inline esp_err_t hwss_mac_get_addr(hwss_mac_t *mac, hwss_eth_mac_addr_t addr){
    return mac->get_addr(mac,addr);
}

hwss_mac_t *hwss_mac_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_mac_config_t *config);
