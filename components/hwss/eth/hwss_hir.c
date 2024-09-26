#include <sys/cdefs.h>
#include "esp_attr.h"
#include "esp_check.h"
#include "esp_event.h"
#include "hwss_eth_event.h"
#include "hwss_hir.h"

static const char *TAG = "hwss_hir";

typedef struct{
    hwss_hir_t super;
    
    gpio_num_t              io_num;
    hwss_trigger_t          tri;
    esp_event_loop_handle_t elp_hdl;
}hwss_hir_pro_t;

IRAM_ATTR static void hwss_hir_isr_handler(void *args){
    hwss_hir_pro_t *hir_pro=(hwss_hir_pro_t *)args;
    BaseType_t unblock=pdTRUE;
    esp_event_isr_post_to(hir_pro->elp_hdl,HWSS_HIR_EVENT,HWSS_HIR_EVENT_TRIGGER,NULL,0,&unblock);
}

esp_err_t hwss_hir_init(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;
    hwss_hir_pro_t *hir_pro=__containerof(hir,hwss_hir_pro_t,super);
    gpio_config_t io_cfg={
        .pin_bit_mask=1ull<<hir_pro->io_num,
        .mode=GPIO_MODE_INPUT,
    };

    if(hir_pro->tri==HWSS_TRIGGER_POSEDGE){
        io_cfg.pull_up_en=0;
        io_cfg.pull_down_en=1;
        io_cfg.intr_type=GPIO_INTR_POSEDGE;
    }
    else{
        io_cfg.pull_up_en=1;
        io_cfg.pull_down_en=0;
        io_cfg.intr_type=GPIO_INTR_NEGEDGE;
    }
    ESP_GOTO_ON_ERROR(gpio_config(&io_cfg),err,TAG,"cannot setup pin");

err:
    return ret;
}

esp_err_t hwss_hir_deinit(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;
    hwss_hir_pro_t *hir_pro=__containerof(hir,hwss_hir_pro_t,super);

    if(hir->is_started)
        hwss_hir_stop(hir);

    ESP_GOTO_ON_ERROR(gpio_isr_handler_remove(hir_pro->io_num),err,TAG,"cannot deinit");
    ESP_GOTO_ON_ERROR(gpio_intr_disable(hir_pro->io_num),err,TAG,"cannot deinit");

err:
    return ret;
}

esp_err_t hwss_hir_start(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;
    hwss_hir_pro_t *hir_pro=__containerof(hir,hwss_hir_pro_t,super);

    if(hir->is_started)
        return ret;
    ESP_GOTO_ON_ERROR(gpio_intr_enable(hir_pro->io_num),err,TAG,"cannot enable irq");
    ESP_GOTO_ON_ERROR(gpio_isr_handler_add(hir_pro->io_num,hwss_hir_isr_handler,(void *)hir_pro),
                    err,TAG,"cannot enable irq");
    hir->is_started=true;
err:
    return ret;
}

esp_err_t hwss_hir_stop(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;
    hwss_hir_pro_t *hir_pro=__containerof(hir,hwss_hir_pro_t,super);

    if(!hir->is_started)
        return ret;
    
    ESP_GOTO_ON_ERROR(gpio_isr_handler_remove(hir_pro->io_num),err,TAG,"cannot disable irq");
    ESP_GOTO_ON_ERROR(gpio_intr_disable(hir_pro->io_num),err,TAG,"cannot disable irq");
    hir->is_started=false;
err:
    return ret;
}

hwss_hir_t *hwss_hir_new(esp_event_loop_handle_t elp_hdl, const hwss_hir_config_t *config){
    hwss_hir_t *ret=NULL;
    hwss_hir_pro_t *hir=NULL;

    hir=calloc(1,sizeof(hwss_hir_pro_t));
    ESP_GOTO_ON_FALSE(ret,NULL,err,TAG,"fail to calloc hwss_hir_pro_t");

    hir->io_num=config->io_num;
    hir->tri=config->tri;
    hir->elp_hdl=elp_hdl;

    hir->super.is_started=false;
    return &hir->super;

err:
    return ret;
}