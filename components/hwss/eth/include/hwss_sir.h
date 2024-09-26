#pragma once
#include "esp_err.h"
#include "esp_event.h"
#include "hwss_sku.h"
#include "hwss_io.h"

typedef struct{
    uint32_t    check_state_period_ms;
}hwss_sir_config_t;

typedef struct hwss_sir_s hwss_sir_t;

struct hwss_sir_s{
    bool        is_started;

    esp_err_t   (*init)(hwss_sir_t *sir);
    esp_err_t   (*deinit)(hwss_sir_t *sir);

    esp_err_t   (*start)(hwss_sir_t *sir);
    esp_err_t   (*stop)(hwss_sir_t *sir);
};

static inline esp_err_t hwss_sir_init(hwss_sir_t *sir){
    return sir->init(sir);
}

static inline esp_err_t hwss_sir_deinit(hwss_sir_t *sir){
    return sir->deinit(sir);
}

static inline esp_err_t hwss_sir_start(hwss_sir_t *sir){
    return sir->start(sir);
}

static inline esp_err_t hwss_sir_stop(hwss_sir_t *sir){
    return sir->stop(sir);
}

hwss_sir_t *hwss_sir_new(hwss_sku_t sku, esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_sir_config_t *config);

