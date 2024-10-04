#include <machine/endian.h>
#include <sys/cdefs.h>
#include "esp_check.h"
#include "hwss_eth_type.h"
#include "hwss_sscm_drv_wiznet.h"
#include "drv_w5100s.h"

static const char *TAG = "w5100s.hwss_sscm";

typedef union{
    #if BYTE_ORDER == LITTLE_ENDIAN
    struct{
        uint8_t intr :4;
        uint8_t reserve :1;
        uint8_t pppterm :1;
        uint8_t unreach :1;
        uint8_t cnft :1;
    };
    #else
    struct{
        uint8_t cnft :1;
        uint8_t unreach :1;
        uint8_t pppterm :1;
        uint8_t reserve :1;
        uint8_t intr :4;
    };
    #endif
    uint8_t val;
}imr_reg_t;

typedef struct{
    hwss_sscm_drv_t super;

    hwss_io_t       *io;
}hwss_sscm_drv_w5100s_t;

////////
//////// HSO State Control Manager Driver Implement
////////
static esp_err_t hwss_sscm_drv_w5100s_get_sock_global_intr_bits(hwss_sscm_drv_t *drv, uint16_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5100s_t *drv_w5100s=__containerof(drv,hwss_sscm_drv_w5100s_t,super);

    uint8_t intr8=0;
    ESP_GOTO_ON_ERROR(W5100S_getIR(drv_w5100s->io,&intr8),err,TAG,"cannot read SIR");
    *intr=(uint16_t)(intr8&0x0F);
err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5100s_set_sock_global_intr_enable(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5100s_t *drv_w5100s=__containerof(drv,hwss_sscm_drv_w5100s_t,super);
    imr_reg_t imr;

    ESP_GOTO_ON_ERROR(W5100S_getIMR(drv_w5100s->io,&imr.val),err,TAG,"cannot read IMR");
    if(en)
        imr.intr|=1<<id;
    else
        imr.intr&=~(1<<id);
    ESP_GOTO_ON_ERROR(W5100S_setIMR(drv_w5100s->io,&imr.val),err,TAG,"cannot write IMR");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5100s_set_sock_global_intr_enable_all(hwss_sscm_drv_t *drv, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5100s_t *drv_w5100s=__containerof(drv,hwss_sscm_drv_w5100s_t,super);
    imr_reg_t imr;

    ESP_GOTO_ON_ERROR(W5100S_getIMR(drv_w5100s->io,&imr.val),err,TAG,"cannot read IMR");
    if(en)
        imr.intr=0x0F;
    else
        imr.intr=0x00;
    ESP_GOTO_ON_ERROR(W5100S_setIMR(drv_w5100s->io,&imr.val),err,TAG,"cannot write IMR");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5100s_get_sock_intr(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5100s_t *drv_w5100s=__containerof(drv,hwss_sscm_drv_w5100s_t,super);
    uint8_t snir=0;

    ESP_GOTO_ON_ERROR(W5100S_getSn_IR(drv_w5100s->io,id,&snir),err,TAG,"cannot read Sn_IR");

    if(snir&W5100S_Sn_IR_SENDOK)
        *intr|= HWSS_SOCK_INTR_SEND_OK;

    if(snir&W5100S_Sn_IR_TIMEOUT)
        *intr|=HWSS_SOCK_INTR_TIMEOUT;

    if(snir&W5100S_Sn_IR_RECV)
        *intr|=HWSS_SOCK_INTR_RECV;
    
    if(snir&W5100S_Sn_IR_DISCON)
        *intr|=HWSS_SOCK_INTR_DISCONN;

    if(snir&W5100S_Sn_IR_CON)
        *intr|=HWSS_SOCK_INTR_CONNECT;

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5100s_set_sock_intr_enable(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5100s_t *drv_w5100s=__containerof(drv,hwss_sscm_drv_w5100s_t,super);
    uint8_t snimr=0x00;

    if(en)
        snimr=W5100S_Sn_IR_SENDOK|W5100S_Sn_IR_TIMEOUT|W5100S_Sn_IR_RECV|W5100S_Sn_IR_DISCON|W5100S_Sn_IR_CON;
    ESP_GOTO_ON_ERROR(W5100S_setSn_IMR(drv_w5100s->io,id,&snimr),err,TAG,"cannot write Sn_IMR");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5100s_clear_sock_intr(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5100s_t *drv_w5100s=__containerof(drv,hwss_sscm_drv_w5100s_t,super);
    uint8_t snir=W5100S_Sn_IR_SENDOK|W5100S_Sn_IR_TIMEOUT|W5100S_Sn_IR_RECV|W5100S_Sn_IR_DISCON|W5100S_Sn_IR_CON;

    ESP_GOTO_ON_ERROR(W5100S_setSn_IR(drv_w5100s->io,id,&snir),err,TAG,"cannot write Sn_IR");

err:
    return ret;
}

hwss_sscm_drv_t *hwss_sscm_drv_new_w5100s(hwss_io_t *io){
    hwss_sscm_drv_t *ret=NULL;
    hwss_sscm_drv_w5100s_t *drv=NULL;

    drv=calloc(1,sizeof(hwss_sscm_drv_w5100s_t));
    ESP_GOTO_ON_FALSE(drv,NULL,err,TAG,"fail to calloc sscm_drv_w5100s");

    drv->io=io;

    drv->super.clear_sock_intr=hwss_sscm_drv_w5100s_clear_sock_intr;
    drv->super.get_sock_global_intr_bits=hwss_sscm_drv_w5100s_get_sock_global_intr_bits;
    drv->super.get_sock_intr=hwss_sscm_drv_w5100s_get_sock_intr;
    drv->super.set_sock_global_intr_enable=hwss_sscm_drv_w5100s_set_sock_global_intr_enable;
    drv->super.set_sock_global_intr_enable_all=hwss_sscm_drv_w5100s_set_sock_global_intr_enable_all;
    drv->super.set_sock_intr_enable=hwss_sscm_drv_w5100s_set_sock_intr_enable;

    return &drv->super;

err:
    return ret;
}