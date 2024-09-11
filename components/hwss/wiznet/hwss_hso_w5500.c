#include <cstddef>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_hso_wiznet.h"

#define W5500_SOCK_TOTAL_NUM                         8

static const char *TAG = "w5500.hwss_hso";

typedef struct{
    hwss_hso_t super;
}hwss_hso_w5500_t;

static esp_err_t hwss_hso_w5500_init(hwss_hso_t *hsock){

}

static esp_err_t hwss_hso_w5500_deinit(hwss_hso_t *hsock){

}

esp_err_t (*set_sock_proto)(hwss_hso_t *hsock, hwss_sockid_t id, const hwss_proto_t *proto);
esp_err_t (*get_sock_proto)(hwss_hso_t *hsock, hwss_sockid_t id, hwss_proto_t *proto);

esp_err_t (*set_sock_source_port)(hwss_hso_t *hsock, hwss_sockid_t id, const hwss_port_t *port);
esp_err_t (*get_sock_source_port)(hwss_hso_t *hsock, hwss_sockid_t id, hwss_port_t *port);

esp_err_t (*set_sock_dest_port)(hwss_hso_t *hsock, hwss_sockid_t id, const hwss_port_t *port);
esp_err_t (*get_sock_dest_port)(hwss_hso_t *hsock, hwss_sockid_t id, hwss_port_t *port);

esp_err_t (*set_sock_dest_mac_addr)(hwss_hso_t *hsock, hwss_sockid_t id, const hwss_mac_addr_t addr);
esp_err_t (*get_sock_dest_mac_addr)(hwss_hso_t *hsock, hwss_sockid_t id, hwss_mac_addr_t addr);

esp_err_t (*set_sock_keepalive_tick)(hwss_hso_t *hsock, hwss_sockid_t id, const uint8_t *tick);
esp_err_t (*get_sock_keepalive_tick)(hwss_hso_t *hsock, hwss_sockid_t id, uint8_t *tick);

esp_err_t (*sock_send)(hwss_hso_t *hsock, hwss_sockid_t id, uint8_t *data, uint16_t len);
esp_err_t (*sock_recv)(hwss_hso_t *hsock, hwss_sockid_t id, uint8_t *data, uint16_t *len);
esp_err_t (*sock_drop)(hwss_hso_t *hsock, hwss_sockid_t id, uint16_t len);
