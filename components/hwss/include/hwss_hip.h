#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    hwss_io_t *io;

    esp_err_t (*init)(void *hip);
    esp_err_t (*deinit)(void *hip);

    esp_err_t (*set_gateway_addr)(void *hip, const uint8_t addr[4]);
    esp_err_t (*get_gateway_addr)(void *hip, uint8_t addr[4]);

    esp_err_t (*set_subnet_mask)(void *hip, const uint8_t mask[4]);
    esp_err_t (*get_subnet_mask)(void *hip, uint8_t mask[4]);

    esp_err_t (*set_source_addr)(void *hip, const uint8_t addr[4]);
    esp_err_t (*get_source_addr)(void *hip, uint8_t addr[4]);
}hwss_hip_t;
