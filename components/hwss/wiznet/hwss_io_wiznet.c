#include "esp_check.h"
#include "hwss_io_wiznet.h"

HWSS_IO_TYPE_LIST(W5500_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_TYPE_END};
HWSS_IO_TYPE_LIST(W5300_IO_SUPPORT_LIST)={HWSS_IO_PARALLEL_8, HWSS_IO_PARALLEL_16, HWSS_IO_TYPE_END};
HWSS_IO_TYPE_LIST(W5100_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_PARALLEL_8,HWSS_IO_TYPE_END};
HWSS_IO_TYPE_LIST(W5100S_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_PARALLEL_8,HWSS_IO_TYPE_END};

static const char *TAG="wiznet.io";

hwss_io_t *hwss_io_new_w5500(hwss_io_type type, void *io_config){
    hwss_io_t *ret = NULL;

    ESP_GOTO_ON_FALSE(hwss_io_type_supported(W5500_IO_SUPPORT_LIST,type),NULL,err,TAG,"unsupported io type");


err:
    if(ret!=NULL){

        free(ret);
    }
    return ret;
}

