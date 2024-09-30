#include <sys/cdefs.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_check.h"
#include "hwss_event.h"
#include "hwss_eth_opt.h"
#include "hwss_eth_event.h"
#include "hwss_eth.h"

static const char *TAG = "hwss_eth";
static const char *HWSS_ETH_NAME_HEADER="HWSSETH_";
static const hwss_eth_ip4_addr_t BLANK_ADDR={0,0,0,0};
static uint8_t hwss_eth_gen_id=0;

typedef struct{
    hwss_eth_t          super;

    uint8_t             en_socknum;

    EventGroupHandle_t  *sock_egps;
    EventGroupHandle_t  global_egp;

    bool                sock_use_stas[HWSS_ETH_SOCKNUM_MAX];
    hwss_hso_socksta_t  sock_stas[HWSS_ETH_SOCKNUM_MAX];
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
    hwss_eth_sockid_t id=*(hwss_eth_sockid_t *)event_data;
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

static void hwss_eth_sock_disconn_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data){
    hwss_eth_t *eth=(hwss_eth_t *)arg;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);
    hwss_eth_sockid_t id=*(hwss_eth_sockid_t *)event_data;
    hwss_hso_socksta_t sta;
    EventBits_t rbits;

    ESP_RETURN_VOID_ON_ERROR(hwss_hso_get_sock_state(eth->hso,id,&sta),TAG,"fail to get sock state");
    rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CLOSE);
    if(rbits&HWSS_SOCK_EVENTBIT_CLOSE)
        xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CLOSE);
    
    if(sta==HWSS_HSO_SOCK_TCP_CLOSEW){
        ESP_RETURN_VOID_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_CLOSE),TAG,"fail to close sock");

        uint32_t cnt=HWSS_ETH_SOCK_STATE_CHECK_MAX_TRY;
        while(cnt--){
            ESP_RETURN_VOID_ON_ERROR(hwss_hso_get_sock_state(eth->hso,id,&sta),TAG,"cannot fetch sock state");
            if(sta==HWSS_HSO_SOCK_CLOSED)
                break;
            vTaskDelay(pdMS_TO_TICKS(HWSS_ETH_SOCK_STATE_CHECK_PERIOD_MS));
        }
        eth_pro->sock_stas[id]=sta;
        ESP_RETURN_VOID_ON_FALSE(cnt!=0,TAG,"close sock timeout");
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

    for(hwss_eth_sockid_t id=0;id<eth_pro->en_socknum;id++)
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
    for(hwss_eth_sockid_t id=0;id<eth_pro->en_socknum;id++)
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
    ESP_GOTO_ON_ERROR(esp_event_handler_register_with(eth->elp_hdl,HWSS_SSCM_EVENT,HWSS_SSCM_EVENT_DISCONN,hwss_eth_sock_disconn_handler,
                    (void *)eth),err,TAG,"cannot hook sock disconnect handler");
    
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


esp_err_t hwss_eth_print_info(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_LOGI(TAG,"########################");
    ESP_LOGI(TAG,"Device:\t %s",eth->name);

    switch (eth->sku)
    {
        case HWSS_ETH_SKU_CH394:
        case HWSS_ETH_SKU_CH395:
            ESP_LOGI(TAG,"Vendor:\t WCH");
            break;

        case HWSS_ETH_SKU_W5100S:
        case HWSS_ETH_SKU_W5500:
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
        case HWSS_ETH_SKU_W5500:
            ESP_LOGI(TAG,"Model:\t W5500");
            ESP_LOGI(TAG,"MAX SOCKs:\t 8");
            break;

        case HWSS_ETH_SKU_W5100S:
            ESP_LOGI(TAG,"Model:\t W5100S");
            ESP_LOGI(TAG,"MAX SOCKs:\t 4");
            break;

        default:
            ESP_LOGI(TAG,"Model:\t Unknow");
            break;
    }

    ESP_LOGI(TAG,"EN SOCKs: \t %u",eth_pro->en_socknum);
    if(eth->opt.hdc)
        ESP_LOGI(TAG,"Feature: \t DHCP");

    if(eth->opt.hppe)
        ESP_LOGI(TAG,"Feature: \t PPPOE");
    
    if(eth->opt.hsl)
        ESP_LOGI(TAG,"Feature: \t SOCK-less Ping&ARP");
    
    ESP_LOGI(TAG,"Chip Ver: \t 0x%04X",chipver);

    ESP_LOGI(TAG,"########################");

err:
    return ret;
}

