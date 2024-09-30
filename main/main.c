#include <stdio.h>
#include <rom/ets_sys.h>
#include "esp_log.h"
#include "hwss_eth_event.h"
#include "hwss_eth_config.h"
#include "hwss_eth.h"

#include "drv_ch394.h"

const gpio_num_t GPIO_RST_PIN=9;
const gpio_num_t GPIO_IR_PIN=14;

static const char *TAG="main";

hwss_io_spi_config_t cfg={
    .spi_host_id=SPI2_HOST,
    .cs_io_num=10,
    .speed_khz=20*1000
};

// hwss_phy_config_t pcfg={
//     .check_period_ms=10,
//     .reset_timeout_ms=500
// };

// hwss_mac_config_t mcfg={
//     .addr={0x00,0x08,0xDC,0x01,0x02,0x04},
//     .use_burnin_addr=false
// };

// hwss_hnet_config_t ncfg={
//     .check_state_period_ms=50,
//     .retry_cnt=8,
//     .retry_time_ms=200,
//     .ppp_cp_magic_num=0,
//     .ppp_cp_request_time_ms=1000
// };

// hwss_hsl_config_t lcfg={
//     .retry_cnt=0,
//     .retry_time_ms=100,
//     .check_state_period_ms=10
// };

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
    spi_bus_config_t bcfg={
        .sclk_io_num=12,
        .miso_io_num=13,
        .mosi_io_num=11,
        .quadhd_io_num=-1,
        .quadwp_io_num=-1,
    };

    // spi_bus_initialize(SPI2_HOST,&bcfg,SPI_DMA_CH_AUTO);

    // gpio_config_t gxcfg={
    //     .pin_bit_mask=1ull<<GPIO_RST_PIN,
    //     .mode=GPIO_MODE_OUTPUT
    // };
    // gpio_config(&gxcfg);
    // gpio_set_level(GPIO_RST_PIN,1);

    // vTaskDelay(pdMS_TO_TICKS(500));
    // gpio_set_level(GPIO_RST_PIN,0);
    // vTaskDelay(pdMS_TO_TICKS(500));
    // gpio_set_level(GPIO_RST_PIN,1);
    // vTaskDelay(pdMS_TO_TICKS(5000));
    
    // hwss_io_t *gio=hwss_io_new(HWSS_ETH_SKU_CH394,HWSS_IO_SPI,&cfg);
    // if(gio->init(gio)!=ESP_OK)
    //     ESP_LOGE(TAG,"IO Failed");

    // hwss_eth_mac_addr_t ch394_mac;
    // CH394_GetMAC(gio,ch394_mac);
    // ESP_LOGI(TAG,"MAC: %X:%X:%X:%X:%X:%X",ch394_mac[0],ch394_mac[1],ch394_mac[2],ch394_mac[3],ch394_mac[4],ch394_mac[5]);
    // uint8_t ver;
    // CH394_GetCHIPV(gio,&ver);
    // ESP_LOGI(TAG,"VER:%X",ver);
    // uint8_t phys;
    // CH394_GetPHY_CFG(gio,&phys);
    // ESP_LOGI(TAG,"PHYS:%X",phys);
    // while(1);

    cache=heap_caps_malloc(1024,MALLOC_CAP_DMA);

    spi_bus_initialize(SPI2_HOST,&bcfg,SPI_DMA_CH_AUTO);

    // io=hwss_io_new_w5100s(HWSS_IO_SPI,&cfg);
    // hir=hwss_hir_new(hdl,&ircfg);
    // phy=hwss_phy_new_w5100s(hdl,io,&pcfg);
    // mac=hwss_mac_new_w5100s(io,&mcfg);
    // hnet=hwss_hnet_new_w5100s(hdl,io,&ncfg);
    // hso=hwss_hso_new_w5100s(hdl,io,hir,&scfg);
    // hsl=hwss_hsl_new_w5100s(hdl,io,&lcfg);

    hwss_eth_config_t eth_config=HWSS_ETH_W5500_DEFAULT_CONFIG(HWSS_IO_SPI,&cfg,GPIO_IR_PIN,GPIO_RST_PIN);

    uint8_t size[]={4,4,4,4};
    hwss_eth_config_set_tx_rx_buffsize_kb(&eth_config,4,size,size);

    eth=hwss_eth_new(&eth_config);

    esp_event_handler_register_with(eth->elp_hdl,HWSS_EVENT_ANY_BASE,HWSS_EVENT_ANY_ID,hwss_event_handler,NULL);

    gpio_install_isr_service(0);
    esp_event_loop_create_default();

    hwss_eth_init(eth);
    hwss_eth_print_info(eth);

    hwss_eth_start(eth);

    hwss_eth_ip4_addr_t ip={10,0,0,5};
    hwss_eth_ip4_addr_t gip={10,0,0,1};
    // hwss_eth_ip4_addr_t ip={192,168,0,10};
    // hwss_eth_ip4_addr_t gip={192,168,0,1};
    hwss_eth_ip4_addr_t mask={255,255,255,0};

    eth->hnet->set_source_addr(eth->hnet,ip);
    eth->hnet->set_gateway_addr(eth->hnet,gip);
    eth->hnet->set_subnet_mask(eth->hnet,mask);

    /////// TCP TEST /////////
    hwss_eth_port_t sport=5590;
    hwss_eth_port_t dport=5700;
    hwss_eth_ip4_addr_t dip={10,0,0,10};

    hwss_eth_sock_create(eth,HWSS_PROTO_TCP,&sockid);
    hwss_hso_set_sock_source_port(eth->hso,sockid,&sport);
    hwss_hso_set_sock_dest_addr(eth->hso,sockid,dip);
    hwss_hso_set_sock_dest_port(eth->hso,sockid,&dport);

    ESP_LOGI(TAG,"USE SOCK %X",sockid);
    vTaskDelay(pdMS_TO_TICKS(1000));

    uint8_t ccnt=0;
    esp_err_t ret;
    uint16_t rlen;

    while(1){
        hwss_eth_sock_open(eth,sockid);
        ret=hwss_eth_sock_connect(eth,sockid);
        if(ret!=ESP_OK){
            hwss_eth_sock_close(eth,sockid);
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        ESP_LOGI(TAG,"CONNECTED");
        ccnt=0;
        while(1){
            ret=hwss_eth_sock_recv_pending(eth,sockid);
            if(ret!=ESP_OK)
                break;
            hwss_eth_sock_recv(eth,sockid,cache,&rlen);
            ESP_LOGI(TAG,"LOOP");
            hwss_eth_sock_send(eth,sockid,cache,rlen);
            ccnt++;

            if(ccnt==5){
                hwss_eth_sock_disconn(eth,sockid);
                break;
            }
                
        }
        hwss_eth_sock_close(eth,sockid);
    }
    


    // uint8_t sta;
    // while(1){
    //     hwss_hso_get_sock_state_raw(eth->hso,sockid,&sta);
    //     ESP_LOGI(TAG,"STA: %X",sta);
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }
    
    /////// UDP TEST /////////
    // hwss_proto_t pro=HWSS_PROTO_UDP;
    // hwss_eth_port_t sport=5590;

    // hwss_eth_ip4_addr_t destip={10,0,0,255};
    // hwss_eth_port_t disport=5900;

    // hso->set_sock_proto(hso,0,&pro);
    // hso->set_sock_source_port(hso,0,&sport);
    // hso->set_sock_dest_addr(hso,0,destip);
    // hso->set_sock_dest_port(hso,0,&disport);

    // hso->ctrl_sock(hso,0,HWSS_HSO_SOCKCTRL_OPEN);

    // hwss_hso_socksta_t socksta=0;
    // while(1){
    //     hso->get_sock_state(hso,0,&socksta);
    //     if(socksta==HWSS_HSO_SOCK_OPENED)
    //         break;
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ESP_LOGI(TAG,"sock opened!");

    // while(1){
    //     hso->write_tx_buffer(hso,0,(uint8_t *)data,strlen(data));
    //     hso->ctrl_sock(hso,0,HWSS_HSO_SOCKCTRL_SEND);
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }

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
