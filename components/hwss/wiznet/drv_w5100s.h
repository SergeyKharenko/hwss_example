#pragma once
#include <string.h>
#include "hwss_io.h"
#include "w5100s.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////
// Common Register I/O function //
/////////////////////////////////

inline esp_err_t W5100S_setMR(hwss_io_t *io, const uint8_t *mr){
    return io->write(io,0,W5100S_REG_MR,mr);
}

inline esp_err_t W5100S_getMR(hwss_io_t *io, uint8_t *mr){
    return io->read(io,0,W5100S_REG_MR,mr);
}

inline esp_err_t W5100S_setGAR(hwss_io_t *io, const uint8_t *gar){
    return io->write_buf(io,0,W5100S_REG_GAR,gar,4);
}

inline esp_err_t W5100S_getGAR(hwss_io_t *io, uint8_t *gar){
    return io->read_buf(io,0,W5100S_REG_GAR,gar,4);
}

inline esp_err_t W5100S_setSUBR(hwss_io_t *io, const uint8_t *subr){
    return io->write_buf(io,0,W5100S_REG_SUBR,subr,4);
}

inline esp_err_t W5100S_getSUBR(hwss_io_t *io, uint8_t *subr){
    return io->read_buf(io,0,W5100S_REG_SUBR,subr,4);
}

inline esp_err_t W5100S_setSHAR(hwss_io_t *io, const uint8_t *shar){
    return io->write_buf(io,0,W5100S_REG_SHAR,shar,6);
}

inline esp_err_t W5100S_getSHAR(hwss_io_t *io, uint8_t *shar){
    return io->read_buf(io,0,W5100S_REG_SHAR,shar,6);
}

inline esp_err_t W5100S_setSIPR(hwss_io_t *io, const uint8_t *sipr){
    return io->write_buf(io,0,W5100S_REG_SIPR,sipr,4);
}

inline esp_err_t W5100S_getSIPR(hwss_io_t *io, uint8_t *sipr){
    return io->read_buf(io,0,W5100S_REG_SIPR,sipr,4);
}

inline esp_err_t W5100S_setINTPTMR(hwss_io_t *io, const uint16_t *intptmr){
    return io->write_buf(io,0,W5100S_REG_INTPTMR,(uint8_t *)intptmr,2);
}

inline esp_err_t W5100S_getINTPTMR(hwss_io_t *io, uint16_t *intptmr){
    return io->read_buf(io,0,W5100S_REG_INTPTMR,(uint8_t *)intptmr,2);
}

inline esp_err_t W5100S_setIR(hwss_io_t *io, const uint8_t *ir){
    return io->write(io,0,W5100S_REG_IR,ir);
}

inline esp_err_t W5100S_getIR(hwss_io_t *io, uint8_t *ir){
    return io->read(io,0,W5100S_REG_IR,ir);
}

inline esp_err_t W5100S_setIMR(hwss_io_t *io, const uint8_t *imr){
    return io->write(io,0,W5100S_REG_IMR,imr);
}

inline esp_err_t W5100S_getIMR(hwss_io_t *io, uint8_t *imr){
    return io->read(io,0,W5100S_REG_IMR,imr);
}

inline esp_err_t W5100S_setRTR(hwss_io_t *io, const uint16_t *rtr){
    return io->write_buf(io,0,W5100S_REG_RTR,(uint8_t *)rtr,2);
}

inline esp_err_t W5100S_getRTR(hwss_io_t *io, uint16_t *rtr){
    return io->read_buf(io,0,W5100S_REG_RTR,(uint8_t *)rtr,2);
}

inline esp_err_t W5100S_setRCR(hwss_io_t *io, const uint8_t *rcr){
    return io->write(io,0,W5100S_REG_RCR,rcr);
}

inline esp_err_t W5100S_getRCR(hwss_io_t *io, uint8_t *rcr){
    return io->read(io,0,W5100S_REG_RCR,rcr);
}

inline esp_err_t W5100S_setRMSR(hwss_io_t *io, const uint8_t *rmsr){
    return io->write(io,0,W5100S_REG_RMSR,rmsr);
}

inline esp_err_t W5100S_getRMSR(hwss_io_t *io, uint8_t *rmsr){
    return io->read(io,0,W5100S_REG_RMSR,rmsr);
}

