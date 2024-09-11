#include <cstddef>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_hpp_wiznet.h"

static const char *TAG = "w5500.hwss_hpp";

#define CP_REQ_MS2TICK(ms)              ms/25
#define CP_REQ_TICK2MS(tick)            tick*25

typedef struct{
    hwss_hpp_t super;
    uint8_t cp_request_tick;
    uint8_t cp_magic_num;
}hwss_hpp_w5500_t;

esp_err_t hwss_hpp_w5500_init(hwss_hpp_t *hpp){
    esp_err_t ret=ESP_OK;
    hwss_hpp_w5500_t *hpp_w5500=__containerof(hpp,hwss_hpp_w5500_t,super);

    ESP_GOTO_ON_ERROR(setPTIMER(hpp->io,&(hpp_w5500->cp_request_tick)),err,TAG,"cannot write PTIMER");
    ESP_GOTO_ON_ERROR(setPMAGIC(hpp->io,&(hpp_w5500->cp_magic_num)),err,TAG,"cannot write PMAGIC");
err:
    return ret;
}

esp_err_t hwss_hpp_w5500_deinit(hwss_hpp_t *hpp){
    return ESP_OK;
}

esp_err_t hwss_hpp_w5500_set_link_cp_request_time(hwss_hpp_t *hpp, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hpp_w5500_t *hpp_w5500=__containerof(hpp,hwss_hpp_w5500_t,super);
    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(*ms)<=0xFF,ESP_ERR_INVALID_ARG,err,TAG,"control protocol request time too long");

    uint8_t tick=(uint8_t) CP_REQ_MS2TICK(*ms);
    ESP_GOTO_ON_ERROR(setPTIMER(hpp->io,&tick),err,TAG,"cannot write PTIMER");
    hpp_w5500->cp_request_tick=tick;
err:
    return ret;    
}

esp_err_t hwss_hpp_w5500_get_link_cp_request_time(hwss_hpp_t *hpp, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    uint8_t tick=0;
    ESP_GOTO_ON_ERROR(getPTIMER(hpp->io,&tick),err,TAG,"cannot read PTIMER");
    *ms=(uint16_t) CP_REQ_TICK2MS(tick);
err:
    return ret;    
}

esp_err_t hwss_hpp_w5500_set_link_cp_magic_num(hwss_hpp_t *hpp, const uint8_t *num){
    esp_err_t ret=ESP_OK;
    hwss_hpp_w5500_t *hpp_w5500=__containerof(hpp,hwss_hpp_w5500_t,super);

    ESP_GOTO_ON_ERROR(setPMAGIC(hpp->io,num),err,TAG,"cannot write PMAGIC");
    hpp_w5500->cp_magic_num=*num;
err:
    return ret;
}

esp_err_t hwss_hpp_w5500_get_link_cp_magic_num(hwss_hpp_t *hpp, uint8_t *num){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getPMAGIC(hpp->io,num),err,TAG,"cannot read PMAGIC");
err:
    return ret;
}

esp_err_t hwss_hpp_w5500_set_dest_mac_addr(hwss_hpp_t *hpp, const uint8_t *mac_addr){
    
}

esp_err_t hwss_hpp_w5500_get_dest_mac_addr(hwss_hpp_t *hpp, uint8_t *mac_addr){

}

esp_err_t (*set_sess_id)(hwss_hpp_t *hpp, const uint16_t *id);
esp_err_t (*get_sess_id)(hwss_hpp_t *hpp, uint16_t *id);

esp_err_t (*set_max_recv_unit)(hwss_hpp_t *hpp, const uint16_t *unit);
esp_err_t (*get_max_recv_unit)(hwss_hpp_t *hpp, uint16_t *unit);