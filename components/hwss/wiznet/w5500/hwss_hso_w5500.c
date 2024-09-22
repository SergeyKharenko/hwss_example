#include <sys/cdefs.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5500.h"
#include "hwss_hso_scm.h"
#include "hwss_hso_wiznet.h"

#define W5500_SOCK_TOTAL_NUM                                8
#define W5500_SOCK_CACHE_SIZE_KB                            16

static const uint8_t    w5500_sock_mode_default=            Sn_MR_CLOSE;
static const uint16_t   w5500_sock_port_defualt=            0x0000;
static const uint8_t    w5500_sock_dest_mac_addr_default[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static const uint8_t    w5500_sock_dest_addr_default[]=     {0x00,0x00,0x00,0x00};
static const uint16_t   w5500_sock_dest_port_default=       0x0000;
static const uint16_t   w5500_sock_mss_default=             0x0000;
static const uint8_t    w5500_sock_keepalive_tick_default=  0x00;
static const uint16_t   w5500_sock_frag_default=            0x4000;
 
static const char *TAG = "w5500.hwss_hso";

typedef union{
    struct{
        uint8_t protocol :4;
        uint8_t unicastb_mip6b :1;
        uint8_t nd_mc_mmb :1;
        uint8_t bcastb :1;
        uint8_t multi_mfen :1;
    };
    uint8_t val;
}sn_mr_reg_t;

typedef struct{
    hwss_hso_t          super;
    
    uint32_t            sock_check_state_period_ms;
    uint8_t             en_sock_num;
    uint8_t             txbuf_size_kb[W5500_SOCK_TOTAL_NUM];
    uint8_t             rxbuf_size_kb[W5500_SOCK_TOTAL_NUM];
    uint16_t            irq_inter_tick;

    hwss_hso_socksta_t  sock_state[W5500_SOCK_TOTAL_NUM];
    uint8_t             sock_state_raw[W5500_SOCK_TOTAL_NUM];
    esp_timer_handle_t  sock_check_state_timer;
}hwss_hso_w5500_t;

////////
//////// HSO State Control Manager Driver Implement
////////
static esp_err_t hwss_hso_scm_w5500_get_sock_global_intr(hwss_hso_scm_t *hso_scm, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    ESP_GOTO_ON_ERROR(getSIR(hso->io,intr),err,TAG,"cannot read SIR");
err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_set_sock_global_intr_enable(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t simr=0;

    ESP_GOTO_ON_ERROR(getSIMR(hso->io,&simr),err,TAG,"cannot read SIMR");
    if(en)
        simr|=1<<id;
    else
        simr&=~(1<<id);
    ESP_GOTO_ON_ERROR(setSIMR(hso->io,&simr),err,TAG,"cannot write SIMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_set_sock_global_intr_enable_all(hwss_hso_scm_t *hso_scm, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t simr=0x00;

    if(en)
        simr=0xFF;
    ESP_GOTO_ON_ERROR(setSIMR(hso->io,&simr),err,TAG,"cannot write SIMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_get_sock_intr(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snir=0;

    ESP_GOTO_ON_ERROR(getSn_IR(hso->io,id,&snir),err,TAG,"cannot read Sn_IR");

    if(snir&Sn_IR_SENDOK)
        *intr|= HWSS_HSO_INTR_SEND_OK;

    if(snir&Sn_IR_TIMEOUT)
        *intr|=HWSS_HSO_INTR_TIMEOUT;

    if(snir&Sn_IR_RECV)
        *intr|=HWSS_HSO_INTR_RECV;
    
    if(snir&Sn_IR_DISCON)
        *intr|=HWSS_HSO_INTR_DISCONN;

    if(snir&Sn_IR_CON)
        *intr|=HWSS_HSO_INTR_CONNECT;

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_set_sock_intr_enable(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snimr=0x00;

    if(en)
        snimr=Sn_IR_SENDOK|Sn_IR_TIMEOUT|Sn_IR_RECV|Sn_IR_DISCON|Sn_IR_CON;
    ESP_GOTO_ON_ERROR(setSn_IMR(hso->io,id,&snimr),err,TAG,"cannot write Sn_IMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5500_clear_sock_intr(hwss_hso_scm_t *hso_scm, hwss_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snir=Sn_IR_SENDOK|Sn_IR_TIMEOUT|Sn_IR_RECV|Sn_IR_DISCON|Sn_IR_CON;

    ESP_GOTO_ON_ERROR(setSn_IR(hso->io,&snir),err,TAG,"cannot write Sn_IR");

err:
    return ret;
}

////////
//////// HSO Implement
////////


static void hwss_hso_w5500_check_state_timer_cb(void *args){
    hwss_hso_w5500_t *hso_w5500=(hwss_hso_w5500_t *)args;
    uint8_t snsr=0;

    for(hwss_sockid_t id=0;id<hso_w5500->en_sock_num;id++){
        if(getSn_SR(hso_w5500->super.io,id,&snsr)!=ESP_OK){
            ESP_LOGE(TAG,"cannot read Sn_SR");
            return;
        }
        
        switch(snsr){
            case SOCK_CLOSED: hso_w5500->sock_state[id]=HWSS_HSO_SOCK_CLOSED; break;
            case SOCK_INIT: hso_w5500->sock_state[id]=HWSS_HSO_SOCK_TCP_INIT; break;
            case SOCK_LISTEN: hso_w5500->sock_state[id]=HWSS_HSO_SOCK_TCP_LISTEN; break;
            case SOCK_ESTABLISHED: hso_w5500->sock_state[id]=HWSS_HSO_SOCK_TCP_ESTAB; break;
            case SOCK_FIN_WAIT:
            case SOCK_CLOSING:
            case SOCK_TIME_WAIT:
            case SOCK_CLOSE_WAIT: hso_w5500->sock_state[id]=HWSS_HSO_SOCK_TCP_CLOSEW; break;
            case SOCK_UDP: 
            case SOCK_MACRAW: hso_w5500->sock_state[id]=HWSS_HSO_SOCK_OPENED;break;
            default hso_w5500->sock_state[id]=HWSS_HSO_SOCK_CUSTOM_STATE;break;
        }
        hso_w5500->sock_state_raw[id]=snsr;
    }
}

static esp_err_t hwss_hso_w5500_init(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);
    hwss_hso_scm_t *hso_scm=hso->scm;

    ESP_GOTO_ON_ERROR(setINTLEVEL(hso->io,&(hso_w5500->irq_inter_tick)),err,TAG,"cannot write INTLEVEL");

    for(hwss_sockid_t id=0;id<W5500_SOCK_TOTAL_NUM;id++){
        ESP_GOTO_ON_ERROR(setSn_MR(hso->io,id,&w5500_sock_mode_default),err,TAG,"cannot write Sn_MR");
        ESP_GOTO_ON_ERROR(setSn_PORT(hso->io,id,&w5500_sock_port_defualt),err,TAG,"cannot write Sn_PORT");
        ESP_GOTO_ON_ERROR(setSn_DHAR(hso->io,id,w5500_sock_dest_mac_addr_default),err,TAG,"cannot write Sn_DHAR");
        ESP_GOTO_ON_ERROR(setSn_DIPR(hso->io,id,w5500_sock_dest_addr_default),err,TAG,"cannot write Sn_DIPR");
        ESP_GOTO_ON_ERROR(setSn_DPORT(hso->io,id,&w5500_sock_dest_port_default),err,TAG,"cannot write Sn_DPORT");
        ESP_GOTO_ON_ERROR(setSn_MSSR(hso->io,id,&w5500_sock_mss_default),err,TAG,"cannot write Sn_MSSR");
        ESP_GOTO_ON_ERROR(setSn_RXBUF_SIZE(hso->io,id,&(hso_w5500->rxbuf_size_kb[id])),err,TAG,"cannot write Sn_RXBUF_SIZE");
        ESP_GOTO_ON_ERROR(setSn_TXBUF_SIZE(hso->io,id,&(hso_w5500->txbuf_size_kb[id])),err,TAG,"cannot write Sn_TXBUF_SIZE");
        ESP_GOTO_ON_ERROR(setSn_FRAG(hso->io,id,&w5500_sock_frag_default),err,TAG,"cannot write Sn_FRAG");
        ESP_GOTO_ON_ERROR(setSn_KPALVTR(hso->io,id,&w5500_sock_keepalive_tick_default),err,TAG,"cannot write Sn_KPALVTR");
    }

    ESP_GOTO_ON_ERROR(esp_timer_start_periodic(hso_w5500->sock_check_state_timer,hso_w5500->check_state_period_ms*1000),
                        err,TAG,"fail to start sock timer");

    ESP_GOTO_ON_ERROR(hso_scm->init(hso_scm),err,TAG,"fail to init hso_scm");

err:
    return ret;
}

static esp_err_t hwss_hso_w5500_deinit(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);
    hwss_hso_scm_t *hso_scm=hso->scm;

    ESP_GOTO_ON_ERROR(hso_scm->deinit(hso_scm),err,TAG,"fail to stop hso_scm");
    ESP_GOTO_ON_ERROR(esp_timer_stop(hso_w5500->sock_check_state_timer),err,TAG,"fail to stop sock timer");

err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sock_proto(hwss_hso_t *hso, hwss_sockid_t id, const hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    
    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");
    
    switch (*proto)
    {
        case HWSS_PROTO_CLOSE: snmr.protocol=Sn_MR_CLOSE; break;
        case HWSS_PROTO_MACRAW: snmr.protocol=Sn_MR_MACRAW; break;
        case HWSS_PROTO_TCP: snmr.protocol=Sn_MR_TCP; break;
        case HWSS_PROTO_UDP: snmr.protocol=Sn_MR_UDP; break;
        default: return ESP_ERR_NOT_SUPPORTED;
    }

    ESP_GOTO_ON_ERROR(setSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_proto(hwss_hso_t *hso, hwss_sockid_t id, hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    
    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    switch (snmr.protocol)
    {
        case Sn_MR_CLOSE: *proto=HWSS_PROTO_CLOSE; break;
        case Sn_MR_MACRAW: *proto=HWSS_PROTO_MACRAW; break;
        case Sn_MR_TCP: *proto=HWSS_PROTO_TCP; break;
        case Sn_MR_UDP: *proto=HWSS_PROTO_UDP; break;
        default: *proto=HWSS_PROTO_UNKNOW; break;
    }

err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sockmode_opt(hwss_hso_t *hso, hwss_sockid_t id, const void *opt){
    esp_err_t ret= ESP_OK;

    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    hwss_hso_wiznet_sockmode_opt_t *wiznet_opt=(hwss_hso_wiznet_sockmode_opt_t *)opt;

    if(wiznet_opt->multicast||wiznet_opt->mac_filter)
        snmr.multi_mfen=0x01;
    else
        snmr.multi_mfen=0x00;

    if(wiznet_opt->broadcast_block)
        snmr.bcastb=0x01;
    else
        snmr.bcastb=0x00;

    if(wiznet_opt->nodelay_ack||wiznet_opt->multicast||wiznet_opt->multicast_block)
        snmr.nd_mc_mmb=0x01;
    else
        snmr.nd_mc_mmb=0x00;

    if(wiznet_opt->unicast_block||wiznet_opt->ipv6_block)
        snmr.unicastb_mip6b=0x01;
    else
        snmr.unicastb_mip6b=0x00;

    ESP_GOTO_ON_ERROR(setSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sockmode_opt(hwss_hso_t *hso, hwss_sockid_t id, void *opt){
    esp_err_t ret= ESP_OK;

    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    hwss_hso_wiznet_sockmode_opt_t *wiznet_opt=(hwss_hso_wiznet_sockmode_opt_t *)opt;
    memset(wiznet_opt,0,sizeof(hwss_hso_wiznet_sockmode_opt_t));

    if(snmr.multi_mfen){
        wiznet_opt->multicast=true;
        wiznet_opt->mac_filter=true;
    }

    if(snmr.bcastb)
        wiznet_opt->broadcast_block=true;

    if(snmr.nd_mc_mmb){
        wiznet_opt->nodelay_ack=true;
        wiznet_opt->multicast=true;
        wiznet_opt->multicast_block=true;
    }

    if(snmr.unicastb_mip6b){
        wiznet_opt->unicast_block=true;
        wiznet_opt->ipv6_block=true;
    }

err:
    return ret;    
}

static esp_err_t hwss_hso_w5500_set_sock_source_port(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(setSn_PORT(hso->io,id,port),err,TAG,"cannot write Sn_PORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_source_port(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(getSn_PORT(hso->io,id,port),err,TAG,"cannot read Sn_PORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sock_dest_port(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(setSn_DPORT(hso->io,id,port),err,TAG,"cannot write Sn_DPORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_dest_port(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(getSn_DPORT(hso->io,id,port),err,TAG,"cannot read Sn_DPORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sock_dest_mac_addr(hwss_hso_t *hso, hwss_sockid_t id, const hwss_mac_addr_t addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(setSn_DHAR(hso->io,id,addr),err,TAG,"cannot write Sn_DHAR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_dest_mac_addr(hwss_hso_t *hso, hwss_sockid_t id, hwss_mac_addr_t addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(getSn_DHAR(hso->io,id,addr),err,TAG,"cannot read Sn_DHAR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sock_dest_addr(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(setSn_DIPR(hso->io,id,addr),err,TAG,"cannot write Sn_DIPR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_dest_addr(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(getSn_DIPR(hso->io,id,addr),err,TAG,"cannot read Sn_DIPR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sock_keepalive_tick(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *tick){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(setSn_KPALVTR(hso->io,id,tick),err,TAG,"cannot write Sn_KPALVTR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_keepalive_tick(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *tick){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(getSn_KPALVTR(hso->io,id,tick),err,TAG,"cannot read Sn_KPALVTR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_set_sockact_state(hwss_hso_t *hso, hwss_sockid_t id, const hwss_hso_sockact_sta_t *sta){
    esp_err_t ret= ESP_OK;
    hwss_hso_scm_t *hso_scm=hso->scm;
    ESP_GOTO_ON_ERROR(hso_scm->set_sock_state(hso_scm,id,sta),err,TAG,"cannot set sock%u state",id);
err:
    return ret;    
}

static esp_err_t hwss_hso_w5500_get_sockact_state(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_sockact_sta_t *sta){
    esp_err_t ret= ESP_OK;
    hwss_hso_scm_t *hso_scm=hso->scm;
    ESP_GOTO_ON_ERROR(hso_scm->get_sock_state(hso_scm,id,sta),err,TAG,"cannot get sock%u state",id);
err:
    return ret;
}

static esp_err_t hwss_hso_w5500_get_sock_state(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_socksta_t *sta){
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);

    *sta=hso_w5500->sock_state[id];
    return ESP_OK;
}

static esp_err_t hwss_hso_w5500_get_sock_state_raw(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *sta){
    hwss_hso_w5500_t *hso_w5500=__containerof(hso,hwss_hso_w5500_t,super);

    *sta=hso_w5500->sock_state_raw[id];
    return ESP_OK;
}

hwss_hso_t *hwss_hso_new_w5500(hwss_io_t *io, const hwss_hso_config_t *config){
    hwss_hso_t *ret=NULL;
    hwss_hso_w5500_t* hso=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");
    ESP_GOTO_ON_FALSE(config->en_sock_num>1,NULL,err,TAG,"at least 2 sockets should be enabled");
    ESP_GOTO_ON_FALSE(config->en_sock_num<=W5500_SOCK_TOTAL_NUM,NULL,err,TAG,"max socket of W5500 is 8");
    ESP_GOTO_ON_FALSE(config->txbuf_size_kb && config->rxbuf_size_kb,NULL,err,TAG,"cannot set tx/rxbuf_size_kb to null");

    uint16_t tx_cache_size_kb=0, rx_cache_size_kb=0;
    for(hwss_sockid_t id=0;id<config->en_sock_num;id++){
        tx_cache_size_kb+=config->txbuf_size_kb[id];
        rx_cache_size_kb+=config->rxbuf_size_kb[id];
    }

    ESP_GOTO_ON_FALSE(tx_cache_size_kb<=W5500_SOCK_CACHE_SIZE_KB && rx_cache_size_kb<=W5500_SOCK_CACHE_SIZE_KB,
                        NULL,err,TAG,"sum of tx/rx cache size should not be over 16KB");

    if(config->irq_gpio_num<0 && config->sock_polling_period_ms<5)
        ESP_LOGW(TAG,"polling period too short might SERIOUSLY affects the execution of other tasks!");

    hso=calloc(1,sizeof(hwss_hso_w5500_t));
    ESP_GOTO_ON_FALSE(hso,NULL,err,TAG,"calloc hso failed");

    hso->super.io=io;
    hso->super.init=hwss_hso_w5500_init;
    hso->super.deinit=hwss_hso_w5500_deinit;
    hso->super.set_sock_proto=hwss_hso_w5500_set_sock_proto;
    hso->super.get_sock_proto=hwss_hso_w5500_get_sock_proto;
    hso->super.set_sockmode_opt=hwss_hso_w5500_set_sockmode_opt;
    hso->super.get_sockmode_opt=hwss_hso_w5500_get_sockmode_opt;
    hso->super.set_sock_source_port=hwss_hso_w5500_set_sock_source_port;
    hso->super.get_sock_source_port=hwss_hso_w5500_get_sock_source_port;
    hso->super.set_sock_dest_port=hwss_hso_w5500_set_sock_dest_port;
    hso->super.get_sock_dest_port=hwss_hso_w5500_get_sock_dest_port;
    hso->super.set_sock_dest_mac_addr=hwss_hso_w5500_set_sock_dest_mac_addr;
    hso->super.get_sock_dest_mac_addr=hwss_hso_w5500_get_sock_dest_mac_addr;
    hso->super.set_sock_keepalive_tick=hwss_hso_w5500_set_sock_keepalive_tick;
    hso->super.get_sock_keepalive_tick=hwss_hso_w5500_get_sock_keepalive_tick;
    hso->super.set_sockact_state=hwss_hso_w5500_set_sockact_state;
    hso->super.get_sockact_state=hwss_hso_w5500_get_sockact_state;
    hso->super.get_sock_state=hwss_hso_w5500_get_sock_state;
    hso->super.get_sock_state_raw=hwss_hso_w5500_get_sock_state_raw;

    hso->en_sock_num=config->en_sock_num;
    if(config->irq_inter_time_us==0)
        hso->irq_inter_tick=0;
    else
        hso->irq_inter_tick=config->irq_inter_time_us*150/4-1;
    memcpy(hso->txbuf_size_kb,config->txbuf_size_kb,hso->en_sock_num);
    memcpy(hso->rxbuf_size_kb,config->rxbuf_size_kb,hso->en_sock_num);
    hso->sock_check_state_period_ms=config->sock_check_state_period_ms;

    esp_timer_create_args_t timer_arg={
        .arg=hso,
        .name="hwss_hso_w5500_check_state_timer",
        .callback=hwss_hso_w5500_check_state_timer_cb,
        .skip_unhandled_events=true
    };
    ESP_GOTO_ON_FALSE(esp_timer_create(&timer_arg,&hso->sock_check_state_timer)==ESP_OK,NULL,err,TAG,"create sock check state timer failed");

    hwss_hso_scm_config_t scm_config={
        .en_sock_num=config->en_sock_num,
        .irq_gpio_num=config->irq_gpio_num,
        .irq_handler_task_prio=config->irq_handler_task_prio,
        .irq_handler_task_stack_size=config->irq_handler_task_stack_size,
        .sock_active_threshold_ms=config->sock_active_threshold_ms,
        .sock_polling_period_ms=config->sock_polling_period_ms,
        .sock_total_num=W5500_SOCK_TOTAL_NUM
    };

    hso->super.scm=hwss_hso_scm_new(&hso->super,&scm_config);
    ESP_GOTO_ON_FALSE(hso->super.scm,NULL,err,TAG,"cannot create hso_scm");

    hwss_hso_scm_t *hso_scm=hso->super.scm;
    hso_scm->drv.get_sock_global_intr=hwss_hso_scm_w5500_get_sock_global_intr;
    hso_scm->drv.set_sock_global_intr_enable=hwss_hso_scm_w5500_set_sock_global_intr_enable;
    hso_scm->drv.set_sock_global_intr_enable_all=hwss_hso_scm_w5500_set_sock_global_intr_enable_all;
    hso_scm->drv.get_sock_intr=hwss_hso_scm_w5500_get_sock_intr;
    hso_scm->drv.set_sock_intr_enable=hwss_hso_scm_w5500_set_sock_intr_enable;
    hso_scm->drv.clear_sock_intr=hwss_hso_scm_w5500_clear_sock_intr;

    return &hso->super;

err:
    if(hso){
        if(hso->super.scm)
            free(hso->super.scm);
        free(hso);
    }
    return ret;
}
