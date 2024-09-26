#include "hwss_sscm_drv.h"

////////
//////// HSO State Control Manager Driver Implement
////////
static esp_err_t hwss_hso_scm_w5500_get_sock_global_intr(hwss_hso_scm_t *hso_scm, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);
    ESP_GOTO_ON_ERROR(W5500_getSIR(hso_w5500->io,hso_w5500->dma_gintr),err,TAG,"cannot read SIR");
    *intr=*(hso_w5500->dma_gintr);
err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_set_sock_global_intr_enable(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t simr=0;

    ESP_GOTO_ON_ERROR(W5500_getSIMR(hso_w5500->io,&simr),err,TAG,"cannot read SIMR");
    if(en)
        simr|=1<<id;
    else
        simr&=~(1<<id);
    ESP_GOTO_ON_ERROR(W5500_setSIMR(hso_w5500->io,&simr),err,TAG,"cannot write SIMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_set_sock_global_intr_enable_all(hwss_hso_scm_t *hso_scm, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t simr=0x00;

    if(en)
        simr=0xFF;
    ESP_GOTO_ON_ERROR(W5500_setSIMR(hso_w5500->io,&simr),err,TAG,"cannot write SIMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_get_sock_intr(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);

    ESP_GOTO_ON_ERROR(W5500_getSn_IR(hso_w5500->io,id,hso_w5500->dma_sintr),err,TAG,"cannot read Sn_IR");
    uint8_t snir=*hso_w5500->dma_sintr;
    
    if(snir&W5500_Sn_IR_SENDOK)
        *intr|= HWSS_HSO_INTR_SEND_OK;

    if(snir&W5500_Sn_IR_TIMEOUT)
        *intr|=HWSS_HSO_INTR_TIMEOUT;

    if(snir&W5500_Sn_IR_RECV)
        *intr|=HWSS_HSO_INTR_RECV;
    
    if(snir&W5500_Sn_IR_DISCON)
        *intr|=HWSS_HSO_INTR_DISCONN;

    if(snir&W5500_Sn_IR_CON)
        *intr|=HWSS_HSO_INTR_CONNECT;

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_set_sock_intr_enable(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snimr=0x00;

    if(en)
        snimr=W5500_Sn_IR_SENDOK|W5500_Sn_IR_TIMEOUT|W5500_Sn_IR_RECV|W5500_Sn_IR_DISCON|W5500_Sn_IR_CON;
    ESP_GOTO_ON_ERROR(W5500_setSn_IMR(hso_w5500->io,id,&snimr),err,TAG,"cannot write Sn_IMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_clear_sock_intr(hwss_hso_scm_t *hso_scm, hwss_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snir=W5500_Sn_IR_SENDOK|W5500_Sn_IR_TIMEOUT|W5500_Sn_IR_RECV|W5500_Sn_IR_DISCON|W5500_Sn_IR_CON;

    ESP_GOTO_ON_ERROR(W5500_setSn_IR(hso_w5500->io,id,&snir),err,TAG,"cannot write Sn_IR");

err:
    return ret;
}
