#pragma once
#include "esp_timer.h"
#include "driver/gpio.h"
#include "hwss_hir.h"
#include "hwss_hso.h"

// State Control Manager

#define HWSS_HSO_INTR_SEND_OK           1<<0
#define HWSS_HSO_INTR_TIMEOUT           1<<1
#define HWSS_HSO_INTR_RECV              1<<2
#define HWSS_HSO_INTR_DISCONN           1<<3
#define HWSS_HSO_INTR_CONNECT           1<<4

typedef struct{
    uint8_t     sock_total_num;
    uint8_t     en_sock_num;

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;
}hwss_hso_scm_config_t;

typedef struct hwss_hso_scm_timer_arg_s hwss_hso_scm_timer_arg_t;
typedef struct hwss_hso_scm_s hwss_hso_scm_t;

struct hwss_hso_scm_s{
    hwss_hso_t  *hso;
    hwss_hir_t  *hir;

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;

    uint8_t     en_sock_num;
    uint8_t     active_sock_num;

    esp_event_loop_handle_t elp_hdl;

    hwss_hso_sockact_sta_t  *sockact_sta_list;
    
    esp_timer_handle_t      *socktimer_list;
    esp_timer_handle_t      sock_polling_timer;

    hwss_hso_scm_timer_arg_t *socktimer_args;

    bool        is_started;

    esp_err_t (*init)(hwss_hso_scm_t *hso_scm);
    esp_err_t (*deinit)(hwss_hso_scm_t *hso_scm);

    esp_err_t (*start)(hwss_hso_scm_t *hso_scm);
    esp_err_t (*stop)(hwss_hso_scm_t *hso_scm);

    esp_err_t (*set_sock_state)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, const hwss_hso_sockact_sta_t *state);
    esp_err_t (*get_sock_state)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, hwss_hso_sockact_sta_t *state);
    
    struct{
        esp_err_t (*get_sock_global_intr)(hwss_hso_scm_t *hso_scm, uint8_t *intr);
        esp_err_t (*set_sock_global_intr_enable)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en);
        esp_err_t (*set_sock_global_intr_enable_all)(hwss_hso_scm_t *hso_scm, bool en);
        esp_err_t (*get_sock_intr)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t *intr);
        esp_err_t (*set_sock_intr_enable)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en);
        esp_err_t (*clear_sock_intr)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id);
    }drv;
};

struct hwss_hso_scm_timer_arg_s{
    hwss_hso_scm_t  *hso_scm;
    hwss_sockid_t   id;
};

hwss_hso_scm_t *hwss_hso_scm_new(esp_event_loop_handle_t elp_hdl, hwss_hso_t *hso, hwss_hir_t  *hir, const hwss_hso_scm_config_t *config);
