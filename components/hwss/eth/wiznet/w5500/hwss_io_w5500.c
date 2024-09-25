#include <sys/cdefs.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_check.h"

#include "w5500.h"
#include "hwss_io_wiznet.h"
#include "hwss_io_wiznet_base.h"

const hwss_io_types_t W5500_IO_SUPPORT_LIST={HWSS_IO_SPI,HWSS_IO_TYPE_END};
// HWSS_IO_TYPE_LIST(W5300_IO_SUPPORT_LIST)={HWSS_IO_PARALLEL_8, HWSS_IO_PARALLEL_16, HWSS_IO_TYPE_END};
// HWSS_IO_TYPE_LIST(W5100_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_PARALLEL_8,HWSS_IO_TYPE_END};

static const char *TAG="w5500.hwss_io";

static esp_err_t hwss_io_w5500_spi_init(hwss_io_t *io){
    return hwss_io_wiznet_spi_init(io);
}

static esp_err_t hwss_io_w5500_spi_deinit(hwss_io_t *io){
    return hwss_io_wiznet_spi_deinit(io);
}

static esp_err_t hwss_io_w5500_spi_read(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data){
    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_),
        .length = 8,
        .rx_buffer = data
    };

    return hwss_io_wiznet_spi_trans(io,&trans);
}

static esp_err_t hwss_io_w5500_spi_write(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data){
    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_),
        .length = 8,
        .tx_buffer = data
    };

    return hwss_io_wiznet_spi_trans(io,&trans);
}

static esp_err_t hwss_io_w5500_spi_read_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, uint32_t data_len){
    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_),
        .length = 8 * data_len,
        .rx_buffer = data
    };

    return hwss_io_wiznet_spi_trans(io,&trans);
}

static esp_err_t hwss_io_w5500_spi_write_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, uint32_t data_len){
    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_),
        .length = 8 * data_len,
        .tx_buffer = data
    };

    return hwss_io_wiznet_spi_trans(io,&trans);
}


static hwss_io_t *hwss_io_new_w5500_spi(const hwss_io_spi_config_t* config){
    hwss_io_t *ret=NULL;
    hwss_io_wiznet_spi_t *io_w5500_spi=NULL;

    io_w5500_spi=hwss_io_new_wiznet_spi(config);
    ESP_GOTO_ON_FALSE(io_w5500_spi,NULL,err,TAG,"cannot create io_w5500_spi");
    
    io_w5500_spi->spi_config.command_bits=0;
    io_w5500_spi->spi_config.address_bits=24;
    io_w5500_spi->spi_config.dummy_bits=0;
    io_w5500_spi->spi_config.mode=0;

    io_w5500_spi->super.init=hwss_io_w5500_spi_init;
    io_w5500_spi->super.deinit=hwss_io_w5500_spi_deinit;
    io_w5500_spi->super.read=hwss_io_w5500_spi_read;
    io_w5500_spi->super.write=hwss_io_w5500_spi_write;
    io_w5500_spi->super.read_buf=hwss_io_w5500_spi_read_buf;
    io_w5500_spi->super.write_buf=hwss_io_w5500_spi_write_buf;

    return &(io_w5500_spi->super);

err:
    if(io_w5500_spi!=NULL){
        free(io_w5500_spi);
    }
    return ret;
}

hwss_io_t *hwss_io_new_w5500(hwss_io_type_t type, const void *io_config){
    ESP_RETURN_ON_FALSE(hwss_io_type_supported(W5500_IO_SUPPORT_LIST,type),NULL,TAG,"unsupported io type");
    ESP_RETURN_ON_FALSE(io_config, NULL, TAG, "cannot set io config to null");
    return hwss_io_new_w5500_spi(io_config);
}
