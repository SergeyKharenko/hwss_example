#pragma once
#include "esp_event.h"
#include "driver/gpio.h"
#include "hwss_type.h"

typedef struct{
    gpio_num_t      io_num;
    hwss_trigger_t  tri;
}hwss_hir_config_t;

typedef struct hwss_hir_s hwss_hir_t;

struct hwss_hir_s{
    bool                    is_started;
};

hwss_hir_t *hwss_hir_new(esp_event_loop_handle_t elp_hdl, const hwss_hir_config_t *config);
esp_err_t hwss_hir_init(hwss_hir_t *hir);
esp_err_t hwss_hir_deinit(hwss_hir_t *hir);

esp_err_t hwss_hir_start(hwss_hir_t *hir);
esp_err_t hwss_hir_stop(hwss_hir_t *hir);