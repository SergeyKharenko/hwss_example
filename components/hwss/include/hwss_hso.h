#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    
}hwss_hsock_config_t;

typedef struct
{
    hwss_io_t *io;
    
    esp_err_t (*init)(void *hsock);
    esp_err_t (*deinit)(void *hsock);

    esp_err_t (*acquire)(void *hsock, hwss_sock_id_t *id);
    esp_err_t (*release)(void *hsock, hwss_sock_id_t id);

    esp_err_t (*set_sock_proto)(void *hsock, hwss_sock_id_t id, hwss_sock_proto_t proto);
    esp_err_t (*get_sock_proto)(void *hsock, hwss_sock_id_t id, hwss_sock_proto_t *proto);

    esp_err_t (*set_sock_source_port)(void *hsock, hwss_sock_id_t id, hwss_sock_port_t port);
    esp_err_t (*get_sock_source_port)(void *hsock, hwss_sock_id_t id, hwss_sock_port_t *port);

    esp_err_t (*set_sock_dest_port)(void *hsock, hwss_sock_id_t id, hwss_sock_port_t port);
    esp_err_t (*get_sock_dest_port)(void *hsock, hwss_sock_id_t id, hwss_sock_port_t *port);

    esp_err_t (*set_sock_dest_mac_addr)(void *hsock, hwss_sock_id_t id, hwss_mac_addr_t addr);
    esp_err_t (*get_sock_dest_mac_addr)(void *hsock, hwss_sock_id_t id, hwss_mac_addr_t addr);

    esp_err_t (*set_sock_keepalive_tick)(void *hsock, hwss_sock_id_t id, uint8_t tick);
    esp_err_t (*get_sock_keepalive_tick)(void *hsock, hwss_sock_id_t id, uint8_t *tick);

    esp_err_t (*sock_send)(void *hsock, hwss_sock_id_t id, uint8_t *data, uint16_t len);
    esp_err_t (*sock_recv)(void *hsock, hwss_sock_id_t id, uint8_t *data, uint16_t len);
    esp_err_t (*sock_drop)(void *hsock, hwss_sock_id_t id, uint16_t len);
}hwss_hsock_t;
