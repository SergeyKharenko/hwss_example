#pragma once
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_rom_sys.h"

#include "hwss_io.h"
#include "hwss_cache.h"
#include "hwss_eth_opt.h"

typedef enum{
    HWSS_IO_SPI_PRO_NOGAP=0x00,
    HWSS_IO_SPI_PRO_GAP_DD=0x01,
    HWSS_IO_SPI_PRO_GAP_HH=0x02,
    HWSS_IO_SPI_PRO_GAP_HD=0x04,
}hwss_io_spi_pro_gap_t;

typedef struct{
    spi_host_device_t       spi_host_id;
    gpio_num_t              cs_io_num;
    uint32_t                speed_khz;

    uint8_t                 head_size;
}hwss_io_spi_pro_config_t;

typedef struct{
    hwss_io_t           super;
    spi_device_handle_t hdl;
    SemaphoreHandle_t   lock;
    
    spi_host_device_t               spi_host_id;
    spi_device_interface_config_t   spi_config;

    uint8_t             *cache;

    uint8_t             cmd_bytes;
    uint8_t             addr_bytes;

    uint32_t             delay_hh;
    uint32_t             delay_hd;
    uint32_t             delay_dd;
}hwss_io_spi_pro_t;

void hwss_io_spi_pro_inter_init(hwss_io_spi_pro_t *io_spi_pro);

esp_err_t hwss_io_spi_pro_write_head(hwss_io_spi_pro_t *io_spi_pro, uint16_t cmd, uint64_t addr, hwss_io_spi_pro_gap_t gap);
esp_err_t hwss_io_spi_pro_write_data(hwss_io_spi_pro_t *io_spi_pro, const uint8_t *data, size_t len, hwss_io_spi_pro_gap_t gap);
esp_err_t hwss_io_spi_pro_read_data(hwss_io_spi_pro_t *io_spi_pro, uint8_t *data, size_t len, hwss_io_spi_pro_gap_t gap);

static inline bool HWSS_IO_SPI_PRO_LOCK(hwss_io_spi_pro_t *io){
    return (xSemaphoreTake(io->lock,pdMS_TO_TICKS(HWSS_IO_LOCK_TIMEOUT_MS)) == pdTRUE);
}

static inline bool HWSS_IO_SPI_PRO_UNLOCK(hwss_io_spi_pro_t *io){
    return (xSemaphoreGive(io->lock) == pdTRUE);
}

static inline esp_err_t hwss_io_spi_pro_init(hwss_io_t *io){
    hwss_io_spi_pro_t *io_spi_pro=__containerof(io,hwss_io_spi_pro_t,super);

    io_spi_pro->cache=hwss_cache_dma_alloc(HWSS_IO_CACHE_SIZE);
    io_spi_pro->cmd_bytes=io_spi_pro->spi_config.command_bits>>8;
    io_spi_pro->addr_bytes=io_spi_pro->spi_config.address_bits>>8;
    
    return spi_bus_add_device(io_spi_pro->spi_host_id,&(io_spi_pro->spi_config),&(io_spi_pro->hdl));
}

static inline esp_err_t hwss_io_spi_pro_deinit(hwss_io_t *io){
    hwss_io_spi_pro_t *io_spi_pro=__containerof(io,hwss_io_spi_pro_t,super);

    esp_err_t ret=spi_bus_remove_device(io_spi_pro->hdl);
    if(io_spi_pro->cache)
        free(io_spi_pro->cache);
    return ret;
}

static inline esp_err_t hwss_io_spi_pro_cache_write(hwss_io_t *io, uint16_t cmd, uint64_t addr, const uint8_t *data, size_t len, 
                                                    hwss_io_spi_pro_gap_t gap){
    esp_err_t ret=ESP_OK;
    hwss_io_spi_pro_t *io_spi_pro=__containerof(io,hwss_io_spi_pro_t,super);

    if(len>HWSS_IO_CACHE_SIZE)
        return ESP_ERR_INVALID_SIZE;

    if(gap==HWSS_IO_SPI_PRO_NOGAP){
        spi_transaction_t trans={
            .addr=addr,
            .cmd=cmd,
            .tx_buffer=io_spi_pro->cache,
            .rx_buffer=NULL,
            .length=(len<<3)
        };

        if(!HWSS_IO_SPI_PRO_LOCK(io_spi_pro))
            return ESP_ERR_TIMEOUT;
        memcpy(io_spi_pro->cache,data,len);
        ret=spi_device_polling_transmit(io_spi_pro->hdl,&trans);
        HWSS_IO_SPI_PRO_UNLOCK(io_spi_pro);
    }
    else{
        // spi_device_t *dev=io_spi_pro->hdl;

        // spi_device_acquire_bus(dev,portMAX_DELAY);
        // HWSS_IO_SPI_PRO_LOCK(io_spi_pro);

        // host->polling = true;

        // ESP_LOGV(SPI_TAG, "polling trans");
        // dev->host->cur_cs = dev->id;

        // if(gap&HWSS_IO_SPI_PRO_GAP_HH){
            
        // }
        // else{

        // }

        // if(gap&HWSS_IO_SPI_PRO_GAP_HD)
        //     esp_rom_delay_us(io_spi_pro->delay_hd);

        
        // if(gap&HWSS_IO_SPI_PRO_GAP_DD){
            

        // }
        // else{
            
            
        // }

        // HWSS_IO_SPI_PRO_UNLOCK(io_spi_pro);
        // spi_device_release_bus(dev);
    }
    return ret;
}

static inline esp_err_t hwss_io_spi_pro_cache_read(hwss_io_t *io, uint16_t cmd, uint64_t addr, uint8_t *data, size_t len,
                                                    hwss_io_spi_pro_gap_t gap){
    esp_err_t ret=ESP_OK;
    hwss_io_spi_pro_t *io_spi_pro=__containerof(io,hwss_io_spi_pro_t,super);

    if(len>HWSS_IO_CACHE_SIZE)
        return ESP_ERR_INVALID_SIZE;

    spi_transaction_t trans={
        .addr=addr,
        .cmd=cmd,
        .tx_buffer=NULL,
        .rx_buffer=io_spi_pro->cache,
        .length=(len<<3)
    };

    if(!HWSS_IO_SPI_PRO_LOCK(io_spi_pro))
        return ESP_ERR_TIMEOUT;
    ret=spi_device_polling_transmit(io_spi_pro->hdl,&trans);
    memcpy(data,io_spi_pro->cache,len);
    HWSS_IO_SPI_PRO_UNLOCK(io_spi_pro);

    return ret;
}

static inline hwss_io_spi_pro_t *hwss_io_new_spi_pro(const hwss_io_spi_pro_config_t* config){
    hwss_io_spi_pro_t *io_spi_pro=NULL;
    io_spi_pro = (hwss_io_spi_pro_t *) calloc(1,sizeof(hwss_io_spi_pro_t));
    if(io_spi_pro==NULL)
        return NULL;

    io_spi_pro->spi_host_id= config->spi_host_id;
    io_spi_pro->spi_config.clock_speed_hz=config->speed_khz*1000;
    io_spi_pro->spi_config.spics_io_num=config->cs_io_num;
    io_spi_pro->spi_config.queue_size=4;

    io_spi_pro->lock=xSemaphoreCreateMutex();

    return io_spi_pro;
}