inline esp_err_t W5100S_setTMSR(hwss_io_t *io, const uint8_t *tmsr){
    return io->write(io,0,W5100S_REG_TMSR,tmsr);
}

inline esp_err_t W5100S_getTMSR(hwss_io_t *io, uint8_t *tmsr){
    return io->read(io,0,W5100S_REG_TMSR,tmsr);
}

inline esp_err_t W5100S_setPTIMER(hwss_io_t *io, const uint8_t *ptimer){
    return io->write(io,0,W5100S_REG_PTIMER,ptimer);
}

inline esp_err_t W5100S_getPTIMER(hwss_io_t *io, uint8_t *ptimer){
    return io->read(io,0,W5100S_REG_PTIMER,ptimer);
}

inline esp_err_t W5100S_setPMAGIC(hwss_io_t *io, const uint8_t *pmagic){
    return io->write(io,0,W5100S_REG_PMAGIC,pmagic);
}

inline esp_err_t W5100S_getPMAGIC(hwss_io_t *io, uint8_t *pmagic){
    return io->read(io,0,W5100S_REG_PMAGIC,pmagic);
}

inline esp_err_t W5100S_setIR2(hwss_io_t *io, const uint8_t *ir2){
    return io->write(io,0,W5100S_REG_IR2,ir2);
}

inline esp_err_t W5100S_getIR2(hwss_io_t *io, uint8_t *ir2){
    return io->read(io,0,W5100S_REG_IR2,ir2);
}

inline esp_err_t W5100S_setIMR2(hwss_io_t *io, const uint8_t *imr2){
    return io->write(io,0,W5100S_REG_IMR2,imr2);
}

inline esp_err_t W5100S_getIMR2(hwss_io_t *io, uint8_t *imr2){
    return io->read(io,0,W5100S_REG_IMR2,imr2);
}

inline esp_err_t W5100S_setMR2(hwss_io_t *io, const uint8_t *mr2){
    return io->write(io,0,W5100S_REG_MR2,mr2);
}

inline esp_err_t W5100S_getMR2(hwss_io_t *io, uint8_t *mr2){
    return io->read(io,0,W5100S_REG_MR2,mr2);
}

inline esp_err_t W5100S_setPHAR(hwss_io_t *io, const uint8_t *phar){
    return io->write_buf(io,0,W5100S_REG_PHAR,phar,6);
}

inline esp_err_t W5100S_getPHAR(hwss_io_t *io, uint8_t *phar){
    return io->read_buf(io,0,W5100S_REG_PHAR,phar,6);
}

inline esp_err_t W5100S_setPSIDR(hwss_io_t *io, const uint16_t *psid){
    return io->write_buf(io,0,W5100S_REG_PSIDR,(uint8_t *)psid,2);
}

inline esp_err_t W5100S_getPSIDR(hwss_io_t *io, uint16_t *psid){
    return io->read_buf(io,0,W5100S_REG_PSIDR,(uint8_t *)psid,2);
}

inline esp_err_t W5100S_setPMRUR(hwss_io_t *io, const uint16_t *pmru){
    return io->write_buf(io,0,W5100S_REG_PMRUR,(uint8_t *)pmru,2);
}

inline esp_err_t W5100S_getPMRU(hwss_io_t *io, uint16_t *pmru){
    return io->read_buf(io,0,W5100S_REG_PMRUR,(uint8_t *)pmru,2);
}

inline esp_err_t W5100S_getUIPR(hwss_io_t *io, uint8_t* uipr){
    return io->read_buf(io,0,W5100S_REG_UIPR,uipr,4);
}

inline esp_err_t W5100S_getUPORTR(hwss_io_t *io, uint16_t *uportr){
    return io->read_buf(io,0,W5100S_REG_UPORTR,(uint8_t *)uportr,2);
}

inline esp_err_t W5100S_getPHYSR(hwss_io_t *io, uint8_t *physr){
    return io->read(io,0,W5100S_REG_PHYSR,physr);
}

inline esp_err_t W5100S_getPHYSR1(hwss_io_t *io, uint8_t *physr1){
    return io->read(io,0,W5100S_REG_PHYSR1,physr1);
}

inline esp_err_t W5100S_getPHYAR(hwss_io_t *io, uint8_t *phyar){
    return io->read(io,0,W5100S_REG_PHYAR,phyar);
}

