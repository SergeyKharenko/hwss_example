#pragma once

#include "esp_event.h"
#include "hwss_eth_sku.h"
#include "hwss_io.h"
#include "hwss_eth_type.h"

typedef struct{
    uint16_t    retry_time_ms;
    uint8_t     retry_cnt;
}hwss_hsl_config_t;

typedef struct hwss_hsl_s hwss_hsl_t;

struct hwss_hsl_s{
    esp_err_t (*init)(hwss_hsl_t *hsl);
    esp_err_t (*deinit)(hwss_hsl_t *hsl);

    esp_err_t (*set_peer_addr)(hwss_hsl_t *hsl, const hwss_eth_ip4_addr_t addr);
    esp_err_t (*get_peer_addr)(hwss_hsl_t *hsl, hwss_eth_ip4_addr_t addr);

    esp_err_t (*get_peer_mac_addr)(hwss_hsl_t *hsl, hwss_eth_mac_addr_t addr);

    esp_err_t (*set_ping_seqnum)(hwss_hsl_t *hsl, const uint16_t *num);
    esp_err_t (*get_ping_seqnum)(hwss_hsl_t *hsl, uint16_t *num);

    esp_err_t (*set_ping_id)(hwss_hsl_t *hsl, const uint16_t *id);
    esp_err_t (*get_ping_id)(hwss_hsl_t *hsl, uint16_t *id);

    esp_err_t (*send_ping)(hwss_hsl_t *hsl);
    esp_err_t (*send_arp)(hwss_hsl_t *hsl);
};

static inline esp_err_t hwss_hsl_init(hwss_hsl_t *hsl){
    return hsl->init(hsl);
}

static inline esp_err_t hwss_hsl_deinit(hwss_hsl_t *hsl){
    return hsl->deinit(hsl);
}

static inline esp_err_t hwss_hsl_set_peer_addr(hwss_hsl_t *hsl, const hwss_eth_ip4_addr_t addr){
    return hsl->set_peer_addr(hsl,addr);
}

static inline esp_err_t hwss_hsl_get_peer_addr(hwss_hsl_t *hsl, hwss_eth_ip4_addr_t addr){
    return hsl->get_peer_addr(hsl,addr);
}

static inline esp_err_t hwss_hsl_get_peer_mac_addr(hwss_hsl_t *hsl, hwss_eth_mac_addr_t addr){
    return hsl->get_peer_mac_addr(hsl,addr);
}

static inline esp_err_t hwss_hsl_set_ping_seqnum(hwss_hsl_t *hsl, const uint16_t *num){
    return hsl->set_ping_seqnum(hsl,num);
}

static inline esp_err_t hwss_hsl_get_ping_seqnum(hwss_hsl_t *hsl, uint16_t *num){
    return hsl->get_ping_seqnum(hsl,num);
}

static inline esp_err_t hwss_hsl_set_ping_id(hwss_hsl_t *hsl, const uint16_t *id){
    return hsl->set_ping_id(hsl,id);
}

static inline esp_err_t hwss_hsl_get_ping_id(hwss_hsl_t *hsl, uint16_t *id){
    return hsl->get_ping_id(hsl,id);
}

static inline esp_err_t hwss_hsl_send_ping(hwss_hsl_t *hsl){
    return hsl->send_ping(hsl);
}

static inline esp_err_t hwss_hsl_send_arp(hwss_hsl_t *hsl){
    return hsl->send_arp(hsl);
}

hwss_hsl_t *hwss_hsl_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_hsl_config_t *config);
