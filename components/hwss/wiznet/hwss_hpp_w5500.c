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

static esp_err_t hwss_hpp_w5500_init(hwss_hpp_t *hpp){
    esp_err_t ret=ESP_OK;
    hwss_hpp_w5500_t *hpp_w5500=__containerof(hpp,hwss_hpp_w5500_t,super);

    ESP_GOTO_ON_ERROR(setPTIMER(hpp->io,&(hpp_w5500->cp_request_tick)),err,TAG,"cannot write PTIMER");
    ESP_GOTO_ON_ERROR(setPMAGIC(hpp->io,&(hpp_w5500->cp_magic_num)),err,TAG,"cannot write PMAGIC");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_deinit(hwss_hpp_t *hpp){
    return ESP_OK;
}

static esp_err_t hwss_hpp_w5500_set_link_cp_request_time(hwss_hpp_t *hpp, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hpp_w5500_t *hpp_w5500=__containerof(hpp,hwss_hpp_w5500_t,super);
    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(*ms)<=0xFF,ESP_ERR_INVALID_ARG,err,TAG,"control protocol request time too long");

    uint8_t tick=(uint8_t) CP_REQ_MS2TICK(*ms);
    ESP_GOTO_ON_ERROR(setPTIMER(hpp->io,&tick),err,TAG,"cannot write PTIMER");
    hpp_w5500->cp_request_tick=tick;
err:
    return ret;    
}

static esp_err_t hwss_hpp_w5500_get_link_cp_request_time(hwss_hpp_t *hpp, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    uint8_t tick=0;
    ESP_GOTO_ON_ERROR(getPTIMER(hpp->io,&tick),err,TAG,"cannot read PTIMER");
    *ms=(uint16_t) CP_REQ_TICK2MS(tick);
err:
    return ret;    
}

static esp_err_t hwss_hpp_w5500_set_link_cp_magic_num(hwss_hpp_t *hpp, const uint8_t *num){
    esp_err_t ret=ESP_OK;
    hwss_hpp_w5500_t *hpp_w5500=__containerof(hpp,hwss_hpp_w5500_t,super);

    ESP_GOTO_ON_ERROR(setPMAGIC(hpp->io,num),err,TAG,"cannot write PMAGIC");
    hpp_w5500->cp_magic_num=*num;
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_get_link_cp_magic_num(hwss_hpp_t *hpp, uint8_t *num){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getPMAGIC(hpp->io,num),err,TAG,"cannot read PMAGIC");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_set_dest_mac_addr(hwss_hpp_t *hpp, const uint8_t *mac_addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(setPHAR(hpp->io,mac_addr),err,TAG,"cannot write PHAR");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_get_dest_mac_addr(hwss_hpp_t *hpp, uint8_t *mac_addr){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getPHAR(hpp->io,mac_addr),err,TAG,"cannot read PHAR");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_set_sess_id(hwss_hpp_t *hpp, const uint16_t *id){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(setPSID(hpp->io,id),err,TAG,"cannot write PSID");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_get_sess_id(hwss_hpp_t *hpp, uint16_t *id){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getPSID(hpp->io,id),err,TAG,"cannot read PSID");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_set_max_recv_unit(hwss_hpp_t *hpp, const uint16_t *unit){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(setPMRU(hpp->io,unit),err,TAG,"cannot write PMRU");
err:
    return ret;
}

static esp_err_t hwss_hpp_w5500_get_max_recv_unit(hwss_hpp_t *hpp, uint16_t *unit){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(getPMRU(hpp->io,unit),err,TAG,"cannot read PMRU");
err:
    return ret;
}

hwss_hpp_t *hwss_hpp_new_w5500(hwss_io_t *io, hwss_hpp_config_t *config){
    hwss_hpp_t *ret=NULL;
    hwss_hpp_w5500_t* hpp=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");

    hpp=calloc(1,sizeof(hwss_hpp_w5500_t));
    ESP_GOTO_ON_FALSE(hpp,NULL,err,TAG,"calloc hpp failed!");

    hpp->super.io=io;
    hpp->super.init=hwss_hpp_w5500_init;
    hpp->super.deinit=hwss_hpp_w5500_deinit;
    hpp->super.set_link_cp_request_time=hwss_hpp_w5500_set_link_cp_request_time;
    hpp->super.get_link_cp_request_time=hwss_hpp_w5500_get_link_cp_request_time;
    hpp->super.set_link_cp_magic_num=hwss_hpp_w5500_set_link_cp_magic_num;
    hpp->super.get_link_cp_magic_num=hwss_hpp_w5500_get_link_cp_magic_num;
    hpp->super.set_dest_mac_addr=hwss_hpp_w5500_set_dest_mac_addr;
    hpp->super.get_dest_mac_addr=hwss_hpp_w5500_get_dest_mac_addr;
    hpp->super.set_sess_id=hwss_hpp_w5500_set_sess_id;
    hpp->super.get_sess_id=hwss_hpp_w5500_get_sess_id;
    hpp->super.set_max_recv_unit=hwss_hpp_w5500_set_max_recv_unit;
    hpp->super.get_max_recv_unit=hwss_hpp_w5500_get_max_recv_unit;

    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(config->cp_request_time_ms)<=0xFF,NULL,
                        err,TAG,"control protocol request time too long");
    hpp->cp_request_tick=(uint8_t) CP_REQ_MS2TICK(config->cp_request_time_ms);
    hpp->cp_magic_num=config->cp_magic_num;
    return hpp->super;
err:
    if(hpp)
        free(hpp);
    return ret;
}