#pragma once

#include "esp_event.h"
#include "hwss_sku.h"
#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    uint16_t    retry_time_ms;
    uint8_t     retry_cnt;

    uint32_t    check_state_period_ms;
}hwss_hsl_config_t;

typedef struct hwss_hsl_s hwss_hsl_t;

struct hwss_hsl_s{
    hwss_io_t *io;
    esp_event_loop_handle_t elp_hdl;

    esp_err_t (*init)(hwss_hsl_t *hsl);
    esp_err_t (*deinit)(hwss_hsl_t *hsl);

    esp_err_t (*start)(hwss_hsl_t *hsl);
    esp_err_t (*stop)(hwss_hsl_t *hsl);

    esp_err_t (*set_peer_addr)(hwss_hsl_t *hsl, const hwss_ip_addr_t addr);
    esp_err_t (*get_peer_addr)(hwss_hsl_t *hsl, hwss_ip_addr_t addr);

    esp_err_t (*get_peer_mac_addr)(hwss_hsl_t *hsl, hwss_mac_addr_t addr);

    esp_err_t (*set_ping_seqnum)(hwss_hsl_t *hsl, const uint16_t *num);
    esp_err_t (*get_ping_seqnum)(hwss_hsl_t *hsl, uint16_t *num);

    esp_err_t (*set_ping_id)(hwss_hsl_t *hsl, const uint16_t *id);
    esp_err_t (*get_ping_id)(hwss_hsl_t *hsl, uint16_t *id);

    esp_err_t (*send_ping)(hwss_hsl_t *hsl);
    esp_err_t (*send_arp)(hwss_hsl_t *hsl);
};



hwss_hsl_t *hwss_hsl_new(hwss_sku_t sku,esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hsl_config_t *config);