inline esp_err_t W5100S_getPHYRAR(hwss_io_t *io, uint8_t *phyrar){
    return io->read(io,0,W5100S_REG_PHYRAR,phyrar);
}

inline esp_err_t W5100S_setPHYRAR(hwss_io_t *io, const uint8_t *phyrar){
    return io->write(io,0,W5100S_REG_PHYRAR,phyrar);
}

inline esp_err_t W5100S_getPHYDIR(hwss_io_t *io, uint8_t *phydir){
    return io->read(io,0,W5100S_REG_PHYDIR,phydir);
}

inline esp_err_t W5100S_setPHYDIR(hwss_io_t *io, const uint8_t *phydir){
    return io->write(io,0,W5100S_REG_PHYDIR,phydir);
}

inline esp_err_t W5100S_getPHYDOR(hwss_io_t *io, uint8_t *phydor){
    return io->read(io,0,W5100S_REG_PHYDOR,phydor);
}

inline esp_err_t W5100S_setPHYDOR(hwss_io_t *io, const uint8_t *phydor){
    return io->write(io,0,W5100S_REG_PHYDOR,phydor);
}

inline esp_err_t W5100S_getPHYACR(hwss_io_t *io, uint8_t *phyacr){
    return io->read(io,0,W5100S_REG_PHYACR,phyacr);
}

inline esp_err_t W5100S_setPHYACR(hwss_io_t *io, const uint8_t *phyacr){
    return io->write(io,0,W5100S_REG_PHYACR,phyacr);
}

inline esp_err_t W5100S_getPHYDIVR(hwss_io_t *io, uint8_t *phydivr){
    return io->read(io,0,W5100S_REG_PHYDIVR,phydivr);
}

inline esp_err_t W5100S_setPHYDIVR(hwss_io_t *io, const uint8_t *phydivr){
    return io->write(io,0,W5100S_REG_PHYDIVR,phydivr);
}

inline esp_err_t W5100S_setPHYCR0(hwss_io_t *io, const uint8_t *phycr0){
    return io->write(io,0,W5100S_REG_PHYCR0,phycr0);
}

inline esp_err_t W5100S_getPHYCR1(hwss_io_t *io, uint8_t *phycr1){
    return io->read(io,0,W5100S_REG_PHYCR0,phycr1);
}

inline esp_err_t W5100S_setPHYCR1(hwss_io_t *io, const uint8_t *phycr1){
    return io->write(io,0,W5100S_REG_PHYCR1,phycr1);
}

inline esp_err_t W5100S_getSLCR(hwss_io_t *io, uint8_t *slcr){
    return io->read(io,0,W5100S_REG_SLCR,slcr);
}

inline esp_err_t W5100S_setSLCR(hwss_io_t *io, const uint8_t *slcr){
    return io->write(io,0,W5100S_REG_SLCR,slcr);
}

inline esp_err_t W5100S_getSLRTR(hwss_io_t *io, uint16_t *slrtr){
    return io->read_buf(io,0,W5100S_REG_SLRTR,(uint8_t *)slrtr,2);
}

inline esp_err_t W5100S_setSLRTR(hwss_io_t *io, const uint16_t *slrtr){
    return io->write_buf(io,0,W5100S_REG_SLRTR,(uint8_t *)slrtr,2);
}

inline esp_err_t W5100S_getSLRCR(hwss_io_t *io, uint8_t *slrcr){
    return io->read(io,0,W5100S_REG_SLRCR,slrcr);
}

inline esp_err_t W5100S_setSLRCR(hwss_io_t *io, const uint8_t *slrcr){
    return io->write(io,0,W5100S_REG_SLRCR,slrcr);
}

inline esp_err_t W5100S_setSLPIPR(hwss_io_t *io, const uint8_t *slpipr){
    return io->write_buf(io,0,W5100S_REG_SLPIPR,slpipr,4);
}

inline esp_err_t W5100S_getSLPIPR(hwss_io_t *io, uint8_t *slpipr){
    return io->read_buf(io,0,W5100S_REG_SLPIPR,slpipr,4);
}

inline esp_err_t W5100S_setSLPHAR(hwss_io_t *io, const uint8_t *slphar){
    return io->write_buf(io,0,W5100S_REG_SLPHAR,slphar,6);
}

inline esp_err_t W5100S_getSLPHAR(hwss_io_t *io, uint8_t *slphar){
    return io->read_buf(io,0,W5100S_REG_SLPHAR,slphar,6);
}

