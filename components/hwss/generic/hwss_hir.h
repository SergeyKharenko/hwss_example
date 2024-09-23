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
    gpio_num_t              io_num;
    hwss_trigger_t          tri;

    esp_event_loop_handle_t elp_hdl;

    bool                    is_started;

    esp_err_t (*init)(hwss_hir_t *hir);
    esp_err_t (*deinit)(hwss_hir_t *hir);

    esp_err_t (*start)(hwss_hir_t *hir);
    esp_err_t (*stop)(hwss_hir_t *hir);

    esp_err_t (*register_handler)(hwss_hir_t *hir, esp_event_handler_t handler, void *args);
    esp_err_t (*unregister_handler)(hwss_hir_t *hir, esp_event_handler_t handler);
};

hwss_hir_t *hwss_hir_new(esp_event_loop_handle_t elp_hdl, const hwss_hir_config_t *config);
