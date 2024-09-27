#pragma once

#include "hwss_eth_sku.h"
#include "hwss_opt.h"
#include "hwss_io.h"
#include "hwss_hir.h"
#include "hwss_eth_type.h"

typedef enum{
    HWSS_HSO_SOCK_CLOSED,
    HWSS_HSO_SOCK_OPENED,

    HWSS_HSO_SOCK_TCP_INIT,
    HWSS_HSO_SOCK_TCP_LISTEN,
    HWSS_HSO_SOCK_TCP_ESTAB,
    HWSS_HSO_SOCK_TCP_CLOSEW,

    HWSS_HSO_SOCK_CUSTOM_STATE
}hwss_hso_socksta_t;

typedef enum{
    HWSS_HSO_SOCKCTRL_OPEN,
    HWSS_HSO_SOCKCTRL_LISTEN,
    HWSS_HSO_SOCKCTRL_CONNECT,
    HWSS_HSO_SOCKCTRL_DISCONN,
    HWSS_HSO_SOCKCTRL_CLOSE,
    HWSS_HSO_SOCKCTRL_SEND,
    HWSS_HSO_SOCKCTRL_SENDMAC,
    HWSS_HSO_SOCKCTRL_SENDKEEP,
    HWSS_HSO_SOCKCTRL_RECV
}hwss_hso_sockctrl_t;

typedef struct{
    uint8_t     en_socknum;
    uint8_t     tx_buffsize_kb[HWSS_SOCKNUM_MAX];
    uint8_t     rx_buffsize_kb[HWSS_SOCKNUM_MAX];

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;
}hwss_hso_config_t;

typedef struct hwss_hso_s hwss_hso_t;

struct hwss_hso_s{
    esp_err_t (*init)(hwss_hso_t *hso);
    esp_err_t (*deinit)(hwss_hso_t *hso);

    esp_err_t (*ctrl_sock)(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_hso_sockctrl_t ctrl);

    esp_err_t (*write_tx_buffer)(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *data, uint16_t len);
    esp_err_t (*read_rx_buffer)(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *data, uint16_t len);
    esp_err_t (*drop_rx_buffer)(hwss_hso_t *hso, hwss_eth_sockid_t id);
    
    esp_err_t (*get_tx_free_length)(hwss_hso_t *hso, hwss_eth_sockid_t id, uint16_t *len);
    esp_err_t (*get_rx_length)(hwss_hso_t *hso, hwss_eth_sockid_t id, uint16_t *len);

    esp_err_t (*set_sock_proto)(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_proto_t *proto);
    esp_err_t (*get_sock_proto)(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_proto_t *proto);

    esp_err_t (*set_sockmode_opt)(hwss_hso_t *hso, hwss_eth_sockid_t id, const void *opt);
    esp_err_t (*get_sockmode_opt)(hwss_hso_t *hso, hwss_eth_sockid_t id, void *opt);

    esp_err_t (*set_sock_source_port)(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_port_t *port);
    esp_err_t (*get_sock_source_port)(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_port_t *port);

    esp_err_t (*set_sock_dest_port)(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_port_t *port);
    esp_err_t (*get_sock_dest_port)(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_port_t *port);

    esp_err_t (*set_sock_dest_mac_addr)(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_mac_addr_t addr);
    esp_err_t (*get_sock_dest_mac_addr)(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_mac_addr_t addr);

    esp_err_t (*set_sock_dest_addr)(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *addr);
    esp_err_t (*get_sock_dest_addr)(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *addr);

    esp_err_t (*set_sock_keepalive_tick)(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *tick);
    esp_err_t (*get_sock_keepalive_tick)(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *tick);

    esp_err_t (*get_sock_state)(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_hso_socksta_t *sta);
    esp_err_t (*get_sock_state_raw)(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *sta);
};

static inline esp_err_t hwss_hso_init(hwss_hso_t *hso){
    return hso->init(hso);
}

