#pragma once
#include <string.h>
#include "hwss_eth_type.h"
#include "hwss_io.h"
#include "w5500.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////
// Common Register I/O function //
/////////////////////////////////

static inline esp_err_t W5500_setMR(hwss_io_t *io, const uint8_t *mr){
    return io->write(io,0,W5500_REG_MR,mr);
}

static inline esp_err_t W5500_getMR(hwss_io_t *io, uint8_t *mr){
    return io->read(io,0,W5500_REG_MR,mr);
}

static inline esp_err_t W5500_setGAR(hwss_io_t *io, const uint8_t *gar){
    return io->write_buf(io,0,W5500_REG_GAR,gar,4);
}

static inline esp_err_t W5500_getGAR(hwss_io_t *io, uint8_t *gar){
    return io->read_buf(io,0,W5500_REG_GAR,gar,4);
}

static inline esp_err_t W5500_setSUBR(hwss_io_t *io, const uint8_t *subr){
    return io->write_buf(io,0,W5500_REG_SUBR,subr,4);
}

static inline esp_err_t W5500_getSUBR(hwss_io_t *io, uint8_t *subr){
    return io->read_buf(io,0,W5500_REG_SUBR,subr,4);
}

static inline esp_err_t W5500_setSHAR(hwss_io_t *io, const uint8_t *shar){
    return io->write_buf(io,0,W5500_REG_SHAR,shar,6);
}

static inline esp_err_t W5500_getSHAR(hwss_io_t *io, uint8_t *shar){
    return io->read_buf(io,0,W5500_REG_SHAR,shar,6);
}

static inline esp_err_t W5500_setSIPR(hwss_io_t *io, const uint8_t *sipr){
    return io->write_buf(io,0,W5500_REG_SIPR,sipr,4);
}

static inline esp_err_t W5500_getSIPR(hwss_io_t *io, uint8_t *sipr){
    return io->read_buf(io,0,W5500_REG_SIPR,sipr,4);
}

