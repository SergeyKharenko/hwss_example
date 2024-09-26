#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_w5500.h"
#include "hwss_cvr_wiznet.h"

static const char *TAG = "w5500.hwss_cvr";

typedef struct{
    hwss_cvr_t super;

    hwss_io_t *io;
}hwss_cvr_w5500_t;

static esp_err_t hwss_cvr_w5500_get_chip_version(hwss_cvr_t* cvr, hwss_chipver_t *cv){
    esp_err_t ret=ESP_OK;
    hwss_cvr_w5500_t *cvr_w5500=__containerof(cvr,hwss_cvr_w5500_t,super);

    uint8_t ver=0;
    ESP_GOTO_ON_ERROR(W5500_getVERSIONR(cvr_w5500->io,&ver),err,TAG,"cannot read VERSIONR");
    *cv=(hwss_chipver_t) ver;

err:
    return ret;
}

static esp_err_t hwss_cvr_w5500_self_test(hwss_cvr_t* cvr){
    esp_err_t ret=ESP_OK;
    hwss_cvr_w5500_t *cvr_w5500=__containerof(cvr,hwss_cvr_w5500_t,super);

    uint8_t ver=0;
    ESP_GOTO_ON_ERROR(W5500_getVERSIONR(cvr_w5500->io,&ver),err,TAG,"cannot read VERSIONR");
    
    if(ver!=0x04){
        ESP_LOGE(TAG,"actual Chip Version is %X, should be 0x04 instead",ver);
        ret=ESP_FAIL;
    }

err:
    return ret;
}

hwss_cvr_t *hwss_cvr_new_w5500(hwss_io_t *io){
    hwss_cvr_t* ret=NULL;
    hwss_cvr_w5500_t *cvr=NULL;

    cvr=calloc(1,sizeof(hwss_cvr_w5500_t));
    ESP_GOTO_ON_FALSE(cvr,NULL,err,TAG,"fail to calloc cvr_w5500");

    cvr->io=io;
    cvr->super.get_chip_version=hwss_cvr_w5500_get_chip_version;
    cvr->super.self_test=hwss_cvr_w5500_self_test;

    return &cvr->super;

err:
    return ret;
}