inline esp_err_t W5100S_setPINGSEQR(hwss_io_t *io, const uint16_t *pingseqr){
    return io->write_buf(io,0,W5100S_REG_PINGSEQR,(uint8_t *)pingseqr,2);
}

inline esp_err_t W5100S_getPINGSEQR(hwss_io_t *io, uint16_t *pingseqr){
    return io->read_buf(io,0,W5100S_REG_PINGSEQR,(uint8_t *)pingseqr,2);
}

inline esp_err_t W5100S_setPINGIDR(hwss_io_t *io, const uint16_t *pingidr){
    return io->write_buf(io,0,W5100S_REG_PINGIDR,(uint8_t *)pingidr,2);
}

inline esp_err_t W5100S_getPINGIDR(hwss_io_t *io, uint16_t *pingidr){
    return io->read_buf(io,0,W5100S_REG_PINGIDR,(uint8_t *)pingidr,2);
}

inline esp_err_t W5100S_getSLIMR(hwss_io_t *io, uint8_t *slimr){
    return io->read(io,0,W5100S_REG_SLIMR,slimr);
}

inline esp_err_t W5100S_setSLIMR(hwss_io_t *io, const uint8_t *slimr){
    return io->write(io,0,W5100S_REG_SLIMR,slimr);
}

inline esp_err_t W5100S_getSLIR(hwss_io_t *io, uint8_t *slir){
    return io->read(io,0,W5100S_REG_SLIR,slir);
}

inline esp_err_t W5100S_setSLIR(hwss_io_t *io, const uint8_t *slir){
    return io->write(io,0,W5100S_REG_SLIR,slir);
}

inline esp_err_t W5100S_setCHIPLOCK(hwss_io_t *io, bool lock){
    uint8_t reg=0x00;
    if(lock)
        reg= 0xFF;
    else
        reg= 0xCE;
    return io->write(io,0,W5100S_REG_CHIPLCKR,&reg);
}

inline esp_err_t W5100S_setNETLOCK(hwss_io_t *io, bool lock){
    uint8_t reg=0x00;
    if(lock)
        reg= 0x3A;
    else
        reg= 0xC5;
    return io->write(io,0,W5100S_REG_NETLCKR,&reg);
}

inline esp_err_t W5100S_setPHYLOCK(hwss_io_t *io, bool lock){
    uint8_t reg=0x00;
    if(lock)
        reg= 0xFF;
    else
        reg= 0x53;
    return io->write(io,0,W5100S_REG_PHYLCKR,&reg);
}

inline esp_err_t W5100S_getVERR(hwss_io_t *io, uint8_t *ver){
    return io->read(io,0,W5100S_REG_VERR,ver);
}

///////////////////////////////////
// Socket N register I/O function //
///////////////////////////////////

inline esp_err_t W5100S_setSn_MR(hwss_io_t *io, uint8_t sn, const uint8_t *mr){
    return io->write(io,0,W5100S_REG_Sn_MR(sn),mr);
}

inline esp_err_t W5100S_getSn_MR(hwss_io_t *io, uint8_t sn, uint8_t *mr){
    return io->read(io,0,W5100S_REG_Sn_MR(sn),mr);
}

inline esp_err_t W5100S_setSn_CR(hwss_io_t *io, uint8_t sn, const uint8_t *cr){
    return io->write(io,0,W5100S_REG_Sn_CR(sn),cr);
}

inline esp_err_t W5100S_getSn_CR(hwss_io_t *io, uint8_t sn, uint8_t *cr){
    return io->read(io,0,W5100S_REG_Sn_CR(sn),cr);
}

inline esp_err_t W5100S_setSn_IR(hwss_io_t *io, uint8_t sn, const uint8_t *ir){
    return io->write(io,0,W5100S_REG_Sn_IR(sn),ir);
}

inline esp_err_t W5100S_getSn_IR(hwss_io_t *io, uint8_t sn, uint8_t *ir){
    return io->read(io,0,W5100S_REG_Sn_IR(sn),ir);
}

inline esp_err_t W5100S_setSn_IMR(hwss_io_t *io, uint8_t sn, const uint8_t *imr){
    return io->write(io,0,W5100S_REG_Sn_IMR(sn),imr);
}

