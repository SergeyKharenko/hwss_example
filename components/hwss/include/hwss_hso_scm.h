#pragma once
#include "esp_timer.h"
#include "driver/gpio.h"
#include "hwss_hso.h"

// State Control Manager

#define HWSS_HSO_INTR_SEND_OK           1<<0
#define HWSS_HSO_INTR_TIMEOUT           1<<1
#define HWSS_HSO_INTR_RECV              1<<2
#define HWSS_HSO_INTR_DISCONN           1<<3
#define HWSS_HSO_INTR_CONNECT           1<<4

typedef struct{
    hwss_hso_t  *hso;

    uint8_t     sock_total_num;
    uint8_t     en_sock_num;

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;

    gpio_num_t  irq_gpio_num;
    uint32_t    polling_task_stack_size;
    uint32_t    polling_task_prio;
}hwss_hso_scm_config_t;

typedef struct hwss_hso_scm_s hwss_hso_scm_t;

struct hwss_hso_scm_s{
    hwss_hso_t  *hso;

    uint32_t    sock_polling_period_ms;
    uint32_t    sock_active_threshold_ms;

    gpio_num_t  irq_gpio_num;

    uint32_t    polling_task_stack_size;
    uint32_t    polling_task_prio;

    uint32_t    irq_task_stack_size;
    uint32_t    irq_task_prio;

    uint8_t     en_sock_num;
    uint8_t     active_sock_num;

    hwss_hso_sockact_sta_t  *sockact_sta_list;
    
    esp_timer_handle_t      *socktimer_list;

    TaskHandle_t            polling_handler;
    TaskHandle_t            irq_handler;


    esp_err_t (*init)(hwss_hso_scm_t *hso_scm);
    esp_err_t (*deinit)(hwss_hso_scm_t *hso_scm);

    esp_err_t (*set_sock_state)(hwss_hso_scm_t *hso_scm, const hwss_hso_sockact_sta_t *state);
    esp_err_t (*get_sock_state)(hwss_hso_scm_t *hso_scm, hwss_hso_sockact_sta_t *state);
    
    struct{
        esp_err_t (*set_sock_global_intr)(hwss_hso_scm_t *hso_scm, const uint8_t *intr);
        esp_err_t (*get_sock_global_intr)(hwss_hso_scm_t *hso_scm, uint8_t *intr);
        esp_err_t (*set_sock_global_intr_enable)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en);
        esp_err_t (*get_sock_intr)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t *intr);
        esp_err_t (*clear_sock_intr)(hwss_hso_scm_t *hso_scm, hwss_sockid_t id);
    }drv;
};

esp_err_t hwss_hso_scm_initialize(hwss_hso_scm_t *hso_scm, const hwss_hso_scm_config_t *config);
