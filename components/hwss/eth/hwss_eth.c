#include <sys/cdefs.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_check.h"
#include "hwss_event.h"
#include "hwss_eth_event.h"
#include "hwss_eth.h"

static const char *TAG = "hwss_eth";
static const char *HWSS_ETH_NAME_HEADER="HWSSETH_";
static uint8_t hwss_eth_gen_id=0;

typedef struct{
    hwss_eth_t super;

    uint8_t     en_socknum;

    EventGroupHandle_t *sock_egps;
    EventGroupHandle_t global_egp;
}hwss_eth_pro_t;

static void hwss_eth_hir_handler(void *arg, esp_event_base_t event_base,
                                    int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;
    hwss_sscm_intr_process(eth->sscm);
}

static void hwss_eth_phy_state_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);
    EventBits_t rbits;
    
    switch (event_id){
        case HWSS_PHY_EVENT_CONNECT:
            hwss_sir_start(eth->sir);
            hwss_sscm_start(eth->sscm);
            esp_event_post(HWSS_ETH_EVENT,HWSS_ETH_EVENT_CONNECTED,(void *)eth, sizeof(hwss_eth_t *),0);
            rbits=xEventGroupSetBits(eth_pro->global_egp,HWSS_PHY_EVENTBIT_CONNECTED);
            if(rbits&HWSS_PHY_EVENTBIT_CONNECTED)
                xEventGroupClearBits(eth_pro->global_egp,HWSS_PHY_EVENTBIT_CONNECTED);
            break;
        
        case HWSS_PHY_EVENT_DISCONN:
            hwss_sir_stop(eth->sir);
            hwss_sscm_stop(eth->sscm);
            esp_event_post(HWSS_ETH_EVENT,HWSS_ETH_EVENT_DISCONNECTED,(void *)eth, sizeof(hwss_eth_t *),0);
            rbits=xEventGroupSetBits(eth_pro->global_egp,HWSS_PHY_EVENTBIT_DISCONNECTED);
            if(rbits&HWSS_PHY_EVENTBIT_DISCONNECTED)
                xEventGroupClearBits(eth_pro->global_egp,HWSS_PHY_EVENTBIT_DISCONNECTED);
            break;
        default:
            break;
    }
}

static void hwss_eth_sir_state_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;

    switch (event_id){
        case HWSS_SIR_EVENT_IP_CONFLICT:
            
            break;
        
        case HWSS_SIR_EVENT_DEST_UNREACH:

            break;

        case HWSS_SIR_EVENT_PPPOE_CLOSE:

            break;

        default:
            break;
    }
}

static void hwss_eth_sscm_state_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);
    hwss_sockid_t id=*(hwss_sockid_t *)event_data;
    EventBits_t rbits;

    switch(event_id){
        case HWSS_SSCM_EVENT_CONNECT:
            rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CONNECT);
            if(rbits&HWSS_SOCK_EVENTBIT_CONNECT)
                xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CONNECT);
        break;

        case HWSS_SSCM_EVENT_DISCONN:
            rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_DISCONN);
            if(rbits&HWSS_SOCK_EVENTBIT_DISCONN)
                xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_DISCONN);
        break;

        case HWSS_SSCM_EVENT_RECV:
            rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_RECV);
            if(rbits&HWSS_SOCK_EVENTBIT_RECV)
                xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_RECV);
        break;

        case HWSS_SSCM_EVENT_SEND_OK:
            rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_SEND_OK);
            if(rbits&HWSS_SOCK_EVENTBIT_SEND_OK)
                xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_SEND_OK);
        break;
        
        case HWSS_SSCM_EVENT_TIMEOUT:
            rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_TIMEOUT);
            if(rbits&HWSS_SOCK_EVENTBIT_TIMEOUT)
                xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_TIMEOUT);
        break;

        default: break;
    }

}