inline esp_err_t W5100S_getSn_IMR(hwss_io_t *io, uint8_t sn, uint8_t *imr){
    return io->read(io,0,W5100S_REG_Sn_IMR(sn),imr);
}

inline esp_err_t W5100S_getSn_SR(hwss_io_t *io, uint8_t sn, uint8_t *sr){
    return io->read(io,0,W5100S_REG_Sn_SR(sn),sr);
}

inline esp_err_t W5100S_setSn_PORT(hwss_io_t *io, uint8_t sn, const uint16_t *port){
    return io->write_buf(io,0,W5100S_REG_Sn_PORT(sn),(uint8_t *)port,2);
}

inline esp_err_t W5100S_getSn_PORT(hwss_io_t *io, uint8_t sn, uint16_t *port){
    return io->read_buf(io,0,W5100S_REG_Sn_PORT(sn),(uint8_t *)port,2);
}

inline esp_err_t W5100S_setSn_DHAR(hwss_io_t *io, uint8_t sn, const uint8_t *dhar){
    return io->write_buf(io,0,W5100S_REG_Sn_DHAR(sn),dhar,6);
}

inline esp_err_t W5100S_getSn_DHAR(hwss_io_t *io, uint8_t sn, uint8_t *dhar){
    return io->read_buf(io,0,W5100S_REG_Sn_DHAR(sn),dhar,6);
}

inline esp_err_t W5100S_setSn_DIPR(hwss_io_t *io, uint8_t sn, const uint8_t *dipr){
    return io->write_buf(io,0,W5100S_REG_Sn_DIPR(sn),dipr,4);
}

inline esp_err_t W5100S_getSn_DIPR(hwss_io_t *io, uint8_t sn, uint8_t *dipr){
    return io->read_buf(io,0,W5100S_REG_Sn_DIPR(sn),dipr,4);
}

inline esp_err_t W5100S_setSn_DPORT(hwss_io_t *io, uint8_t sn, const uint16_t *dport){
    return io->write_buf(io,0,W5100S_REG_Sn_DPORT(sn),(uint8_t *)dport,2);
}

inline esp_err_t W5100S_getSn_DPORT(hwss_io_t *io, uint8_t sn, uint16_t *dport){
    return io->read_buf(io,0,W5100S_REG_Sn_DPORT(sn),(uint8_t *)dport,2);
}

inline esp_err_t W5100S_setSn_MSSR(hwss_io_t *io, uint8_t sn, const uint16_t *mss){
    return io->write_buf(io,0,W5100S_REG_Sn_MSSR(sn),(uint8_t *)mss, 2);
}

inline esp_err_t W5100S_getSn_MSSR(hwss_io_t *io, uint8_t sn, uint16_t *mss){
    return io->read_buf(io,0,W5100S_REG_Sn_MSSR(sn),(uint8_t *)mss, 2);
}

inline esp_err_t W5100S_setSn_TOS(hwss_io_t *io, uint8_t sn, const uint8_t *tos){
    return io->write(io,0,W5100S_REG_Sn_TOS(sn),tos);
}

inline esp_err_t W5100S_getSn_TOS(hwss_io_t *io, uint8_t sn, uint8_t *tos){
    return io->read(io,0,W5100S_REG_Sn_TOS(sn),tos);
}

inline esp_err_t W5100S_setSn_TTL(hwss_io_t *io, uint8_t sn, const uint8_t *ttl){
    return io->write(io,0,W5100S_REG_Sn_TTL(sn),ttl);
}

inline esp_err_t W5100S_getSn_TTL(hwss_io_t *io, uint8_t sn, uint8_t *ttl){
    return io->read(io,0,W5100S_REG_Sn_TTL(sn),ttl);
}

inline esp_err_t W5100S_setSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *rxbufsize){
    return io->write(io,0,W5100S_REG_Sn_RXBUF_SIZE(sn),rxbufsize);
}

inline esp_err_t W5100S_getSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *rxbufsize){
    return io->read(io,0,W5100S_REG_Sn_RXBUF_SIZE(sn),rxbufsize);
}

inline esp_err_t W5100S_setSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *txbufsize){
    return io->write(io,0,W5100S_REG_Sn_TXBUF_SIZE(sn),txbufsize);
}

inline esp_err_t W5100S_getSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *txbufsize){
    return io->read(io,0,W5100S_REG_Sn_TXBUF_SIZE(sn),txbufsize);
}

