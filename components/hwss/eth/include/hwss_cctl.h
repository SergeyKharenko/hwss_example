#pragma once
#include "esp_err.h"
#include "driver/gpio.h"
#include "hwss_sku.h"
#include "hwss_io.h"

typedef struct hwss_cctl_s hwss_cctl_t;

typedef struct{
    gpio_num_t  rst_ionum;
    uint32_t    hw_rst_ms;
}hwss_cctl_config_t;

struct hwss_cctl_s{
    esp_err_t (*init)(hwss_cctl_t *cctl);
    esp_err_t (*deinit)(hwss_cctl_t *cctl);
    esp_err_t (*chip_reset)(hwss_cctl_t *cctl);
    esp_err_t (*enable_pppoe)(hwss_cctl_t *cctl, bool en);
    esp_err_t (*enable_wol)(hwss_cctl_t *cctl, bool en);
    esp_err_t (*enable_ping_block)(hwss_cctl_t *cctl, bool en);
    esp_err_t (*enable_force_arp)(hwss_cctl_t *cctl, bool en);
    esp_err_t (*set_intr_wait_time)(hwss_cctl_t *cctl, const uint16_t *us);
};

static inline esp_err_t hwss_cctl_init(hwss_cctl_t *cctl){
    return cctl->init(cctl);
}

static inline esp_err_t hwss_cctl_deinit(hwss_cctl_t *cctl){
    return cctl->deinit(cctl);
}

static inline esp_err_t hwss_cctl_chip_reset(hwss_cctl_t *cctl){
    return cctl->chip_reset(cctl);
}

static inline esp_err_t hwss_cctl_enable_pppoe(hwss_cctl_t *cctl, bool en){
    return cctl->enable_pppoe(cctl,en);
}

static inline esp_err_t hwss_cctl_enable_wol(hwss_cctl_t *cctl, bool en){
    return cctl->enable_wol(cctl,en);
}

static inline esp_err_t hwss_cctl_enable_ping_block(hwss_cctl_t *cctl, bool en){
    return cctl->enable_ping_block(cctl,en);
}

static inline esp_err_t hwss_cctl_enable_force_arp(hwss_cctl_t *cctl, bool en){
    return cctl->enable_force_arp(cctl,en);
}

static inline esp_err_t hwss_cctl_set_intr_wait_time(hwss_cctl_t *cctl, const uint16_t *us){
    return cctl->set_intr_wait_time(cctl,us);
}

hwss_cctl_t *hwss_cctl_new(hwss_sku_t sku, hwss_io_t *io, const hwss_cctl_config_t *config);
