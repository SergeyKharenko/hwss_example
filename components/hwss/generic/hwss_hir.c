#include "esp_check.h"
#include "hwss_hir.h"

static const char *TAG = "hwss_hir";

IRAM_ATTR static void hwss_hir_isr_handler(void *args){
    hwss_hir_t *hir=(hwss_hir_t *)args;
    if(hir->handler!=NULL)
        hir->handler(hir->args);
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

static esp_err_t hwss_hir_setIRQ_handler(hwss_hir_t *hir, void (*handler)(void *), void *args){
    esp_err_t ret=ESP_OK;
    if(hir->is_started){
        ESP_GOTO_ON_ERROR(hir->stop(hir),err,TAG,"cannot pause hir");
        hir->handler=handler;
        hir->args=args;
        ESP_GOTO_ON_ERROR(hir->start(hir),err,TAG,"cannot restart hir");
    }
    else{
        hir->handler=handler;
        hir->args=args;
    }
err:
    return ret;
}

hwss_hir_t *hwss_hir_new(const hwss_hir_config_t *config){
    hwss_hir_t *ret=NULL;

    ret=calloc(1,sizeof(hwss_hir_t));
    ESP_GOTO_ON_FALSE(ret,NULL,err,TAG,"fail to calloc hwss_hir_t");

    ret->args=NULL;
    ret->handler=NULL;
    ret->io_num=config->io_num;
    ret->tri=config->tri;
    ret->is_started=false;
    
    ret->init=hwss_hir_init;
    ret->deinit=hwss_hir_deinit;
    ret->start=hwss_hir_start;
    ret->stop=hwss_hir_stop;
    ret->setIRQ_handler=hwss_hir_setIRQ_handler;
    
err:
    return ret;
}