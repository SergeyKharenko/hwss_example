#include "machine/endian.h"
#include "driver/../../src/gpspi/spi_master.c"

#include "hwss_io_spi_pro.h"

#if BYTE_ORDER==LITTLE_ENDIAN
#define SPI_PRO_HTON16(v)                                       __builtin_bswap16(v)
#define SPI_PRO_HTON64(v)                                       __builtin_bswap64(v)
#else
#define SPI_PRO_HTON16(v)                                       (v)
#define SPI_PRO_HTON64(v)                                       (v)
#endif

static const char *TAG="hwss_io_spi_pro";

static inline void SPI_PRO_DELAY_TICKS(uint32_t ticks){
    while(ticks--);
};

static esp_err_t SPI_MASTER_ISR_ATTR hwss_io_spi_pro_setup(hwss_io_spi_pro_t *io_spi_pro, spi_transaction_t *trans_desc){
    esp_err_t ret;
    ret = check_trans_valid(io_spi_pro->hdl, trans_desc);
    if (ret != ESP_OK) {
        return ret;
    }
    SPI_CHECK(!spi_bus_device_is_polling(io_spi_pro->hdl), "Cannot send polling transaction while the previous polling transaction is not terminated.", ESP_ERR_INVALID_STATE);

    spi_host_t *host = io_spi_pro->hdl->host;
    spi_trans_priv_t priv_polling_trans = { .trans = trans_desc, };
    ret = setup_priv_desc(host, &priv_polling_trans);
    if (ret != ESP_OK) {
        return ret;
    }

    /* If device_acquiring_lock is set to handle, it means that the user has already
     * acquired the bus thanks to the function `spi_device_acquire_bus()`.
     * In that case, we don't need to take the lock again. */
    if (host->device_acquiring_lock != io_spi_pro->hdl) {
        /* The user cannot ask for the CS to keep active has the bus is not locked/acquired. */
        if ((trans_desc->flags & SPI_TRANS_CS_KEEP_ACTIVE) != 0) {
            ret = ESP_ERR_INVALID_ARG;
        } else {
            ret = spi_bus_lock_acquire_start(handle->dev_lock, portMAX_DELAY);
        }
    } else {
        ret = spi_bus_lock_wait_bg_done(handle->dev_lock, portMAX_DELAY);
    }
    if (ret != ESP_OK) {
        uninstall_priv_desc(&priv_polling_trans);
        ESP_LOGE(SPI_TAG, "polling can't get buslock");
        return ret;
    }
    //After holding the buslock, common resource can be accessed !!

    //Polling, no interrupt is used.
    host->polling = true;
    host->cur_trans_buf = priv_polling_trans;

    ESP_LOGV(SPI_TAG, "polling trans");

    spi_hal_dev_config_t *hal_dev = &(((spi_dev_t *)io_spi_pro->dev)->hal_dev);

    dev->host->cur_cs = dev->id;

    //Reconfigure according to device settings, the function only has effect when the dev_id is changed.
    spi_setup_device(dev);

    //set the transaction specific configuration each time before a transaction setup
    spi_hal_trans_config_t hal_trans = {};
    spi_format_hal_trans_struct(dev, &priv_polling_trans, &hal_trans);
    spi_hal_setup_trans(hal, hal_dev, &hal_trans);
    s_spi_prepare_data(dev, &hal_trans);
    return ESP_OK;
}

static inline void hwss_io_spi_pro_update_bitlen(hwss_io_spi_pro_t *io_spi_pro, size_t bitlen){
    ((spi_hal_context_t *)io_spi_pro->hal)->trans_config.tx_bitlen=bitlen;
    ((spi_hal_context_t *)io_spi_pro->hal)->trans_config.rx_bitlen=bitlen;
    spi_ll_set_mosi_bitlen(io_spi_pro->hw, bitlen);
    spi_ll_set_miso_bitlen(io_spi_pro->hw, bitlen);
}

static inline void hwss_io_spi_pro_prepare_data(hwss_io_spi_pro_t *io_spi_pro, uint8_t *tx, uint8_t *rx){
    ((spi_device_t *)io_spi_pro->dev)->host->hal.trans_config.rcv_buffer=rx;
    ((spi_device_t *)io_spi_pro->dev)->host->hal.trans_config.send_buffer=tx;
    s_spi_prepare_data(io_spi_pro->dev,&(dev->host->hal.trans_config));
}