hwss_eth_t *hwss_eth_new(const hwss_eth_config_t *config){
    hwss_eth_t *ret=NULL;
    hwss_eth_pro_t *eth_pro=NULL;

    eth_pro=calloc(1,sizeof(hwss_eth_pro_t));
    ESP_GOTO_ON_FALSE(eth_pro,NULL,err,TAG,"fail to calloc eth_pro");

    eth_pro->super.sku=config->sku;
    eth_pro->en_socknum=config->en_socknum;
    sprintf(eth_pro->super.name,"%s%02X",HWSS_ETH_NAME_HEADER,hwss_eth_gen_id);

    eth_pro->sock_egps=calloc(eth_pro->en_socknum,sizeof(EventGroupHandle_t));
    ESP_GOTO_ON_FALSE(eth_pro->sock_egps,NULL,err,TAG,"fail to calloc sock eventgroups");

    if(esp_event_loop_create(&(config->elp_args),&(eth_pro->super.elp_hdl))!=ESP_OK){
        ESP_LOGE(TAG,"fail to create event loop");
        return ret;
    }

    ESP_GOTO_ON_FALSE(eth_pro->super.io=hwss_io_new(config->sku,config->io_type,config->io_config),
                        NULL,err,TAG,"cannot create hwss_io");
    ESP_GOTO_ON_FALSE(eth_pro->super.hir=hwss_hir_new(eth_pro->super.elp_hdl,&config->hir),NULL,err,TAG,
                        "fail to create hwss_hir");
    ESP_GOTO_ON_FALSE(eth_pro->super.sir=hwss_sir_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,&config->sir),
                        NULL,err,TAG,"fail to create hwss_sir");
    ESP_GOTO_ON_FALSE(eth_pro->super.cctl=hwss_cctl_new(config->sku,eth_pro->super.io,&config->cctl),NULL,err,TAG,
                        "fail to create hwss_cctl");
    ESP_GOTO_ON_FALSE(eth_pro->super.phy=hwss_phy_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,&config->phy),NULL,
                        err,TAG,"fail to create hwss_phy");
    ESP_GOTO_ON_FALSE(eth_pro->super.mac=hwss_mac_new(config->sku,eth_pro->super.io,&config->mac),NULL,err,TAG,
                        "fail to create hwss_mac");
    ESP_GOTO_ON_FALSE(eth_pro->super.hnet=hwss_hnet_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,&config->hnet),
                        NULL,err,TAG, "fail to create hwss_hnet");
    ESP_GOTO_ON_FALSE(eth_pro->super.hso=hwss_hso_new(config->sku,eth_pro->super.elp_hdl,eth_pro->super.io,eth_pro->super.hir,
                        &config->hso), NULL,err,TAG, "fail to create hwss_hso");

    hwss_sscm_drv_t *sscm_drv=NULL;
    ESP_GOTO_ON_FALSE(sscm_drv=hwss_sscm_drv_new(config->sku,eth_pro->super.io),NULL,err,TAG,"fail to create hwss_sscm_drv");
    ESP_GOTO_ON_FALSE(eth_pro->super.sscm=hwss_sscm_new(eth_pro->super.elp_hdl,sscm_drv,&config->sscm),NULL,err,TAG,
                        "fail to create hwss_sscm");

    ESP_GOTO_ON_FALSE(eth_pro->super.cvr=hwss_cvr_new(config->sku,eth_pro->super.io),NULL,err,TAG,"fail to create hwss_cvr");

    if(config->has_hppe)
        ESP_GOTO_ON_FALSE(eth_pro->super.opt.hppe=hwss_hppe_new(config->sku,eth_pro->super.io,&config->hppe),NULL,err,TAG,
                            "fail to create hwss_hppe");
    else
        eth_pro->super.opt.hppe=NULL;

    hwss_eth_gen_id++;
    return &eth_pro->super;
err:
    return ret;
}

esp_err_t hwss_eth_init(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_ERROR(hwss_io_init(eth->io),err,TAG,"fail to initialize io");
    ESP_GOTO_ON_ERROR(hwss_cctl_init(eth->cctl),err,TAG,"fail to initialize cctl");
    ESP_GOTO_ON_ERROR(hwss_cctl_chip_reset(eth->cctl),err,TAG,"fail to process chip reset");
    ESP_GOTO_ON_ERROR(hwss_hir_init(eth->hir),err,TAG,"fail to initialize hir");
    ESP_GOTO_ON_ERROR(hwss_sir_init(eth->sir),err,TAG,"fail to initialize sir");
    ESP_GOTO_ON_ERROR(hwss_phy_init(eth->phy),err,TAG,"fail to initialize phy");
    ESP_GOTO_ON_ERROR(hwss_mac_init(eth->mac),err,TAG,"fail to initialize mac");
    ESP_GOTO_ON_ERROR(hwss_hnet_init(eth->hnet),err,TAG,"fail to initialize hnet");
    ESP_GOTO_ON_ERROR(hwss_hso_init(eth->hso),err,TAG,"fail to initialize hso");

    if(eth->opt.hppe)
        ESP_GOTO_ON_ERROR(hwss_hppe_init(eth->opt.hppe),err,TAG,"fail to initialize hppe");

    ESP_GOTO_ON_ERROR(hwss_cvr_self_test(eth->cvr),err,TAG,"fail to pass selftest");

    for(hwss_sockid_t id=0;id<eth_pro->en_socknum;id++)
        eth_pro->sock_egps[id]=xEventGroupCreate();
    eth_pro->global_egp=xEventGroupCreate();
    
err:
    return ret;
}   

