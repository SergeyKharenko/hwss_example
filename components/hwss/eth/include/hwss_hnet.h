#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    uint16_t    retry_time_ms;
    uint8_t     retry_cnt;

    uint16_t    ppp_cp_request_time_ms;
    uint8_t     ppp_cp_magic_num;

    uint32_t    check_state_period_ms;
}hwss_hnet_config_t;

typedef struct hwss_hnet_s hwss_hnet_t;

struct hwss_hnet_s{
    bool        is_started;

    esp_err_t (*init)(hwss_hnet_t *hnet);
    esp_err_t (*deinit)(hwss_hnet_t *hnet);

    esp_err_t (*start)(hwss_hnet_t *hnet);
    esp_err_t (*stop)(hwss_hnet_t *hnet);

    esp_err_t (*set_gateway_addr)(hwss_hnet_t *hnet, const hwss_ip_addr_t addr);
    esp_err_t (*get_gateway_addr)(hwss_hnet_t *hnet, hwss_ip_addr_t addr);

    esp_err_t (*set_subnet_mask)(hwss_hnet_t *hnet, const hwss_ip_addr_t mask);
    esp_err_t (*get_subnet_mask)(hwss_hnet_t *hnet, hwss_ip_addr_t mask);

    esp_err_t (*set_source_addr)(hwss_hnet_t *hnet, const hwss_ip_addr_t addr);
    esp_err_t (*get_source_addr)(hwss_hnet_t *hnet, hwss_ip_addr_t addr);

    esp_err_t (*set_retry_time)(hwss_hnet_t *hnet, const uint16_t *ms);
    esp_err_t (*get_retry_time)(hwss_hnet_t *hnet, uint16_t *ms);

    esp_err_t (*set_retry_cnt)(hwss_hnet_t *hnet, const uint8_t *cnt);
    esp_err_t (*get_retry_cnt)(hwss_hnet_t *hnet, uint8_t *cnt);

    esp_err_t (*get_unreachable_addr)(hwss_hnet_t *hnet, hwss_ip_addr_t addr);
    esp_err_t (*get_unreachable_port)(hwss_hnet_t *hnet, hwss_port_t *port);

    esp_err_t (*set_ppp_link_cp_request_time)(hwss_hnet_t *hnet, const uint16_t *ms);
    esp_err_t (*get_ppp_link_cp_request_time)(hwss_hnet_t *hnet, uint16_t *ms);

    esp_err_t (*set_ppp_link_cp_magic_num)(hwss_hnet_t *hnet, const uint8_t *num);
    esp_err_t (*get_ppp_link_cp_magic_num)(hwss_hnet_t *hnet, uint8_t *num);

    esp_err_t (*set_ppp_dest_mac_addr)(hwss_hnet_t *hnet, const hwss_mac_addr_t mac_addr);
    esp_err_t (*get_ppp_dest_mac_addr)(hwss_hnet_t *hnet, hwss_mac_addr_t mac_addr);

    esp_err_t (*set_ppp_sess_id)(hwss_hnet_t *hnet, const uint16_t *id);
    esp_err_t (*get_ppp_sess_id)(hwss_hnet_t *hnet, uint16_t *id);

    esp_err_t (*set_ppp_max_recv_unit)(hwss_hnet_t *hnet, const uint16_t *unit);
    esp_err_t (*get_ppp_max_recv_unit)(hwss_hnet_t *hnet, uint16_t *unit);
};

hwss_hnet_t *hwss_hnet_new(hwss_sku_t sku,esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hnet_config_t *config);
