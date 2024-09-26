#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_cctl_wiznet.h"

static const char *TAG = "w5500.hwss_cctl";

typedef struct{
    hwss_cctl_t super;

    hwss_io_t   *io;
    gpio_num_t  rst_ionum;

    uint32_t    hw_rst_ms;
}hwss_cctl_w5500_t;

esp_err_t hwss_cctl_w5500_init(hwss_cctl_t *cctl){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);
    
    if(cctl_w5500->rst_ionum>=0){
        gpio_config_t io_config={
            .mode=GPIO_MODE_OUTPUT,
            .pin_bit_mask=(1ull<<cctl_w5500->rst_ionum)
        };

        ESP_GOTO_ON_ERROR(gpio_config(&io_config),err,TAG,"fail to init reset pin");
        gpio_set_level(cctl_w5500->rst_ionum,1);
    }

err:
    return ret;
}

esp_err_t hwss_cctl_w5500_deinit(hwss_cctl_t *cctl){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);

    if(cctl_w5500->rst_ionum>=0)
        ESP_GOTO_ON_ERROR(gpio_reset_pin(cctl_w5500->rst_ionum),err,TAG,"fail to deinit reset pin");
err:
    return ret;
}

esp_err_t hwss_cctl_w5500_chip_reset(hwss_cctl_t *cctl){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);

    if(cctl_w5500->rst_ionum>=0){
        gpio_set_level(cctl_w5500->rst_ionum,0);
        vTaskDelay(pdMS_TO_TICKS(1));
        gpio_set_level(cctl_w5500->rst_ionum,1);
        vTaskDelay(pdMS_TO_TICKS(cctl_w5500->hw_rst_ms));
    }
    else{
        uint8_t mr=0;
        ESP_GOTO_ON_ERROR(W5500_getMR(cctl_w5500->io,&mr),err,TAG,"cannot read MR");
        mr|=W5500_MR_RST;
        ESP_GOTO_ON_ERROR(W5500_setMR(cctl_w5500->io,&mr),err,TAG,"cannot write MR");

        uint16_t max_loop=100;
        while(1){
            vTaskDelay(pdMS_TO_TICKS(2));
            ESP_GOTO_ON_ERROR(W5500_getMR(cctl_w5500->io,&mr),err,TAG,"cannot read MR");
            if((mr&W5500_MR_RST)==0)
                break;
            if(max_loop==0){
                ret=ESP_ERR_TIMEOUT;
                break;
            }
            else
                max_loop--;
        }
    }

err:
    return ret;
}

esp_err_t hwss_cctl_w5500_enable_pppoe(hwss_cctl_t *cctl, bool en){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);
    uint8_t mr=0;

    ESP_GOTO_ON_ERROR(W5500_getMR(cctl_w5500->io,&mr),err,TAG,"cannot read MR");
    if(en)
        mr|=W5500_MR_PPPOE;
    else
        mr&=(~W5500_MR_PPPOE);
    ESP_GOTO_ON_ERROR(W5500_setMR(cctl_w5500->io,&mr),err,TAG,"cannot write MR");

err:
    return ret;
}

esp_err_t hwss_cctl_w5500_enable_wol(hwss_cctl_t *cctl, bool en){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);
    uint8_t mr=0;

    ESP_GOTO_ON_ERROR(W5500_getMR(cctl_w5500->io,&mr),err,TAG,"cannot read MR");
    if(en)
        mr|=W5500_MR_WOL;
    else
        mr&=(~W5500_MR_WOL);
    ESP_GOTO_ON_ERROR(W5500_setMR(cctl_w5500->io,&mr),err,TAG,"cannot write MR");

err:
    return ret;
}

esp_err_t hwss_cctl_w5500_enable_ping_block(hwss_cctl_t *cctl, bool en){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);
    uint8_t mr=0;

    ESP_GOTO_ON_ERROR(W5500_getMR(cctl_w5500->io,&mr),err,TAG,"cannot read MR");
    if(en)
        mr|=W5500_MR_PB;
    else
        mr&=(~W5500_MR_PB);
    ESP_GOTO_ON_ERROR(W5500_setMR(cctl_w5500->io,&mr),err,TAG,"cannot write MR");

err:
    return ret;
}

esp_err_t hwss_cctl_w5500_enable_force_arp(hwss_cctl_t *cctl, bool en){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);
    uint8_t mr=0;

    ESP_GOTO_ON_ERROR(W5500_getMR(cctl_w5500->io,&mr),err,TAG,"cannot read MR");
    if(en)
        mr|=W5500_MR_FARP;
    else
        mr&=(~W5500_MR_FARP);
    ESP_GOTO_ON_ERROR(W5500_setMR(cctl_w5500->io,&mr),err,TAG,"cannot write MR");

err:
    return ret;
}

esp_err_t hwss_cctl_w5500_set_intr_wait_time(hwss_cctl_t *cctl, const uint16_t *us){
    esp_err_t ret=ESP_OK;
    hwss_cctl_w5500_t *cctl_w5500=__containerof(cctl,hwss_cctl_w5500_t,super);

    uint16_t ticks;
    if(*us==0)
        ticks=0;
    else
        ticks=*us*150/4-1;

    ESP_GOTO_ON_ERROR(W5500_setINTLEVEL(cctl_w5500->io,&(ticks)),err,TAG,"cannot write INTLEVEL");

err:
    return ret;
}

hwss_cctl_t *hwss_cctl_new_w5500(hwss_io_t *io, const hwss_cctl_config_t *config){
    hwss_cctl_t *ret=NULL;
    hwss_cctl_w5500_t *cctl=NULL;

    cctl=calloc(1,sizeof(hwss_cctl_w5500_t));
    ESP_GOTO_ON_FALSE(cctl,NULL,err,TAG,"fail to calloc cctl_w5500");

    cctl->io=io;
    cctl->rst_ionum=config->rst_ionum;
    cctl->hw_rst_ms=config->hw_rst_ms;

    cctl->super.init=hwss_cctl_w5500_init;
    cctl->super.deinit=hwss_cctl_w5500_deinit;
    cctl->super.chip_reset=hwss_cctl_w5500_chip_reset;
    cctl->super.enable_wol=hwss_cctl_w5500_enable_wol;
    cctl->super.enable_pppoe=hwss_cctl_w5500_enable_pppoe;
    cctl->super.enable_ping_block=hwss_cctl_w5500_enable_ping_block;
    cctl->super.enable_force_arp=hwss_cctl_w5500_enable_force_arp;
    cctl->super.set_intr_wait_time=hwss_cctl_w5500_set_intr_wait_time;

    return &(cctl->super);

err:
    return ret;
}