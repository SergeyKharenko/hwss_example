#pragma once

#include "hwss_eth_event.h"
#include "hwss_io.h"
#include "hwss_eth_type.h"

typedef struct{
    uint16_t    retry_time_ms;
    uint8_t     retry_cnt;
}hwss_hnet_config_t;

typedef struct hwss_hnet_s hwss_hnet_t;

struct hwss_hnet_s{
    esp_err_t (*init)(hwss_hnet_t *hnet);
    esp_err_t (*deinit)(hwss_hnet_t *hnet);

    esp_err_t (*set_gateway_addr)(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr);
    esp_err_t (*get_gateway_addr)(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr);

    esp_err_t (*set_subnet_mask)(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t mask);
    esp_err_t (*get_subnet_mask)(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t mask);

    esp_err_t (*set_source_addr)(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr);
    esp_err_t (*get_source_addr)(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr);

    esp_err_t (*set_retry_time)(hwss_hnet_t *hnet, const uint16_t *ms);
    esp_err_t (*get_retry_time)(hwss_hnet_t *hnet, uint16_t *ms);

    esp_err_t (*set_retry_cnt)(hwss_hnet_t *hnet, const uint8_t *cnt);
    esp_err_t (*get_retry_cnt)(hwss_hnet_t *hnet, uint8_t *cnt);

    esp_err_t (*get_unreachable_addr)(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr);
    esp_err_t (*get_unreachable_port)(hwss_hnet_t *hnet, hwss_eth_port_t *port);
};

static inline esp_err_t hwss_hnet_init(hwss_hnet_t *hnet){
    return hnet->init(hnet);
}

static inline esp_err_t hwss_hnet_deinit(hwss_hnet_t *hnet){
    return hnet->deinit(hnet);
}

static inline esp_err_t hwss_hnet_set_gateway_addr(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr){
    return hnet->set_gateway_addr(hnet,addr);
}

static inline esp_err_t hwss_hnet_get_gateway_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    return hnet->get_gateway_addr(hnet,addr);
}

static inline esp_err_t hwss_hnet_set_subnet_mask(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t mask){
    return hnet->set_subnet_mask(hnet,mask);
}

static inline esp_err_t hwss_hnet_get_subnet_mask(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t mask){
    return hnet->get_subnet_mask(hnet,mask);
}

static inline esp_err_t hwss_hnet_set_source_addr(hwss_hnet_t *hnet, const hwss_eth_ip4_addr_t addr){
    return hnet->set_source_addr(hnet,addr);
}

static inline esp_err_t hwss_hnet_get_source_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    return hnet->get_source_addr(hnet,addr);
}

static inline esp_err_t hwss_hnet_set_retry_time(hwss_hnet_t *hnet, const uint16_t *ms){
    return hnet->set_retry_time(hnet,ms);
}

static inline esp_err_t hwss_hnet_get_retry_time(hwss_hnet_t *hnet, uint16_t *ms){
    return hnet->get_retry_time(hnet,ms);
}

static inline esp_err_t hwss_hnet_set_retry_cnt(hwss_hnet_t *hnet, const uint8_t *cnt){
    return hnet->set_retry_cnt(hnet,cnt);
}

static inline esp_err_t hwss_hnet_get_retry_cnt(hwss_hnet_t *hnet, uint8_t *cnt){
    return hnet->get_retry_cnt(hnet,cnt);
}

static inline esp_err_t hwss_hnet_get_unreachable_addr(hwss_hnet_t *hnet, hwss_eth_ip4_addr_t addr){
    return hnet->get_unreachable_addr(hnet,addr);
}

static inline esp_err_t hwss_hnet_get_unreachable_port(hwss_hnet_t *hnet, hwss_eth_port_t *port){
    return hnet->get_unreachable_port(hnet,port);
}

hwss_hnet_t *hwss_hnet_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_hnet_config_t *config);
