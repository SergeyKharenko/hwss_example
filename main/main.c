#include <stdio.h>
#include <rom/ets_sys.h>
#include "esp_log.h"
#include "hwss_hso_wiznet.h"
#include "hwss_hnet_wiznet.h"
#include "hwss_mac_wiznet.h"
#include "hwss_phy_wiznet.h"
#include "hwss_io_wiznet.h"
#include "hwss_hsl_wiznet.h"
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
    .addr={0x00,0x08,0xDC,0x01,0x02,0x04},
    .use_burnin_addr=false
};

hwss_hnet_config_t ncfg={
    .check_state_period_ms=50,
    .retry_cnt=8,
    .retry_time_ms=200,
    .ppp_cp_magic_num=0,
    .ppp_cp_request_time_ms=1000
};

hwss_hsl_config_t lcfg={
    .retry_cnt=0,
    .retry_time_ms=100,
    .check_state_period_ms=10
};

hwss_io_t *io;
hwss_hir_t *hir;
hwss_phy_t *phy;
hwss_mac_t *mac;
hwss_hnet_t *hnet;
hwss_hso_t *hso;
hwss_hsl_t *hsl;

hwss_mac_addr_t dest_mac;

static void hwss_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data){
    switch (event_id)
    {
    case HWSS_INTER_EVENT_PHY_CONNECT:
        ESP_LOGI(TAG,"CONNECTED!");
        hnet->start(hnet);
        hso->start(hso);
        break;
    
    case HWSS_INTER_EVENT_PHY_DISCONN:
        ESP_LOGW(TAG,"DISCONNECTED!");
        hnet->stop(hnet);
        hso->stop(hso);
        break;

    case HWSS_INTER_EVENT_HSL_PING:
        hsl->get_peer_mac_addr(hsl,dest_mac);
        ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",dest_mac[0],dest_mac[1],dest_mac[2],
                    dest_mac[3],dest_mac[4],dest_mac[5]);
        break;
    
    case HWSS_INTER_EVENT_HSL_TIMEOUT:
        ESP_LOGW(TAG,"PING FAIL!");
        break;

    default:
        ESP_LOGE(TAG,"CCC");
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

    hwss_hir_config_t ircfg={
        .io_num=14,
        .tri=HWSS_TRIGGER_NEGEDGE
    };

    static uint8_t sz_cfg[]={2,2,2,2};

    hwss_hso_config_t scfg={
        .en_sock_num=4,
        .rxbuf_size_kb=sz_cfg,
        .txbuf_size_kb=sz_cfg,
        .irq_inter_time_us=10,
        .sock_active_threshold_ms=1000,
        .sock_polling_period_ms=5
    };

    // hwss_io_t *io=hwss_io_new_w5500(HWSS_IO_SPI,&cfg);
    // hwss_phy_t *phy=hwss_phy_new_w5500(io,&pcfg);
    // hwss_mac_t *mac=hwss_mac_new_w5500(io,&mcfg);
    // hwss_hnet_t *hnet=hwss_hnet_new_w5500(io,&ncfg);

    esp_event_loop_args_t largs={
        .queue_size=16,
        .task_core_id=tskNO_AFFINITY,
        .task_name="BASIC LOOP",
        .task_priority=10,
        .task_stack_size=8192
    };

    esp_event_loop_handle_t hdl;
    esp_event_loop_create(&largs,&hdl);
    
    io=hwss_io_new_w5100s(HWSS_IO_SPI,&cfg);
    hir=hwss_hir_new(hdl,&ircfg);
    phy=hwss_phy_new_w5100s(hdl,io,&pcfg);
    mac=hwss_mac_new_w5100s(io,&mcfg);
    hnet=hwss_hnet_new_w5100s(hdl,io,&ncfg);
    hso=hwss_hso_new_w5100s(hdl,io,hir,&scfg);
    hsl=hwss_hsl_new_w5100s(hdl,io,&lcfg);

    // io=hwss_io_new_w5500(HWSS_IO_SPI,&cfg);
    // hir=hwss_hir_new(hdl,&ircfg);
    // phy=hwss_phy_new_w5500(hdl,io,&pcfg);
    // mac=hwss_mac_new_w5500(io,&mcfg);
    // hnet=hwss_hnet_new_w5500(hdl,io,&ncfg);
    // hso=hwss_hso_new_w5500(hdl,io,hir,&scfg);

    esp_event_handler_register_with(hdl,HWSS_INTER_EVENT,HWSS_EVENT_ANY_ID,hwss_event_handler,NULL);

    io->init(io);
    phy->init(phy);
    mac->init(mac);
    hnet->init(hnet);
    hso->init(hso);
    hsl->init(hsl);
    
    phy->start(phy);
    // hsl->start(hsl);

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

    hwss_proto_t pro=HWSS_PROTO_UDP;
    hso->set_sock_proto(hso,0,&pro);
    hso->ctrl_sock(hso,0,HWSS_HSO_SOCKCTRL_OPEN);

    // hwss_ip_addr_t pip={192,168,0,24};
    // uint16_t pid=0;
    // uint16_t pseq=0;
    // hsl->set_peer_addr(hsl,pip);
    // hsl->set_ping_id(hsl,&pid);
    // hsl->set_ping_seqnum(hsl,&pseq);

    // W5500_setSHAR(io,mac);

    // W5500_getSHAR(io,mac_res);

    // ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",mac_res[0],mac_res[1],mac_res[2],mac_res[3],mac_res[4],mac_res[5]);
    // uint8_t phy_sta;
    while(1){
        // hsl->send_ping(hsl); 
        // vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
