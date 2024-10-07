#pragma once

#include <stdbool.h>

#include "driver/dedic_gpio.h"


#include "driver/uart.h"

#include "esp_err.h"
#include "hwss_eth_sku.h"
#include "hwss_eth_type.h"

typedef struct hwss_io_s hwss_io_t;

struct hwss_io_s{
    esp_err_t (*init)(hwss_io_t *io);
    esp_err_t (*deinit)(hwss_io_t *io);

    esp_err_t (*read)(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data);
    esp_err_t (*write)(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data);

    esp_err_t (*read_buf)(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, size_t data_len);
    esp_err_t (*write_buf)(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, size_t data_len);

    esp_err_t (*read_mem)(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, size_t data_len);
    esp_err_t (*write_mem)(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, size_t data_len);
};



// typedef hwss_io_spi_config_t hwss_io_spi_pro_config_t;

// typedef struct{
//     uint8_t             io_width;
//     gpio_num_t          io_nums[16];

//     uint8_t             addr_io_width;
//     gpio_num_t          addr_io_nums[8];

//     gpio_num_t          wr_io_num;
//     gpio_num_t          rd_io_num;
// }hwss_io_parallel_config_t;

// typedef struct{
//     gpio_num_t          tx_io_num;
//     gpio_num_t          rx_io_num;

//     uart_port_t         port;
//     uart_config_t       uart_cfg;
// }hwss_io_uart_config_t;

static inline esp_err_t hwss_io_init(hwss_io_t *io){
    return io->init(io);
}

static inline esp_err_t hwss_io_deinit(hwss_io_t *io){
    return io->deinit(io);
}

static inline esp_err_t hwss_io_read(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data){
    return io->read(io,cmd,addr,data);
}

static inline esp_err_t hwss_io_write(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data){
    return io->write(io,cmd,addr,data);
}

static inline esp_err_t hwss_io_read_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, size_t data_len){
    return io->read_buf(io,cmd,addr,data,data_len);
}

static inline esp_err_t hwss_io_write_buf(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, size_t data_len){
    return io->write_buf(io,cmd,addr,data,data_len);
}

static inline esp_err_t hwss_io_read_mem(hwss_io_t *io, uint32_t cmd, uint32_t addr, uint8_t *data, size_t data_len){
    return io->read_mem(io,cmd,addr,data,data_len);
}

static inline esp_err_t hwss_io_write_mem(hwss_io_t *io, uint32_t cmd, uint32_t addr, const uint8_t *data, size_t data_len){
    return io->write_mem(io,cmd,addr,data,data_len);
}

hwss_io_t *hwss_io_new(hwss_eth_sku_t sku, hwss_io_type_t io_type, void *io_config);
