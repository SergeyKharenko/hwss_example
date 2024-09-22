#pragma once
#include "driver/gpio.h"
#include "hwss_type.h"

typedef struct hwss_hir_s hwss_hir_t;

typedef struct{
    gpio_num_t      io_num;
    hwss_trigger_t  tri;
}hwss_hir_config_t;


struct hwss_hir_s{
    gpio_num_t      io_num;
    hwss_trigger_t  tri;

    void            (*handler)(void *);
    void            *args;

    bool            is_started;

    esp_err_t (*init)(hwss_hir_t *hir);
    esp_err_t (*deinit)(hwss_hir_t *hir);

    esp_err_t (*start)(hwss_hir_t *hir);
    esp_err_t (*stop)(hwss_hir_t *hir);

    esp_err_t (*setIRQ_handler)(hwss_hir_t *hir, void (*handler)(void *), void *args);
};

hwss_hir_t *hwss_hir_new(const hwss_hir_config_t *config);