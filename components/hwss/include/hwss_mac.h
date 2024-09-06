#pragma once

#include "hwss_io.h"
#include "hwss_type.h"

typedef struct{
    hwss_io_t *io;
    esp_err_t (*set_addr)(void *mac, uint8_t *addr);
    esp_err_t (*get_addr)(void *mac, uint8_t *addr);
}hwss_mac_t;
