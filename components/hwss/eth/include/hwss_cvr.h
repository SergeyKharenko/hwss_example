#pragma once
#include "hwss_eth_type.h"
#include "hwss_eth_sku.h"
#include "hwss_io.h"

typedef struct hwss_cvr_s hwss_cvr_t;

struct hwss_cvr_s{
    esp_err_t (*get_chip_version)(hwss_cvr_t* cvr, hwss_chipver_t *cv);
    esp_err_t (*self_test)(hwss_cvr_t* cvr);
};

static inline esp_err_t hwss_cvr_get_chip_version(hwss_cvr_t* cvr, hwss_chipver_t *cv){
    return cvr->get_chip_version(cvr,cv);
}

static inline esp_err_t hwss_cvr_self_test(hwss_cvr_t* cvr){
    return cvr->self_test(cvr);
}

hwss_cvr_t *hwss_cvr_new(hwss_eth_sku_t sku, hwss_io_t *io);