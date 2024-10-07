#pragma once

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "hwss_io.h"
#include "hwss_cache.h"
#include "hwss_eth_opt.h"

typedef struct{
    spi_host_device_t   spi_host_id;
    gpio_num_t          cs_io_num;
    uint32_t            speed_khz;
} hwss_io_spi_config_t;

typedef struct{
    hwss_io_t           super;
    spi_device_handle_t hdl;
    SemaphoreHandle_t   lock;
    
    spi_host_device_t               spi_host_id;
    spi_device_interface_config_t   spi_config;

    void                *cache;
}hwss_io_spi_t;

static inline bool HWSS_IO_SPI_LOCK(hwss_io_spi_t *io){
    return (xSemaphoreTake(io->lock,pdMS_TO_TICKS(HWSS_IO_LOCK_TIMEOUT_MS)) == pdTRUE);
}

static inline bool HWSS_IO_SPI_UNLOCK(hwss_io_spi_t *io){
    return (xSemaphoreGive(io->lock) == pdTRUE);
}

static inline esp_err_t hwss_io_spi_init(hwss_io_t *io){
    hwss_io_spi_t *io_spi=__containerof(io,hwss_io_spi_t,super);

    io_spi->cache=hwss_cache_dma_alloc(HWSS_IO_CACHE_SIZE);
    return spi_bus_add_device(io_spi->spi_host_id,&(io_spi->spi_config),&(io_spi->hdl));
}

static inline esp_err_t hwss_io_spi_deinit(hwss_io_t *io){
    hwss_io_spi_t *io_spi=__containerof(io,hwss_io_spi_t,super);

    esp_err_t ret=spi_bus_remove_device(io_spi->hdl);
    if(io_spi->cache)
        free(io_spi->cache);
    return ret;
}

static inline esp_err_t hwss_io_spi_cache_write(hwss_io_t *io, uint16_t cmd, uint64_t addr, const uint8_t *data, size_t len){
    esp_err_t ret=ESP_OK;
    hwss_io_spi_t *io_spi=__containerof(io,hwss_io_spi_t,super);

    if(len>HWSS_IO_CACHE_SIZE)
        return ESP_ERR_INVALID_SIZE;

    spi_transaction_t trans={
        .addr=addr,
        .cmd=cmd,
        .tx_buffer=io_spi->cache,
        .rx_buffer=NULL,
        .length=(len<<3)
    };

    if(!HWSS_IO_SPI_LOCK(io_spi))
        return ESP_ERR_TIMEOUT;
    memcpy(io_spi->cache,data,len);
    ret=spi_device_polling_transmit(io_spi->hdl,&trans);
    HWSS_IO_SPI_UNLOCK(io_spi);

    return ret;
}

static inline esp_err_t hwss_io_spi_cache_read(hwss_io_t *io, uint16_t cmd, uint64_t addr, uint8_t *data, size_t len){
    esp_err_t ret=ESP_OK;
    hwss_io_spi_t *io_spi=__containerof(io,hwss_io_spi_t,super);

    if(len>HWSS_IO_CACHE_SIZE)
        return ESP_ERR_INVALID_SIZE;

    spi_transaction_t trans={
        .addr=addr,
        .cmd=cmd,
        .tx_buffer=NULL,
        .rx_buffer=io_spi->cache,
        .length=(len<<3)
    };

    if(!HWSS_IO_SPI_LOCK(io_spi))
        return ESP_ERR_TIMEOUT;
    ret=spi_device_polling_transmit(io_spi->hdl,&trans);
    memcpy(data,io_spi->cache,len);
    HWSS_IO_SPI_UNLOCK(io_spi);

    return ret;
}

static inline esp_err_t hwss_io_spi_direct_write(hwss_io_t *io, uint16_t cmd, uint64_t addr, const uint8_t *data, size_t len){
    esp_err_t ret=ESP_OK;
    hwss_io_spi_t *io_spi=__containerof(io,hwss_io_spi_t,super);

    spi_transaction_t trans={
        .addr=addr,
        .cmd=cmd,
        .tx_buffer=data,
        .rx_buffer=NULL,
        .length=(len<<3)
    };

    if(!HWSS_IO_SPI_LOCK(io_spi))
        return ESP_ERR_TIMEOUT;
    ret=spi_device_polling_transmit(io_spi->hdl,&trans);
    HWSS_IO_SPI_UNLOCK(io_spi);

    return ret;
}

static inline esp_err_t hwss_io_spi_direct_read(hwss_io_t *io, uint16_t cmd, uint64_t addr, uint8_t *data, size_t len){
    esp_err_t ret=ESP_OK;
    hwss_io_spi_t *io_spi=__containerof(io,hwss_io_spi_t,super);

    spi_transaction_t trans={
        .addr=addr,
        .cmd=cmd,
        .tx_buffer=NULL,
        .rx_buffer=data,
        .length=(len<<3)
    };

    if(!HWSS_IO_SPI_LOCK(io_spi))
        return ESP_ERR_TIMEOUT;
    ret=spi_device_polling_transmit(io_spi->hdl,&trans);
    HWSS_IO_SPI_UNLOCK(io_spi);

    return ret;
}

static inline hwss_io_spi_t *hwss_io_new_spi(const hwss_io_spi_config_t* config){
    hwss_io_spi_t *io_spi=NULL;
    io_spi = (hwss_io_spi_t *) calloc(1,sizeof(hwss_io_spi_t));
    if(io_spi==NULL)
        return NULL;

    io_spi->spi_host_id= config->spi_host_id;
    io_spi->spi_config.clock_speed_hz=config->speed_khz*1000;
    io_spi->spi_config.spics_io_num=config->cs_io_num;
    io_spi->spi_config.queue_size=4;

    io_spi->lock=xSemaphoreCreateMutex();
    return io_spi;
}