esp_err_t hwss_eth_clear_host_netinfo(hwss_eth_t *eth){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(hwss_hnet_set_source_addr(eth->hnet,BLANK_ADDR),err,TAG,"fail to clear host addr");
    ESP_GOTO_ON_ERROR(hwss_hnet_set_subnet_mask(eth->hnet,BLANK_ADDR),err,TAG,"fail to clear subnet mask");
    ESP_GOTO_ON_ERROR(hwss_hnet_set_gateway_addr(eth->hnet,BLANK_ADDR),err,TAG,"fail to clear gateway addr");

err:
    return ret;
}

esp_err_t hwss_eth_sock_create(hwss_eth_t *eth, hwss_proto_t proto ,hwss_eth_sockid_t *id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    switch (proto)
    {
        case HWSS_PROTO_TCP:
        case HWSS_PROTO_UDP:
            for(hwss_eth_sockid_t sid=0;sid<eth_pro->en_socknum;sid++){
                if(eth_pro->sock_use_stas[eth_pro->en_socknum-sid-1])
                    continue;
                hwss_eth_sockid_t actid=eth_pro->en_socknum-sid-1;
                ESP_GOTO_ON_ERROR(hwss_hso_set_sock_proto(eth->hso,actid,&proto),err,TAG,"fail to setup sock protocal");
                eth_pro->sock_use_stas[actid]=true;
                *id=actid;
                return ESP_OK;
            }
            return ESP_FAIL;

        case HWSS_PROTO_MACRAW:
            if(eth_pro->sock_use_stas[0]){
                ESP_LOGE(TAG,"Only SOCK0 support MACRAW, but it has been ocupied!");
                return ESP_FAIL;
            }
            ESP_GOTO_ON_ERROR(hwss_hso_set_sock_proto(eth->hso,0,&proto),err,TAG,"fail to setup sock protocal");
            eth_pro->sock_use_stas[0]=true;
            *id=0;
            break;
        
        default: return ESP_ERR_INVALID_ARG;
    }

err:
    return ret;
}

esp_err_t hwss_eth_sock_create_with(hwss_eth_t *eth, hwss_proto_t proto , hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(id<eth_pro->en_socknum,ESP_ERR_INVALID_ARG,err,TAG,"invalid sock id");
    ESP_GOTO_ON_FALSE(!eth_pro->sock_use_stas[id],ESP_FAIL,err,TAG,"sock occupied");
    
    switch (proto)
    {
        case HWSS_PROTO_TCP:
        case HWSS_PROTO_UDP:
            ESP_GOTO_ON_ERROR(hwss_hso_set_sock_proto(eth->hso,id,&proto),err,TAG,"fail to setup sock protocal");
            break;

        case HWSS_PROTO_MACRAW:
            ESP_GOTO_ON_FALSE(id==0,ESP_ERR_NOT_SUPPORTED,err,TAG,"only sock0 support MACRAW");
            ESP_GOTO_ON_ERROR(hwss_hso_set_sock_proto(eth->hso,id,&proto),err,TAG,"fail to setup sock protocal");
            break;
        
        default: return ESP_ERR_NOT_SUPPORTED;
    }
    
    eth_pro->sock_use_stas[id]=true;

err:
    return ret;
}

