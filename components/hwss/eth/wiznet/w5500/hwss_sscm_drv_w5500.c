#include <sys/cdefs.h>
#include "esp_check.h"
#include "hwss_type.h"
#include "hwss_sscm_drv_wiznet.h"
#include "drv_w5500.h"

static const char *TAG = "w5500.hwss_sscm";

typedef struct{
    hwss_sscm_drv_t super;

    hwss_io_t       *io;
    uint8_t         *dma_gintr;
    uint8_t         *dma_sintr;
}hwss_sscm_drv_w5500_t;

////////
//////// HSO State Control Manager Driver Implement
////////
static esp_err_t hwss_sscm_drv_w5500_get_sock_global_intr(hwss_sscm_drv_t *drv, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5500_t *drv_w5500=__containerof(drv,hwss_sscm_drv_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_getSIR(drv_w5500->io,drv_w5500->dma_gintr),err,TAG,"cannot read SIR");
    *intr=*(drv_w5500->dma_gintr);
err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5500_set_sock_global_intr_enable(hwss_sscm_drv_t *drv, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5500_t *drv_w5500=__containerof(drv,hwss_sscm_drv_w5500_t,super);
    uint8_t simr=0;

    ESP_GOTO_ON_ERROR(W5500_getSIMR(drv_w5500->io,&simr),err,TAG,"cannot read SIMR");
    if(en)
        simr|=1<<id;
    else
        simr&=~(1<<id);
    ESP_GOTO_ON_ERROR(W5500_setSIMR(drv_w5500->io,&simr),err,TAG,"cannot write SIMR");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5500_set_sock_global_intr_enable_all(hwss_sscm_drv_t *drv, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5500_t *drv_w5500=__containerof(drv,hwss_sscm_drv_w5500_t,super);
    uint8_t simr=0x00;

    if(en)
        simr=0xFF;
    ESP_GOTO_ON_ERROR(W5500_setSIMR(drv_w5500->io,&simr),err,TAG,"cannot write SIMR");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5500_get_sock_intr(hwss_sscm_drv_t *drv, hwss_sockid_t id, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5500_t *drv_w5500=__containerof(drv,hwss_sscm_drv_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_getSn_IR(drv_w5500->io,id,drv_w5500->dma_sintr),err,TAG,"cannot read Sn_IR");
    uint8_t snir=*drv_w5500->dma_sintr;
    
    if(snir&W5500_Sn_IR_SENDOK)
        *intr|= HWSS_SOCK_INTR_SEND_OK;

    if(snir&W5500_Sn_IR_TIMEOUT)
        *intr|=HWSS_SOCK_INTR_TIMEOUT;

    if(snir&W5500_Sn_IR_RECV)
        *intr|=HWSS_SOCK_INTR_RECV;
    
    if(snir&W5500_Sn_IR_DISCON)
        *intr|=HWSS_SOCK_INTR_DISCONN;

    if(snir&W5500_Sn_IR_CON)
        *intr|=HWSS_SOCK_INTR_CONNECT;

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5500_set_sock_intr_enable(hwss_sscm_drv_t *drv, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5500_t *drv_w5500=__containerof(drv,hwss_sscm_drv_w5500_t,super);
    uint8_t snimr=0x00;

    if(en)
        snimr=W5500_Sn_IR_SENDOK|W5500_Sn_IR_TIMEOUT|W5500_Sn_IR_RECV|W5500_Sn_IR_DISCON|W5500_Sn_IR_CON;
    ESP_GOTO_ON_ERROR(W5500_setSn_IMR(drv_w5500->io,id,&snimr),err,TAG,"cannot write Sn_IMR");

err:
    return ret;
}

static esp_err_t hwss_sscm_drv_w5500_clear_sock_intr(hwss_sscm_drv_t *drv, hwss_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_sscm_drv_w5500_t *drv_w5500=__containerof(drv,hwss_sscm_drv_w5500_t,super);
    uint8_t snir=W5500_Sn_IR_SENDOK|W5500_Sn_IR_TIMEOUT|W5500_Sn_IR_RECV|W5500_Sn_IR_DISCON|W5500_Sn_IR_CON;

    ESP_GOTO_ON_ERROR(W5500_setSn_IR(drv_w5500->io,id,&snir),err,TAG,"cannot write Sn_IR");

err:
    return ret;
}

hwss_sscm_drv_t *hwss_sscm_drv_new_w5500(hwss_io_t *io){
    hwss_sscm_drv_t *ret=NULL;
    hwss_sscm_drv_w5500_t *drv=NULL;

    drv=calloc(1,sizeof(hwss_sscm_drv_w5500_t));
    ESP_GOTO_ON_FALSE(drv,NULL,err,TAG,"fail to calloc sscm_drv_w5500");

    drv->io=io;
    drv->dma_gintr=heap_caps_malloc(1,MALLOC_CAP_DMA);
    drv->dma_sintr=heap_caps_malloc(1,MALLOC_CAP_DMA);

    ESP_GOTO_ON_FALSE(drv->dma_gintr&&drv->dma_sintr,NULL,err,TAG,"cannot malloc dma cache");

    drv->super.clear_sock_intr=hwss_sscm_drv_w5500_clear_sock_intr;
    drv->super.get_sock_global_intr=hwss_sscm_drv_w5500_get_sock_global_intr;
    drv->super.get_sock_intr=hwss_sscm_drv_w5500_get_sock_intr;
    drv->super.set_sock_global_intr_enable=hwss_sscm_drv_w5500_set_sock_global_intr_enable;
    drv->super.set_sock_global_intr_enable_all=hwss_sscm_drv_w5500_set_sock_global_intr_enable_all;
    drv->super.set_sock_intr_enable=hwss_sscm_drv_w5500_set_sock_intr_enable;

    return &drv->super;

err:
    return ret;
}