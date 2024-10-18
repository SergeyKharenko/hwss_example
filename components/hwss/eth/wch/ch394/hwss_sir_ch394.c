#include <sys/cdefs.h>
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_ch394.h"
#include "hwss_eth_event.h"
#include "hwss_sir_wch.h"

static const char *TAG = "ch394.hwss_sir";

typedef struct{
    hwss_sir_t super;

    esp_event_loop_handle_t elp_hdl;
    hwss_io_t *io;

    uint32_t    check_state_period_ms;
    esp_timer_handle_t check_state_timer;
}hwss_sir_ch394_t;

static void hwss_sir_ch394_check_state_timer_cb(void *args){
    hwss_sir_ch394_t* sir_ch394=(hwss_sir_ch394_t *)args;

    uint8_t gint;
    if(CH394_GetGINT(sir_ch394->io,&gint)!=ESP_OK){
        ESP_LOGE(TAG,"cannot read GINT");
        return;
    }

    if(gint&CH394_GINT_IP_CONFLI){        
        if(esp_event_post_to(sir_ch394->elp_hdl,HWSS_SIR_EVENT,HWSS_SIR_EVENT_IP_CONFLICT,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(gint&CH394_GINT_UNREACH){        
        if(esp_event_post_to(sir_ch394->elp_hdl,HWSS_SIR_EVENT,HWSS_SIR_EVENT_DEST_UNREACH,NULL,0,0)!=ESP_OK){
            ESP_LOGE(TAG,"fail to post event");
        }
    }

    if(CH394_SetGINT(sir_ch394->io,&gint)!=ESP_OK){
        ESP_LOGE(TAG,"cannot write GINT");
        return;
    }
}

static esp_err_t hwss_sir_ch394_init(hwss_sir_t *sir){
    esp_err_t ret=ESP_OK;
    hwss_sir_ch394_t *sir_ch394=__containerof(sir,hwss_sir_ch394_t,super);

    uint8_t gint=CH394_GINT_IP_CONFLI|CH394_GINT_UNREACH|CH394_GINT_MP;
    ESP_GOTO_ON_ERROR(CH394_SetGINT(sir_ch394->io,&gint),err,TAG,"cannot write GINT");

    uint8_t ginte=0x00;
    ESP_GOTO_ON_ERROR(CH394_SetGINTE(sir_ch394->io,&ginte),err,TAG,"cannot write GINTE");

err:
    return ret;
}

static esp_err_t hwss_sir_ch394_deinit(hwss_sir_t *sir){
    hwss_sir_ch394_t *sir_ch394=__containerof(sir,hwss_sir_ch394_t,super);
    
    if(!sir->is_started)
        return ESP_OK;
    sir->is_started=false;
    return esp_timer_stop(sir_ch394->check_state_timer);
}

static esp_err_t hwss_sir_ch394_start(hwss_sir_t *sir){
    hwss_sir_ch394_t *sir_ch394=__containerof(sir,hwss_sir_ch394_t,super);
    if(sir->is_started)
        return ESP_OK;
    sir->is_started=true;
    return esp_timer_start_periodic(sir_ch394->check_state_timer,sir_ch394->check_state_period_ms*1000);
}

static esp_err_t hwss_sir_ch394_stop(hwss_sir_t *sir){
    hwss_sir_ch394_t *sir_ch394=__containerof(sir,hwss_sir_ch394_t,super);
    if(!sir->is_started)
        return ESP_OK;
    sir->is_started=false;
    return esp_timer_stop(sir_ch394->check_state_timer);
}

hwss_sir_t *hwss_sir_new_ch394(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_sir_config_t *config){
    hwss_sir_t *ret=NULL;
    hwss_sir_ch394_t *sir=NULL;

    sir=calloc(1,sizeof(hwss_sir_ch394_t));
    ESP_GOTO_ON_FALSE(sir,NULL,err,TAG,"fail to calloc sir_ch394");

    if(config->check_state_period_ms<5)
        ESP_LOGW(TAG,"check period too short might SERIOUSLY affects the execution of other tasks!");

    sir->super.is_started=false;
    sir->super.init=hwss_sir_ch394_init;
    sir->super.deinit=hwss_sir_ch394_deinit;
    sir->super.start=hwss_sir_ch394_start;
    sir->super.stop=hwss_sir_ch394_stop;

    sir->io=io;
    sir->elp_hdl=elp_hdl;
    sir->check_state_period_ms=config->check_state_period_ms;

    esp_timer_create_args_t timer_arg={
        .name="hwss_sir_ch394_check_state_timer",
        .callback=hwss_sir_ch394_check_state_timer_cb,
        .arg=sir,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&sir->check_state_timer)==ESP_OK,NULL,err,TAG,"create check state timer failed");

    return &sir->super;
err:
    return ret;
}