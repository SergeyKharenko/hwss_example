#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef enum{
    HWSS_HSO_SOCKACT_ACTIVE,
    HWSS_HSO_SOCKACT_IDLE,
    HWSS_HSO_SOCKACT_PENDING_IDLE,
    HWSS_HSO_SOCKACT_GENERIC
}hwss_hso_sockact_sta_t;

typedef struct{
    uint32_t    sock_check_state_period_ms;

    uint8_t     en_sock_num;
    uint8_t     *txbuf_size_kb;
    uint8_t     *rxbuf_size_kb;

    uint16_t    irq_inter_time_us;
}hwss_hso_config_t;

typedef struct hwss_hso_s hwss_hso_t;

struct hwss_hso_s{
    hwss_io_t *io;
    
    esp_err_t (*init)(hwss_hso_t *hso);
    esp_err_t (*deinit)(hwss_hso_t *hso);

    esp_err_t (*set_sock_proto)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_proto_t *proto);
    esp_err_t (*get_sock_proto)(hwss_hso_t *hso, hwss_sockid_t id, hwss_proto_t *proto);

    esp_err_t (*set_sock_source_port)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port);
    esp_err_t (*get_sock_source_port)(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port);

    esp_err_t (*set_sock_dest_port)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port);
    esp_err_t (*get_sock_dest_port)(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port);

    esp_err_t (*set_sock_dest_mac_addr)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_mac_addr_t addr);
    esp_err_t (*get_sock_dest_mac_addr)(hwss_hso_t *hso, hwss_sockid_t id, hwss_mac_addr_t addr);

    esp_err_t (*set_sock_keepalive_tick)(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *tick);
    esp_err_t (*get_sock_keepalive_tick)(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *tick);

    esp_err_t (*set_sock_act_state)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_hso_sockact_sta_t *sta);
    esp_err_t (*get_sock_act_state)(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_sockact_sta_t *sta);

    esp_err_t (*set_sock_state)(hwss_hso_t *hso, hwss_sockid_t id, const hwss_hso_socksta_t *sta);
    esp_err_t (*get_sock_state)(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_socksta_t *sta);
};

typedef struct{
    hwss_hso_t      *hso;
    hwss_sockid_t   id;
    uint16_t        arg;
}hwss_hso_event_msg_t;