esp_err_t hwss_eth_sock_destroy(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(eth_pro->sock_stas[id]==HWSS_HSO_SOCK_CLOSED,ESP_FAIL,err,TAG,"sock is still running");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_CLOSE),err,TAG,"fail to close sock");
    eth_pro->sock_use_stas[id]=false;
err:    
    return ret;
}

esp_err_t hwss_eth_sock_open(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(eth_pro->sock_use_stas[id],ESP_FAIL,err,TAG,"cannot operate a unallocated sock");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_OPEN),err,TAG,"cannot open the sock");

    hwss_hso_socksta_t sta;
    uint32_t cnt=HWSS_ETH_SOCK_STATE_CHECK_MAX_TRY;
    while(cnt--){
        ESP_GOTO_ON_ERROR(hwss_hso_get_sock_state(eth->hso,id,&sta),err,TAG,"cannot fetch sock state");
        if(sta==HWSS_HSO_SOCK_OPENED || sta==HWSS_HSO_SOCK_TCP_INIT)
            break;
        vTaskDelay(pdMS_TO_TICKS(HWSS_ETH_SOCK_STATE_CHECK_PERIOD_MS));
    }
    eth_pro->sock_stas[id]=sta;
    ESP_GOTO_ON_FALSE(cnt!=0,ESP_ERR_TIMEOUT,err,TAG,"open sock timeout");
err:
    return ret;
}

esp_err_t hwss_eth_sock_listen(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(eth_pro->sock_use_stas[id],ESP_FAIL,err,TAG,"cannot operate a unallocated sock");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_LISTEN),err,TAG,"cannot listen the sock");

    hwss_hso_socksta_t sta;
    uint32_t cnt=HWSS_ETH_SOCK_STATE_CHECK_MAX_TRY;
    while (cnt--){
        ESP_GOTO_ON_ERROR(hwss_hso_get_sock_state(eth->hso,id,&sta),err,TAG,"cannot fetch sock state");
        if(sta==HWSS_HSO_SOCK_TCP_LISTEN)
            break;
        vTaskDelay(pdMS_TO_TICKS(HWSS_ETH_SOCK_STATE_CHECK_PERIOD_MS));
    }
    eth_pro->sock_stas[id]=sta;
    ESP_GOTO_ON_FALSE(cnt!=0,ESP_ERR_TIMEOUT,err,TAG,"listen sock timeout");
    
err:
    return ret;
}

esp_err_t hwss_eth_sock_accept_pending(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    EventBits_t bit=xEventGroupWaitBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CONNECT|HWSS_SOCK_EVENTBIT_CLOSE,
                    pdTRUE,pdFALSE,portMAX_DELAY);
    if(bit&HWSS_SOCK_EVENTBIT_CLOSE)
        ret=ESP_ERR_NOT_FINISHED;
    
    return ret;
}

esp_err_t hwss_eth_sock_connect(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(eth_pro->sock_use_stas[id],ESP_FAIL,err,TAG,"cannot operate a unallocated sock");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_CONNECT),err,TAG,"cannot connect the sock");

    EventBits_t bit=xEventGroupWaitBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CONNECT|HWSS_SOCK_EVENTBIT_TIMEOUT,
                    pdTRUE,pdFALSE,portMAX_DELAY);

    if(bit&HWSS_SOCK_EVENTBIT_CONNECT)
        eth_pro->sock_stas[id]=HWSS_HSO_SOCK_TCP_ESTAB;
    else{
        ESP_LOGE(TAG,"sock connect timeout");
        eth_pro->sock_stas[id]=HWSS_HSO_SOCK_CLOSED;
    }
    
err:
    return ret;
}

esp_err_t hwss_eth_sock_disconn(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(eth_pro->sock_use_stas[id],ESP_FAIL,err,TAG,"cannot operate a unallocated sock");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_DISCONN),err,TAG,"cannot disconnect the sock");

    EventBits_t bit=xEventGroupWaitBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_DISCONN|HWSS_SOCK_EVENTBIT_TIMEOUT,
                    pdTRUE,pdFALSE,portMAX_DELAY);

    if(bit&HWSS_SOCK_EVENTBIT_TIMEOUT)
        ESP_LOGE(TAG,"sock disconnect timeout");
    eth_pro->sock_stas[id]=HWSS_HSO_SOCK_CLOSED;

