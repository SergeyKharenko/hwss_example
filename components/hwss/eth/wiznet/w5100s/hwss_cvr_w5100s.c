#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5100s.h"
#include "hwss_cvr_wiznet.h"

static const char *TAG = "w5100s.hwss_cvr";

typedef struct{
    hwss_cvr_t super;

    hwss_io_t *io;
}hwss_cvr_w5100s_t;

static esp_err_t hwss_cvr_w5100s_get_chip_version(hwss_cvr_t* cvr, hwss_chipver_t *cv){
    esp_err_t ret=ESP_OK;
    hwss_cvr_w5100s_t *cvr_w5100s=__containerof(cvr,hwss_cvr_w5100s_t,super);

    uint8_t ver=0;
    ESP_GOTO_ON_ERROR(W5100S_getVERR(cvr_w5100s->io,&ver),err,TAG,"cannot read VERR");
    *cv=(hwss_chipver_t) ver;

err:
    return ret;
}

static esp_err_t hwss_cvr_w5100s_self_test(hwss_cvr_t* cvr){
    esp_err_t ret=ESP_OK;
    hwss_cvr_w5100s_t *cvr_w5100s=__containerof(cvr,hwss_cvr_w5100s_t,super);

    uint8_t ver=0;
    ESP_GOTO_ON_ERROR(W5100S_getVERR(cvr_w5100s->io,&ver),err,TAG,"cannot read VERR");
    
    if(ver!=0x51){
        ESP_LOGE(TAG,"actual Chip Version is 0x%X, should be 0x51 instead",ver);
        ret=ESP_FAIL;
    }

err:
    return ret;
}

hwss_cvr_t *hwss_cvr_new_w5100s(hwss_io_t *io){
    hwss_cvr_t* ret=NULL;
    hwss_cvr_w5100s_t *cvr=NULL;

    cvr=calloc(1,sizeof(hwss_cvr_w5100s_t));
    ESP_GOTO_ON_FALSE(cvr,NULL,err,TAG,"fail to calloc cvr_w5100s");

    cvr->io=io;
    cvr->super.get_chip_version=hwss_cvr_w5100s_get_chip_version;
    cvr->super.self_test=hwss_cvr_w5100s_self_test;

    return &cvr->super;

err:
    return ret;
}