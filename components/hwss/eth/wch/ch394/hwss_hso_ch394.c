#include <sys/cdefs.h>
#include <machine/endian.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_timer.h"
#include "drv_ch394.h"
#include "hwss_hso_wiznet.h"

#define CH394_UDP_HEADER_LEN                                   8
#define CH394_MACRAW_HEADER_LEN                                16

#define CH394_UDP_HEADER_ADDR_BIAS                             0
#define CH394_UDP_HEADER_PORT_BIAS                             CH394_UDP_HEADER_ADDR_BIAS+HWSS_ETH_IP4_ADDR_LEN
#define CH394_UDP_HEADER_LEN_BIAS                              CH394_UDP_HEADER_PORT_BIAS+sizeof(hwss_eth_port_t)

#define CH394_MACRAW_HEADER_LEN_BIAS                           0
#define CH394_MACRAW_HEADER_DADDR_BIAS                         CH394_MACRAW_HEADER_LEN_BIAS+sizeof(uint16_t)
#define CH394_MACRAW_HEADER_SADDR_BIAS                         CH394_MACRAW_HEADER_DADDR_BIAS+HWSS_ETH_MAC_ADDR_LEN
#define CH394_MACRAW_HEADER_TYPE_BIAS                          CH394_MACRAW_HEADER_SADDR_BIAS+HWSS_ETH_MAC_ADDR_LEN