static inline esp_err_t W5500_setINTLEVEL(hwss_io_t *io, const uint16_t *intlevel){
    uint16_t nv=hwss_eth_htons(*intlevel);
    return io->write_buf(io,0,W5500_REG_INTLEVEL,(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getINTLEVEL(hwss_io_t *io, uint16_t *intlevel){
    esp_err_t ret=io->read_buf(io,0,W5500_REG_INTLEVEL,(uint8_t *)intlevel,2);
    *intlevel=hwss_eth_ntohs(*intlevel);
    return ret;
}

static inline esp_err_t W5500_setIR(hwss_io_t *io, const uint8_t *ir){
    return io->write(io,0,W5500_REG_IR,ir);
}

static inline esp_err_t W5500_getIR(hwss_io_t *io, uint8_t *ir){
    return io->read(io,0,W5500_REG_IR,ir);
}

static inline esp_err_t W5500_setIMR(hwss_io_t *io, const uint8_t *imr){
    return io->write(io,0,W5500_REG_IMR,imr);
}

static inline esp_err_t W5500_getIMR(hwss_io_t *io, uint8_t *imr){
    return io->read(io,0,W5500_REG_IMR,imr);
}

static inline esp_err_t W5500_setSIR(hwss_io_t *io, const uint8_t *sir){
    return io->write(io,0,W5500_REG_SIR,sir);
}

static inline esp_err_t W5500_getSIR(hwss_io_t *io, uint8_t *sir){
    return io->read(io,0,W5500_REG_SIR,sir);
}

static inline esp_err_t W5500_setSIMR(hwss_io_t *io, const uint8_t *simr){
    return io->write(io,0,W5500_REG_SIMR,simr);
}

static inline esp_err_t W5500_getSIMR(hwss_io_t *io, uint8_t *simr){
    return io->read(io,0,W5500_REG_SIMR,simr);
}

static inline esp_err_t W5500_setRTR(hwss_io_t *io, const uint16_t *rtr){
    uint16_t nv=hwss_eth_htons(*rtr);
    return io->write_buf(io,0,W5500_REG_RTR,(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getRTR(hwss_io_t *io, uint16_t *rtr){
    esp_err_t ret = io->read_buf(io,0,W5500_REG_RTR,(uint8_t *)rtr,2);
    *rtr=hwss_eth_ntohs(*rtr);
    return ret;
}

static inline esp_err_t W5500_setRCR(hwss_io_t *io, const uint8_t *rcr){
    return io->write(io,0,W5500_REG_RCR,rcr);
}

static inline esp_err_t W5500_getRCR(hwss_io_t *io, uint8_t *rcr){
    return io->read(io,0,W5500_REG_RCR,rcr);
}

static inline esp_err_t W5500_setPTIMER(hwss_io_t *io, const uint8_t *ptimer){
    return io->write(io,0,W5500_REG_PTIMER,ptimer);
}

static inline esp_err_t W5500_getPTIMER(hwss_io_t *io, uint8_t *ptimer){
    return io->read(io,0,W5500_REG_PTIMER,ptimer);
}

static inline esp_err_t W5500_setPMAGIC(hwss_io_t *io, const uint8_t *pmagic){
    return io->write(io,0,W5500_REG_PMAGIC,pmagic);
}

static inline esp_err_t W5500_getPMAGIC(hwss_io_t *io, uint8_t *pmagic){
    return io->read(io,0,W5500_REG_PMAGIC,pmagic);
}

static inline esp_err_t W5500_setPHAR(hwss_io_t *io, const uint8_t *phar){
    return io->write_buf(io,0,W5500_REG_PHAR,phar,6);
}

static inline esp_err_t W5500_getPHAR(hwss_io_t *io, uint8_t *phar){
    return io->read_buf(io,0,W5500_REG_PHAR,phar,6);
}

static inline esp_err_t W5500_setPSID(hwss_io_t *io, const uint16_t *psid){
    uint16_t nv=hwss_eth_htons(*psid);
    return io->write_buf(io,0,W5500_REG_PSID,(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getPSID(hwss_io_t *io, uint16_t *psid){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_PSID,(uint8_t *)psid,2);
    *psid=hwss_eth_ntohs(*psid);
    return ret;
}

static inline esp_err_t W5500_setPMRU(hwss_io_t *io, const uint16_t *pmru){
    uint16_t nv=hwss_eth_htons(*pmru);
    return io->write_buf(io,0,W5500_REG_PMRU,(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getPMRU(hwss_io_t *io, uint16_t *pmru){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_PMRU,(uint8_t *)pmru,2);
    *pmru=hwss_eth_ntohs(*pmru);
    return ret;
}

static inline esp_err_t W5500_getUIPR(hwss_io_t *io, uint8_t* uipr){
    return io->read_buf(io,0,W5500_REG_UIPR,uipr,4);
}

static inline esp_err_t W5500_getUPORTR(hwss_io_t *io, uint16_t *uportr){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_UPORTR,(uint8_t *)uportr,2);
    *uportr=hwss_eth_ntohs(*uportr);
    return ret;
}

static inline esp_err_t W5500_setPHYCFGR(hwss_io_t *io, const uint8_t *phycfgr){
    return io->write(io,0,W5500_REG_PHYCFGR,phycfgr);
}

static inline esp_err_t W5500_getPHYCFGR(hwss_io_t *io, uint8_t *phycfgr){
    return io->read(io,0,W5500_REG_PHYCFGR,phycfgr);
}

static inline esp_err_t W5500_getVERSIONR(hwss_io_t *io, uint8_t *ver){
    return io->read(io,0,W5500_REG_VERSIONR,ver);
}

///////////////////////////////////
// Socket N register I/O function //
///////////////////////////////////

static inline esp_err_t W5500_setSn_MR(hwss_io_t *io, uint8_t sn, const uint8_t *mr){
    return io->write(io,0,W5500_REG_Sn_MR(sn),mr);
}

static inline esp_err_t W5500_getSn_MR(hwss_io_t *io, uint8_t sn, uint8_t *mr){
    return io->read(io,0,W5500_REG_Sn_MR(sn),mr);
}

static inline esp_err_t W5500_setSn_CR(hwss_io_t *io, uint8_t sn, const uint8_t *cr){
    return io->write(io,0,W5500_REG_Sn_CR(sn),cr);
}

static inline esp_err_t W5500_getSn_CR(hwss_io_t *io, uint8_t sn, uint8_t *cr){
    return io->read(io,0,W5500_REG_Sn_CR(sn),cr);
}

static inline esp_err_t W5500_setSn_IR(hwss_io_t *io, uint8_t sn, const uint8_t *ir){
    return io->write(io,0,W5500_REG_Sn_IR(sn),ir);
}

static inline esp_err_t W5500_getSn_IR(hwss_io_t *io, uint8_t sn, uint8_t *ir){
    return io->read(io,0,W5500_REG_Sn_IR(sn),ir);
}

static inline esp_err_t W5500_setSn_IMR(hwss_io_t *io, uint8_t sn, const uint8_t *imr){
    return io->write(io,0,W5500_REG_Sn_IMR(sn),imr);
}

static inline esp_err_t W5500_getSn_IMR(hwss_io_t *io, uint8_t sn, uint8_t *imr){
    return io->read(io,0,W5500_REG_Sn_IMR(sn),imr);
}

static inline esp_err_t W5500_getSn_SR(hwss_io_t *io, uint8_t sn, uint8_t *sr){
    return io->read(io,0,W5500_REG_Sn_SR(sn),sr);
}

static inline esp_err_t W5500_setSn_PORT(hwss_io_t *io, uint8_t sn, const uint16_t *port){
    uint16_t nv=hwss_eth_htons(*port);
    return io->write_buf(io,0,W5500_REG_Sn_PORT(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getSn_PORT(hwss_io_t *io, uint8_t sn, uint16_t *port){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_PORT(sn),(uint8_t *)port,2);
    *port=hwss_eth_ntohs(*port);
    return ret;
}

static inline esp_err_t W5500_setSn_DHAR(hwss_io_t *io, uint8_t sn, const uint8_t *dhar){
    return io->write_buf(io,0,W5500_REG_Sn_DHAR(sn),dhar,6);
}

static inline esp_err_t W5500_getSn_DHAR(hwss_io_t *io, uint8_t sn, uint8_t *dhar){
    return io->read_buf(io,0,W5500_REG_Sn_DHAR(sn),dhar,6);
}

static inline esp_err_t W5500_setSn_DIPR(hwss_io_t *io, uint8_t sn, const uint8_t *dipr){
    return io->write_buf(io,0,W5500_REG_Sn_DIPR(sn),dipr,4);
}

static inline esp_err_t W5500_getSn_DIPR(hwss_io_t *io, uint8_t sn, uint8_t *dipr){
    return io->read_buf(io,0,W5500_REG_Sn_DIPR(sn),dipr,4);
}

static inline esp_err_t W5500_setSn_DPORT(hwss_io_t *io, uint8_t sn, const uint16_t *dport){
    uint16_t nv=hwss_eth_htons(*dport);
    return io->write_buf(io,0,W5500_REG_Sn_DPORT(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getSn_DPORT(hwss_io_t *io, uint8_t sn, uint16_t *dport){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_DPORT(sn),(uint8_t *)dport,2);
    *dport=hwss_eth_ntohs(*dport);
    return ret;
}

static inline esp_err_t W5500_setSn_MSSR(hwss_io_t *io, uint8_t sn, const uint16_t *mss){
    uint16_t nv=hwss_eth_htons(*mss);
    return io->write_buf(io,0,W5500_REG_Sn_MSSR(sn),(uint8_t *)&nv, 2);
}

static inline esp_err_t W5500_getSn_MSSR(hwss_io_t *io, uint8_t sn, uint16_t *mss){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_MSSR(sn),(uint8_t *)mss, 2);
    *mss=hwss_eth_ntohs(*mss);
    return ret;
}

static inline esp_err_t W5500_setSn_TOS(hwss_io_t *io, uint8_t sn, const uint8_t *tos){
    return io->write(io,0,W5500_REG_Sn_TOS(sn),tos);
}

static inline esp_err_t W5500_getSn_TOS(hwss_io_t *io, uint8_t sn, uint8_t *tos){
    return io->read(io,0,W5500_REG_Sn_TOS(sn),tos);
}

static inline esp_err_t W5500_setSn_TTL(hwss_io_t *io, uint8_t sn, const uint8_t *ttl){
    return io->write(io,0,W5500_REG_Sn_TTL(sn),ttl);
}

static inline esp_err_t W5500_getSn_TTL(hwss_io_t *io, uint8_t sn, uint8_t *ttl){
    return io->read(io,0,W5500_REG_Sn_TTL(sn),ttl);
}

static inline esp_err_t W5500_setSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *rxbufsize){
    return io->write(io,0,W5500_REG_Sn_RXBUF_SIZE(sn),rxbufsize);
}

static inline esp_err_t W5500_getSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *rxbufsize){
    return io->read(io,0,W5500_REG_Sn_RXBUF_SIZE(sn),rxbufsize);
}

static inline esp_err_t W5500_setSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *txbufsize){
    return io->write(io,0,W5500_REG_Sn_TXBUF_SIZE(sn),txbufsize);
}

static inline esp_err_t W5500_getSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *txbufsize){
    return io->read(io,0,W5500_REG_Sn_TXBUF_SIZE(sn),txbufsize);
}

static inline esp_err_t W5500_getSn_TX_FSR(hwss_io_t *io, uint8_t sn, uint16_t *fsr){
    uint16_t fsr_p =0;
    uint16_t fsr_c =0;

    do{
        if(io->read_buf(io,0,W5500_REG_Sn_TX_FSR(sn),(uint8_t *)&fsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(fsr_p!=0){
            if(io->read_buf(io,0,W5500_REG_Sn_TX_FSR(sn),(uint8_t *)&fsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(fsr_p!=fsr_c);

    *fsr=hwss_eth_ntohs(fsr_c);
    return ESP_OK;
}

static inline esp_err_t W5500_getSn_TX_RD(hwss_io_t *io, uint8_t sn, uint16_t *txrd){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_TX_RD(sn),(uint8_t *)txrd,2);
    *txrd=hwss_eth_ntohs(*txrd);
    return ret;
}

static inline esp_err_t W5500_setSn_TX_WR(hwss_io_t *io, uint8_t sn, const uint16_t *txwr){
    uint16_t nv=hwss_eth_htons(*txwr);
    return io->write_buf(io,0,W5500_REG_Sn_TX_WR(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getSn_TX_WR(hwss_io_t *io, uint8_t sn, uint16_t *txwr){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_TX_WR(sn),(uint8_t *)txwr,2);
    *txwr=hwss_eth_ntohs(*txwr);
    return ret;
}

static inline esp_err_t W5500_getSn_RX_RSR(hwss_io_t *io, uint8_t sn, uint16_t *rsr){
    uint16_t rsr_p=0;
    uint16_t rsr_c=0;

    do{
        if(io->read_buf(io,0,W5500_REG_Sn_RX_RSR(sn),(uint8_t *)&rsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(rsr_p!=0){
            if(io->read_buf(io,0,W5500_REG_Sn_RX_RSR(sn),(uint8_t *)&rsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(rsr_p!=rsr_c);

    *rsr=hwss_eth_ntohs(rsr_c);
    return ESP_OK;
}

static inline esp_err_t W5500_setSn_RX_RD(hwss_io_t *io, uint8_t sn, const uint16_t *rxrd){
    uint16_t nv=hwss_eth_htons(*rxrd);
    return io->write_buf(io,0,W5500_REG_Sn_RX_RD(sn),(uint8_t *)&nv,2);
}

static inline esp_err_t W5500_getSn_RX_RD(hwss_io_t *io, uint8_t sn, uint16_t *rxrd){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_RX_RD(sn),(uint8_t *)rxrd,2);
    *rxrd=hwss_eth_ntohs(*rxrd);
    return ret;
}

static inline esp_err_t W5500_getSn_RX_WR(hwss_io_t *io, uint8_t sn, uint16_t *rxwr){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_RX_WR(sn),(uint8_t *)rxwr,2);
    *rxwr=hwss_eth_ntohs(*rxwr);
    return ret;
}

static inline esp_err_t W5500_setSn_FRAG(hwss_io_t *io, uint8_t sn, const uint16_t *frag){
    uint16_t nv=hwss_eth_htons(*frag);
    return io->write_buf(io,0,W5500_REG_Sn_FRAG(sn),(uint8_t *)&nv, 2);
}

static inline esp_err_t W5500_getSn_FRAG(hwss_io_t *io, uint8_t sn, uint16_t *frag){
    esp_err_t ret= io->read_buf(io,0,W5500_REG_Sn_FRAG(sn),(uint8_t *)frag, 2);
    *frag=hwss_eth_ntohs(*frag);
    return ret;
}

static inline esp_err_t W5500_setSn_KPALVTR(hwss_io_t *io, uint8_t sn, const uint8_t *kpalvt){
    return io->write(io,0,W5500_REG_Sn_KPALVTR(sn),kpalvt);
}

static inline esp_err_t W5500_getSn_KPALVTR(hwss_io_t *io, uint8_t sn, uint8_t *kpalvt){
    return io->read(io,0,W5500_REG_Sn_KPALVTR(sn),kpalvt);
}

///////////////////////////////////
// Socket N Buffer I/O function //
///////////////////////////////////

static inline esp_err_t W5500_writeSn_TXBUF(hwss_io_t *io, uint8_t sn, uint16_t addr, const uint8_t *data, uint16_t len){
    return io->write_mem(io,0,(uint32_t)(W5500_TXBUF_BLOCK(sn)<<3)+(addr<<8),data,len);
}

static inline esp_err_t W5500_readSn_RXBUF(hwss_io_t *io, uint8_t sn, uint16_t addr, uint8_t *data, uint16_t len){
    return io->read_mem(io,0,(uint32_t)(W5500_RXBUF_BLOCK(sn)<<3)+(addr<<8),data,len);
}

static inline esp_err_t W5500_readSn_RXBUF_with_cache(hwss_io_t *io, uint8_t sn, uint16_t addr, uint8_t *data, uint16_t len){
    return io->read_buf(io,0,(uint32_t)(W5500_RXBUF_BLOCK(sn)<<3)+(addr<<8),data,len);
}

#ifdef __cplusplus
}
#endif
