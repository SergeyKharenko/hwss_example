#pragma once

#include <string.h>
#include "hwss_eth_type.h"
#include "hwss_io.h"
#include "ch394.h"

///////////////////////////////////////////////////////
/////////////////// Global Registers //////////////////
///////////////////////////////////////////////////////

static inline esp_err_t CH394_SetMODE(hwss_io_t *io, const uint8_t *mode){
    return io->write(io,0,CH394_REG_MODE,mode);
}

static inline esp_err_t CH394_SetGWIP(hwss_io_t *io, const uint8_t *gwip){
    return io->write_buf(io,0,CH394_REG_GWIP0,gwip,4);
}

static inline esp_err_t CH394_SetSMIP(hwss_io_t *io, const uint8_t *smip){
    return io->write_buf(io,0,CH394_REG_SMIP0,smip,4);
}

static inline esp_err_t CH394_SetMAC(hwss_io_t *io, const uint8_t *mac){
    return io->write_buf(io,0,CH394_REG_MAC0,mac,6);
}

static inline esp_err_t CH394_SetIP(hwss_io_t *io, const uint8_t *ip){
    return io->write_buf(io,0,CH394_REG_IP0,ip,4);
}

static inline esp_err_t CH394_SetIIT(hwss_io_t *io, const uint16_t *iit){
    uint16_t nv=hwss_eth_htons(*iit);
    return io->write_buf(io,0,CH394_REG_IIT0,(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetGINT(hwss_io_t *io, const uint8_t *gint){
    return io->write(io,0,CH394_REG_GINT,gint);
}

static inline esp_err_t CH394_SetGINTE(hwss_io_t *io, const uint8_t *ginte){
    return io->write(io,0,CH394_REG_GINTE,ginte);
}

static inline esp_err_t CH394_SetSINTE(hwss_io_t *io, const uint8_t *sinte){
    return io->write(io,0,CH394_REG_SINTE,sinte);
}

static inline esp_err_t CH394_SetRTIME(hwss_io_t *io, const uint16_t *rtime){
    uint16_t nv=hwss_eth_htons(*rtime);
    return io->write_buf(io,0,CH394_REG_RTIME0,(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetRCNT(hwss_io_t *io, const uint8_t *rcnt){
    return io->write(io,0,CH394_REG_RCNT,rcnt);
}

static inline esp_err_t CH394_SetPHY_CFG(hwss_io_t *io, const uint8_t *phycfg){
    return io->write(io,0,CH394_REG_PHY_CFG,phycfg);
}

static inline esp_err_t CH394_GetMODE(hwss_io_t *io, uint8_t *mode){
    return io->read(io,0,CH394_REG_MODE,mode);
}

static inline esp_err_t CH394_GetGWIP(hwss_io_t *io, uint8_t *gwip){
    return io->read_buf(io,0,CH394_REG_GWIP0,gwip,4);
}

static inline esp_err_t CH394_GetSMIP(hwss_io_t *io, uint8_t *smip){
    return io->read_buf(io,0,CH394_REG_SMIP0,smip,4);
}

static inline esp_err_t CH394_GetMAC(hwss_io_t *io, uint8_t *mac){
    return io->read_buf(io,0,CH394_REG_MAC0,mac,6);
}

static inline esp_err_t CH394_GetIP(hwss_io_t *io, uint8_t *ip){
    return io->read_buf(io,0,CH394_REG_IP0,ip,4);
}

static inline esp_err_t CH394_GetIIT(hwss_io_t *io, uint16_t *iit){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_IIT0,(uint8_t *)iit,2);
    *iit=hwss_eth_ntohs(*iit);
    return ret;
}

static inline esp_err_t CH394_GetGINT(hwss_io_t *io, uint8_t *gint){
    return io->read(io,0,CH394_REG_GINT,gint);
}

static inline esp_err_t CH394_GetGINTE(hwss_io_t *io, uint8_t *ginte){
    return io->read(io,0,CH394_REG_GINTE,ginte);
}

static inline esp_err_t CH394_GetSINT(hwss_io_t *io, uint8_t *sint){
    return io->read(io,0,CH394_REG_SINT,sint);
}

static inline esp_err_t CH394_GetSINTE(hwss_io_t *io, uint8_t *sinte){
    return io->read(io,0,CH394_REG_SINTE,sinte);
}

static inline esp_err_t CH394_GetRTIME(hwss_io_t *io, uint16_t *rtime){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_RTIME0,(uint8_t *)rtime,2);
    *rtime=hwss_eth_ntohs(*rtime);
    return ret;
}

static inline esp_err_t CH394_GetRCNT(hwss_io_t *io, uint8_t *rcnt){
    return io->read(io,0,CH394_REG_RCNT,rcnt);
}

static inline esp_err_t CH394_GetUNIP(hwss_io_t *io, uint8_t *unip){
    return io->read_buf(io,0,CH394_REG_UNIP0,unip,4);
}

static inline esp_err_t CH394_GetUNPORT(hwss_io_t *io, uint16_t *unport){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_UNPORT0,(uint8_t *)unport,2);
    *unport=hwss_eth_ntohs(*unport);
    return ret;
}

static inline esp_err_t CH394_GetPHY_CFG(hwss_io_t *io, uint8_t *phycfg){
    return io->read(io,0,CH394_REG_PHY_CFG,phycfg);
}

static inline esp_err_t CH394_GetCHIPV(hwss_io_t *io, uint8_t *chipv){
    return io->read(io,0,CH394_REG_CHIPV,chipv);
}

///////////////////////////////////////////////////////
/////////////////// Socket Registers //////////////////
///////////////////////////////////////////////////////

static inline esp_err_t CH394_SetSn_MODE(hwss_io_t *io, uint8_t sn, const uint8_t *mode){
    return io->write(io,0,CH394_REG_Sn_MODE(sn),mode);
}

static inline esp_err_t CH394_SetSn_CTRL(hwss_io_t *io, uint8_t sn, const uint8_t *ctrl){
    return io->write(io,0,CH394_REG_Sn_CTRL(sn),ctrl);
}

static inline esp_err_t CH394_SetSn_INT(hwss_io_t *io, uint8_t sn, const uint8_t *intr){
    return io->write(io,0,CH394_REG_Sn_INT(sn),intr);
}

static inline esp_err_t CH394_SetSn_PORT(hwss_io_t *io, uint8_t sn, const uint16_t *port){
    uint16_t nv=hwss_eth_htons(*port);
    return io->write_buf(io,0,CH394_REG_Sn_PORT0(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetSn_DMAC(hwss_io_t *io, uint8_t sn, const uint8_t *dmac){
    return io->write_buf(io,0,CH394_REG_Sn_DMAC0(sn),dmac,6);
}

static inline esp_err_t CH394_SetSn_DIP(hwss_io_t *io, uint8_t sn, const uint8_t *dip){
    return io->write_buf(io,0,CH394_REG_Sn_DIP0(sn),dip,4);
}

static inline esp_err_t CH394_SetSn_DPORT(hwss_io_t *io, uint8_t sn, const uint16_t *dport){
    uint16_t nv=hwss_eth_htons(*dport);
    return io->write_buf(io,0,CH394_REG_Sn_DPORT0(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t CH394Q_SetSn_MTU(hwss_io_t *io, uint8_t sn, const uint16_t *mtu){
    uint16_t nv=hwss_eth_htons(*mtu);
    return io->write_buf(io,0,CH394_REG_Sn_MTU0(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetSn_TOS(hwss_io_t *io, uint8_t sn, const uint8_t *tos){
    return io->write(io,0,CH394_REG_Sn_TOS(sn),tos);
}

static inline esp_err_t CH394Q_SetSn_TTL(hwss_io_t *io, uint8_t sn, const uint8_t *ttl){
    return io->write(io,0,CH394_REG_Sn_TTL(sn),ttl);
}

static inline esp_err_t CH394_SetSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *rxsize){
    return io->write(io,0,CH394_REG_Sn_RXBUF_SIZE(sn),rxsize);
}

static inline esp_err_t CH394_SetSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *txsize){
    return io->write(io,0,CH394_REG_Sn_TXBUF_SIZE(sn),txsize);
}

static inline esp_err_t CH394_SetSn_TX_WR(hwss_io_t *io, uint8_t sn, const uint16_t *txwr){
    uint16_t nv=hwss_eth_htons(*txwr);
    return io->write_buf(io,0,CH394_REG_Sn_TX_WR0(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetSn_RX_RD(hwss_io_t *io, uint8_t sn, const uint16_t *rxrd){
    uint16_t nv=hwss_eth_htons(*rxrd);
    return io->write_buf(io,0,CH394_REG_Sn_RX_RD0(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetSn_INTE(hwss_io_t *io, uint8_t sn, const uint8_t *inte){
    return io->write(io,0,CH394_REG_Sn_INTE(sn),inte);
}

static inline esp_err_t CH394_SetSn_IPF(hwss_io_t *io, uint8_t sn, const uint16_t *ipf){
    uint16_t nv=hwss_eth_htons(*ipf);
    return io->write_buf(io,0,CH394_REG_Sn_IPF0(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t CH394_SetSn_KEEPALIVE(hwss_io_t *io, uint8_t sn, const uint8_t *kl){
    return io->write(io,0,CH394_REG_Sn_KEEPALIVE(sn),kl);
}

static inline esp_err_t CH394_GetSn_MODE(hwss_io_t *io, uint8_t sn, uint8_t *mode){
    return io->read(io,0,CH394_REG_Sn_MODE(sn),mode);
}

static inline esp_err_t CH394_GetSn_CTRL(hwss_io_t *io, uint8_t sn, uint8_t *ctrl){
    return io->read(io,0,CH394_REG_Sn_CTRL(sn),ctrl);
}

static inline esp_err_t CH394_GetSn_INT(hwss_io_t *io, uint8_t sn, uint8_t *intr){
    return io->read(io,0,CH394_REG_Sn_INT(sn),intr);
}

static inline esp_err_t CH394_GetSn_STA(hwss_io_t *io, uint8_t sn, uint8_t *sta){
    return io->read(io,0,CH394_REG_Sn_STA(sn),sta);
}

static inline esp_err_t CH394_GetSn_PORT(hwss_io_t *io, uint8_t sn, uint16_t *port){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_PORT0(sn),(uint8_t *)port,2);
    *port=hwss_eth_ntohs(*port);
    return ret;
}

static inline esp_err_t CH394_GetSn_DMAC(hwss_io_t *io, uint8_t sn, uint8_t *dmac){
    return io->read_buf(io,0,CH394_REG_Sn_DMAC0(sn),dmac,6);
}

static inline esp_err_t CH394_GetSn_DIP(hwss_io_t *io, uint8_t sn, uint8_t * dip){
    return io->read_buf(io,0,CH394_REG_Sn_DIP0(sn),dip,4);
}

static inline esp_err_t CH394_GetSn_DPORT(hwss_io_t *io, uint8_t sn, uint16_t *dport){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_DPORT0(sn),(uint8_t *)dport,2);
    *dport=hwss_eth_ntohs(*dport);
    return ret;
}

static inline esp_err_t CH394_GetSn_MTU(hwss_io_t *io, uint8_t sn, uint16_t *mtu){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_MTU0(sn),(uint8_t *)mtu,2);
    *mtu=hwss_eth_ntohs(*mtu);
    return ret;
}

static inline esp_err_t CH394_GetSn_TOS(hwss_io_t *io, uint8_t sn, uint8_t *tos){
    return io->read(io,0,CH394_REG_Sn_TOS(sn),tos);
}

static inline esp_err_t CH394_GetSn_TTL(hwss_io_t *io, uint8_t sn, uint8_t *ttl){
    return io->read(io,0,CH394_REG_Sn_TTL(sn),ttl);
}

static inline esp_err_t CH394_GetSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *rx_size){
    return io->read(io,0,CH394_REG_Sn_RXBUF_SIZE(sn),rx_size);
}

static inline esp_err_t CH394_GetSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *tx_size){
    return io->read(io,0,CH394_REG_Sn_TXBUF_SIZE(sn),tx_size);
}

static inline esp_err_t CH394_GetSn_TX_FS(hwss_io_t *io, uint8_t sn, uint16_t *txfs){
    uint16_t txfs_c=0;
    uint16_t txfs_p=0;

    do{
        if(io->read_buf(io,0,CH394_REG_Sn_TX_FS0(sn),(uint8_t *)&txfs_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(txfs_p!=0){
            if(io->read_buf(io,0,CH394_REG_Sn_TX_FS0(sn),(uint8_t *)&txfs_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(txfs_c!=txfs_p);

    *txfs=hwss_eth_ntohs(txfs_c);
    return ESP_OK;
}

static inline esp_err_t CH394_GetSn_RX_RS(hwss_io_t *io, uint8_t sn, uint16_t *rxrs){
    uint16_t rxrs_c=0;
    uint16_t rxrs_p=0;

    do{
        if(io->read_buf(io,0,CH394_REG_Sn_RX_RS0(sn),(uint8_t *)&rxrs_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(rxrs_p!=0){
            if(io->read_buf(io,0,CH394_REG_Sn_RX_RS0(sn),(uint8_t *)&rxrs_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(rxrs_c!=rxrs_p);

    *rxrs=hwss_eth_ntohs(rxrs_c);
    return ESP_OK;
}

static inline esp_err_t CH394_GetSn_TX_RD(hwss_io_t *io, uint8_t sn, uint16_t *txrd){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_TX_RD0(sn),(uint8_t *)txrd,2);
    *txrd=hwss_eth_ntohs(*txrd);
    return ret;
}

static inline esp_err_t CH394_GetSn_TX_WR(hwss_io_t *io, uint8_t sn, uint16_t *txwr){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_TX_WR0(sn),(uint8_t *)txwr,2);
    *txwr=hwss_eth_ntohs(*txwr);
    return ret;
}

static inline esp_err_t CH394_GetSn_RX_RD(hwss_io_t *io, uint8_t sn, uint16_t *rxrd){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_RX_RD0(sn),(uint8_t *)rxrd,2);
    *rxrd=hwss_eth_ntohs(*rxrd);
    return ret;
}

static inline esp_err_t CH394_GetSn_RX_WR(hwss_io_t *io, uint8_t sn, uint16_t *rxwr){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_RX_WR0(sn),(uint8_t *)rxwr,2);
    *rxwr=hwss_eth_ntohs(*rxwr);
    return ret;
}

static inline esp_err_t CH394_GetSn_INTE(hwss_io_t *io, uint8_t sn, uint8_t *inte){
    return io->read(io,0,CH394_REG_Sn_INTE(sn),inte);
}

static inline esp_err_t CH394_GetSn_IPF(hwss_io_t *io, uint8_t sn, uint16_t *ipf){
    esp_err_t ret=io->read_buf(io,0,CH394_REG_Sn_IPF0(sn),(uint8_t *)ipf,2);
    *ipf=hwss_eth_ntohs(*ipf);
    return ret;
}

static inline esp_err_t CH394_GetSn_KEEPALIVE(hwss_io_t *io, uint8_t sn, uint8_t *kl){
    return io->read(io,0,CH394_REG_Sn_KEEPALIVE(sn),kl);
}


///////////////////////////////////
// Socket Buffer I/O function //
///////////////////////////////////

static inline esp_err_t CH394_writeSn_TXBUF(hwss_io_t *io, uint8_t sn, uint16_t addr, const uint8_t *data, uint16_t len){
    return io->write_mem(io,0,(uint32_t)(CH394_TXBUF_BLOCK(sn)+(addr<<8)),data,len);
}

static inline esp_err_t CH394_readSn_RXBUF(hwss_io_t *io, uint8_t sn, uint16_t addr, uint8_t *data, uint16_t len){
    return io->read_mem(io,0,(uint32_t)(CH394_RXBUF_BLOCK(sn)+(addr<<8)),data,len);
}
