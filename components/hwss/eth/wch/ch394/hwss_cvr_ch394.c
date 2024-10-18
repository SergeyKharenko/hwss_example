#include <sys/cdefs.h>
#include "esp_check.h"
#include "drv_ch394.h"
#include "hwss_cvr_wch.h"

static const char *TAG = "ch394.hwss_cvr";

typedef struct{
    hwss_cvr_t super;

    hwss_io_t *io;
}hwss_cvr_ch394_t;

static esp_err_t hwss_cvr_ch394_get_chip_version(hwss_cvr_t* cvr, hwss_chipver_t *cv){
    esp_err_t ret=ESP_OK;
    hwss_cvr_ch394_t *cvr_ch394=__containerof(cvr,hwss_cvr_ch394_t,super);

    uint8_t ver=0;
    ESP_GOTO_ON_ERROR(CH394_GetCHIPV(cvr_ch394->io,&ver),err,TAG,"cannot read CHIPV");
    *cv=(hwss_chipver_t) ver;

err:
    return ret;
}

static esp_err_t hwss_cvr_ch394_self_test(hwss_cvr_t* cvr){
    esp_err_t ret=ESP_OK;
    hwss_cvr_ch394_t *cvr_ch394=__containerof(cvr,hwss_cvr_ch394_t,super);

    uint8_t ver=0;
    ESP_GOTO_ON_ERROR(CH394_GetCHIPV(cvr_ch394->io,&ver),err,TAG,"cannot read CHIPV");
    
    if(ver!=0x03){
        ESP_LOGE(TAG,"actual Chip Version is 0x%X, should be 0x03 instead",ver);
        ret=ESP_FAIL;
    }

err:
    return ret;
}

hwss_cvr_t *hwss_cvr_new_ch394(hwss_io_t *io){
    hwss_cvr_t* ret=NULL;
    hwss_cvr_ch394_t *cvr=NULL;

    cvr=calloc(1,sizeof(hwss_cvr_ch394_t));
    ESP_GOTO_ON_FALSE(cvr,NULL,err,TAG,"fail to calloc cvr_ch394");

    cvr->io=io;
    cvr->super.get_chip_version=hwss_cvr_ch394_get_chip_version;
    cvr->super.self_test=hwss_cvr_ch394_self_test;

    return &cvr->super;

err:
    return ret;
}