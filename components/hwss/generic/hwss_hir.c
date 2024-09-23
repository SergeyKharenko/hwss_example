#include "esp_attr.h"
#include "esp_check.h"
#include "esp_event.h"
#include "hwss_event.h"
#include "hwss_hir.h"

static const char *TAG = "hwss_hir";

IRAM_ATTR static void hwss_hir_isr_handler(void *args){
    hwss_hir_t *hir=(hwss_hir_t *)args;
    BaseType_t unblock=pdTRUE;
    esp_event_isr_post_to(hir->elp_hdl,HWSS_INTER_EVENT,HWSS_HIR_EVENTBIT_TRIGGER,NULL,0,&unblock);
}

static esp_err_t hwss_hir_init(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;
    gpio_config_t io_cfg={
        .pin_bit_mask=1ull<<hir->io_num,
        .mode=GPIO_MODE_INPUT,
    };

    if(hir->tri==HWSS_TRIGGER_POSEDGE){
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

static esp_err_t hwss_hir_deinit(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;

    if(hir->is_started)
        hir->stop(hir);

    ESP_GOTO_ON_ERROR(gpio_isr_handler_remove(hir->io_num),err,TAG,"cannot deinit");
    ESP_GOTO_ON_ERROR(gpio_intr_disable(hir->io_num),err,TAG,"cannot deinit");

err:
    return ret;
}

static esp_err_t hwss_hir_start(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;

    if(hir->is_started)
        return ret;
    ESP_GOTO_ON_ERROR(gpio_intr_enable(hir->io_num),err,TAG,"cannot enable irq");
    ESP_GOTO_ON_ERROR(gpio_isr_handler_add(hir->io_num,hwss_hir_isr_handler,(void *)hir),
                    err,TAG,"cannot enable irq");
    hir->is_started=true;
err:
    return ret;
}

static esp_err_t hwss_hir_stop(hwss_hir_t *hir){
    esp_err_t ret=ESP_OK;
    if(!hir->is_started)
        return ret;
    
    ESP_GOTO_ON_ERROR(gpio_isr_handler_remove(hir->io_num),err,TAG,"cannot disable irq");
    ESP_GOTO_ON_ERROR(gpio_intr_disable(hir->io_num),err,TAG,"cannot disable irq");
    hir->is_started=false;
err:
    return ret;
}

static esp_err_t hwss_hir_register_handler(hwss_hir_t *hir, esp_event_handler_t handler, void *args){
    return esp_event_handler_register_with(hir->elp_hdl,HWSS_INTER_EVENT,HWSS_HIR_EVENTBIT_TRIGGER,handler,args);
}

static esp_err_t hwss_hir_unregister_handler(hwss_hir_t *hir, esp_event_handler_t handler){
    return esp_event_handler_unregister_with(hir->elp_hdl,HWSS_INTER_EVENT,HWSS_HIR_EVENTBIT_TRIGGER,handler);
}

hwss_hir_t *hwss_hir_new(esp_event_loop_handle_t elp_hdl, const hwss_hir_config_t *config){
    hwss_hir_t *ret=NULL;

    ret=calloc(1,sizeof(hwss_hir_t));
    ESP_GOTO_ON_FALSE(ret,NULL,err,TAG,"fail to calloc hwss_hir_t");

    ret->io_num=config->io_num;
    ret->tri=config->tri;
    ret->is_started=false;
    ret->elp_hdl=elp_hdl;
    
    ret->init=hwss_hir_init;
    ret->deinit=hwss_hir_deinit;
    ret->start=hwss_hir_start;
    ret->stop=hwss_hir_stop;
    ret->register_handler=hwss_hir_register_handler;
    ret->unregister_handler=hwss_hir_unregister_handler;

err:
    return ret;
}