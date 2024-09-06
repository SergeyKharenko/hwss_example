#pragma once

#include <stdbool.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_err.h"

#include "hwss_type.h"

typedef struct{
    void *ctx;

    void *(*init)(const void* cfg);
    esp_err_t (*deinit)(void *ctx);

    esp_err_t (*read_register)(void *ctx, uint32_t cmd, uint32_t addr, uint8_t *data);
    esp_err_t (*write_register)(void *ctx, uint32_t cmd, uint32_t addr, uint8_t data);

    esp_err_t (*read_memory)(void *ctx, uint32_t cmd, uint32_t addr, void *data, uint32_t data_len);
    esp_err_t (*write_memory)(void *ctx, uint32_t cmd, uint32_t addr, void *data, uint32_t data_len);
}hwss_io_t;


typedef struct{
    spi_host_device_t spi_host_id;
    spi_device_interface_config_t spi_devcfg;
}hwss_io_spi_config_t;

typedef struct{
    uint8_t io_width;
    gpio_num_t io_nums[16];

    uint8_t addr_io_width;
    gpio_num_t addr_io_nums[8];

    gpio_num_t wr_io_num;
    gpio_num_t rd_io_num;
}hwss_io_parallel_config_t;

typedef struct{
    gpio_num_t tx_io_num;
    gpio_num_t rx_io_num;
    uart_port_t port;
    uart_config_t uart_cfg;
}hwss_io_uart_config_t;