inline esp_err_t W5100S_getSn_TX_FSR(hwss_io_t *io, uint8_t sn, uint16_t *fsr){
    uint16_t fsr_p =0;
    uint16_t fsr_c =0;

    do{
        if(io->read_buf(io,0,W5100S_REG_Sn_TX_FSR(sn),(uint8_t *)&fsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(fsr_p!=0){
            if(io->read_buf(io,0,W5100S_REG_Sn_TX_FSR(sn),(uint8_t *)&fsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(fsr_p!=fsr_c);

    memcpy(fsr,&fsr_c,2);
    return ESP_OK;
}

inline esp_err_t W5100S_getSn_TX_RD(hwss_io_t *io, uint8_t sn, uint16_t *txrd){
    return io->read_buf(io,0,W5100S_REG_Sn_TX_RD(sn),(uint8_t *)txrd,2);
}

inline esp_err_t W5100S_setSn_TX_WR(hwss_io_t *io, uint8_t sn, const uint16_t *txwr){
    return io->write_buf(io,0,W5100S_REG_Sn_TX_WR(sn),(uint8_t *)txwr,2);
}

inline esp_err_t W5100S_getSn_TX_WR(hwss_io_t *io, uint8_t sn, uint16_t *txwr){
    return io->read_buf(io,0,W5100S_REG_Sn_TX_WR(sn),(uint8_t *)txwr,2);
}

inline esp_err_t W5100S_getSn_RX_RSR(hwss_io_t *io, uint8_t sn, uint16_t *rsr){
    uint16_t rsr_p=0;
    uint16_t rsr_c=0;

    do{
        if(io->read_buf(io,0,W5100S_REG_Sn_RX_RSR(sn),(uint8_t *)&rsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(rsr_p!=0){
            if(io->read_buf(io,0,W5100S_REG_Sn_RX_RSR(sn),(uint8_t *)&rsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(rsr_p!=rsr_c);

    memcpy(rsr,&rsr_c,2);
    return ESP_OK;
}

inline esp_err_t W5100S_setSn_RX_RD(hwss_io_t *io, uint8_t sn, const uint16_t *rxrd){
    return io->write_buf(io,0,W5100S_REG_Sn_RX_RD(sn),(uint8_t *)rxrd,2);
}

inline esp_err_t W5100S_getSn_RX_RD(hwss_io_t *io, uint8_t sn, uint16_t *rxrd){
    return io->read_buf(io,0,W5100S_REG_Sn_RX_RD(sn),(uint8_t *)rxrd,2);
}

inline esp_err_t W5100S_getSn_RX_WR(hwss_io_t *io, uint8_t sn, uint16_t *rxwr){
    return io->read_buf(io,0,W5100S_REG_Sn_RX_WR(sn),(uint8_t *)rxwr,2);
}

inline esp_err_t W5100S_setSn_FRAGR(hwss_io_t *io, uint8_t sn, const uint16_t *frag){
    return io->write_buf(io,0,W5100S_REG_Sn_FRAGR(sn),(uint8_t *)frag, 2);
}

inline esp_err_t W5100S_getSn_FRAGR(hwss_io_t *io, uint8_t sn, uint16_t *frag){
    return io->read_buf(io,0,W5100S_REG_Sn_FRAGR(sn),(uint8_t *)frag, 2);
}

inline esp_err_t W5100S_setSn_MR2(hwss_io_t *io, uint8_t sn, const uint8_t *mr2){
    return io->write(io,0,W5100S_REG_Sn_MR2(sn),mr2);
}

inline esp_err_t W5100S_getSn_MR2(hwss_io_t *io, uint8_t sn, uint8_t *mr2){
    return io->read(io,0,W5100S_REG_Sn_MR2(sn),mr2);
}

inline esp_err_t W5100S_setSn_KPALVTR(hwss_io_t *io, uint8_t sn, const uint8_t *kpalvt){
    return io->write(io,0,W5100S_REG_Sn_KPALVTR(sn),kpalvt);
}

inline esp_err_t W5100S_getSn_KPALVTR(hwss_io_t *io, uint8_t sn, uint8_t *kpalvt){
    return io->read(io,0,W5100S_REG_Sn_KPALVTR(sn),kpalvt);
}

#ifdef __cplusplus
}
#endif
