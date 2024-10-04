#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5100s.h"
#include "hwss_hppe_wiznet.h"

static const char *TAG = "w5100s.hwss_hppe";

#define CP_REQ_MS2TICK(ms)              ms/25
#define CP_REQ_TICK2MS(tick)            tick*25

typedef struct{
    hwss_hppe_t super;

    hwss_io_t   *io;

    uint8_t     cp_request_tick;
    uint8_t     cp_magic_num;
}hwss_hppe_w5100s_t;

static esp_err_t hwss_hppe_w5100s_init(hwss_hppe_t *hppe){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setPTIMER(hppe_w5100s->io,&(hppe_w5100s->cp_request_tick)),err,TAG,"cannot write PTIMER");
    ESP_GOTO_ON_ERROR(W5100S_setPMAGIC(hppe_w5100s->io,&(hppe_w5100s->cp_magic_num)),err,TAG,"cannot write PMAGIC");

err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_deinit(hwss_hppe_t *hppe){
    return ESP_OK;
}

static esp_err_t hwss_hppe_w5100s_set_link_cp_request_time(hwss_hppe_t *hppe, const uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(*ms)<=0xFF,ESP_ERR_INVALID_ARG,err,TAG,"control protocol request time too long");

    uint8_t tick=(uint8_t) CP_REQ_MS2TICK(*ms);
    hppe_w5100s->cp_request_tick=tick;

    ESP_GOTO_ON_ERROR(W5100S_setPTIMER(hppe_w5100s->io,&tick),err,TAG,"cannot write PTIMER");

err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_get_link_cp_request_time(hwss_hppe_t *hppe, uint16_t *ms){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);
    uint8_t tick=0;

    ESP_GOTO_ON_ERROR(W5100S_getPTIMER(hppe_w5100s->io,&tick),err,TAG,"cannot read PTIMER");
    *ms=(uint16_t) CP_REQ_TICK2MS(tick);
err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_set_link_cp_magic_num(hwss_hppe_t *hppe, const uint8_t *num){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    hppe_w5100s->cp_magic_num=*num;
    ESP_GOTO_ON_ERROR(W5100S_setPMAGIC(hppe_w5100s->io,num),err,TAG,"cannot write PMAGIC");
err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_get_link_cp_magic_num(hwss_hppe_t *hppe, uint8_t *num){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getPMAGIC(hppe_w5100s->io,num),err,TAG,"cannot read PMAGIC");
err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_set_dest_mac_addr(hwss_hppe_t *hppe, const hwss_eth_mac_addr_t mac_addr){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setPHAR(hppe_w5100s->io,mac_addr),err,TAG,"cannot write PHAR");
err:
    return ret;    
}

static esp_err_t hwss_hppe_w5100s_get_dest_mac_addr(hwss_hppe_t *hppe, hwss_eth_mac_addr_t mac_addr){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getPHAR(hppe_w5100s->io,mac_addr),err,TAG,"cannot read PHAR");
err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_set_sess_id(hwss_hppe_t *hppe, const uint16_t *id){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setPSID(hppe_w5100s->io,id),err,TAG,"cannot write PSID");
err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_get_sess_id(hwss_hppe_t *hppe, uint16_t *id){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getPSID(hppe_w5100s->io,id),err,TAG,"cannot read PSID");
err:
    return ret;    
}

static esp_err_t hwss_hppe_w5100s_set_max_recv_unit(hwss_hppe_t *hppe, const uint16_t *unit){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_setPMRU(hppe_w5100s->io,unit),err,TAG,"cannot write PMRU");
err:
    return ret;
}

static esp_err_t hwss_hppe_w5100s_get_max_recv_unit(hwss_hppe_t *hppe, uint16_t *unit){
    esp_err_t ret=ESP_OK;
    hwss_hppe_w5100s_t *hppe_w5100s=__containerof(hppe,hwss_hppe_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getPMRU(hppe_w5100s->io,unit),err,TAG,"cannot read PMRU");
err:
    return ret;    
}


hwss_hppe_t *hwss_hppe_new_w5100s(hwss_io_t *io, const hwss_hppe_config_t *config){
    hwss_hppe_t *ret=NULL;
    hwss_hppe_w5100s_t *hppe=NULL;

    hppe=calloc(1,sizeof(hwss_hppe_w5100s_t));
    ESP_GOTO_ON_FALSE(hppe,NULL,err,TAG,"fail to calloc hppe_w5100s");

    hppe->super.init=hwss_hppe_w5100s_init;
    hppe->super.deinit=hwss_hppe_w5100s_deinit;
    hppe->super.set_link_cp_request_time=hwss_hppe_w5100s_set_link_cp_request_time;
    hppe->super.get_link_cp_request_time=hwss_hppe_w5100s_get_link_cp_request_time;
    hppe->super.set_link_cp_magic_num=hwss_hppe_w5100s_set_link_cp_magic_num;
    hppe->super.get_link_cp_magic_num=hwss_hppe_w5100s_get_link_cp_magic_num;
    hppe->super.set_dest_mac_addr=hwss_hppe_w5100s_set_dest_mac_addr;
    hppe->super.get_dest_mac_addr=hwss_hppe_w5100s_get_dest_mac_addr;
    hppe->super.set_sess_id=hwss_hppe_w5100s_set_sess_id;
    hppe->super.get_sess_id=hwss_hppe_w5100s_get_sess_id;
    hppe->super.set_max_recv_unit=hwss_hppe_w5100s_set_max_recv_unit;
    hppe->super.get_max_recv_unit=hwss_hppe_w5100s_get_max_recv_unit;

    hppe->io=io;
    ESP_GOTO_ON_FALSE(CP_REQ_MS2TICK(config->cp_request_time_ms)<=0xFF,NULL,
                err,TAG,"control protocol request time too long");
    hppe->cp_request_tick=(uint8_t) CP_REQ_MS2TICK(config->cp_request_time_ms);
    hppe->cp_magic_num=config->cp_magic_num;

    return &hppe->super;

err:
    return ret;
}