static esp_err_t SPI_MASTER_ISR_ATTR hwss_io_spi_pro_write_head(hwss_io_spi_pro_t *io_spi_pro, uint16_t cmd, uint64_t addr, hwss_io_spi_pro_gap_t gap){
    esp_err_t ret=ESP_OK;

    uint8_t bias=0;
    if(io_spi_pro->cmd_bytes!=0){
        if(io_spi_pro->cmd_bytes==1)
            io_spi_pro->cache[0]=(uint8_t)cmd;
        else{
            cmd=SPI_PRO_HTON16(cmd);
            memcpy(io_spi_pro->cache,&cmd,2);
        }
    }

    bias+=io_spi_pro->cmd_bytes;

    if(io_spi_pro->addr_bytes!=0){
        addr=SPI_PRO_HTON64(addr);
        memcpy(io_spi_pro->cache+bias,((uint8_t *)&addr)+sizeof(uint64_t)-io_spi_pro->addr_bytes,io_spi_pro->addr_bytes);
    }
    
    bias+=io_spi_pro->addr_bytes;
    
    if(bias==0)
        return ESP_OK;

    spi_transaction_ext_t trans={
        .base={
            .flags=SPI_TRANS_CS_KEEP_ACTIVE,
        },
        .address_bits=0,
        .command_bits=0,
        .dummy_bits=0
    };

    ESP_GOTO_ON_ERROR(hwss_io_spi_pro_setup(io_spi_pro->hdl,&trans),err,TAG,"cannot setup spi trans");

    spi_hal_context_t *hal = &(io_spi_pro->hdl->host->hal);
    if(gap&HWSS_IO_SPI_PRO_GAP_HH){
        hwss_io_spi_pro_update_bitlen(io_spi_pro->hdl,8);
        for(uint8_t i=0;i<bias-1;i++){
            hwss_io_spi_pro_prepare_data(io_spi_pro->hdl,io_spi_pro->cache+i,NULL);
            spi_hal_user_start(hal);
            while (!spi_hal_usr_is_done(hal));
            if(io_spi_pro->delay_hh)
                SPI_PRO_DELAY_TICKS(io_spi_pro->delay_hh);
        }
        hwss_io_spi_pro_prepare_data(io_spi_pro->hdl,io_spi_pro->cache+bias-1,NULL);
        spi_hal_user_start(hal);
        while (!spi_hal_usr_is_done(hal));
    }
    else{
        hwss_io_spi_pro_update_bitlen(io_spi_pro->hdl,bias<<3);
        hwss_io_spi_pro_prepare_data(io_spi_pro->hdl,io_spi_pro->cache,NULL);
        spi_hal_user_start(hal);
        while (!spi_hal_usr_is_done(hal));
    }

err:
    return ret;
}

static esp_err_t SPI_MASTER_ISR_ATTR hwss_io_spi_pro_cache_write_data(hwss_io_spi_pro_t *io_spi_pro, const uint8_t *data, 
                                                                            size_t len, hwss_io_spi_pro_gap_t gap){
    spi_hal_context_t *hal = &(io_spi_pro->hdl->host->hal);
    spi_dev_t *hw = hal->hw;
    
    memcpy(io_spi_pro->cache,data,len);

    if(gap&HWSS_IO_SPI_PRO_GAP_DD){
        hwss_io_spi_pro_update_bitlen(io_spi_pro->hdl,8);
        for(size_t i=0;i<len-1;i++){
            hwss_io_spi_pro_prepare_data(io_spi_pro->hdl,io_spi_pro->cache+i,NULL);
            spi_hal_user_start(hal);
            while (!spi_hal_usr_is_done(hal));
            if(io_spi_pro->delay_dd)
                SPI_PRO_DELAY_TICKS(io_spi_pro->delay_dd);
        }
        spi_ll_master_keep_cs(hw,0);
        hwss_io_spi_pro_prepare_data(io_spi_pro->hdl,io_spi_pro->cache+len-1,NULL);
        spi_hal_user_start(hal);
        while (!spi_hal_usr_is_done(hal));
    }
    else{
        spi_ll_master_keep_cs(hw,0);
        hwss_io_spi_pro_update_bitlen(io_spi_pro->hdl,len<<3);
        hwss_io_spi_pro_prepare_data(io_spi_pro->hdl,io_spi_pro->cache,NULL);

        spi_hal_user_start(hal);
        while (!spi_hal_usr_is_done(hal));
    }

    return ESP_OK;
}

esp_err_t SPI_MASTER_ISR_ATTR hwss_io_spi_pro_cache_read_data(hwss_io_spi_pro_t *io_spi_pro, uint8_t *data, size_t len, hwss_io_spi_pro_gap_t gap){
    esp_err_t ret=ESP_OK;

    if(gap&HWSS_IO_SPI_PRO_GAP_DD){
        
    }
    else{
        if(io_spi_pro->delay_dd){
            
        }
        else{
            
        }
    }

    return ret;
}

esp_err_t hwss_io_spi_pro_init(hwss_io_t *io){
    hwss_io_spi_pro_t *io_spi_pro=__containerof(io,hwss_io_spi_pro_t,super);

    io_spi_pro->cache=hwss_cache_dma_alloc(HWSS_IO_CACHE_SIZE);
    io_spi_pro->cmd_bytes=io_spi_pro->spi_config.command_bits>>8;
    io_spi_pro->addr_bytes=io_spi_pro->spi_config.address_bits>>8;
    
    
    ESP_RETURN_ON_ERROR(spi_bus_add_device(io_spi_pro->spi_host_id,&(io_spi_pro->spi_config),&(io_spi_pro->hdl)),
                        TAG,"fail to init spi_pro");
    io_spi_pro->dev=&(io_spi_pro->hdl);
    io_spi_pro->hal=&(io_spi_pro->hdl->host->hal);
    io_spi_pro->hw=((spi_hal_context_t *)io_spi_pro->hal)->hw;

    return ESP_OK;
}

esp_err_t hwss_io_spi_pro_deinit(hwss_io_t *io){
    hwss_io_spi_pro_t *io_spi_pro=__containerof(io,hwss_io_spi_pro_t,super);

    esp_err_t ret=spi_bus_remove_device(io_spi_pro->hdl);
    if(io_spi_pro->cache)
        free(io_spi_pro->cache);
    return ret;
}
