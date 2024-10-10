#pragma once
#include <string.h>
#include "hwss_eth_type.h"
#include "hwss_io.h"
#include "w6100.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////
// Common Register I/O function //
/////////////////////////////////

static inline esp_err_t W6100_getCIDR(hwss_io_t *io, uint16_t *cidr){
    esp_err_t ret=io->read_buf(io,0,W6100_REG_CIDR,(uint8_t *)cidr,2);
    *cidr=hwss_eth_ntohs(*cidr);
    return ret;
}

static inline esp_err_t W6100_getVER(hwss_io_t *io, uint16_t *ver){
    esp_err_t ret=io->read_buf(io,0,W6100_REG_VER,(uint8_t *)ver,2);
    *ver=hwss_eth_ntohs(*ver);
    return ret;
}

static inline esp_err_t W6100_setNET4MR(hwss_io_t *io, const uint8_t *mr){
    return io->write(io,0,W6100_REG_NET4MR,mr);
}

static inline esp_err_t W6100_getNET4MR(hwss_io_t *io, uint8_t *mr){
    return io->read(io,0,W6100_REG_NET4MR,mr);
}

static inline esp_err_t W6100_setNET6MR(hwss_io_t *io, const uint8_t *mr){
    return io->write(io,0,W6100_REG_NET6MR,mr);
}

static inline esp_err_t W6100_getNET6MR(hwss_io_t *io, uint8_t *mr){
    return io->read(io,0,W6100_REG_NET6MR,mr);
}

static inline esp_err_t W6100_setNETMR(hwss_io_t *io, const uint8_t *mr){
    return io->write(io,0,W6100_REG_NETMR,mr);
}

static inline esp_err_t W6100_getNETMR(hwss_io_t *io, uint8_t *mr){
    return io->read(io,0,W6100_REG_NETMR,mr);
}

static inline esp_err_t W6100_setNETMR2(hwss_io_t *io, const uint8_t *mr2){
    return io->write(io,0,W6100_REG_NETMR2,mr2);
}

static inline esp_err_t W6100_getNETMR2(hwss_io_t *io, uint8_t *mr2){
    return io->read(io,0,W6100_REG_NETMR2,mr2);
}

static inline esp_err_t W6100_setGAR(hwss_io_t *io, const uint8_t *gar){
    return io->write_buf(io,0,W6100_REG_GAR,gar,4);
}

static inline esp_err_t W6100_getGAR(hwss_io_t *io, uint8_t *gar){
    return io->read_buf(io,0,W6100_REG_GAR,gar,4);
}

static inline esp_err_t W6100_setSUBR(hwss_io_t *io, const uint8_t *subr){
    return io->write_buf(io,0,W6100_REG_SUBR,subr,4);
}

static inline esp_err_t W6100_getSUBR(hwss_io_t *io, uint8_t *subr){
    return io->read_buf(io,0,W6100_REG_SUBR,subr,4);
}

static inline esp_err_t W6100_setSHAR(hwss_io_t *io, const uint8_t *shar){
    return io->write_buf(io,0,W6100_REG_SHAR,shar,6);
}

static inline esp_err_t W6100_getSHAR(hwss_io_t *io, uint8_t *shar){
    return io->read_buf(io,0,W6100_REG_SHAR,shar,6);
}

static inline esp_err_t W6100_setSIPR(hwss_io_t *io, const uint8_t *sipr){
    return io->write_buf(io,0,W6100_REG_SIPR,sipr,4);
}

static inline esp_err_t W6100_getSIPR(hwss_io_t *io, uint8_t *sipr){
    return io->read_buf(io,0,W6100_REG_SIPR,sipr,4);
}

