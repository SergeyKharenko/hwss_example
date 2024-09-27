#pragma once
#include "hwss_eth_sku.h"
#include "hwss_io.h"

typedef struct{
    uint16_t    cp_request_time_ms;
    uint8_t     cp_magic_num;
}hwss_hppe_config_t;

typedef struct hwss_hppe_s hwss_hppe_t;

struct hwss_hppe_s{
    esp_err_t (*init)(hwss_hppe_t *hppe);
    esp_err_t (*deinit)(hwss_hppe_t *hppe);

    esp_err_t (*set_link_cp_request_time)(hwss_hppe_t *hppe, const uint16_t *ms);
    esp_err_t (*get_link_cp_request_time)(hwss_hppe_t *hppe, uint16_t *ms);

    esp_err_t (*set_link_cp_magic_num)(hwss_hppe_t *hppe, const uint8_t *num);
    esp_err_t (*get_link_cp_magic_num)(hwss_hppe_t *hppe, uint8_t *num);

    esp_err_t (*set_dest_mac_addr)(hwss_hppe_t *hppe, const hwss_eth_mac_addr_t mac_addr);
    esp_err_t (*get_dest_mac_addr)(hwss_hppe_t *hppe, hwss_eth_mac_addr_t mac_addr);

    esp_err_t (*set_sess_id)(hwss_hppe_t *hppe, const uint16_t *id);
    esp_err_t (*get_sess_id)(hwss_hppe_t *hppe, uint16_t *id);

    esp_err_t (*set_max_recv_unit)(hwss_hppe_t *hppe, const uint16_t *unit);
    esp_err_t (*get_max_recv_unit)(hwss_hppe_t *hppe, uint16_t *unit);
};

static inline esp_err_t hwss_hppe_init(hwss_hppe_t *hppe){
    return hppe->init(hppe);
}

static inline esp_err_t hwss_hppe_deinit(hwss_hppe_t *hppe){
    return hppe->deinit(hppe);
}

static inline esp_err_t hwss_hppe_set_link_cp_request_time(hwss_hppe_t *hppe, const uint16_t *ms){
    return hppe->set_link_cp_request_time(hppe,ms);
}

static inline esp_err_t hwss_hppe_get_link_cp_request_time(hwss_hppe_t *hppe, uint16_t *ms){
    return hppe->get_link_cp_request_time(hppe,ms);
}

static inline esp_err_t hwss_hppe_set_link_cp_magic_num(hwss_hppe_t *hppe, const uint8_t *num){
    return hppe->set_link_cp_magic_num(hppe,num);
}

static inline esp_err_t hwss_hppe_get_link_cp_magic_num(hwss_hppe_t *hppe, uint8_t *num){
    return hppe->get_link_cp_magic_num(hppe,num);
}

static inline esp_err_t hwss_hppe_set_dest_mac_addr(hwss_hppe_t *hppe, const hwss_eth_mac_addr_t mac_addr){
    return hppe->set_dest_mac_addr(hppe,mac_addr);
}

static inline esp_err_t hwss_hppe_get_dest_mac_addr(hwss_hppe_t *hppe, hwss_eth_mac_addr_t mac_addr){
    return hppe->get_dest_mac_addr(hppe,mac_addr);
}

static inline esp_err_t hwss_hppe_set_sess_id(hwss_hppe_t *hppe, const uint16_t *id){
    return hppe->set_sess_id(hppe,id);
}

static inline esp_err_t hwss_hppe_get_sess_id(hwss_hppe_t *hppe, uint16_t *id){
    return hppe->get_sess_id(hppe,id);
}

static inline esp_err_t hwss_hppe_set_max_recv_unit(hwss_hppe_t *hppe, const uint16_t *unit){
    return hppe->set_max_recv_unit(hppe,unit);
}

static inline esp_err_t hwss_hppe_get_max_recv_unit(hwss_hppe_t *hppe, uint16_t *unit){
    return hppe->get_max_recv_unit(hppe,unit);
}

hwss_hppe_t *hwss_hppe_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_hppe_config_t *config);
