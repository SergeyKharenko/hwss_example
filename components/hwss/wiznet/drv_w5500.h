#pragma once
#include <string.h>
#include "hwss_io.h"
#include "w5500.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////
// Common Register I/O function //
/////////////////////////////////

inline esp_err_t setMR(hwss_io_t *io, const uint8_t *mr){
    return io->write(io,0,MR,mr);
}

inline esp_err_t getMR(hwss_io_t *io, uint8_t *mr){
    return io->read(io,0,MR,mr);
}

inline esp_err_t setGAR(hwss_io_t *io, const uint8_t *gar){
    return io->write_buf(io,0,GAR,gar,4);
}

inline esp_err_t getGAR(hwss_io_t *io, uint8_t *gar){
    return io->read_buf(io,0,GAR,gar,4);
}

inline esp_err_t setSUBR(hwss_io_t *io, const uint8_t *subr){
    return io->write_buf(io,0,SUBR,subr,4);
}

inline esp_err_t getSUBR(hwss_io_t *io, uint8_t *subr){
    return io->read_buf(io,0,SUBR,subr,4);
}

inline esp_err_t setSHAR(hwss_io_t *io, const uint8_t *shar){
    return io->write_buf(io,0,SHAR,shar,6);
}

inline esp_err_t getSHAR(hwss_io_t *io, uint8_t *shar){
    return io->read_buf(io,0,SHAR,shar,6);
}

inline esp_err_t setSIPR(hwss_io_t *io, const uint8_t *sipr){
    return io->write_buf(io,0,SIPR,sipr,4);
}

inline esp_err_t getSIPR(hwss_io_t *io, uint8_t *sipr){
    return io->read_buf(io,0,SIPR,sipr,4);
}

inline esp_err_t setINTLEVEL(hwss_io_t *io, const uint16_t *intlevel){
    return io->write_buf(io,0,INTLEVEL,(uint8_t *)intlevel,2);
}

inline esp_err_t getINTLEVEL(hwss_io_t *io, uint16_t *intlevel){
    return io->read_buf(io,0,INTLEVEL,(uint8_t *)intlevel,2);
}

inline esp_err_t setIR(hwss_io_t *io, const uint8_t *ir){
    return io->write(io,0,IR,ir);
}

inline esp_err_t getIR(hwss_io_t *io, uint8_t *ir){
    return io->read(io,0,IR,ir);
}

inline esp_err_t setIMR(hwss_io_t *io, const uint8_t *imr){
    return io->write(io,0,_IMR_,imr);
}

inline esp_err_t getIMR(hwss_io_t *io, uint8_t *imr){
    return io->read(io,0,_IMR_,imr);
}

inline esp_err_t setSIR(hwss_io_t *io, const uint8_t *sir){
    return io->write(io,0,SIR,sir);
}

inline esp_err_t getSIR(hwss_io_t *io, uint8_t *sir){
    return io->read(io,0,SIR,sir);
}

inline esp_err_t setSIMR(hwss_io_t *io, const uint8_t *simr){
    return io->write(io,0,SIMR,simr);
}

inline esp_err_t getSIMR(hwss_io_t *io, uint8_t *simr){
    return io->read(io,0,SIMR,simr);
}

inline esp_err_t setRTR(hwss_io_t *io, const uint16_t *rtr){
    return io->write_buf(io,0,_RTR_,(uint8_t *)rtr,2);
}

inline esp_err_t getRTR(hwss_io_t *io, uint16_t *rtr){
    return io->read_buf(io,0,_RTR_,(uint8_t *)rtr,2);
}

inline esp_err_t setRCR(hwss_io_t *io, const uint8_t *rcr){
    return io->write(io,0,_RCR_,rcr);
}

inline esp_err_t getRCR(hwss_io_t *io, uint8_t *rcr){
    return io->read(io,0,_RCR_,rcr);
}

inline esp_err_t setPTIMER(hwss_io_t *io, const uint8_t *ptimer){
    return io->write(io,0,PTIMER,ptimer);
}

inline esp_err_t getPTIMER(hwss_io_t *io, uint8_t *ptimer){
    return io->read(io,0,PTIMER,ptimer);
}

inline esp_err_t setPMAGIC(hwss_io_t *io, const uint8_t *pmagic){
    return io->write(io,0,PMAGIC,pmagic);
}

inline esp_err_t getPMAGIC(hwss_io_t *io, uint8_t *pmagic){
    return io->read(io,0,PMAGIC,pmagic);
}

inline esp_err_t setPHAR(hwss_io_t *io, const uint8_t *phar){
    return io->write_buf(io,0,PHAR,phar,6);
}

inline esp_err_t getPHAR(hwss_io_t *io, uint8_t *phar){
    return io->read_buf(io,0,PHAR,phar,6);
}

inline esp_err_t setPSID(hwss_io_t *io, const uint16_t *psid){
    return io->write_buf(io,0,PSID,(uint8_t *)psid,2);
}