static inline esp_err_t hwss_hso_deinit(hwss_hso_t *hso){
    return hso->deinit(hso);
}

static inline esp_err_t hwss_hso_ctrl_sock(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_hso_sockctrl_t ctrl){
    return hso->ctrl_sock(hso,id,ctrl);
}

static inline esp_err_t hwss_hso_write_tx_buffer(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *data, uint16_t len){
    return hso->write_tx_buffer(hso,id,data,len);
}

static inline esp_err_t hwss_hso_read_rx_buffer(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *data, uint16_t len){
    return hso->read_rx_buffer(hso,id,data,len);
}

static inline esp_err_t hwss_hso_drop_rx_buffer(hwss_hso_t *hso, hwss_eth_sockid_t id){
    return hso->drop_rx_buffer(hso,id);
}

static inline esp_err_t hwss_hso_get_tx_free_length(hwss_hso_t *hso, hwss_eth_sockid_t id, uint16_t *len){
    return hso->get_tx_free_length(hso,id,len);
}

static inline esp_err_t hwss_hso_get_rx_length(hwss_hso_t *hso, hwss_eth_sockid_t id, uint16_t *len){
    return hso->get_rx_length(hso,id,len);
}

static inline esp_err_t hwss_hso_set_sock_proto(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_proto_t *proto){
    return hso->set_sock_proto(hso,id,proto);
}

static inline esp_err_t hwss_hso_get_sock_proto(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_proto_t *proto){
    return hso->get_sock_proto(hso,id,proto);
}

static inline esp_err_t hwss_hso_set_sockmode_opt(hwss_hso_t *hso, hwss_eth_sockid_t id, const void *opt){
    return hso->set_sockmode_opt(hso,id,opt);
}

static inline esp_err_t hwss_hso_get_sockmode_opt(hwss_hso_t *hso, hwss_eth_sockid_t id, void *opt){
    return hso->get_sockmode_opt(hso,id,opt);
}

static inline esp_err_t hwss_hso_set_sock_source_port(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_port_t *port){
    return hso->set_sock_source_port(hso,id,port);
}

static inline esp_err_t hwss_hso_get_sock_source_port(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_port_t *port){
    return hso->get_sock_source_port(hso,id,port);
}

static inline esp_err_t hwss_hso_set_sock_dest_port(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_port_t *port){
    return hso->set_sock_dest_port(hso,id,port);
}

static inline esp_err_t hwss_hso_get_sock_dest_port(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_port_t *port){
    return hso->get_sock_dest_port(hso,id,port);
}

static inline esp_err_t hwss_hso_set_sock_dest_mac_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_mac_addr_t addr){
    return hso->set_sock_dest_mac_addr(hso,id,addr);
}

static inline esp_err_t hwss_hso_get_sock_dest_mac_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_mac_addr_t addr){
    return hso->get_sock_dest_mac_addr(hso,id,addr);
}

static inline esp_err_t hwss_hso_set_sock_dest_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *addr){
    return hso->set_sock_dest_addr(hso,id,addr);
}

static inline esp_err_t hwss_hso_get_sock_dest_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *addr){
    return hso->get_sock_dest_addr(hso,id,addr);
}

static inline esp_err_t hwss_hso_set_sock_keepalive_tick(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *tick){
    return hso->set_sock_keepalive_tick(hso,id,tick);
}

static inline esp_err_t hwss_hso_get_sock_keepalive_tick(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *tick){
    return hso->get_sock_keepalive_tick(hso,id,tick);
}

static inline esp_err_t hwss_hso_get_sock_state(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_hso_socksta_t *sta){
    return hso->get_sock_state(hso,id,sta);
}

static inline esp_err_t hwss_hso_get_sock_state_raw(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *sta){
    return hso->get_sock_state_raw(hso,id,sta);
}

hwss_hso_t *hwss_hso_new(hwss_eth_sku_t sku, esp_event_loop_handle_t elp, hwss_io_t *io, hwss_hir_t* hir, const hwss_hso_config_t *config);
