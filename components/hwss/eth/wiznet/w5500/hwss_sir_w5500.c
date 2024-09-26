#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5500.h"
#include "hwss_eth_event.h"
#include "hwss_sir_wiznet.h"

static const char *TAG = "w5500.hwss_sir";

typedef struct{
    hwss_sir_t super;

    esp_event_loop_handle_t elp_hdl;
    hwss_io_t *io;

    uint32_t    check_state_period_ms;
    esp_timer_handle_t check_state_timer;
}hwss_sir_w5500_t;

static void hwss_sir_w5500_check_state_timer_cb(void *args){
    hwss_sir_w5500_t* sir_w5500=(hwss_sir_w5500_t *)args;

    uint8_t ir;
    if(W5500_getIR(sir_w5500->io,&ir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot read IR");
        return;
    }

    if(ir&W5500_IR_CONFLICT){        
        if(esp_event_post_to(sir_w5500->elp_hdl,HWSS_SIR_EVENT,HWSS_SIR_EVENT_IP_CONFLICT,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(ir&W5500_IR_UNREACH){        
        if(esp_event_post_to(sir_w5500->elp_hdl,HWSS_SIR_EVENT,HWSS_SIR_EVENT_DEST_UNREACH,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(ir&W5500_IR_PPPoE){        
        if(esp_event_post_to(sir_w5500->elp_hdl,HWSS_SIR_EVENT,HWSS_SIR_EVENT_PPPOE_CLOSE,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(W5500_setIR(sir_w5500->io,&ir)!=ESP_OK){
        ESP_LOGE(TAG,"cannot write IR");
        return;
    }
}

static esp_err_t hwss_sir_w5500_init(hwss_sir_t *sir){
    esp_err_t ret=ESP_OK;
    hwss_sir_w5500_t *sir_w5500=__containerof(sir,hwss_sir_w5500_t,super);

    uint8_t ir=W5500_IR_CONFLICT|W5500_IR_UNREACH|W5500_IR_PPPoE|W5500_IR_MP;
    ESP_GOTO_ON_ERROR(W5500_setIR(sir_w5500->io,&ir),err,TAG,"cannot write IR");

    uint8_t imr=0x00;
    ESP_GOTO_ON_ERROR(W5500_setIMR(sir_w5500->io,&imr),err,TAG,"cannot write IMR");

err:
    return ret;
}

static esp_err_t hwss_sir_w5500_deinit(hwss_sir_t *sir){
    hwss_sir_w5500_t *sir_w5500=__containerof(sir,hwss_sir_w5500_t,super);
    
    if(!sir->is_started)
        return ESP_OK;
    sir->is_started=false;
    return esp_timer_stop(sir_w5500->check_state_timer);
}

static esp_err_t hwss_sir_w5500_start(hwss_sir_t *sir){
    hwss_sir_w5500_t *sir_w5500=__containerof(sir,hwss_sir_w5500_t,super);
    if(sir->is_started)
        return ESP_OK;
    sir->is_started=true;
    return esp_timer_start_periodic(sir_w5500->check_state_timer,sir_w5500->check_state_period_ms*1000);
}

static esp_err_t hwss_sir_w5500_stop(hwss_sir_t *sir){
    hwss_sir_w5500_t *sir_w5500=__containerof(sir,hwss_sir_w5500_t,super);
    if(!sir->is_started)
        return ESP_OK;
    sir->is_started=false;
    return esp_timer_stop(sir_w5500->check_state_timer);
}

hwss_sir_t *hwss_sir_new_w5500(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_sir_config_t *config){
    hwss_sir_t *ret=NULL;
    hwss_sir_w5500_t *sir=NULL;

    sir=calloc(1,sizeof(hwss_sir_w5500_t));
    ESP_GOTO_ON_FALSE(sir,NULL,err,TAG,"fail to calloc sir_w5500");

    if(config->check_state_period_ms<5)
        ESP_LOGW(TAG,"check period too short might SERIOUSLY affects the execution of other tasks!");

    sir->super.is_started=false;
    sir->super.init=hwss_sir_w5500_init;
    sir->super.deinit=hwss_sir_w5500_deinit;
    sir->super.start=hwss_sir_w5500_start;
    sir->super.stop=hwss_sir_w5500_stop;

    sir->io=io;
    sir->elp_hdl=elp_hdl;
    sir->check_state_period_ms=config->check_state_period_ms;

    esp_timer_create_args_t timer_arg={
        .name="hwss_sir_w5500_check_state_timer",
        .callback=hwss_sir_w5500_check_state_timer_cb,
        .arg=sir,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&sir->check_state_timer)==ESP_OK,NULL,err,TAG,"create check state timer failed");

    return &sir->super;
err:
    return ret;
}