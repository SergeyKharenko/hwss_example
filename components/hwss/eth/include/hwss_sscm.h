#pragma once
#include "esp_timer.h"
#include "driver/gpio.h"
#include "hwss_hir.h"
#include "hwss_sscm_drv.h"

// State Control Manager

typedef enum{
    HWSS_SSCM_POLICY_NO_INTR_POLLING=0,
    HWSS_SSCM_POLICY_INTR_WAKEUP_POLLING=1,
    HWSS_SSCM_POLICY_HYBRID=2
}hwss_sscm_policy_t;

typedef struct{
    hwss_sscm_policy_t  policy;
    uint8_t     en_socknum;

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;
}hwss_sscm_config_t;

typedef struct hwss_sscm_s hwss_sscm_t;

struct hwss_sscm_s{
    bool        is_started;
};

hwss_sscm_t *hwss_sscm_new(esp_event_loop_handle_t elp_hdl, hwss_sscm_drv_t *drv, const hwss_sscm_config_t *config);

esp_err_t hwss_sscm_init(hwss_sscm_t *sscm);
esp_err_t hwss_sscm_deinit(hwss_sscm_t *sscm);

esp_err_t hwss_sscm_start(hwss_sscm_t *sscm);
esp_err_t hwss_sscm_stop(hwss_sscm_t *sscm);

esp_err_t hwss_sscm_set_sock_state(hwss_sscm_t *sscm, hwss_eth_sockid_t id, const hwss_sockact_sta_t *state);
esp_err_t hwss_sscm_get_sock_state(hwss_sscm_t *sscm, hwss_eth_sockid_t id, hwss_sockact_sta_t *state);

esp_err_t hwss_sscm_intr_process(hwss_sscm_t *sscm);
