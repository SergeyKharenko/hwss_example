#include <stdio.h>
#include <rom/ets_sys.h>
#include "esp_log.h"
#include "hwss_hnet_wiznet.h"
#include "hwss_mac_wiznet.h"
#include "hwss_phy_wiznet.h"
#include "hwss_io_wiznet.h"
#include "hwss_event.h"
#include "driver/gpio.h"
#include "drv_w5500.h"
#include "drv_w5100s.h"

#define GPIO_RST_PIN                    9

static const char *TAG="main";



hwss_io_spi_config_t cfg={
    .spi_host_id=SPI2_HOST,
    .cs_io_num=10,
    .speed_khz=20*1000
};

hwss_phy_config_t pcfg={
    .check_period_ms=10,
    .reset_timeout_ms=500
};

hwss_mac_config_t mcfg={
    .addr={0x00,0x08,0xDC,0x01,0x02,0x03},
    .use_burnin_addr=false
};

hwss_hnet_config_t ncfg={
    .check_state_period_ms=50,
    .retry_cnt=8,
    .retry_time_ms=200,
    .ppp_cp_magic_num=0,
    .ppp_cp_request_time_ms=1000
};

static void hwss_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data){
    switch (event_id)
    {
    case HWSS_EVENT_CONNECTED:
        ESP_LOGI(TAG,"CONNECTED!");
        break;
    
    case HWSS_EVENT_DISCONNECTED:
        ESP_LOGW(TAG,"DISCONNECTED!");
        break;

    default:
        break;
    }

}

void app_main(void)
{
    gpio_config_t gcfg={
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=1ull<<GPIO_RST_PIN
    };
    gpio_config(&gcfg);
    gpio_set_level(GPIO_RST_PIN,1);
    ets_delay_us(1);
    gpio_set_level(GPIO_RST_PIN,0);
    ets_delay_us(1);
    gpio_set_level(GPIO_RST_PIN,1);
    vTaskDelay(pdMS_TO_TICKS(100));


    spi_bus_config_t bcfg={
        .sclk_io_num=12,
        .miso_io_num=13,
        .mosi_io_num=11,
        .quadhd_io_num=-1,
        .quadwp_io_num=-1,
    };

    spi_bus_initialize(SPI2_HOST,&bcfg,SPI_DMA_CH_AUTO);

    /*
    hwss_io_t *io=hwss_io_new_w5500(HWSS_IO_SPI,&cfg);
    hwss_phy_t *phy=hwss_phy_new_w5500(io,&pcfg);
    hwss_mac_t *mac=hwss_mac_new_w5500(io,&mcfg);
    hwss_hnet_t *hnet=hwss_hnet_new_w5500(io,&ncfg);

    hwss_event_loop_create();
    // hwss_event_handler_register(HWSS_EVENT,HWSS_EVENT_ANY_ID,hwss_event_handler,NULL);

    io->init(io);
    phy->init(phy);
    mac->init(mac);
    hnet->init(hnet);
    
    // uint8_t ver=0;
    // W5500_getVERSIONR(io,&ver);
    // ESP_LOGW(TAG,"Version:%X",ver);

    // hwss_mac_addr_t mac=(uint8_t []){0x00,0x08,0xDC,0x02,0x03,0x04};
    hwss_mac_addr_t mac_res;
    mac->get_addr(mac,mac_res);
    // W5500_getSHAR(io,mac_res);

    ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",mac_res[0],mac_res[1],mac_res[2],mac_res[3],mac_res[4],mac_res[5]);

    hwss_ip_addr_t ip={192,168,0,10};
    hwss_ip_addr_t gip={192,168,0,1};
    hwss_ip_addr_t mask={255,255,255,0};

    hnet->set_source_addr(hnet,ip);
    hnet->set_gateway_addr(hnet,gip);
    hnet->set_subnet_mask(hnet,mask);

    */
    uint8_t ver;
    hwss_io_t *io=hwss_io_new_w5100s(HWSS_IO_SPI,&cfg);
    io->init(io);
    W5100S_getRMSR(io, &ver);
    ESP_LOGI(TAG,"VER: %X",ver);


    // W5500_setSHAR(io,mac);

    // W5500_getSHAR(io,mac_res);

    // ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",mac_res[0],mac_res[1],mac_res[2],mac_res[3],mac_res[4],mac_res[5]);
    // uint8_t phy_sta;
    while(1){
        // W5500_getPHYCFGR(io,&phy_sta);
        // ESP_LOGI(TAG,"%X",phy_sta);
        // vTaskDelay(pdMS_TO_TICKS(200));
    }
}
