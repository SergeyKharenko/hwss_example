#include <sys/cdefs.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_w5100s.h"
#include "hwss_hso_scm.h"
#include "hwss_hso_wiznet.h"

#define W5100S_SOCK_TOTAL_NUM                                4
#define W5100S_SOCK_CACHE_SIZE_KB                            16

static const uint8_t    w5100s_sock_mode_default=            W5100S_Sn_MR_CLOSE;
static const uint16_t   w5100s_sock_port_defualt=            0x0000;
static const uint8_t    w5100s_sock_dest_mac_addr_default[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static const uint8_t    w5100s_sock_dest_addr_default[]=     {0x00,0x00,0x00,0x00};
static const uint16_t   w5100s_sock_dest_port_default=       0x0000;
static const uint16_t   w5100s_sock_mss_default=             0x0000;
static const uint8_t    w5100s_sock_keepalive_tick_default=  0x00;
static const uint16_t   w5100s_sock_frag_default=            0x4000;
 
static const char *TAG = "w5100s.hwss_hso";

typedef union{
    struct{
        uint8_t intr :4;
        uint8_t reserve :1;
        uint8_t pppterm :1;
        uint8_t unreach :1;
        uint8_t cnft :1;
    };
    uint8_t val;
}imr_reg_t;

typedef union{
    struct{
        uint8_t protocol :4;
        uint8_t reserve :1;
        uint8_t nd_mc :1;
        uint8_t mf :1;
        uint8_t multi :1;
    };
    uint8_t val;
}sn_mr_reg_t;

typedef union{
    struct{
        uint8_t unib :1;
        uint8_t brdb :1;
        uint8_t reserve1 :2;
        uint8_t ipv6blk :1;
        uint8_t mmblk :1;
        uint8_t mbblk :1;
        uint8_t reserve2 :1;
    };
    uint8_t val;
}sn_mr2_reg_t;

typedef struct{
    hwss_hso_t          super;
    
    uint8_t             en_sock_num;
    uint8_t             txbuf_size_kb[W5100S_SOCK_TOTAL_NUM];
    uint8_t             rxbuf_size_kb[W5100S_SOCK_TOTAL_NUM];
    uint16_t            irq_inter_tick;

    bool                is_started;
}hwss_hso_w5100s_t;

////////
//////// HSO State Control Manager Driver Implement
////////
static esp_err_t hwss_hso_scm_w5100s_get_sock_global_intr(hwss_hso_scm_t *hso_scm, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t stat;

    ESP_GOTO_ON_ERROR(W5100S_getIR(hso->io,&stat),err,TAG,"cannot read IR");
    *intr=stat&0x0F;
err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5100s_set_sock_global_intr_enable(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    imr_reg_t imr;

    ESP_GOTO_ON_ERROR(W5100S_getIMR(hso->io,&imr.val),err,TAG,"cannot read IMR");
    if(en)
        imr.intr|=1<<id;
    else
        imr.intr&=~(1<<id);
    ESP_GOTO_ON_ERROR(W5100S_setIMR(hso->io,&imr.val),err,TAG,"cannot write IMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5100s_set_sock_global_intr_enable_all(hwss_hso_scm_t *hso_scm, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    imr_reg_t imr;

    ESP_GOTO_ON_ERROR(W5100S_getIMR(hso->io,&imr.val),err,TAG,"cannot read IMR");
    if(en)
        imr.intr=0x0F;
    else
        imr.intr=0x00;
    ESP_GOTO_ON_ERROR(W5100S_setIMR(hso->io,&imr.val),err,TAG,"cannot write IMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5100s_get_sock_intr(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, uint8_t *intr){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snir=0;

    ESP_GOTO_ON_ERROR(W5100S_getSn_IR(hso->io,id,&snir),err,TAG,"cannot read Sn_IR");

    if(snir&W5100S_Sn_IR_SENDOK)
        *intr|= HWSS_HSO_INTR_SEND_OK;

    if(snir&W5100S_Sn_IR_TIMEOUT)
        *intr|=HWSS_HSO_INTR_TIMEOUT;

    if(snir&W5100S_Sn_IR_RECV)
        *intr|=HWSS_HSO_INTR_RECV;
    
    if(snir&W5100S_Sn_IR_DISCON)
        *intr|=HWSS_HSO_INTR_DISCONN;

    if(snir&W5100S_Sn_IR_CON)
        *intr|=HWSS_HSO_INTR_CONNECT;

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5100s_set_sock_intr_enable(hwss_hso_scm_t *hso_scm, hwss_sockid_t id, bool en){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snimr=0x00;

    if(en)
        snimr=W5100S_Sn_IR_SENDOK|W5100S_Sn_IR_TIMEOUT|W5100S_Sn_IR_RECV|W5100S_Sn_IR_DISCON|W5100S_Sn_IR_CON;
    ESP_GOTO_ON_ERROR(W5100S_setSn_IMR(hso->io,id,&snimr),err,TAG,"cannot write Sn_IMR");

err:
    return ret;
}

static esp_err_t hwss_hso_scm_w5100s_clear_sock_intr(hwss_hso_scm_t *hso_scm, hwss_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_hso_t *hso=hso_scm->hso;
    uint8_t snir=W5100S_Sn_IR_SENDOK|W5100S_Sn_IR_TIMEOUT|W5100S_Sn_IR_RECV|W5100S_Sn_IR_DISCON|W5100S_Sn_IR_CON;

    ESP_GOTO_ON_ERROR(W5100S_setSn_IR(hso->io,id,&snir),err,TAG,"cannot write Sn_IR");

err:
    return ret;
}

////////
//////// HSO Implement
////////

static esp_err_t hwss_hso_w5100s_init(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    hwss_hso_scm_t *hso_scm=hso->scm;

    ESP_GOTO_ON_ERROR(W5100S_setINTPTMR(hso->io,&(hso_w5100s->irq_inter_tick)),err,TAG,"cannot write INTPTMR");

    for(hwss_sockid_t id=0;id<W5100S_SOCK_TOTAL_NUM;id++){
        ESP_GOTO_ON_ERROR(W5100S_setSn_MR(hso->io,id,&w5100s_sock_mode_default),err,TAG,"cannot write Sn_MR");
        ESP_GOTO_ON_ERROR(W5100S_setSn_PORT(hso->io,id,&w5100s_sock_port_defualt),err,TAG,"cannot write Sn_PORT");
        ESP_GOTO_ON_ERROR(W5100S_setSn_DHAR(hso->io,id,w5100s_sock_dest_mac_addr_default),err,TAG,"cannot write Sn_DHAR");
        ESP_GOTO_ON_ERROR(W5100S_setSn_DIPR(hso->io,id,w5100s_sock_dest_addr_default),err,TAG,"cannot write Sn_DIPR");
        ESP_GOTO_ON_ERROR(W5100S_setSn_DPORT(hso->io,id,&w5100s_sock_dest_port_default),err,TAG,"cannot write Sn_DPORT");
        ESP_GOTO_ON_ERROR(W5100S_setSn_MSSR(hso->io,id,&w5100s_sock_mss_default),err,TAG,"cannot write Sn_MSSR");
        ESP_GOTO_ON_ERROR(W5100S_setSn_RXBUF_SIZE(hso->io,id,&(hso_w5100s->rxbuf_size_kb[id])),err,TAG,"cannot write Sn_RXBUF_SIZE");
        ESP_GOTO_ON_ERROR(W5100S_setSn_TXBUF_SIZE(hso->io,id,&(hso_w5100s->txbuf_size_kb[id])),err,TAG,"cannot write Sn_TXBUF_SIZE");
        ESP_GOTO_ON_ERROR(W5100S_setSn_FRAGR(hso->io,id,&w5100s_sock_frag_default),err,TAG,"cannot write Sn_FRAGR");
        ESP_GOTO_ON_ERROR(W5100S_setSn_KPALVTR(hso->io,id,&w5100s_sock_keepalive_tick_default),err,TAG,"cannot write Sn_KPALVTR");
    }

    ESP_GOTO_ON_ERROR(hso_scm->init(hso_scm),err,TAG,"fail to init hso_scm");

err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_deinit(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    hwss_hso_scm_t *hso_scm=hso->scm;

    if(hso_w5100s->is_started)
        ESP_GOTO_ON_ERROR(hso->stop(hso),err,TAG,"fail to stop hwss_hso");

    ESP_GOTO_ON_ERROR(hso_scm->deinit(hso_scm),err,TAG,"fail to deinit hso_scm");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_start(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    hwss_hso_scm_t *hso_scm=hso->scm;

    if(hso_w5100s->is_started)
        return ret;
    
    ESP_GOTO_ON_ERROR(hso_scm->start(hso_scm),err,TAG,"cannot start hwss_hso_scm");
    hso_w5100s->is_started=true;
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_stop(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    hwss_hso_scm_t *hso_scm=hso->scm;

    if(!hso_w5100s->is_started)
        return ret;
    
    ESP_GOTO_ON_ERROR(hso_scm->stop(hso_scm),err,TAG,"cannot stop hwss_hso_scm");
    hso_w5100s->is_started=false;
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_ctrl_sock(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_sockctrl_t ctrl){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    uint8_t sncr=0;

    while(1){
        ESP_GOTO_ON_ERROR(W5100S_getSn_CR(hso->io,id,&sncr),err,TAG,"cannot read Sn_CR");
        if(sncr!=0)
            continue;
        else
            break;
    }

    switch (ctrl)
    {
    case HWSS_HSO_SOCKCTRL_OPEN: sncr|=W5100S_Sn_CR_OPEN; break;
    case HWSS_HSO_SOCKCTRL_LISTEN: sncr|=W5100S_Sn_CR_LISTEN; break;
    case HWSS_HSO_SOCKCTRL_CONNECT: sncr|=W5100S_Sn_CR_CONNECT; break;
    case HWSS_HSO_SOCKCTRL_DISCONN: sncr|=W5100S_Sn_CR_DISCON; break;
    case HWSS_HSO_SOCKCTRL_CLOSE: sncr|=W5100S_Sn_CR_CLOSE; break;
    case HWSS_HSO_SOCKCTRL_SEND: sncr|=W5100S_Sn_CR_SEND; break;
    case HWSS_HSO_SOCKCTRL_SENDMAC: sncr|=W5100S_Sn_CR_SEND_MAC; break;
    case HWSS_HSO_SOCKCTRL_SENDKEEP: sncr|=W5100S_Sn_CR_SEND_KEEP; break;
    case HWSS_HSO_SOCKCTRL_RECV: sncr|=W5100S_Sn_CR_RECV; break;
    default:
        ret=ESP_ERR_NOT_SUPPORTED;
        goto err;
    }

    ESP_GOTO_ON_ERROR(W5100S_setSn_CR(hso->io,id,&sncr),err,TAG,"cannot write Sn_CR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_write_tx_buffer(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *data, uint16_t len){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    uint16_t ptr=0;
    
    ESP_GOTO_ON_ERROR(W5100S_getSn_TX_WR(hso->io,id,&ptr),err,TAG,"cannot get tx pointer");
    
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_read_rx_buffer(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *data, uint16_t len);
static esp_err_t hwss_hso_w5100s_get_rx_length(hwss_hso_t *hso, hwss_sockid_t id, uint16_t *len);

static esp_err_t hwss_hso_w5100s_set_sock_proto(hwss_hso_t *hso, hwss_sockid_t id, const hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    
    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(W5100S_getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");
    
    switch (*proto)
    {
        case HWSS_PROTO_CLOSE: snmr.protocol=W5100S_Sn_MR_CLOSE; break;
        case HWSS_PROTO_MACRAW: snmr.protocol=W5100S_Sn_MR_MACRAW; break;
        case HWSS_PROTO_TCP: snmr.protocol=W5100S_Sn_MR_TCP; break;
        case HWSS_PROTO_UDP: snmr.protocol=W5100S_Sn_MR_UDP; break;
        default: return ESP_ERR_NOT_SUPPORTED;
    }

    ESP_GOTO_ON_ERROR(W5100S_setSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_proto(hwss_hso_t *hso, hwss_sockid_t id, hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    
    sn_mr_reg_t snmr;
    ESP_GOTO_ON_ERROR(W5100S_getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    switch (snmr.protocol)
    {
        case W5100S_Sn_MR_CLOSE: *proto=HWSS_PROTO_CLOSE; break;
        case W5100S_Sn_MR_MACRAW: *proto=HWSS_PROTO_MACRAW; break;
        case W5100S_Sn_MR_TCP: *proto=HWSS_PROTO_TCP; break;
        case W5100S_Sn_MR_UDP: *proto=HWSS_PROTO_UDP; break;
        default: *proto=HWSS_PROTO_UNKNOW; break;
    }

err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_set_sockmode_opt(hwss_hso_t *hso, hwss_sockid_t id, const void *opt){
    esp_err_t ret= ESP_OK;

    sn_mr_reg_t snmr;
    sn_mr2_reg_t snmr2;
    hwss_hso_wiznet_sockmode_opt_t *wiznet_opt=(hwss_hso_wiznet_sockmode_opt_t *)opt;

    memset(&snmr.val,0,sizeof(sn_mr_reg_t));
    memset(&snmr2.val,0,sizeof(sn_mr2_reg_t));

    if(wiznet_opt->udp_multicast)
        snmr.multi=0x01;
    
    if(wiznet_opt->macraw_mac_filter)
        snmr.mf=0x01;
    
    if(wiznet_opt->nodelay_ack)
        snmr.nd_mc=0x01;
    
    if(wiznet_opt->macraw_broadcast_block)
        snmr2.mbblk=0x01;
    
    if(wiznet_opt->macraw_multicast_block)
        snmr2.mmblk=0x01;

    if(wiznet_opt->macraw_ipv6_block)
        snmr2.ipv6blk=0x01;
    
    if(wiznet_opt->udp_broadcast_block)
        snmr2.brdb=0x01;
    
    if(wiznet_opt->udp_unicast_block)
        snmr2.unib=0x01;

    ESP_GOTO_ON_ERROR(W5100S_setSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
    ESP_GOTO_ON_ERROR(W5100S_setSn_MR2(hso->io,id,&(snmr2.val)),err,TAG,"cannot write Sn_MR2");

err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sockmode_opt(hwss_hso_t *hso, hwss_sockid_t id, void *opt){
    esp_err_t ret= ESP_OK;

    sn_mr_reg_t snmr;
    sn_mr2_reg_t snmr2;

    ESP_GOTO_ON_ERROR(W5100S_getSn_MR(hso->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");
    ESP_GOTO_ON_ERROR(W5100S_getSn_MR2(hso->io,id,&(snmr2.val)),err,TAG,"cannot read Sn_MR2");

    hwss_hso_wiznet_sockmode_opt_t *wiznet_opt=(hwss_hso_wiznet_sockmode_opt_t *)opt;
    memset(wiznet_opt,0,sizeof(hwss_hso_wiznet_sockmode_opt_t));

    if(snmr.multi)
        wiznet_opt->udp_multicast=true;
    
    if(snmr.mf)
        wiznet_opt->macraw_mac_filter=true;

    if(snmr.nd_mc)
        wiznet_opt->nodelay_ack=true;
    else
        wiznet_opt->igmp_v2=true;

    if(snmr2.mmblk)
        wiznet_opt->macraw_broadcast_block=true;
    
    if(snmr2.mbblk)
        wiznet_opt->macraw_multicast_block=true;

    if(snmr2.ipv6blk)
        wiznet_opt->macraw_ipv6_block=true;

    if(snmr2.brdb){
        wiznet_opt->udp_broadcast_block=true;
        wiznet_opt->tcp_force_psh=true;
    }

    if(snmr2.unib)
        wiznet_opt->udp_unicast_block=true;

err:
    return ret;    
}

static esp_err_t hwss_hso_w5100s_set_sock_source_port(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_setSn_PORT(hso->io,id,port),err,TAG,"cannot write Sn_PORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_source_port(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_getSn_PORT(hso->io,id,port),err,TAG,"cannot read Sn_PORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_set_sock_dest_port(hwss_hso_t *hso, hwss_sockid_t id, const hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_setSn_DPORT(hso->io,id,port),err,TAG,"cannot write Sn_DPORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_dest_port(hwss_hso_t *hso, hwss_sockid_t id, hwss_port_t *port){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_getSn_DPORT(hso->io,id,port),err,TAG,"cannot read Sn_DPORT");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_set_sock_dest_mac_addr(hwss_hso_t *hso, hwss_sockid_t id, const hwss_mac_addr_t addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_setSn_DHAR(hso->io,id,addr),err,TAG,"cannot write Sn_DHAR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_dest_mac_addr(hwss_hso_t *hso, hwss_sockid_t id, hwss_mac_addr_t addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_getSn_DHAR(hso->io,id,addr),err,TAG,"cannot read Sn_DHAR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_set_sock_dest_addr(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_setSn_DIPR(hso->io,id,addr),err,TAG,"cannot write Sn_DIPR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_dest_addr(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *addr){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_getSn_DIPR(hso->io,id,addr),err,TAG,"cannot read Sn_DIPR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_set_sock_keepalive_tick(hwss_hso_t *hso, hwss_sockid_t id, const uint8_t *tick){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_setSn_KPALVTR(hso->io,id,tick),err,TAG,"cannot write Sn_KPALVTR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_keepalive_tick(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *tick){
    esp_err_t ret= ESP_OK;

    ESP_GOTO_ON_ERROR(W5100S_getSn_KPALVTR(hso->io,id,tick),err,TAG,"cannot read Sn_KPALVTR");
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_set_sockact_state(hwss_hso_t *hso, hwss_sockid_t id, const hwss_hso_sockact_sta_t *sta){
    esp_err_t ret= ESP_OK;
    hwss_hso_scm_t *hso_scm=hso->scm;
    ESP_GOTO_ON_ERROR(hso_scm->set_sock_state(hso_scm,id,sta),err,TAG,"cannot set sock%u state",id);
err:
    return ret;    
}

static esp_err_t hwss_hso_w5100s_get_sockact_state(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_sockact_sta_t *sta){
    esp_err_t ret= ESP_OK;
    hwss_hso_scm_t *hso_scm=hso->scm;
    ESP_GOTO_ON_ERROR(hso_scm->get_sock_state(hso_scm,id,sta),err,TAG,"cannot get sock%u state",id);
err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_state(hwss_hso_t *hso, hwss_sockid_t id, hwss_hso_socksta_t *sta){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);
    uint8_t snsr;

    ESP_GOTO_ON_ERROR(W5100S_getSn_SR(hso_w5100s->super.io,id,&snsr),err,TAG,"cannot read Sn_SR");
    
    switch(snsr){
        case W5100S_SOCK_CLOSED: *sta=HWSS_HSO_SOCK_CLOSED; break;
        case W5100S_SOCK_INIT: *sta=HWSS_HSO_SOCK_TCP_INIT; break;
        case W5100S_SOCK_LISTEN: *sta=HWSS_HSO_SOCK_TCP_LISTEN; break;
        case W5100S_SOCK_ESTABLISHED: *sta=HWSS_HSO_SOCK_TCP_ESTAB; break;
        case W5100S_SOCK_FIN_WAIT:
        case W5100S_SOCK_CLOSING:
        case W5100S_SOCK_TIME_WAIT:
        case W5100S_SOCK_LAST_ACK:
        case W5100S_SOCK_CLOSE_WAIT: *sta=HWSS_HSO_SOCK_TCP_CLOSEW; break;
        case W5100S_SOCK_UDP: 
        case W5100S_SOCK_MACRAW: *sta=HWSS_HSO_SOCK_OPENED;break;
        default :*sta=HWSS_HSO_SOCK_CUSTOM_STATE;break;
    }

err:
    return ret;
}

static esp_err_t hwss_hso_w5100s_get_sock_state_raw(hwss_hso_t *hso, hwss_sockid_t id, uint8_t *sta){
    esp_err_t ret=ESP_OK;
    hwss_hso_w5100s_t *hso_w5100s=__containerof(hso,hwss_hso_w5100s_t,super);

    ESP_GOTO_ON_ERROR(W5100S_getSn_SR(hso_w5100s->super.io,id,sta),err,TAG,"cannot read Sn_SR");
err:
    return ret;
}

hwss_hso_t *hwss_hso_new_w5100s(esp_event_loop_handle_t elp, hwss_io_t *io, hwss_hir_t *hir, const hwss_hso_config_t *config){
    hwss_hso_t *ret=NULL;
    hwss_hso_w5100s_t* hso=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");
    ESP_GOTO_ON_FALSE(config->en_sock_num>1,NULL,err,TAG,"at least 2 sockets should be enabled");
    ESP_GOTO_ON_FALSE(config->en_sock_num<=W5100S_SOCK_TOTAL_NUM,NULL,err,TAG,"max socket of W5100S is 8");
    ESP_GOTO_ON_FALSE(config->txbuf_size_kb && config->rxbuf_size_kb,NULL,err,TAG,"cannot set tx/rxbuf_size_kb to null");

    uint16_t tx_cache_size_kb=0, rx_cache_size_kb=0;
    for(hwss_sockid_t id=0;id<config->en_sock_num;id++){
        tx_cache_size_kb+=config->txbuf_size_kb[id];
        rx_cache_size_kb+=config->rxbuf_size_kb[id];
    }

    ESP_GOTO_ON_FALSE(tx_cache_size_kb<=W5100S_SOCK_CACHE_SIZE_KB && rx_cache_size_kb<=W5100S_SOCK_CACHE_SIZE_KB,
                        NULL,err,TAG,"sum of tx/rx cache size should not be over 16KB");

    if(hir==NULL && config->sock_polling_period_ms<5)
        ESP_LOGW(TAG,"polling period too short might SERIOUSLY affects the execution of other tasks!");

    hso=calloc(1,sizeof(hwss_hso_w5100s_t));
    ESP_GOTO_ON_FALSE(hso,NULL,err,TAG,"calloc hso failed");

    hso->super.io=io;
    hso->super.init=hwss_hso_w5100s_init;
    hso->super.deinit=hwss_hso_w5100s_deinit;
    hso->super.start=hwss_hso_w5100s_start;
    hso->super.stop=hwss_hso_w5100s_stop;
    hso->super.ctrl_sock=hwss_hso_w5100s_ctrl_sock;
    hso->super.set_sock_proto=hwss_hso_w5100s_set_sock_proto;
    hso->super.get_sock_proto=hwss_hso_w5100s_get_sock_proto;
    hso->super.set_sockmode_opt=hwss_hso_w5100s_set_sockmode_opt;
    hso->super.get_sockmode_opt=hwss_hso_w5100s_get_sockmode_opt;
    hso->super.set_sock_source_port=hwss_hso_w5100s_set_sock_source_port;
    hso->super.get_sock_source_port=hwss_hso_w5100s_get_sock_source_port;
    hso->super.set_sock_dest_port=hwss_hso_w5100s_set_sock_dest_port;
    hso->super.get_sock_dest_port=hwss_hso_w5100s_get_sock_dest_port;
    hso->super.set_sock_dest_mac_addr=hwss_hso_w5100s_set_sock_dest_mac_addr;
    hso->super.get_sock_dest_mac_addr=hwss_hso_w5100s_get_sock_dest_mac_addr;
    hso->super.set_sock_dest_addr=hwss_hso_w5100s_set_sock_dest_addr;
    hso->super.get_sock_dest_addr=hwss_hso_w5100s_get_sock_dest_addr;
    hso->super.set_sock_keepalive_tick=hwss_hso_w5100s_set_sock_keepalive_tick;
    hso->super.get_sock_keepalive_tick=hwss_hso_w5100s_get_sock_keepalive_tick;
    hso->super.set_sockact_state=hwss_hso_w5100s_set_sockact_state;
    hso->super.get_sockact_state=hwss_hso_w5100s_get_sockact_state;
    hso->super.get_sock_state=hwss_hso_w5100s_get_sock_state;
    hso->super.get_sock_state_raw=hwss_hso_w5100s_get_sock_state_raw;

    hso->en_sock_num=config->en_sock_num;
    hso->irq_inter_tick=config->irq_inter_time_us*25;
    memcpy(hso->txbuf_size_kb,config->txbuf_size_kb,hso->en_sock_num);
    memcpy(hso->rxbuf_size_kb,config->rxbuf_size_kb,hso->en_sock_num);
    hso->is_started=false;

    hwss_hso_scm_config_t scm_config={
        .en_sock_num=config->en_sock_num,
        .sock_active_threshold_ms=config->sock_active_threshold_ms,
        .sock_polling_period_ms=config->sock_polling_period_ms,
        .sock_total_num=W5100S_SOCK_TOTAL_NUM
    };

    hso->super.scm=hwss_hso_scm_new(elp,&hso->super,hir,&scm_config);
    ESP_GOTO_ON_FALSE(hso->super.scm,NULL,err,TAG,"cannot create hso_scm");

    hwss_hso_scm_t *hso_scm=hso->super.scm;
    hso_scm->drv.get_sock_global_intr=hwss_hso_scm_w5100s_get_sock_global_intr;
    hso_scm->drv.set_sock_global_intr_enable=hwss_hso_scm_w5100s_set_sock_global_intr_enable;
    hso_scm->drv.set_sock_global_intr_enable_all=hwss_hso_scm_w5100s_set_sock_global_intr_enable_all;
    hso_scm->drv.get_sock_intr=hwss_hso_scm_w5100s_get_sock_intr;
    hso_scm->drv.set_sock_intr_enable=hwss_hso_scm_w5100s_set_sock_intr_enable;
    hso_scm->drv.clear_sock_intr=hwss_hso_scm_w5100s_clear_sock_intr;

    return &hso->super;

err:
    if(hso){
        if(hso->super.scm)
            free(hso->super.scm);
        free(hso);
    }
    return ret;
}
