#include <sys/cdefs.h>
#include "esp_check.h"
#include "hwss_eth_type.h"
#include "hwss_sscm_drv_wch.h"
#include "drv_ch394.h"

static const char *TAG = "ch394.hwss_sscm";

typedef struct{
    hwss_sscm_drv_t super;

    hwss_io_t       *io;
}hwss_sscm_drv_ch394_t;

////////
//////// HSO State Control Manager Driver Implement
////////
static esp_err_t hwss_sscm_drv_ch394_get_sock_global_intr_bits(hwss_sscm_drv_t *drv, uint16_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_ch394_t *drv_ch394=__containerof(drv,hwss_sscm_drv_ch394_t,super);

    uint8_t sint=0;
    ESP_GOTO_ON_ERROR(CH394_GetSINT(drv_ch394->io,&sint),err,TAG,"cannot read SINT");
    *intr=(uint16_t)sint;
err:
    return ret;
}

static esp_err_t hwss_sscm_drv_ch394_set_sock_global_intr_enable(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_ch394_t *drv_ch394=__containerof(drv,hwss_sscm_drv_ch394_t,super);
    uint8_t sinte=0;

    ESP_GOTO_ON_ERROR(CH394_GetSINTE(drv_ch394->io,&sinte),err,TAG,"cannot read SINTE");
    if(en)
        sinte|=1<<id;
    else
        sinte&=~(1<<id);
    ESP_GOTO_ON_ERROR(CH394_SetSINTE(drv_ch394->io,&sinte),err,TAG,"cannot write SINTE");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_ch394_set_sock_global_intr_enable_all(hwss_sscm_drv_t *drv, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_ch394_t *drv_ch394=__containerof(drv,hwss_sscm_drv_ch394_t,super);
    uint8_t sinte=0x00;

    if(en)
        sinte=0xFF;
    ESP_GOTO_ON_ERROR(CH394_SetSINTE(drv_ch394->io,&sinte),err,TAG,"cannot write SINTE");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_ch394_get_sock_intr(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_ch394_t *drv_ch394=__containerof(drv,hwss_sscm_drv_ch394_t,super);
    uint8_t snint=0;

    ESP_GOTO_ON_ERROR(CH394_GetSn_INT(drv_ch394->io,id,&snint),err,TAG,"cannot read Sn_INT");
    
    if(snint&CH394_Sn_INT_SEND_SUC)
        *intr|= HWSS_SOCK_INTR_SEND_OK;

    if(snint&CH394_Sn_INT_TIMEOUT)
        *intr|=HWSS_SOCK_INTR_TIMEOUT;

    if(snint&CH394_Sn_INT_RECV)
        *intr|=HWSS_SOCK_INTR_RECV;
    
    if(snint&CH394_Sn_INT_DISCONNECT)
        *intr|=HWSS_SOCK_INTR_DISCONN;

    if(snint&CH394_Sn_INT_CONNECT)
        *intr|=HWSS_SOCK_INTR_CONNECT;

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_ch394_set_sock_intr_enable(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_ch394_t *drv_ch394=__containerof(drv,hwss_sscm_drv_ch394_t,super);
    uint8_t sninte=0x00;

    if(en)
        sninte=CH394_Sn_INT_SEND_SUC|CH394_Sn_INT_TIMEOUT|CH394_Sn_INT_RECV|CH394_Sn_INT_DISCONNECT|CH394_Sn_INT_CONNECT;
    ESP_GOTO_ON_ERROR(CH394_SetSn_INTE(drv_ch394->io,id,&sninte),err,TAG,"cannot write Sn_INTE");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_ch394_clear_sock_intr(hwss_sscm_drv_t *drv, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_ch394_t *drv_ch394=__containerof(drv,hwss_sscm_drv_ch394_t,super);
    uint8_t snint=CH394_Sn_INT_SEND_SUC|CH394_Sn_INT_TIMEOUT|CH394_Sn_INT_RECV|CH394_Sn_INT_DISCONNECT|CH394_Sn_INT_CONNECT;

    ESP_GOTO_ON_ERROR(CH394_SetSn_INT(drv_ch394->io,id,&snint),err,TAG,"cannot write Sn_INT");

err:
    return ret;
}

hwss_sscm_drv_t *hwss_sscm_drv_new_ch394(hwss_io_t *io){
    hwss_sscm_drv_t *ret=NULL;
    hwss_sscm_drv_ch394_t *drv=NULL;

    drv=calloc(1,sizeof(hwss_sscm_drv_ch394_t));
    ESP_GOTO_ON_FALSE(drv,NULL,err,TAG,"fail to calloc sscm_drv_ch394");

    drv->io=io;

    drv->super.clear_sock_intr=hwss_sscm_drv_ch394_clear_sock_intr;
    drv->super.get_sock_global_intr_bits=hwss_sscm_drv_ch394_get_sock_global_intr_bits;
    drv->super.get_sock_intr=hwss_sscm_drv_ch394_get_sock_intr;
    drv->super.set_sock_global_intr_enable=hwss_sscm_drv_ch394_set_sock_global_intr_enable;
    drv->super.set_sock_global_intr_enable_all=hwss_sscm_drv_ch394_set_sock_global_intr_enable_all;
    drv->super.set_sock_intr_enable=hwss_sscm_drv_ch394_set_sock_intr_enable;

    return &drv->super;

err:
    return ret;
}