static inline esp_err_t W6100_setINTPTMR(hwss_io_t *io, const uint16_t *intptmr){
    uint16_t nv=hwss_eth_htons(*intptmr);
    return io->write_buf(io,0,W6100_REG_INTPTMR,(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getINTPTMR(hwss_io_t *io, uint16_t *intptmr){
    esp_err_t ret=io->read_buf(io,0,W6100_REG_INTPTMR,(uint8_t *)intptmr,2);
    *intptmr=hwss_eth_ntohs(*intptmr);
    return ret;
}

static inline esp_err_t W6100_setIR(hwss_io_t *io, const uint8_t *ir){
    return io->write(io,0,W6100_REG_IR,ir);
}

static inline esp_err_t W6100_getIR(hwss_io_t *io, uint8_t *ir){
    return io->read(io,0,W6100_REG_IR,ir);
}

static inline esp_err_t W6100_setIMR(hwss_io_t *io, const uint8_t *imr){
    return io->write(io,0,W6100_REG_IMR,imr);
}

static inline esp_err_t W6100_getIMR(hwss_io_t *io, uint8_t *imr){
    return io->read(io,0,W6100_REG_IMR,imr);
}

static inline esp_err_t W6100_setSIR(hwss_io_t *io, const uint8_t *sir){
    return io->write(io,0,W6100_REG_SIR,sir);
}

static inline esp_err_t W6100_getSIR(hwss_io_t *io, uint8_t *sir){
    return io->read(io,0,W6100_REG_SIR,sir);
}

static inline esp_err_t W6100_setSIMR(hwss_io_t *io, const uint8_t *simr){
    return io->write(io,0,W6100_REG_SIMR,simr);
}

static inline esp_err_t W6100_getSIMR(hwss_io_t *io, uint8_t *simr){
    return io->read(io,0,W6100_REG_SIMR,simr);
}

static inline esp_err_t W6100_setRTR(hwss_io_t *io, const uint16_t *rtr){
    uint16_t nv=hwss_eth_htons(*rtr);
    return io->write_buf(io,0,W6100_REG_RTR,(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getRTR(hwss_io_t *io, uint16_t *rtr){
    esp_err_t ret = io->read_buf(io,0,W6100_REG_RTR,(uint8_t *)rtr,2);
    *rtr=hwss_eth_ntohs(*rtr);
    return ret;
}

static inline esp_err_t W6100_setRCR(hwss_io_t *io, const uint8_t *rcr){
    return io->write(io,0,W6100_REG_RCR,rcr);
}

static inline esp_err_t W6100_getRCR(hwss_io_t *io, uint8_t *rcr){
    return io->read(io,0,W6100_REG_RCR,rcr);
}

static inline esp_err_t W6100_setPTMR(hwss_io_t *io, const uint8_t *ptmr){
    return io->write(io,0,W6100_REG_PTMR,ptmr);
}

static inline esp_err_t W6100_getPTMR(hwss_io_t *io, uint8_t *ptmr){
    return io->read(io,0,W6100_REG_PTMR,ptmr);
}

static inline esp_err_t W6100_setPMNR(hwss_io_t *io, const uint8_t *pmnr){
    return io->write(io,0,W6100_REG_PMNR,pmnr);
}

static inline esp_err_t W6100_getPMNR(hwss_io_t *io, uint8_t *pmnr){
    return io->read(io,0,W6100_REG_PMNR,pmnr);
}

static inline esp_err_t W6100_setPHAR(hwss_io_t *io, const uint8_t *phar){
    return io->write_buf(io,0,W6100_REG_PHAR,phar,6);
}

static inline esp_err_t W6100_getPHAR(hwss_io_t *io, uint8_t *phar){
    return io->read_buf(io,0,W6100_REG_PHAR,phar,6);
}

static inline esp_err_t W6100_setPSIDR(hwss_io_t *io, const uint16_t *psidr){
    uint16_t nv=hwss_eth_htons(*psidr);
    return io->write_buf(io,0,W6100_REG_PSIDR,(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getPSIDR(hwss_io_t *io, uint16_t *psidr){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_PSIDR,(uint8_t *)psidr,2);
    *psidr=hwss_eth_ntohs(*psidr);
    return ret;
}

static inline esp_err_t W6100_setPMRUR(hwss_io_t *io, const uint16_t *pmrur){
    uint16_t nv=hwss_eth_htons(*pmrur);
    return io->write_buf(io,0,W6100_REG_PMRUR,(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getPMRUR(hwss_io_t *io, uint16_t *pmrur){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_PMRUR,(uint8_t *)pmrur,2);
    *pmrur=hwss_eth_ntohs(*pmrur);
    return ret;
}

static inline esp_err_t W6100_getUIPR(hwss_io_t *io, uint8_t* uipr){
    return io->read_buf(io,0,W6100_REG_UIPR,uipr,4);
}

static inline esp_err_t W6100_getUPORTR(hwss_io_t *io, uint16_t *uportr){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_UPORTR,(uint8_t *)uportr,2);
    *uportr=hwss_eth_ntohs(*uportr);
    return ret;
}

///////////////////////////////////
// Socket N register I/O function //
///////////////////////////////////

static inline esp_err_t W6100_setSn_MR(hwss_io_t *io, uint8_t sn, const uint8_t *mr){
    return io->write(io,0,W6100_REG_Sn_MR(sn),mr);
}

static inline esp_err_t W6100_getSn_MR(hwss_io_t *io, uint8_t sn, uint8_t *mr){
    return io->read(io,0,W6100_REG_Sn_MR(sn),mr);
}

static inline esp_err_t W6100_setSn_CR(hwss_io_t *io, uint8_t sn, const uint8_t *cr){
    return io->write(io,0,W6100_REG_Sn_CR(sn),cr);
}

static inline esp_err_t W6100_getSn_CR(hwss_io_t *io, uint8_t sn, uint8_t *cr){
    return io->read(io,0,W6100_REG_Sn_CR(sn),cr);
}

static inline esp_err_t W6100_setSn_IR(hwss_io_t *io, uint8_t sn, const uint8_t *ir){
    return io->write(io,0,W6100_REG_Sn_IR(sn),ir);
}

static inline esp_err_t W6100_getSn_IR(hwss_io_t *io, uint8_t sn, uint8_t *ir){
    return io->read(io,0,W6100_REG_Sn_IR(sn),ir);
}

static inline esp_err_t W6100_setSn_IMR(hwss_io_t *io, uint8_t sn, const uint8_t *imr){
    return io->write(io,0,W6100_REG_Sn_IMR(sn),imr);
}

static inline esp_err_t W6100_getSn_IMR(hwss_io_t *io, uint8_t sn, uint8_t *imr){
    return io->read(io,0,W6100_REG_Sn_IMR(sn),imr);
}

static inline esp_err_t W6100_getSn_SR(hwss_io_t *io, uint8_t sn, uint8_t *sr){
    return io->read(io,0,W6100_REG_Sn_SR(sn),sr);
}

static inline esp_err_t W6100_setSn_PORT(hwss_io_t *io, uint8_t sn, const uint16_t *port){
    uint16_t nv=hwss_eth_htons(*port);
    return io->write_buf(io,0,W6100_REG_Sn_PORTR(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getSn_PORT(hwss_io_t *io, uint8_t sn, uint16_t *port){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_PORTR(sn),(uint8_t *)port,2);
    *port=hwss_eth_ntohs(*port);
    return ret;
}

static inline esp_err_t W6100_setSn_DHAR(hwss_io_t *io, uint8_t sn, const uint8_t *dhar){
    return io->write_buf(io,0,W6100_REG_Sn_DHAR(sn),dhar,6);
}

static inline esp_err_t W6100_getSn_DHAR(hwss_io_t *io, uint8_t sn, uint8_t *dhar){
    return io->read_buf(io,0,W6100_REG_Sn_DHAR(sn),dhar,6);
}

static inline esp_err_t W6100_setSn_DIPR(hwss_io_t *io, uint8_t sn, const uint8_t *dipr){
    return io->write_buf(io,0,W6100_REG_Sn_DIPR(sn),dipr,4);
}

static inline esp_err_t W6100_getSn_DIPR(hwss_io_t *io, uint8_t sn, uint8_t *dipr){
    return io->read_buf(io,0,W6100_REG_Sn_DIPR(sn),dipr,4);
}

static inline esp_err_t W6100_setSn_DPORT(hwss_io_t *io, uint8_t sn, const uint16_t *dport){
    uint16_t nv=hwss_eth_htons(*dport);
    return io->write_buf(io,0,W6100_REG_Sn_DPORTR(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getSn_DPORT(hwss_io_t *io, uint8_t sn, uint16_t *dport){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_DPORTR(sn),(uint8_t *)dport,2);
    *dport=hwss_eth_ntohs(*dport);
    return ret;
}

static inline esp_err_t W6100_setSn_MSSR(hwss_io_t *io, uint8_t sn, const uint16_t *mss){
    uint16_t nv=hwss_eth_htons(*mss);
    return io->write_buf(io,0,W6100_REG_Sn_MSSR(sn),(uint8_t *)&nv, 2);
}

static inline esp_err_t W6100_getSn_MSSR(hwss_io_t *io, uint8_t sn, uint16_t *mss){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_MSSR(sn),(uint8_t *)mss, 2);
    *mss=hwss_eth_ntohs(*mss);
    return ret;
}

static inline esp_err_t W6100_setSn_TOS(hwss_io_t *io, uint8_t sn, const uint8_t *tos){
    return io->write(io,0,W6100_REG_Sn_TOSR(sn),tos);
}

static inline esp_err_t W6100_getSn_TOS(hwss_io_t *io, uint8_t sn, uint8_t *tos){
    return io->read(io,0,W6100_REG_Sn_TOSR(sn),tos);
}

static inline esp_err_t W6100_setSn_TTL(hwss_io_t *io, uint8_t sn, const uint8_t *ttl){
    return io->write(io,0,W6100_REG_Sn_TTLR(sn),ttl);
}

static inline esp_err_t W6100_getSn_TTL(hwss_io_t *io, uint8_t sn, uint8_t *ttl){
    return io->read(io,0,W6100_REG_Sn_TTLR(sn),ttl);
}

static inline esp_err_t W6100_setSn_RX_BSR(hwss_io_t *io, uint8_t sn, const uint8_t *rxbufsize){
    return io->write(io,0,W6100_REG_Sn_RX_BSR(sn),rxbufsize);
}

static inline esp_err_t W6100_getSn_RX_BSR(hwss_io_t *io, uint8_t sn, uint8_t *rxbufsize){
    return io->read(io,0,W6100_REG_Sn_RX_BSR(sn),rxbufsize);
}

static inline esp_err_t W6100_setSn_TX_BSR(hwss_io_t *io, uint8_t sn, const uint8_t *txbufsize){
    return io->write(io,0,W6100_REG_Sn_TX_BSR(sn),txbufsize);
}

static inline esp_err_t W6100_getSn_TX_BSR(hwss_io_t *io, uint8_t sn, uint8_t *txbufsize){
    return io->read(io,0,W6100_REG_Sn_TX_BSR(sn),txbufsize);
}

static inline esp_err_t W6100_getSn_TX_FSR(hwss_io_t *io, uint8_t sn, uint16_t *fsr){
    uint16_t fsr_p =0;
    uint16_t fsr_c =0;

    do{
        if(io->read_buf(io,0,W6100_REG_Sn_TX_FSR(sn),(uint8_t *)&fsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(fsr_p!=0){
            if(io->read_buf(io,0,W6100_REG_Sn_TX_FSR(sn),(uint8_t *)&fsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(fsr_p!=fsr_c);

    *fsr=hwss_eth_ntohs(fsr_c);
    return ESP_OK;
}

static inline esp_err_t W6100_getSn_TX_RD(hwss_io_t *io, uint8_t sn, uint16_t *txrd){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_TX_RD(sn),(uint8_t *)txrd,2);
    *txrd=hwss_eth_ntohs(*txrd);
    return ret;
}

static inline esp_err_t W6100_setSn_TX_WR(hwss_io_t *io, uint8_t sn, const uint16_t *txwr){
    uint16_t nv=hwss_eth_htons(*txwr);
    return io->write_buf(io,0,W6100_REG_Sn_TX_WR(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getSn_TX_WR(hwss_io_t *io, uint8_t sn, uint16_t *txwr){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_TX_WR(sn),(uint8_t *)txwr,2);
    *txwr=hwss_eth_ntohs(*txwr);
    return ret;
}

static inline esp_err_t W6100_getSn_RX_RSR(hwss_io_t *io, uint8_t sn, uint16_t *rsr){
    uint16_t rsr_p=0;
    uint16_t rsr_c=0;

    do{
        if(io->read_buf(io,0,W6100_REG_Sn_RX_RSR(sn),(uint8_t *)&rsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(rsr_p!=0){
            if(io->read_buf(io,0,W6100_REG_Sn_RX_RSR(sn),(uint8_t *)&rsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(rsr_p!=rsr_c);

    *rsr=hwss_eth_ntohs(rsr_c);
    return ESP_OK;
}

static inline esp_err_t W6100_setSn_RX_RD(hwss_io_t *io, uint8_t sn, const uint16_t *rxrd){
    uint16_t nv=hwss_eth_htons(*rxrd);
    return io->write_buf(io,0,W6100_REG_Sn_RX_RD(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W6100_getSn_RX_RD(hwss_io_t *io, uint8_t sn, uint16_t *rxrd){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_RX_RD(sn),(uint8_t *)rxrd,2);
    *rxrd=hwss_eth_ntohs(*rxrd);
    return ret;
}

static inline esp_err_t W6100_getSn_RX_WR(hwss_io_t *io, uint8_t sn, uint16_t *rxwr){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_RX_WR(sn),(uint8_t *)rxwr,2);
    *rxwr=hwss_eth_ntohs(*rxwr);
    return ret;
}

static inline esp_err_t W6100_setSn_FRGR(hwss_io_t *io, uint8_t sn, const uint16_t *frgr){
    uint16_t nv=hwss_eth_htons(*frgr);
    return io->write_buf(io,0,W6100_REG_Sn_FRGR(sn),(uint8_t *)&nv, 2);
}

static inline esp_err_t W6100_getSn_FRGR(hwss_io_t *io, uint8_t sn, uint16_t *frgr){
    esp_err_t ret= io->read_buf(io,0,W6100_REG_Sn_FRGR(sn),(uint8_t *)frgr, 2);
    *frgr=hwss_eth_ntohs(*frgr);
    return ret;
}

static inline esp_err_t W6100_setSn_KPALVTR(hwss_io_t *io, uint8_t sn, const uint8_t *kpalvt){
    return io->write(io,0,W6100_REG_Sn_KPALVTR(sn),kpalvt);
}

static inline esp_err_t W6100_getSn_KPALVTR(hwss_io_t *io, uint8_t sn, uint8_t *kpalvt){
    return io->read(io,0,W6100_REG_Sn_KPALVTR(sn),kpalvt);
}

///////////////////////////////////
// Socket N Buffer I/O function //
///////////////////////////////////

static inline esp_err_t W6100_writeSn_TXBUF(hwss_io_t *io, uint8_t sn, uint16_t addr, const uint8_t *data, uint16_t len){
    return io->write_mem(io,0,(uint32_t)(W6100_TXBUF_BLOCK(sn)<<3)+(addr<<8),data,len);
}

static inline esp_err_t W6100_readSn_RXBUF(hwss_io_t *io, uint8_t sn, uint16_t addr, uint8_t *data, uint16_t len){
    return io->read_mem(io,0,(uint32_t)(W6100_RXBUF_BLOCK(sn)<<3)+(addr<<8),data,len);
}

static inline esp_err_t W6100_readSn_RXBUF_with_cache(hwss_io_t *io, uint8_t sn, uint16_t addr, uint8_t *data, uint16_t len){
    return io->read_buf(io,0,(uint32_t)(W6100_RXBUF_BLOCK(sn)<<3)+(addr<<8),data,len);
}

#ifdef __cplusplus
}
#endif
