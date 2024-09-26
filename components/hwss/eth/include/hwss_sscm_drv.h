#pragma once
#include "esp_err.h"
#include "hwss_type.h"
#include "hwss_sku.h"
#include "hwss_io.h"

typedef struct hwss_sscm_drv_s hwss_sscm_drv_t;

struct hwss_sscm_drv_s{
    esp_err_t (*get_sock_global_intr)(hwss_sscm_drv_t *drv, uint8_t *intr);
    esp_err_t (*set_sock_global_intr_enable)(hwss_sscm_drv_t *drv, hwss_sockid_t id, bool en);
    esp_err_t (*set_sock_global_intr_enable_all)(hwss_sscm_drv_t *drv, bool en);
    esp_err_t (*get_sock_intr)(hwss_sscm_drv_t *drv, hwss_sockid_t id, uint8_t *intr);
    esp_err_t (*set_sock_intr_enable)(hwss_sscm_drv_t *drv, hwss_sockid_t id, bool en);
    esp_err_t (*clear_sock_intr)(hwss_sscm_drv_t *drv, hwss_sockid_t id);
};

static inline esp_err_t hwss_sscm_drv_get_sock_global_intr(hwss_sscm_drv_t *drv, uint8_t *intr){
    return drv->get_sock_global_intr(drv,intr);
}

static inline esp_err_t hwss_sscm_drv_set_sock_global_intr_enable(hwss_sscm_drv_t *drv, hwss_sockid_t id, bool en){
    return drv->set_sock_global_intr_enable(drv,id,en);
}

static inline esp_err_t hwss_sscm_drv_set_sock_global_intr_enable_all(hwss_sscm_drv_t *drv, bool en){
    return drv->set_sock_global_intr_enable_all(drv,en);
}

static inline esp_err_t hwss_sscm_drv_get_sock_intr(hwss_sscm_drv_t *drv, hwss_sockid_t id, uint8_t *intr){
    return drv->get_sock_intr(drv,id,intr);
}

static inline esp_err_t hwss_sscm_drv_set_sock_intr_enable(hwss_sscm_drv_t *drv, hwss_sockid_t id, bool en){
    return drv->set_sock_intr_enable(drv,id,en);
}

static inline esp_err_t hwss_sscm_drv_clear_sock_intr(hwss_sscm_drv_t *drv, hwss_sockid_t id){
    return drv->clear_sock_intr(drv,id);
}

hwss_sscm_drv_t *hwss_sscm_drv_new(hwss_sku_t sku, hwss_io_t *io);
