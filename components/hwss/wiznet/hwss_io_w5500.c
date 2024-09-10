#include <sys/cdefs.h>
#include "esp_check.h"
#include "freertos/semphr.h"

#include "w5100.h"
#include "hwss_io_wiznet.h"

HWSS_IO_TYPE_LIST(W5500_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_TYPE_END};
HWSS_IO_TYPE_LIST(W5300_IO_SUPPORT_LIST)={HWSS_IO_PARALLEL_8, HWSS_IO_PARALLEL_16, HWSS_IO_TYPE_END};
HWSS_IO_TYPE_LIST(W5100_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_PARALLEL_8,HWSS_IO_TYPE_END};
HWSS_IO_TYPE_LIST(W5100S_IO_SUPPORT_LIST)={HWSS_IO_SPI,HWSS_IO_PARALLEL_8,HWSS_IO_TYPE_END};

#define WIZNET_IO_LOCK_TIMEOUT_MS                           20

static const char *TAG="w5500.hwss_io";

typedef struct{
    hwss_io_t super;
    spi_device_handle_t hdl;
    SemaphoreHandle_t lock;
    hwss_io_spi_config_t cfg;
}hwss_io_w5500_spi_t;

static inline bool IO_W5500_SPI_LOCK(hwss_io_w5500_spi_t *io){
    return xSemaphoreTake(io->lock,pdMS_TO_TICKS(WIZNET_IO_LOCK_TIMEOUT_MS));
}

static inline bool IO_W5500_SPI_UNLOCK(hwss_io_w5500_spi_t *io){
    return xSemaphoreGive(io->lock);
}

static esp_err_t hwss_io_w5500_spi_init(hwss_io_t *io){
    hwss_io_w5500_spi_t *io_w5500_spi=__containerof(io,hwss_io_w5500_spi_t,super);
    return spi_bus_add_device(io_w5500_spi->cfg.spi_host_id,io_w5500_spi->cfg.spi_devcfg,&(io_w5500_spi->hdl));
}

static esp_err_t hwss_io_w5500_spi_deinit(hwss_io_t *io){
    hwss_io_w5500_spi_t *io_w5500_spi=__containerof(io,hwss_io_w5500_spi_t,super);
    return spi_bus_remove_device(io_w5500_spi->hdl);
}

static esp_err_t hwss_io_w5500_spi_read(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data){
    esp_err_t ret = ESP_OK;
    hwss_io_w5500_spi_t *io_w5500_spi=__containerof(io,hwss_io_w5500_spi_t,super);

    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_),
        .length = 8,
        .rx_buffer = data
    };

    if(IO_WIZNET_SPI_LOCK(io_w5500_spi)){
        if(spi_device_polling_transmit(io_w5500_spi->hdl,&trans)!=OK){
            ESP_LOGE(TAG, "%s(%d): read reg failed", __FUNCTION__, __LINE__);
            ret =  ESP_FAIL;
        }
        IO_WIZNET_SPI_UNLOCK(io_w5500_spi);
    }
    else{
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

static esp_err_t hwss_io_w5500_spi_write(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data){
    esp_err_t ret = ESP_OK;
    hwss_io_w5500_spi_t *io_w5500_spi=__containerof(io,hwss_io_w5500_spi_t,super);

    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_),
        .length = 8,
        .tx_buffer = data
    };

    if(IO_WIZNET_SPI_LOCK(io_w5500_spi)){
        if(spi_device_polling_transmit(io_w5500_spi->hdl,&trans)!=OK){
            ESP_LOGE(TAG, "%s(%d): write reg failed", __FUNCTION__, __LINE__);
            ret =  ESP_FAIL;
        }
        IO_WIZNET_SPI_UNLOCK(io_w5500_spi);
    }
    else{
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

static esp_err_t hwss_io_w5500_spi_read_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, uint32_t data_len){
    esp_err_t ret = ESP_OK;
    hwss_io_w5500_spi_t *io_w5500_spi=__containerof(io,hwss_io_w5500_spi_t,super);

    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_),
        .length = 8 * data_len,
        .rx_buffer = data
    };

    if(IO_WIZNET_SPI_LOCK(io_w5500_spi)){
        if(spi_device_polling_transmit(io_w5500_spi->hdl,&trans)!=OK){
            ESP_LOGE(TAG, "%s(%d): read reg failed", __FUNCTION__, __LINE__);
            ret =  ESP_FAIL;
        }
        IO_WIZNET_SPI_UNLOCK(io_w5500_spi);
    }
    else{
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

static esp_err_t hwss_io_w5500_spi_write_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, uint32_t data_len){
    esp_err_t ret = ESP_OK;
    hwss_io_w5500_spi_t *io_w5500_spi=__containerof(io,hwss_io_w5500_spi_t,super);

    spi_transaction_t trans = {
        .addr= addr | (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_),
        .length = 8 * data_len,
        .tx_buffer = data
    };

    if(IO_WIZNET_SPI_LOCK(io_w5500_spi)){
        if(spi_device_polling_transmit(io_w5500_spi->hdl,&trans)!=OK){
            ESP_LOGE(TAG, "%s(%d): write reg failed", __FUNCTION__, __LINE__);
            ret =  ESP_FAIL;
        }
        IO_WIZNET_SPI_UNLOCK(io_w5500_spi);
    }
    else{
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}


static hwss_io_t *hwss_io_new_w5500_spi(hwss_io_spi_config_t* config){
    hwss_io_w5500_spi_t *io_w5500_spi=NULL;

    io_w5500_spi = calloc(1,sizeof(hwss_io_w5500_spi_t));
    ESP_GOTO_ON_FALSE(io_w5500_spi, NULL, err, TAG, "calloc io_wiznet_spi failed");

    config->spi_devcfg.command_bits=0;
    config->spi_devcfg.address_bits=24;
    config->spi_devcfg.dummy_bits=0;
    memcpy(&io_w5500_spi->cfg,config,sizeof(hwss_io_wiznet_spi_t));

    io_w5500_spi->super->init=hwss_io_w5500_spi_init;
    io_w5500_spi->super->deinit=hwss_io_w5500_spi_deinit;
    io_w5500_spi->super->read=hwss_io_w5500_spi_read;
    io_w5500_spi->super->write=hwss_io_w5500_spi_write;
    io_w5500_spi->super->read_buf=hwss_io_w5500_spi_read_buf;
    io_w5500_spi->super->write_buf=hwss_io_w5500_spi_write_buf;
    return &(io_w5500_spi->super);

err:
    if(io_w5500_spi!=NULL){
        free(io_w5500_spi);
    }
    return NULL;
}

hwss_io_t *hwss_io_new_w5500(hwss_io_type type, void *io_config){
    ESP_GOTO_ON_FALSE(hwss_io_type_supported(W5500_IO_SUPPORT_LIST,type),NULL,err,TAG,"unsupported io type");
    ESP_GOTO_ON_FALSE(io_config, NULL, err, TAG, "cannot set io config to null");
    return hwss_io_new_w5500_spi(io_config);
err:
    return NULL;
}

