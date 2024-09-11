#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    uint16_t retry_time_ms;
    uint8_t retry_cnt;
}hwss_hip_config_t;

typedef hwss_hip_s hwss_hip_t;

struct hwss_hip_s{
    hwss_io_t *io;

    esp_err_t (*init)(hwss_hip_t *hip);
    esp_err_t (*deinit)(hwss_hip_t *hip);

    esp_err_t (*set_gateway_addr)(hwss_hip_t *hip, const uint8_t *addr);
    esp_err_t (*get_gateway_addr)(hwss_hip_t *hip, uint8_t *addr);

    esp_err_t (*set_subnet_mask)(hwss_hip_t *hip, const uint8_t *mask);
    esp_err_t (*get_subnet_mask)(hwss_hip_t *hip, uint8_t *mask);

    esp_err_t (*set_source_addr)(hwss_hip_t *hip, const uint8_t *addr);
    esp_err_t (*get_source_addr)(hwss_hip_t *hip, uint8_t *addr);

    esp_err_t (*set_retry_time)(hwss_hip_t *hip, const uint16_t *ms);
    esp_err_t (*get_retry_time)(hwss_hip_t *hip, uint16_t *ms);

    esp_err_t (*set_retry_cnt)(hwss_hip_t *hip, const uint8_t *cnt);
    esp_err_t (*get_retry_cnt)(hwss_hip_t *hip, uint8_t *cnt);

    esp_err_t (*get_unreachable_addr)(hwss_hip_t *hip, uint8_t *addr);
    esp_err_t (*get_unreachable_port)(hwss_hip_t *hip, uint16_t *port);
};
