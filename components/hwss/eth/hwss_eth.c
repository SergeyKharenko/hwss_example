#include <sys/cdefs.h>
#include "esp_check.h"
#include "hwss_eth_event.h"
#include "hwss_eth.h"

static const char *TAG = "hwss_eth";
static const char *HWSS_ETH_NAME_HEADER="HWSSETH_";
static uint8_t hwss_eth_gen_id=0;

typedef struct{
    hwss_eth_t super;
    
    gpio_num_t          rst_ionum;
    uint32_t            rst_timeout_ms;



}hwss_eth_pro_t;

static void hwss_eth_phy_state_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;
    switch (event_id)
    {
    case HWSS_PHY_EVENT_CONNECT:
        eth->hnet->start(eth->hnet);
        eth->hso->start(eth->hso);
        break;
    
    case HWSS_PHY_EVENT_DISCONN:
        eth->hnet->stop(eth->hnet);
        eth->hso->stop(eth->hso);
        break;
    default:
        break;
    }
}

static void hwss_eth_hnet_state_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;
    switch (event_id)
    {
    case HWSS_HNET_EVENT_IP_CONFLICT:
        
        break;
    
    case HWSS_HNET_EVENT_DEST_UNREACH:

        break;

    case HWSS_HNET_EVENT_PPPOE_CLOSE:

        break;

    default:
        break;
    }
}

static void hwss_eth_hso_state_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){


}

hwss_eth_t *hwss_eth_new(const hwss_eth_config_t *config){
    hwss_eth_t *ret=NULL;
    hwss_eth_pro_t *eth_pro=NULL;

    eth_pro=calloc(1,sizeof(hwss_eth_pro_t));
    ESP_GOTO_ON_FALSE(eth_pro,NULL,err,TAG,"fail to calloc eth_pro");

    eth_pro->super.sku=config->sku;
    sprintf(eth_pro->super.name,"%s%02X",HWSS_ETH_NAME_HEADER,hwss_eth_gen_id);

    if(esp_event_loop_create(&(config->elp_args),&(eth_pro->super.elp_hdl))!=ESP_OK){
        ESP_LOGE(TAG,"fail to create event loop");
        return ret;
    }

    ESP_GOTO_ON_FALSE(eth_pro->super.io=hwss_io_new(config->sku,config->io_type,config->io_config),
                        NULL,err,TAG,"cannot create hwss_io");
    ESP_GOTO_ON_FALSE(eth_pro->super.hir=hwss_hir_new(eth_pro->super.elp_hdl,&config->hir),NULL,err,TAG,
                        "fail to create hwss_hir");
    ESP_GOTO_ON_FALSE(eth_pro->super.phy=hwss_phy_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,&config->phy),NULL,
                        err,TAG,"fail to create hwss_phy");
    ESP_GOTO_ON_FALSE(eth_pro->super.mac=hwss_mac_new(config->sku,eth_pro->super.io,&config->mac),NULL,err,TAG,
                        "fail to create hwss_mac");
    ESP_GOTO_ON_FALSE(eth_pro->super.hnet=hwss_hnet_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,&config->hnet),
                        NULL,err,TAG, "fail to create hwss_hnet");
    ESP_GOTO_ON_FALSE(eth_pro->super.hso=hwss_hso_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,eth_pro->super.hir,
                        &config->hso), NULL,err,TAG, "fail to create hwss_hso");

    eth_pro->rst_ionum=config->rst_ionum;
    eth_pro->rst_timeout_ms=config->rst_timeout_ms;

    hwss_eth_gen_id++;
    return &eth_pro->super;
err:
    return ret;
}

esp_err_t hwss_eth_init(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);
    gpio_config_t cfg={
        .pin_bit_mask=(1ull<<(eth_pro->rst_ionum)),
        .mode=GPIO_MODE_OUTPUT
    };
    ESP_GOTO_ON_ERROR(gpio_config(&cfg),err,TAG,"cannot initialize reset pin");
    
    gpio_set_level(eth_pro->rst_ionum,1);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(eth_pro->rst_ionum,0);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(eth_pro->rst_ionum,1);
    vTaskDelay(pdMS_TO_TICKS(eth_pro->rst_timeout_ms));

    ESP_GOTO_ON_ERROR(eth->io->init(eth->io),err,TAG,"fail to initialize io");
    ESP_GOTO_ON_ERROR(eth->hir->init(eth->hir),err,TAG,"fail to initialize hir");
    ESP_GOTO_ON_ERROR(eth->phy->init(eth->phy),err,TAG,"fail to initialize phy");
    ESP_GOTO_ON_ERROR(eth->mac->init(eth->mac),err,TAG,"fail to initialize mac");
    ESP_GOTO_ON_ERROR(eth->hnet->init(eth->hnet),err,TAG,"fail to initialize hnet");
    ESP_GOTO_ON_ERROR(eth->hso->init(eth->hso),err,TAG,"fail to initialize hso");

err:
    return ret;
}   

esp_err_t hwss_eth_deinit(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_ERROR(eth->hso->deinit(eth->hso),err,TAG,"fail to deinit hso");
    ESP_GOTO_ON_ERROR(eth->hnet->deinit(eth->hnet),err,TAG,"fail to deinit hnet");
    ESP_GOTO_ON_ERROR(eth->mac->deinit(eth->mac),err,TAG,"fail to deinit mac");
    ESP_GOTO_ON_ERROR(eth->phy->deinit(eth->phy),err,TAG,"fail to deinit phy");
    ESP_GOTO_ON_ERROR(eth->hir->deinit(eth->hir),err,TAG,"fail to deinit hir");
    ESP_GOTO_ON_ERROR(eth->io->deinit(eth->io),err,TAG,"fail to deinit io");

    ESP_GOTO_ON_ERROR(gpio_reset_pin(eth_pro->rst_ionum),err,TAG,"fail to deinit reset pin");
err:
    return ret;
}

esp_err_t hwss_eth_start(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    
    ESP_GOTO_ON_ERROR(eth->phy->start(eth->phy),err,TAG,"cannot start phy");
    ESP_GOTO_ON_ERROR(eth->hir->start(eth->hir),err,TAG,"cannot start hir");
    ESP_GOTO_ON_ERROR(esp_event_handler_register_with(eth->elp_hdl,HWSS_PHY_EVENT,HWSS_EVENT_ANY_ID,hwss_eth_phy_state_handler,
                    (void *)eth),err,TAG,"cannot hook phy state handler");
err:
    return ret;
}

esp_err_t hwss_eth_stop(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_unregister_with(eth->elp_hdl,HWSS_PHY_EVENT,HWSS_EVENT_ANY_ID,
                    hwss_eth_phy_state_handler),err,TAG,"cannot unhook phy state handler");
    


err:
    return ret;
}