static const uint8_t    ch394_sock_mode_default=            CH394_Sn_MODE_CLOSE;
static const uint16_t   ch394_sock_port_defualt=            0x0000;
static const uint8_t    ch394_sock_dest_mac_addr_default[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static const uint8_t    ch394_sock_dest_addr_default[]=     {0x00,0x00,0x00,0x00};
static const uint16_t   ch394_sock_dest_port_default=       0x0000;
static const uint16_t   ch394_sock_mtu_default=             0x0000;
static const uint8_t    ch394_sock_keepalive_tick_default=  0x00;
static const uint16_t   ch394_sock_ipf_default=             0x4000;
 
static const char *TAG = "ch394.hwss_hso";

typedef union{
    #if BYTE_ORDER == LITTLE_ENDIAN
    struct{
        uint8_t protocol :4;
        uint8_t unicastb_mip6b :1;
        uint8_t nd_mc_mmb :1;
        uint8_t bcastb :1;
        uint8_t multi_mfen :1;
    };
    #else
    struct{
        uint8_t unicastb_mip6b :1;
        uint8_t nd_mc_mmb :1;
        uint8_t bcastb :1;        
        uint8_t multi_mfen :1;
        uint8_t protocol :4;
    };
    #endif
    uint8_t val;
}sn_mr_reg_t;

typedef struct{
    hwss_hso_t          super;

    hwss_io_t           *io;
    
    uint8_t             txbuf_size_kb[CH394_SOCKNUM];
    uint8_t             rxbuf_size_kb[CH394_SOCKNUM];
    uint8_t             en_socknum;
}hwss_hso_ch394_t;


////////
//////// HSO Implement
////////

static esp_err_t hwss_hso_ch394_init(hwss_hso_t *hso){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    for(hwss_eth_sockid_t id=0;id<hso_ch394->en_socknum;id++){
        ESP_GOTO_ON_ERROR(CH394_SetSn_MODE(hso_ch394->io,id,&ch394_sock_mode_default),err,TAG,"cannot write Sn_MODE");
        ESP_GOTO_ON_ERROR(CH394_SetSn_PORT(hso_ch394->io,id,&ch394_sock_port_defualt),err,TAG,"cannot write Sn_PORT");
        ESP_GOTO_ON_ERROR(CH394_SetSn_DMAC(hso_ch394->io,id,ch394_sock_dest_mac_addr_default),err,TAG,"cannot write Sn_DMAC");
        ESP_GOTO_ON_ERROR(CH394_SetSn_DIP(hso_ch394->io,id,ch394_sock_dest_addr_default),err,TAG,"cannot write Sn_DIP");
        ESP_GOTO_ON_ERROR(CH394_SetSn_DPORT(hso_ch394->io,id,&ch394_sock_dest_port_default),err,TAG,"cannot write Sn_DPORT");
        ESP_GOTO_ON_ERROR(CH394_SetSn_MTU(hso_ch394->io,id,&ch394_sock_mtu_default),err,TAG,"cannot write Sn_MTU");
        ESP_GOTO_ON_ERROR(CH394_SetSn_IPF(hso_ch394->io,id,&ch394_sock_ipf_default),err,TAG,"cannot write Sn_IPF");
        ESP_GOTO_ON_ERROR(CH394_SetSn_KEEPALIVE(hso_ch394->io,id,&ch394_sock_keepalive_tick_default),err,TAG,"cannot write Sn_KPALVTR");
    }

    for(hwss_eth_sockid_t id=0;id<CH394_SOCKNUM;id++){
        ESP_GOTO_ON_ERROR(CH394_SetSn_RXBUF_SIZE(hso_ch394->io,id,&(hso_ch394->rxbuf_size_kb[id])),err,TAG,"cannot write Sn_RXBUF_SIZE");
        ESP_GOTO_ON_ERROR(CH394_SetSn_TXBUF_SIZE(hso_ch394->io,id,&(hso_ch394->txbuf_size_kb[id])),err,TAG,"cannot write Sn_TXBUF_SIZE");
    }

err:
    return ret;
}

static esp_err_t hwss_hso_ch394_deinit(hwss_hso_t *hso){
    return ESP_OK;
}

static esp_err_t hwss_hso_ch394_ctrl_sock(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_hso_sockctrl_t ctrl){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    uint8_t sncr=0;

    while(1){
        ESP_GOTO_ON_ERROR(CH394_GetSn_CTRL(hso_ch394->io,id,&sncr),err,TAG,"cannot read Sn_CTRL");
        if(sncr!=0)
            continue;
        else
            break;
    }

    switch (ctrl)
    {
        case HWSS_HSO_SOCKCTRL_OPEN: sncr|=CH394_Sn_CTRL_OPEN; break;
        case HWSS_HSO_SOCKCTRL_LISTEN: sncr|=CH394_Sn_CTRL_LISTEN; break;
        case HWSS_HSO_SOCKCTRL_CONNECT: sncr|=CH394_Sn_CTRL_CONNECT; break;
        case HWSS_HSO_SOCKCTRL_DISCONN: sncr|=CH394_Sn_CTRL_DISCONNECT; break;
        case HWSS_HSO_SOCKCTRL_CLOSE: sncr|=CH394_Sn_CTRL_CLOSE; break;
        case HWSS_HSO_SOCKCTRL_SEND: sncr|=CH394_Sn_CTRL_SEND; break;
        case HWSS_HSO_SOCKCTRL_SENDMAC: sncr|=CH394_Sn_CTRL_SEND_MAC; break;
        case HWSS_HSO_SOCKCTRL_SENDKEEP: sncr|=CH394_Sn_CTRL_SEND_KEEP; break;
        case HWSS_HSO_SOCKCTRL_RECV: sncr|=CH394_Sn_CTRL_RECV; break;
        default:
            ret=ESP_ERR_NOT_SUPPORTED;
            goto err;
    }

    ESP_GOTO_ON_ERROR(CH394_SetSn_CTRL(hso_ch394->io,id,&sncr),err,TAG,"cannot write Sn_CTRL");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_write_tx_buffer(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *data, uint16_t len){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    if(len==0)
        return ret;

    uint16_t txwr=0;
    ESP_GOTO_ON_ERROR(CH394_GetSn_TX_WR(hso_ch394->io,id,&txwr),err,TAG,"cannot read Sn_TX_WR");

    ESP_GOTO_ON_ERROR(CH394_writeSn_TXBUF(hso_ch394->io,id,txwr,data,len),err,TAG,"cannot write Sn_TXBUF");
    txwr+=len;
    ESP_GOTO_ON_ERROR(CH394_SetSn_TX_WR(hso_ch394->io,id,&txwr),err,TAG,"cannot update Sn_TX_WR");

err:
    return ret;
}

static esp_err_t hwss_hso_ch394_read_rx_buffer(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *data, uint16_t len){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    if(len==0)
        return ret;
    
    uint16_t rxrd=0;
    ESP_GOTO_ON_ERROR(CH394_GetSn_RX_RD(hso_ch394->io,id,&rxrd),err,TAG,"cannot read Sn_RX_RD");

    ESP_GOTO_ON_ERROR(CH394_readSn_RXBUF(hso_ch394->io,id,rxrd,data,len),err,TAG,"cannot read Sn_RXBUF");
    rxrd+=len;
    ESP_GOTO_ON_ERROR(CH394_SetSn_RX_RD(hso_ch394->io,id,&rxrd),err,TAG,"cannot update Sn_RX_RD");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_read_rx_buffer_with_header(hwss_hso_t *hso, hwss_eth_sockid_t id, 
                                    hwss_eth_pack_header_t htype, void *header, uint8_t *data, uint16_t *len){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    if(*len==0)
        return ret;

    uint8_t     hlen=0;
    uint16_t    dlen=0;
    switch(htype){
        case HWSS_ETH_PACK_HEADER_UDP: hlen=CH394_UDP_HEADER_LEN;break;
        case HWSS_ETH_PACK_HEADER_MACRAW: hlen=CH394_MACRAW_HEADER_LEN;break;
        default: return ESP_ERR_INVALID_ARG;
    }

    ESP_GOTO_ON_FALSE(*len>=hlen,ESP_ERR_INVALID_SIZE,err,TAG,"corrupted pack detected");

    uint8_t     cache[CH394_MACRAW_HEADER_LEN];
    uint16_t    rxrd=0;
    ESP_GOTO_ON_ERROR(CH394_GetSn_RX_RD(hso_ch394->io,id,&rxrd),err,TAG,"cannot read Sn_RX_RD");

    ESP_GOTO_ON_ERROR(CH394_readSn_RXBUF_with_cache(hso_ch394->io,id,rxrd,cache,hlen),err,TAG,"cannot read Sn_RXBUF");
    *len-=hlen;
    rxrd+=hlen;
    switch(htype){
        case HWSS_ETH_PACK_HEADER_UDP:{
            hwss_eth_udp_header_t *udp_header=(hwss_eth_udp_header_t *)header;
            memcpy(udp_header->addr,cache+CH394_UDP_HEADER_ADDR_BIAS,HWSS_ETH_IP4_ADDR_LEN);
            memcpy(&udp_header->port,cache+CH394_UDP_HEADER_PORT_BIAS,sizeof(hwss_eth_port_t));
            memcpy(&udp_header->len,cache+CH394_UDP_HEADER_LEN_BIAS,sizeof(uint16_t));
            udp_header->port=hwss_eth_ntohs(udp_header->port);
            udp_header->len=hwss_eth_ntohs(udp_header->len);
            dlen=udp_header->len;
            break;
        }
        case HWSS_ETH_PACK_HEADER_MACRAW: {
            hwss_eth_macraw_header_t *macraw_header=(hwss_eth_macraw_header_t *)header;
            memcpy(&macraw_header->len,cache+CH394_MACRAW_HEADER_LEN_BIAS,sizeof(uint16_t));
            memcpy(macraw_header->d_addr,cache+CH394_MACRAW_HEADER_DADDR_BIAS,HWSS_ETH_MAC_ADDR_LEN);
            memcpy(macraw_header->s_addr,cache+CH394_MACRAW_HEADER_SADDR_BIAS,HWSS_ETH_MAC_ADDR_LEN);
            memcpy(&macraw_header->type,cache+CH394_MACRAW_HEADER_TYPE_BIAS,sizeof(uint16_t));
            macraw_header->len=hwss_eth_ntohs(macraw_header->len);
            macraw_header->type=hwss_eth_ntohs(macraw_header->type);
            dlen=macraw_header->len;
            break;
        }
        default: return ESP_ERR_INVALID_ARG;
    }
    
    ESP_GOTO_ON_FALSE(*len>=dlen,ESP_ERR_INVALID_SIZE,err,TAG,"corrupted pack detected");
    ESP_GOTO_ON_ERROR(CH394_readSn_RXBUF(hso_ch394->io,id,rxrd,data,dlen),err,TAG,"cannot read Sn_RXBUF");
    *len-=dlen;
    rxrd+=dlen;
    ESP_GOTO_ON_ERROR(CH394_setSn_RX_RD(hso_ch394->io,id,&rxrd),err,TAG,"cannot update Sn_RX_RD");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_drop_rx_buffer(hwss_hso_t *hso, hwss_eth_sockid_t id){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    
    uint16_t rxp=0;
    ESP_GOTO_ON_ERROR(CH394_getSn_RX_WR(hso_ch394->io,id,&rxp),err,TAG,"cannot read Sn_RX_WR");
    ESP_GOTO_ON_ERROR(CH394_setSn_RX_RD(hso_ch394->io,id,&rxp),err,TAG,"cannot update Sn_RX_RD");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_tx_free_length(hwss_hso_t *hso, hwss_eth_sockid_t id, uint16_t *len){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_TX_FSR(hso_ch394->io,id,len),err,TAG,"cannot read Sn_TX_FSR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_rx_length(hwss_hso_t *hso, hwss_eth_sockid_t id, uint16_t *len){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_RX_RSR(hso_ch394->io,id,len),err,TAG,"cannot read Sn_RX_RSR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_set_sock_proto(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    sn_mr_reg_t snmr;

    ESP_GOTO_ON_ERROR(CH394_getSn_MR(hso_ch394->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");
    
    switch (*proto)
    {
        case HWSS_PROTO_CLOSE: snmr.protocol=CH394_Sn_MR_CLOSE; break;
        case HWSS_PROTO_MACRAW: snmr.protocol=CH394_Sn_MR_MACRAW; break;
        case HWSS_PROTO_TCP: snmr.protocol=CH394_Sn_MR_TCP; break;
        case HWSS_PROTO_UDP: snmr.protocol=CH394_Sn_MR_UDP; break;
        default: return ESP_ERR_NOT_SUPPORTED;
    }

    ESP_GOTO_ON_ERROR(CH394_setSn_MR(hso_ch394->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_proto(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_proto_t *proto){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    sn_mr_reg_t snmr;

    ESP_GOTO_ON_ERROR(CH394_getSn_MR(hso_ch394->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    switch (snmr.protocol)
    {
        case CH394_Sn_MR_CLOSE: *proto=HWSS_PROTO_CLOSE; break;
        case CH394_Sn_MR_MACRAW: *proto=HWSS_PROTO_MACRAW; break;
        case CH394_Sn_MR_TCP: *proto=HWSS_PROTO_TCP; break;
        case CH394_Sn_MR_UDP: *proto=HWSS_PROTO_UDP; break;
        default: *proto=HWSS_PROTO_UNKNOW; break;
    }

err:
    return ret;
}

static esp_err_t hwss_hso_ch394_set_sockmode_opt(hwss_hso_t *hso, hwss_eth_sockid_t id, const void *opt){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    sn_mr_reg_t snmr;
    
    ESP_GOTO_ON_ERROR(CH394_getSn_MR(hso_ch394->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    hwss_hso_wiznet_sockmode_opt_t *wiznet_opt=(hwss_hso_wiznet_sockmode_opt_t *)opt;

    if(wiznet_opt->udp_multicast)
        snmr.multi_mfen=0x01;
    else
        snmr.multi_mfen=0x00;

    if(wiznet_opt->udp_broadcast_block)
        snmr.bcastb=0x01;
    else
        snmr.bcastb=0x00;

    if(wiznet_opt->nodelay_ack)
        snmr.nd_mc_mmb=0x01;
    else
        snmr.nd_mc_mmb=0x00;

    if(wiznet_opt->udp_unicast_block)
        snmr.unicastb_mip6b=0x01;
    else
        snmr.unicastb_mip6b=0x00;

    ESP_GOTO_ON_ERROR(CH394_setSn_MR(hso_ch394->io,id,&(snmr.val)),err,TAG,"cannot write Sn_MR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sockmode_opt(hwss_hso_t *hso, hwss_eth_sockid_t id, void *opt){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    sn_mr_reg_t snmr;

    ESP_GOTO_ON_ERROR(CH394_getSn_MR(hso_ch394->io,id,&(snmr.val)),err,TAG,"cannot read Sn_MR");

    hwss_hso_wiznet_sockmode_opt_t *wiznet_opt=(hwss_hso_wiznet_sockmode_opt_t *)opt;
    memset(wiznet_opt,0,sizeof(hwss_hso_wiznet_sockmode_opt_t));

    if(snmr.multi_mfen){
        wiznet_opt->udp_multicast=true;
        wiznet_opt->macraw_mac_filter=true;
    }

    if(snmr.bcastb){
        wiznet_opt->macraw_broadcast_block=true;
        wiznet_opt->udp_broadcast_block=true;
    }

    if(snmr.nd_mc_mmb){
        wiznet_opt->nodelay_ack=true;
        wiznet_opt->macraw_multicast_block=true;
    }
    else
        wiznet_opt->igmp_v2=true;

    if(snmr.unicastb_mip6b){
        wiznet_opt->udp_unicast_block=true;
        wiznet_opt->macraw_ipv6_block=true;
    }

err:
    return ret;    
}

static esp_err_t hwss_hso_ch394_set_sock_source_port(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_port_t *port){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_setSn_PORT(hso_ch394->io,id,port),err,TAG,"cannot write Sn_PORT");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_source_port(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_port_t *port){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_PORT(hso_ch394->io,id,port),err,TAG,"cannot read Sn_PORT");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_set_sock_dest_port(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_port_t *port){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_setSn_DPORT(hso_ch394->io,id,port),err,TAG,"cannot write Sn_DPORT");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_dest_port(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_port_t *port){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_DPORT(hso_ch394->io,id,port),err,TAG,"cannot read Sn_DPORT");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_set_sock_dest_mac_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, const hwss_eth_mac_addr_t addr){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_setSn_DHAR(hso_ch394->io,id,addr),err,TAG,"cannot write Sn_DHAR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_dest_mac_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_eth_mac_addr_t addr){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_DHAR(hso_ch394->io,id,addr),err,TAG,"cannot read Sn_DHAR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_set_sock_dest_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *addr){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_setSn_DIPR(hso_ch394->io,id,addr),err,TAG,"cannot write Sn_DIPR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_dest_addr(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *addr){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_DIPR(hso_ch394->io,id,addr),err,TAG,"cannot read Sn_DIPR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_set_sock_keepalive_tick(hwss_hso_t *hso, hwss_eth_sockid_t id, const uint8_t *tick){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_setSn_KPALVTR(hso_ch394->io,id,tick),err,TAG,"cannot write Sn_KPALVTR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_keepalive_tick(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *tick){
    esp_err_t ret= ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_KPALVTR(hso_ch394->io,id,tick),err,TAG,"cannot read Sn_KPALVTR");
err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_state(hwss_hso_t *hso, hwss_eth_sockid_t id, hwss_hso_socksta_t *sta){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);
    uint8_t snsr;

    ESP_GOTO_ON_ERROR(CH394_getSn_SR(hso_ch394->io,id,&snsr),err,TAG,"cannot read Sn_SR");
    
    switch(snsr){
        case CH394_SOCK_CLOSED: *sta=HWSS_HSO_SOCK_CLOSED; break;
        case CH394_SOCK_INIT: *sta=HWSS_HSO_SOCK_TCP_INIT; break;
        case CH394_SOCK_LISTEN: *sta=HWSS_HSO_SOCK_TCP_LISTEN; break;
        case CH394_SOCK_ESTABLISHED: *sta=HWSS_HSO_SOCK_TCP_ESTAB; break;
        case CH394_SOCK_CLOSE_WAIT: *sta=HWSS_HSO_SOCK_TCP_CLOSEW; break;
        case CH394_SOCK_UDP: 
        case CH394_SOCK_MACRAW: *sta=HWSS_HSO_SOCK_OPENED;break;
        default :*sta=HWSS_HSO_SOCK_CUSTOM_STATE;break;
    }

err:
    return ret;
}

static esp_err_t hwss_hso_ch394_get_sock_state_raw(hwss_hso_t *hso, hwss_eth_sockid_t id, uint8_t *sta){
    esp_err_t ret=ESP_OK;
    hwss_hso_ch394_t *hso_ch394=__containerof(hso,hwss_hso_ch394_t,super);

    ESP_GOTO_ON_ERROR(CH394_getSn_SR(hso_ch394->io,id,sta),err,TAG,"cannot read Sn_SR");
err:
    return ret;
}

hwss_hso_t *hwss_hso_new_ch394(hwss_io_t *io, hwss_hir_t* hir, const hwss_hso_config_t *config){
    hwss_hso_t *ret=NULL;
    hwss_hso_ch394_t* hso=NULL;

    ESP_GOTO_ON_FALSE(io,NULL,err,TAG,"cannot set io to null");
    ESP_GOTO_ON_FALSE(config,NULL,err,TAG,"cannot set config to null");
    ESP_GOTO_ON_FALSE(config->en_socknum<=CH394_SOCKNUM,NULL,err,TAG,"max socket of CH394 is 8");

    uint16_t tx_cache_size_kb=0, rx_cache_size_kb=0;
    for(hwss_eth_sockid_t id=0;id<CH394_SOCKNUM;id++){
        tx_cache_size_kb+=config->tx_buffsize_kb[id];
        rx_cache_size_kb+=config->rx_buffsize_kb[id];
    }

    ESP_GOTO_ON_FALSE(tx_cache_size_kb<=CH394_SOCK_CACHE_SIZE_KB && rx_cache_size_kb<=CH394_SOCK_CACHE_SIZE_KB,
                        NULL,err,TAG,"sum of tx/rx cache size should not be over 16KB");

    if(hir==NULL && config->sock_polling_period_ms<5)
        ESP_LOGW(TAG,"polling period too short might SERIOUSLY affects the execution of other tasks!");

    hso=calloc(1,sizeof(hwss_hso_ch394_t));
    ESP_GOTO_ON_FALSE(hso,NULL,err,TAG,"calloc hso failed");

    hso->super.init=hwss_hso_ch394_init;
    hso->super.deinit=hwss_hso_ch394_deinit;
    hso->super.ctrl_sock=hwss_hso_ch394_ctrl_sock;
    hso->super.write_tx_buffer=hwss_hso_ch394_write_tx_buffer;
    hso->super.read_rx_buffer=hwss_hso_ch394_read_rx_buffer;
    hso->super.read_rx_buffer_with_header=hwss_hso_ch394_read_rx_buffer_with_header;
    hso->super.drop_rx_buffer=hwss_hso_ch394_drop_rx_buffer;
    hso->super.get_rx_length=hwss_hso_ch394_get_rx_length;
    hso->super.get_tx_free_length=hwss_hso_ch394_get_tx_free_length;
    hso->super.set_sock_proto=hwss_hso_ch394_set_sock_proto;
    hso->super.get_sock_proto=hwss_hso_ch394_get_sock_proto;
    hso->super.set_sockmode_opt=hwss_hso_ch394_set_sockmode_opt;
    hso->super.get_sockmode_opt=hwss_hso_ch394_get_sockmode_opt;
    hso->super.set_sock_source_port=hwss_hso_ch394_set_sock_source_port;
    hso->super.get_sock_source_port=hwss_hso_ch394_get_sock_source_port;
    hso->super.set_sock_dest_port=hwss_hso_ch394_set_sock_dest_port;
    hso->super.get_sock_dest_port=hwss_hso_ch394_get_sock_dest_port;
    hso->super.set_sock_dest_mac_addr=hwss_hso_ch394_set_sock_dest_mac_addr;
    hso->super.get_sock_dest_mac_addr=hwss_hso_ch394_get_sock_dest_mac_addr;
    hso->super.set_sock_dest_addr=hwss_hso_ch394_set_sock_dest_addr;
    hso->super.get_sock_dest_addr=hwss_hso_ch394_get_sock_dest_addr;
    hso->super.set_sock_keepalive_tick=hwss_hso_ch394_set_sock_keepalive_tick;
    hso->super.get_sock_keepalive_tick=hwss_hso_ch394_get_sock_keepalive_tick;
    hso->super.get_sock_state=hwss_hso_ch394_get_sock_state;
    hso->super.get_sock_state_raw=hwss_hso_ch394_get_sock_state_raw;

    hso->io=io;
    hso->en_socknum=config->en_socknum;
    memcpy(hso->txbuf_size_kb,config->tx_buffsize_kb,CH394_SOCKNUM);
    memcpy(hso->rxbuf_size_kb,config->rx_buffsize_kb,CH394_SOCKNUM);

    return &hso->super;

err:
    if(hso){
        free(hso);
    }
    return ret;
}