inline esp_err_t getPSID(hwss_io_t *io, uint16_t *psid){
    return io->read_buf(io,0,PSID,(uint8_t *)psid,2);
}

inline esp_err_t setPMRU(hwss_io_t *io, const uint16_t *pmru){
    return io->write_buf(io,0,PMRU,(uint8_t *)pmru,2);
}

inline esp_err_t getPMRU(hwss_io_t *io, uint16_t *pmru){
    return io->read_buf(io,0,PMRU,(uint8_t *)pmru,2);
}

inline esp_err_t getUIPR(hwss_io_t *io, uint8_t* uipr){
    return io->read_buf(io,0,UIPR,uipr,4);
}

inline esp_err_t getUPORTR(hwss_io_t *io, uint16_t *uportr){
    return io->read_buf(io,0,UPORTR,(uint8_t *)uportr,2);
}

inline esp_err_t setPHYCFGR(hwss_io_t *io, const uint8_t *phycfgr){
    return io->write(io,0,PHYCFGR,phycfgr);
}

inline esp_err_t getPHYCFGR(hwss_io_t *io, uint8_t *phycfgr){
    return io->read(io,0,PHYCFGR,phycfgr);
}

inline esp_err_t getVERSIONR(hwss_io_t *io, uint8_t *ver){
    return io->read(io,0,VERSIONR,ver);
}

///////////////////////////////////
// Socket N register I/O function //
///////////////////////////////////

inline esp_err_t setSn_MR(hwss_io_t *io, uint8_t sn, const uint8_t *mr){
    return io->write(io,0,Sn_MR(sn),mr);
}

inline esp_err_t getSn_MR(hwss_io_t *io, uint8_t sn, uint8_t *mr){
    return io->read(io,0,Sn_MR(sn),mr);
}

inline esp_err_t setSn_CR(hwss_io_t *io, uint8_t sn, const uint8_t *cr){
    return io->write(io,0,Sn_CR(sn),cr);
}

inline esp_err_t getSn_CR(hwss_io_t *io, uint8_t sn, uint8_t *cr){
    return io->read(io,0,Sn_CR(sn),cr);
}

inline esp_err_t setSn_IR(hwss_io_t *io, uint8_t sn, const uint8_t *ir){
    return io->write(io,0,Sn_IR(sn),ir);
}

inline esp_err_t getSn_IR(hwss_io_t *io, uint8_t sn, uint8_t *ir){
    return io->read(io,0,Sn_IR(sn),ir);
}

inline esp_err_t setSn_IMR(hwss_io_t *io, uint8_t sn, const uint8_t *imr){
    return io->write(io,0,Sn_IMR(sn),imr);
}

inline esp_err_t getSn_IMR(hwss_io_t *io, uint8_t sn, uint8_t *imr){
    return io->read(io,0,Sn_IMR(sn),imr);
}

inline esp_err_t getSn_SR(hwss_io_t *io, uint8_t sn, uint8_t *sr){
    return io->read(io,0,Sn_SR(sn),sr);
}

inline esp_err_t setSn_PORT(hwss_io_t *io, uint8_t sn, const uint16_t *port){
    return io->write_buf(io,0,Sn_PORT(sn),(uint8_t *)port,2);
}

inline esp_err_t getSn_PORT(hwss_io_t *io, uint8_t sn, uint16_t *port){
    return io->read_buf(io,0,Sn_PORT(sn),(uint8_t *)port,2);
}

inline esp_err_t setSn_DHAR(hwss_io_t *io, uint8_t sn, const uint8_t *dhar){
    return io->write_buf(io,0,Sn_DHAR(sn),dhar,6);
}

inline esp_err_t getSn_DHAR(hwss_io_t *io, uint8_t sn, uint8_t *dhar){
    return io->read_buf(io,0,Sn_DHAR(sn),dhar,6);
}

inline esp_err_t setSn_DIPR(hwss_io_t *io, uint8_t sn, const uint8_t *dipr){
    return io->write_buf(io,0,Sn_DIPR(sn),dipr,4);
}

inline esp_err_t getSn_DIPR(hwss_io_t *io, uint8_t sn, uint8_t *dipr){
    return io->read_buf(io,0,Sn_DIPR(sn),dipr,4);
}

inline esp_err_t setSn_DPORT(hwss_io_t *io, uint8_t sn, const uint16_t *dport){
    return io->write_buf(io,0,Sn_DPORT(sn),(uint8_t *)dport,2);
}

inline esp_err_t getSn_DPORT(hwss_io_t *io, uint8_t sn, uint16_t *dport){
    return io->read_buf(io,0,Sn_DPORT(sn),(uint8_t *)dport,2);
}

inline esp_err_t setSn_MSSR(hwss_io_t *io, uint8_t sn, const uint16_t *mss){
    return io->write_buf(io,0,Sn_MSSR(sn),(uint8_t *)mss, 2);
}

inline esp_err_t getSn_MSSR(hwss_io_t *io, uint8_t sn, uint16_t *mss){
    return io->read_buf(io,0,Sn_MSSR(sn),(uint8_t *)mss, 2);
}

