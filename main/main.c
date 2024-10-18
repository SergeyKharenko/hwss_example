#include <stdio.h>
#include "sdkconfig.h"
#include <rom/ets_sys.h>
#include "esp_log.h"

#include "hwss_eth_event.h"
#include "hwss_eth_config.h"
#include "hwss_eth.h"

#include "drv_ch394.h"
#include "hwss_io_spi_basic.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
spi_bus_config_t bcfg={
    .sclk_io_num=12,
    .miso_io_num=13,
    .mosi_io_num=11,
    .quadhd_io_num=-1,
    .quadwp_io_num=-1,
};

hwss_io_spi_config_t cfg={
    .spi_host_id=SPI2_HOST,
    .cs_io_num=10,
    .speed_khz=40*1000
};

const gpio_num_t GPIO_RST_PIN=9;
const gpio_num_t GPIO_IR_PIN=14;
#endif

#ifdef CONFIG_IDF_TARGET_ESP32
spi_bus_config_t bcfg={
    .sclk_io_num=14,
    .miso_io_num=12,
    .mosi_io_num=13,
    .quadhd_io_num=-1,
    .quadwp_io_num=-1,
};

hwss_io_spi_config_t cfg={
    .spi_host_id=SPI2_HOST,
    .cs_io_num=15,
    .speed_khz=10*1000
};

const gpio_num_t GPIO_RST_PIN=27;
const gpio_num_t GPIO_IR_PIN=26;
#endif

#ifdef CONFIG_IDF_TARGET_ESP32C3
spi_bus_config_t bcfg={
    .sclk_io_num=6,
    .miso_io_num=2,
    .mosi_io_num=7,
    .quadhd_io_num=-1,
    .quadwp_io_num=-1,
};

hwss_io_spi_config_t cfg={
    .spi_host_id=SPI2_HOST,
    .cs_io_num=10,
    .speed_khz=2000
};

const gpio_num_t GPIO_RST_PIN=4;
const gpio_num_t GPIO_IR_PIN=5;
#endif






// #define TEST_USE_TCP     
// #define TEST_W5500
// #define TEST_W5100
// #define TEST_W6100
#define TEST_CH394

static const char *TAG="main";

hwss_eth_t *eth;
hwss_eth_sockid_t sockid;

hwss_eth_mac_addr_t dest_mac;

static void hwss_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data){
    switch (event_id)
    {
    case HWSS_PHY_EVENT_CONNECT:
        ESP_LOGI(TAG,"CONNECTED!");
        break;
    
    case HWSS_PHY_EVENT_DISCONN:
        ESP_LOGW(TAG,"DISCONNECTED!");
        break;

    case HWSS_HSL_EVENT_PING:
        // eth->hsl->get_peer_mac_addr(eth->hsl,dest_mac);
        // ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",dest_mac[0],dest_mac[1],dest_mac[2],
        //             dest_mac[3],dest_mac[4],dest_mac[5]);
        break;
    
    case HWSS_HSL_EVENT_TIMEOUT:
        ESP_LOGW(TAG,"PING FAIL!");
        break;

    case HWSS_SSCM_EVENT_TIMEOUT:
        ESP_LOGW(TAG,"HSO TIMEOUT");
        break;
    
    case HWSS_SSCM_EVENT_RECV:
        break;

    case HWSS_SSCM_EVENT_SEND_OK:
        ESP_LOGI(TAG,"SEND OK");
        break;

    case HWSS_HIR_EVENT_TRIGGER:
        break;

    case HWSS_SSCM_EVENT_CONNECT: 
        ESP_LOGI(TAG,"SOCK CONNECT");
        break;

    case HWSS_SSCM_EVENT_DISCONN:
        ESP_LOGW(TAG,"SOCK DISCONN");
        break;

    default:
        ESP_LOGE(TAG,"CUS:%lX",event_id);
        break;
    }
}

static uint8_t *cache;

