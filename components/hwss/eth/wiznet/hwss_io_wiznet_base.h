#pragma once
#include <sys/cdefs.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_check.h"
#include "hwss_io.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIZNET_IO_LOCK_TIMEOUT_MS                           20

typedef struct{
    hwss_io_t           super;
    spi_device_handle_t hdl;
    SemaphoreHandle_t   lock;
    
    spi_host_device_t   spi_host_id;
    spi_device_interface_config_t   spi_config;
}hwss_io_wiznet_spi_t;

inline bool IO_WIZNET_SPI_LOCK(hwss_io_wiznet_spi_t *io){
    return (xSemaphoreTake(io->lock,pdMS_TO_TICKS(WIZNET_IO_LOCK_TIMEOUT_MS)) == pdTRUE);
}

inline bool IO_WIZNET_SPI_UNLOCK(hwss_io_wiznet_spi_t *io){
    return (xSemaphoreGive(io->lock) == pdTRUE);
}

inline esp_err_t hwss_io_wiznet_spi_init(hwss_io_t *io){
    hwss_io_wiznet_spi_t *io_wiznet_spi=__containerof(io,hwss_io_wiznet_spi_t,super);

    return spi_bus_add_device(io_wiznet_spi->spi_host_id,&(io_wiznet_spi->spi_config),&(io_wiznet_spi->hdl));
}

inline esp_err_t hwss_io_wiznet_spi_deinit(hwss_io_t *io){
    hwss_io_wiznet_spi_t *io_wiznet_spi=__containerof(io,hwss_io_wiznet_spi_t,super);
    return spi_bus_remove_device(io_wiznet_spi->hdl);
}

inline esp_err_t hwss_io_wiznet_spi_trans(hwss_io_t *io, spi_transaction_t *trans){
    esp_err_t ret=ESP_OK;
    hwss_io_wiznet_spi_t *io_wiznet_spi=__containerof(io,hwss_io_wiznet_spi_t,super);

    if(!IO_WIZNET_SPI_LOCK(io_wiznet_spi))
        return ESP_ERR_TIMEOUT;
    ret=spi_device_polling_transmit(io_wiznet_spi->hdl,trans);
    IO_WIZNET_SPI_UNLOCK(io_wiznet_spi);

    return ret;
}

inline hwss_io_wiznet_spi_t *hwss_io_new_wiznet_spi(const hwss_io_spi_config_t* config){
    hwss_io_wiznet_spi_t *io_wiznet_spi=NULL;
    io_wiznet_spi = (hwss_io_wiznet_spi_t *) calloc(1,sizeof(hwss_io_wiznet_spi_t));
    if(io_wiznet_spi==NULL)
        return NULL;

    io_wiznet_spi->spi_host_id= config->spi_host_id;
    io_wiznet_spi->spi_config.clock_speed_hz=config->speed_khz*1000;
    io_wiznet_spi->spi_config.spics_io_num=config->cs_io_num;
    io_wiznet_spi->spi_config.queue_size=4;

    io_wiznet_spi->lock=xSemaphoreCreateMutex();
    return io_wiznet_spi;
}

typedef struct{
    hwss_io_t           super;
    uint8_t             io_width;
    gpio_num_t          io_nums[16];

    uint8_t             addr_io_width;
    gpio_num_t          addr_io_nums[8];

    gpio_num_t          wr_io_num;
    gpio_num_t          rd_io_num;

    dedic_gpio_bundle_handle_t data_hdl;
    dedic_gpio_bundle_handle_t ctrl_hdl;
    SemaphoreHandle_t   lock;
}hwss_io_wiznet_fast_parallel_t;


inline bool IO_WIZNET_FAST_PARALLEL_LOCK(hwss_io_wiznet_fast_parallel_t *io){
    return (xSemaphoreTake(io->lock,pdMS_TO_TICKS(WIZNET_IO_LOCK_TIMEOUT_MS)) == pdTRUE);
}

inline bool IO_WIZNET_FAST_PARALLEL_UNLOCK(hwss_io_wiznet_fast_parallel_t *io){
    return (xSemaphoreGive(io->lock) == pdTRUE);
}

#ifdef __cplusplus
}
#endif