inline esp_err_t setSn_TOS(hwss_io_t *io, uint8_t sn, const uint8_t *tos){
    return io->write(io,0,Sn_TOS(sn),tos);
}

inline esp_err_t getSn_TOS(hwss_io_t *io, uint8_t sn, uint8_t *tos){
    return io->read(io,0,Sn_TOS(sn),tos);
}

inline esp_err_t setSn_TTL(hwss_io_t *io, uint8_t sn, const uint8_t *ttl){
    return io->write(io,0,Sn_TTL(sn),ttl);
}

inline esp_err_t getSn_TTL(hwss_io_t *io, uint8_t sn, uint8_t *ttl){
    return io->read(io,0,Sn_TTL(sn),ttl);
}

inline esp_err_t setSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *rxbufsize){
    return io->write(io,0,Sn_RXBUF_SIZE(sn),rxbufsize);
}

inline esp_err_t getSn_RXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *rxbufsize){
    return io->read(io,0,Sn_RXBUF_SIZE(sn),rxbufsize);
}

inline esp_err_t setSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, const uint8_t *txbufsize){
    return io->write(io,0,Sn_TXBUF_SIZE(sn),txbufsize);
}

inline esp_err_t getSn_TXBUF_SIZE(hwss_io_t *io, uint8_t sn, uint8_t *txbufsize){
    return io->read(io,0,Sn_TXBUF_SIZE(sn),txbufsize);
}

inline esp_err_t getSn_TX_FSR(hwss_io_t *io, uint8_t sn, uint16_t *fsr){
    uint16_t fsr_p =0;
    uint16_t fsr_c =0;

    do{
        if(io->read_buf(io,0,Sn_TX_FSR(sn),(uint8_t *)&fsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(fsr_p!=0){
            if(io->read_buf(io,0,Sn_TX_FSR(sn),(uint8_t *)&fsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(fsr_p!=fsr_c);

    memcpy(fsr,&fsr_c,2);
    return ESP_OK;
}

inline esp_err_t getSn_TX_RD(hwss_io_t *io, uint8_t sn, uint16_t *txrd){
    return io->read_buf(io,0,Sn_TX_RD(sn),(uint8_t *)txrd,2);
}

inline esp_err_t setSn_TX_WR(hwss_io_t *io, uint8_t sn, const uint16_t *txwr){
    return io->write_buf(io,0,Sn_TX_WR(sn),(uint8_t *)txwr,2);
}

inline esp_err_t getSn_TX_WR(hwss_io_t *io, uint8_t sn, uint16_t *txwr){
    return io->read_buf(io,0,Sn_TX_WR(sn),(uint8_t *)txwr,2);
}

inline esp_err_t getSn_RX_RSR(hwss_io_t *io, uint8_t sn, uint16_t *rsr){
    uint16_t rsr_p=0;
    uint16_t rsr_c=0;

    do{
        if(io->read_buf(io,0,Sn_RX_RSR(sn),(uint8_t *)&rsr_p,2)!=ESP_OK)
            return ESP_FAIL;
        if(rsr_p!=0){
            if(io->read_buf(io,0,Sn_RX_RSR(sn),(uint8_t *)&rsr_c,2)!=ESP_OK)
                return ESP_FAIL;
        }
    }while(rsr_p!=rsr_c);

    memcpy(rsr,&rsr_c,2);
    return ESP_OK;
}

inline esp_err_t setSn_RX_RD(hwss_io_t *io, uint8_t sn, const uint16_t *rxrd){
    return io->write_buf(io,0,Sn_RX_RD(sn),(uint8_t *)rxrd,2);
}

inline esp_err_t getSn_RX_RD(hwss_io_t *io, uint8_t sn, uint16_t *rxrd){
    return io->read_buf(io,0,Sn_RX_RD(sn),(uint8_t *)rxrd,2);
}

inline esp_err_t getSn_RX_WR(hwss_io_t *io, uint8_t sn, uint16_t *rxwr){
    return io->read_buf(io,0,Sn_RX_WR(sn),(uint8_t *)rxwr,2);
}

inline esp_err_t setSn_FRAG(hwss_io_t *io, uint8_t sn, const uint16_t *frag){
    return io->write_buf(io,0,Sn_FRAG(sn),(uint8_t *)frag, 2);
}

inline esp_err_t getSn_FRAG(hwss_io_t *io, uint8_t sn, uint16_t *frag){
    return io->read_buf(io,0,Sn_FRAG(sn),(uint8_t *)frag, 2);
}

inline esp_err_t setSn_KPALVTR(hwss_io_t *io, uint8_t sn, const uint8_t *kpalvt){
    return io->write(io,0,Sn_KPALVTR(sn),kpalvt);
}

inline esp_err_t getSn_KPALVTR(hwss_io_t *io, uint8_t sn, uint8_t *kpalvt){
    return io->read(io,0,Sn_KPALVTR(sn),kpalvt);
}

#ifdef __cplusplus
}
#endif