void app_main(void)
{
    cache=heap_caps_malloc(1024,MALLOC_CAP_DMA);

    spi_bus_initialize(SPI2_HOST,&bcfg,SPI_DMA_CH_AUTO);

#ifdef TEST_W5500
    hwss_eth_config_t eth_config=HWSS_ETH_W5500_DEFAULT_CONFIG(HWSS_IO_SPI,&cfg,GPIO_IR_PIN,GPIO_RST_PIN);

    uint8_t size[]={4,4,4,4};
    hwss_eth_config_set_tx_rx_buffsize_kb(&eth_config,4,size,size);
    eth=hwss_eth_new(&eth_config);
#endif

#ifdef TEST_W5100
    hwss_eth_config_t eth_config=HWSS_ETH_W5100S_DEFAULT_CONFIG(HWSS_IO_SPI,&cfg,GPIO_IR_PIN,GPIO_RST_PIN);

    uint8_t size[]={2,2,2,2};
    hwss_eth_config_set_tx_rx_buffsize_kb(&eth_config,4,size,size);
    eth=hwss_eth_new(&eth_config);
#endif

#ifdef TEST_W6100
    gpio_config_t gpio_cfg={
        .pin_bit_mask=1ull<<GPIO_RST_PIN,
        .mode=GPIO_MODE_OUTPUT,
    };
    gpio_config(&gpio_cfg);
    gpio_set_level(GPIO_RST_PIN,0);
    gpio_set_level(GPIO_RST_PIN,1);
    vTaskDelay(pdMS_TO_TICKS(500));

    hwss_io_t *io=hwss_io_new(HWSS_ETH_SKU_W6100,HWSS_IO_SPI,&cfg);
    hwss_io_init(io);

    uint16_t verr;
    uint16_t cidr;

    while(1){
        W6100_getVER(io,&verr);
        W6100_getCIDR(io,&cidr);
        ESP_LOGI(TAG,"VER:%X \t CIDR:%X",verr,cidr);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

#endif

#ifdef TEST_CH394
    gpio_config_t gpio_cfg={
        .pin_bit_mask=1ull<<GPIO_RST_PIN,
        .mode=GPIO_MODE_OUTPUT,
    };
    gpio_config(&gpio_cfg);
    gpio_set_level(GPIO_RST_PIN,0);
    gpio_set_level(GPIO_RST_PIN,1);
    vTaskDelay(pdMS_TO_TICKS(500));

    hwss_io_t *io=hwss_io_new(HWSS_ETH_SKU_CH394,HWSS_IO_SPI,&cfg);
    hwss_io_init(io);

    uint8_t verr;
    uint8_t phyr;
    hwss_eth_mac_addr_t mac_addr;
    while(1){
        CH394_GetCHIPV(io,&verr);
        CH394_GetPHY_CFG(io,&phyr);
        CH394_GetMAC(io,mac_addr);
        ESP_LOGI(TAG,"VER:%X \t PHY:%X \t MAC:%X:%X:%X:%X:%X:%X",verr,phyr,mac_addr[0],mac_addr[1],mac_addr[2],
                    mac_addr[3],mac_addr[4],mac_addr[5]);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

#endif
    // esp_event_handler_register_with(eth->elp_hdl,HWSS_EVENT_ANY_BASE,HWSS_EVENT_ANY_ID,hwss_event_handler,NULL);

    gpio_install_isr_service(0);
    esp_event_loop_create_default();

    hwss_eth_init(eth);
    hwss_eth_print_info(eth);

    hwss_eth_start(eth);

    hwss_eth_ip4_addr_t ip={192,168,0,10};
    hwss_eth_ip4_addr_t gip={192,168,0,1};
    hwss_eth_ip4_addr_t mask={255,255,255,0};

    eth->hnet->set_source_addr(eth->hnet,ip);
    eth->hnet->set_gateway_addr(eth->hnet,gip);
    eth->hnet->set_subnet_mask(eth->hnet,mask);

#ifdef TEST_USE_TCP
    /////// TCP TEST /////////
    hwss_eth_port_t sport=5590;
    // hwss_eth_port_t dport=5700;
    // hwss_eth_ip4_addr_t dip={10,0,0,10};
    // hwss_eth_ip4_addr_t dip={192,168,0,24};

    hwss_eth_sock_create(eth,HWSS_PROTO_TCP,&sockid);
    hwss_hso_set_sock_source_port(eth->hso,sockid,&sport);
    // hwss_hso_set_sock_dest_addr(eth->hso,sockid,dip);
    // hwss_hso_set_sock_dest_port(eth->hso,sockid,&dport);

    ESP_LOGI(TAG,"USE SOCK %X",sockid);
    vTaskDelay(pdMS_TO_TICKS(1000));

    esp_err_t ret;
    uint16_t rlen;

    while(1){
        hwss_eth_sock_open(eth,sockid);
        hwss_eth_sock_listen(eth,sockid);
        ret=hwss_eth_sock_accept_pending(eth,sockid);
        if(ret!=ESP_OK){
            hwss_eth_sock_close(eth,sockid);
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        while(1){
            ret=hwss_eth_sock_recv_pending(eth,sockid);
            if(ret!=ESP_OK)
                break;
            hwss_eth_sock_recv(eth,sockid,cache,&rlen);
            hwss_eth_sock_send(eth,sockid,cache,rlen);
        }
        hwss_eth_sock_close(eth,sockid);
    }

#else
    /////// UDP TEST /////////
    hwss_eth_port_t sport=5590;

    hwss_eth_sock_create(eth,HWSS_PROTO_UDP,&sockid);
    hwss_hso_set_sock_source_port(eth->hso,sockid,&sport);
    // hwss_hso_set_sock_dest_addr(eth->hso,sockid,dip);
    hwss_eth_sock_open(eth,sockid);

    esp_err_t ret;

    hwss_eth_udp_header_t udp_header;
    bool last_pack=false;
    while(1){
        ret=hwss_eth_sock_recv_pending(eth,sockid);
        if(ret!=ESP_OK)
            break;

        while(1){
            hwss_eth_sock_recv_udp(eth,sockid,&udp_header,cache,&last_pack);
            hwss_hso_set_sock_dest_addr(eth->hso,sockid,udp_header.addr);
            hwss_hso_set_sock_dest_port(eth->hso,sockid,&udp_header.port);
            hwss_eth_sock_send(eth,sockid,cache,udp_header.len);

            // cache[udp_header.len]='\0';
            // ESP_LOGI(TAG,"%s",cache);
            ESP_LOGI(TAG,"IP:%u.%u.%u.%u",udp_header.addr[0],udp_header.addr[1],udp_header.addr[2],udp_header.addr[3]);
            ESP_LOGI(TAG,"PORT:%u",udp_header.port);
            ESP_LOGI(TAG,"LEN:%u",udp_header.len);

            if(last_pack)
                break;
        }
    }
    hwss_eth_sock_close(eth,sockid);
#endif

    // hwss_eth_ip4_addr_t pip={192,168,0,24};
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