esp_err_t hwss_eth_deinit(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_ERROR(hwss_hso_deinit(eth->hso),err,TAG,"fail to deinit hso");
    ESP_GOTO_ON_ERROR(hwss_hnet_deinit(eth->hnet),err,TAG,"fail to deinit hnet");
    ESP_GOTO_ON_ERROR(hwss_mac_deinit(eth->mac),err,TAG,"fail to deinit mac");
    ESP_GOTO_ON_ERROR(hwss_phy_deinit(eth->phy),err,TAG,"fail to deinit phy");
    ESP_GOTO_ON_ERROR(hwss_cctl_deinit(eth->cctl),err,TAG,"fail to deinit cctl");
    ESP_GOTO_ON_ERROR(hwss_sir_deinit(eth->sir),err,TAG,"fail to deinit sir");
    ESP_GOTO_ON_ERROR(hwss_hir_deinit(eth->hir),err,TAG,"fail to deinit hir");
    ESP_GOTO_ON_ERROR(hwss_io_deinit(eth->io),err,TAG,"fail to deinit io");

    if(eth->opt.hppe)
        ESP_GOTO_ON_ERROR(hwss_hppe_deinit(eth->opt.hppe),err,TAG,"fail to deinit hppe");

    vEventGroupDelete(eth_pro->global_egp);
    for(hwss_sockid_t id=0;id<eth_pro->en_socknum;id++)
        vEventGroupDelete(eth_pro->sock_egps[id]);
    
err:
    return ret;
}

esp_err_t hwss_eth_start(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    
    ESP_GOTO_ON_ERROR(hwss_phy_start(eth->phy),err,TAG,"cannot start phy");
    ESP_GOTO_ON_ERROR(hwss_hir_start(eth->hir),err,TAG,"cannot start hir");
    ESP_GOTO_ON_ERROR(esp_event_handler_register_with(eth->elp_hdl,HWSS_PHY_EVENT,HWSS_EVENT_ANY_ID,hwss_eth_phy_state_handler,
                    (void *)eth),err,TAG,"cannot hook phy state handler");
    ESP_GOTO_ON_ERROR(esp_event_handler_register_with(eth->elp_hdl,HWSS_HIR_EVENT,HWSS_EVENT_ANY_ID,hwss_eth_hir_handler,
                    (void *)eth),err,TAG,"cannot hook hir handler");
    ESP_GOTO_ON_ERROR(esp_event_handler_register_with(eth->elp_hdl,HWSS_SSCM_EVENT,HWSS_EVENT_ANY_ID,hwss_eth_sscm_state_handler,
                    (void *)eth),err,TAG,"cannot hook sscm handler");
    ESP_GOTO_ON_ERROR(esp_event_post(HWSS_ETH_EVENT,HWSS_ETH_EVENT_START,(void *)eth,sizeof(hwss_eth_t *),0),err,TAG,
                        "fail to post event");
err:
    return ret;
}

esp_err_t hwss_eth_stop(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_unregister_with(eth->elp_hdl,HWSS_PHY_EVENT,HWSS_EVENT_ANY_ID,
                    hwss_eth_phy_state_handler),err,TAG,"cannot unhook phy state handler");
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_unregister_with(eth->elp_hdl,HWSS_HIR_EVENT,HWSS_EVENT_ANY_ID,
                    hwss_eth_hir_handler),err,TAG,"cannot unhook hir handler");
    ESP_GOTO_ON_ERROR(esp_event_handler_instance_unregister_with(eth->elp_hdl,HWSS_SSCM_EVENT,HWSS_EVENT_ANY_ID,
                    hwss_eth_sscm_state_handler),err,TAG,"cannot unhook sscm state handler");

    ESP_GOTO_ON_ERROR(esp_event_post(HWSS_ETH_EVENT,HWSS_ETH_EVENT_STOP,(void *)eth,sizeof(hwss_eth_t *),0),err,TAG,
                        "fail to post event");
err:
    return ret;
}


esp_err_t hwss_eth_print_info(const hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;

    ESP_LOGI(TAG,"########################");
    ESP_LOGI(TAG,"Device:\t %s",eth->name);

    switch (eth->sku)
    {
        case HWSS_SKU_CH394:
        case HWSS_SKU_CH395:
            ESP_LOGI(TAG,"Vendor:\t WCH");
            break;

        case HWSS_SKU_W5100S:
        case HWSS_SKU_W5500:
            ESP_LOGI(TAG,"Vendor:\t Wiznet");
            break;

        default:
            ESP_LOGI(TAG,"Vendor:\t Unknow");
            break;
    }

    hwss_chipver_t chipver;
    ESP_GOTO_ON_ERROR(hwss_cvr_get_chip_version(eth->cvr,&chipver),err,TAG,"fail to get chip version");

    switch (eth->sku)
    {
        case HWSS_SKU_W5500:
            ESP_LOGI(TAG,"Model:\t W5500");
            ESP_LOGI(TAG,"MAX SOCKs:\t 8");
            ESP_LOGI(TAG,"Features: \t PPPOE");
            break;

        case HWSS_SKU_W5100S:
            ESP_LOGI(TAG,"Model:\t W5100S");
            ESP_LOGI(TAG,"MAX SOCKs:\t 4");
            ESP_LOGI(TAG,"Features: \t SOCK-less Ping&ARP, PPPOE");
            break;

        default:
            ESP_LOGI(TAG,"Model:\t Unknow");
            break;
    }

    ESP_LOGI(TAG,"Chip Ver: \t 0x%04X",chipver);

    ESP_LOGI(TAG,"########################");

err:
    return ret;
}