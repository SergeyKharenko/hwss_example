#pragma once

#include "hwss_io.h"
#include "w5500.h"

#ifdef __cplusplus
extern "C" {
#endif

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

inline esp_err_t setIMR(hwss_io_t *io, const uint8_t imr){
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

inline esp_err_t getSIR(hwss_io_t *io, uint8_t *simr){
    return io->read(io,0,SIMR,simr);
}


#ifdef __cplusplus
}
#endif
