#include <stdio.h>
#include "esp_log.h"
#include "hwss_phy_wiznet.h"
#include "hwss_io_wiznet.h"
#include "driver/gpio.h"
#include "drv_w5500.h"

#define GPIO_RST_PIN                    9

static const char *TAG="main";

void app_main(void)
{
    gpio_config_t gcfg={
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=1ull<<GPIO_RST_PIN
    };
    gpio_config(&gcfg);
    gpio_set_level(GPIO_RST_PIN,1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(GPIO_RST_PIN,0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(GPIO_RST_PIN,1);


    spi_bus_config_t bcfg={
        .sclk_io_num=12,
        .miso_io_num=13,
        .mosi_io_num=11,
        .quadhd_io_num=-1,
        .quadwp_io_num=-1,
    };

    spi_bus_initialize(SPI2_HOST,&bcfg,SPI_DMA_CH_AUTO);

    hwss_io_spi_config_t cfg={
        .spi_host_id=SPI2_HOST,
        .spi_devcfg={
            .clock_speed_hz=40*1000*1000,
            .spics_io_num=10,
            .queue_size=4
        }
    };

    hwss_io_t *io=hwss_io_new_w5500(HWSS_IO_SPI,&cfg);

    hwss_phy_config_t pcfg={
        .check_period_ms=10,
        .reset_timeout_ms=500
    };
    hwss_phy_t *phy=hwss_phy_new_w5500(io,&pcfg);

    io->init(io);
    phy->init(phy);


    uint8_t ver=0;
    W5500_getVERSIONR(io,&ver);
    ESP_LOGW(TAG,"Version:%X",ver);

    hwss_mac_addr_t mac=(uint8_t []){0x00,0x08,0xDC,0x02,0x03,0x04};
    hwss_mac_addr_t mac_res=malloc(6);

    W5500_getSHAR(io,mac_res);

    ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",mac_res[0],mac_res[1],mac_res[2],mac_res[3],mac_res[4],mac_res[5]);

    W5500_setSHAR(io,mac);

    W5500_getSHAR(io,mac_res);

    ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",mac_res[0],mac_res[1],mac_res[2],mac_res[3],mac_res[4],mac_res[5]);

    while(1){
        // W5500_getPHYCFGR(io,&phy_sta);
        // ESP_LOGI(TAG,"%X",phy_sta);
        // vTaskDelay(pdMS_TO_TICKS(200));
    }
}
