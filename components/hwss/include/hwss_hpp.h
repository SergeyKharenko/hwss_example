#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    uint16_t cp_request_time_ms;
    uint8_t cp_magic_num;
}hwss_hpp_config_t;

typedef hwss_hpp_s hwss_hpp_t;

struct hwss_hpp_s{
    hwss_io_t *io;

    esp_err_t (*init)(hwss_hpp_t *hpp);
    esp_err_t (*deinit)(hwss_hpp_t *hpp);

    esp_err_t (*set_link_cp_request_time)(hwss_hpp_t *hpp, const uint16_t *ms);
    esp_err_t (*get_link_cp_request_time)(hwss_hpp_t *hpp, uint16_t *ms);

    esp_err_t (*set_link_cp_magic_num)(hwss_hpp_t *hpp, const uint8_t *num);
    esp_err_t (*get_link_cp_magic_num)(hwss_hpp_t *hpp, uint8_t *num);

    esp_err_t (*set_dest_mac_addr)(hwss_hpp_t *hpp, const uint8_t *mac_addr);
    esp_err_t (*get_dest_mac_addr)(hwss_hpp_t *hpp, uint8_t *mac_addr);

    esp_err_t (*set_sess_id)(hwss_hpp_t *hpp, const uint16_t *id);
    esp_err_t (*get_sess_id)(hwss_hpp_t *hpp, uint16_t *id);

    esp_err_t (*set_max_recv_unit)(hwss_hpp_t *hpp, const uint16_t *unit);
    esp_err_t (*get_max_recv_unit)(hwss_hpp_t *hpp, uint16_t *unit);
};