err:
    return ret;
}

esp_err_t hwss_eth_sock_close(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    ESP_GOTO_ON_FALSE(eth_pro->sock_use_stas[id],ESP_FAIL,err,TAG,"cannot operate a unallocated sock");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_CLOSE),err,TAG,"cannot close the sock");

    hwss_hso_socksta_t sta;
    uint32_t cnt=HWSS_ETH_SOCK_STATE_CHECK_MAX_TRY;
    while (cnt--){
        ESP_GOTO_ON_ERROR(hwss_hso_get_sock_state(eth->hso,id,&sta),err,TAG,"cannot fetch sock state");
        if(sta==HWSS_HSO_SOCK_CLOSED)
            break;
        vTaskDelay(pdMS_TO_TICKS(HWSS_ETH_SOCK_STATE_CHECK_PERIOD_MS));
    }

    eth_pro->sock_stas[id]=sta;
    ESP_GOTO_ON_FALSE(cnt!=0,ESP_ERR_TIMEOUT,err,TAG,"close sock timeout");

    EventBits_t rbits=xEventGroupSetBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CLOSE);
    if(rbits&HWSS_SOCK_EVENTBIT_CLOSE)
        xEventGroupClearBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_CLOSE);

err:
    return ret;
}

esp_err_t hwss_eth_sock_send(hwss_eth_t *eth, hwss_eth_sockid_t id, const uint8_t *data, uint16_t len){
    esp_err_t ret=ESP_OK;

    ESP_GOTO_ON_ERROR(hwss_hso_write_tx_buffer(eth->hso,id,data,len),err,TAG,"cannot write TX Buffer");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_SEND),err,TAG,"cannot send");

err:
    return ret;
}

esp_err_t hwss_eth_sock_send_wait_finish(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    EventBits_t bit=xEventGroupWaitBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_SEND_OK|HWSS_SOCK_EVENTBIT_TIMEOUT,
                                        pdTRUE,pdFALSE,portMAX_DELAY);
    ESP_GOTO_ON_FALSE(bit&HWSS_SOCK_EVENTBIT_SEND_OK,ESP_ERR_TIMEOUT,err,TAG,"sock send timeout");

err:
    return ret;
}

esp_err_t hwss_eth_sock_recv_pending(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_eth_pro_t *eth_pro=__containerof(eth,hwss_eth_pro_t,super);

    EventBits_t bit=xEventGroupWaitBits(eth_pro->sock_egps[id],HWSS_SOCK_EVENTBIT_RECV|HWSS_SOCK_EVENTBIT_CLOSE,
                                        pdTRUE,pdFALSE,portMAX_DELAY);
    if(bit&HWSS_SOCK_EVENTBIT_CLOSE)
        ret=ESP_ERR_NOT_FINISHED;
    return ret;
}

esp_err_t hwss_eth_sock_recv(hwss_eth_t *eth, hwss_eth_sockid_t id, uint8_t *data, uint16_t *len){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(hwss_hso_get_rx_length(eth->hso,id,len),err,TAG,"fail to get RX Length");
    ESP_GOTO_ON_ERROR(hwss_hso_read_rx_buffer(eth->hso,id,data,*len),err,TAG,"fail to read RX Buffer");
    ESP_GOTO_ON_ERROR(hwss_hso_ctrl_sock(eth->hso,id,HWSS_HSO_SOCKCTRL_RECV),err,TAG,"fail to recv");

err:
    return ret;
}

esp_err_t hwss_eth_sock_recv_drop(hwss_eth_t *eth, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    ESP_GOTO_ON_ERROR(hwss_hso_drop_rx_buffer(eth->hso,id),err,TAG,"fail to drop");
err:
    return ret;
}
