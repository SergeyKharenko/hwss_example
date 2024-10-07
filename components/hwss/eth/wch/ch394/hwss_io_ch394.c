#include <sys/cdefs.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_check.h"

#include "ch394.h"
#include "hwss_io_spi_basic.h"
#include "hwss_io_wch.h"

const hwss_io_types_t CH394_IO_SUPPORT_LIST={HWSS_IO_SPI,HWSS_IO_TYPE_END};

static const char *TAG="ch394.hwss_io";

static esp_err_t hwss_io_ch394_spi_init(hwss_io_t *io){
    return hwss_io_spi_init(io);
}

static esp_err_t hwss_io_ch394_spi_deinit(hwss_io_t *io){
    return hwss_io_spi_deinit(io);
}

static esp_err_t hwss_io_ch394_spi_read(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data){
    return hwss_io_spi_cache_read(io,0,addr | _CH394_SPI_READ_,data,1);
}

static esp_err_t hwss_io_ch394_spi_write(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data){
    return hwss_io_spi_cache_write(io,0,addr | _CH394_SPI_WRITE_,data,1);
}

static esp_err_t hwss_io_ch394_spi_read_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, size_t data_len){
    return hwss_io_spi_cache_read(io,0,addr | _CH394_SPI_READ_ ,data,data_len);
}

static esp_err_t hwss_io_ch394_spi_write_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, size_t data_len){
    return hwss_io_spi_cache_write(io,0,addr | _CH394_SPI_WRITE_,data,data_len);
}

static esp_err_t hwss_io_ch394_spi_read_mem(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, size_t data_len){
    return hwss_io_spi_direct_read(io,0,addr | _CH394_SPI_READ_ ,data,data_len);
}

static esp_err_t hwss_io_ch394_spi_write_mem(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, size_t data_len){
    return hwss_io_spi_direct_write(io,0,addr | _CH394_SPI_WRITE_ ,data,data_len);
}

static hwss_io_t *hwss_io_new_ch394_spi(const hwss_io_spi_config_t* config){
    hwss_io_t *ret=NULL;
    hwss_io_spi_t *io_ch394_spi=NULL;

    io_ch394_spi=hwss_io_new_spi(config);
    ESP_GOTO_ON_FALSE(io_ch394_spi,NULL,err,TAG,"cannot create io_ch394_spi");
    
    io_ch394_spi->spi_config.command_bits=0;
    io_ch394_spi->spi_config.address_bits=24;
    io_ch394_spi->spi_config.dummy_bits=0;
    io_ch394_spi->spi_config.mode=0;

    io_ch394_spi->super.init=hwss_io_ch394_spi_init;
    io_ch394_spi->super.deinit=hwss_io_ch394_spi_deinit;
    io_ch394_spi->super.read=hwss_io_ch394_spi_read;
    io_ch394_spi->super.write=hwss_io_ch394_spi_write;
    io_ch394_spi->super.read_buf=hwss_io_ch394_spi_read_buf;
    io_ch394_spi->super.write_buf=hwss_io_ch394_spi_write_buf;
    io_ch394_spi->super.read_mem=hwss_io_ch394_spi_read_mem;
    io_ch394_spi->super.write_mem=hwss_io_ch394_spi_write_mem;

    return &(io_ch394_spi->super);

err:
    if(io_ch394_spi!=NULL){
        free(io_ch394_spi);
    }
    return ret;
}

hwss_io_t *hwss_io_new_ch394(hwss_io_type_t type, const void *io_config){
    ESP_RETURN_ON_FALSE(hwss_io_type_supported(CH394_IO_SUPPORT_LIST,type),NULL,TAG,"unsupported io type");
    ESP_RETURN_ON_FALSE(io_config, NULL, TAG, "cannot set io config to null");
    return hwss_io_new_ch394_spi(io_config);
}
