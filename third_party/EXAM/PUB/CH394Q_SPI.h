/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_SPI.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/04/23
 * Description        : CH394Q SPI header file
 *******************************************************************************/
#ifndef CH394Q_SPI_H_
#define CH394Q_SPI_H_
#include "debug.h"

void    CH394Q_SPIPort_Init(void);
uint8_t SPI1_WriteRead(uint8_t data);
void    SPIWriteCH394QData(uint8_t mdata);
uint8_t SPIReadCH394QData(void);
void    SPICH394QStart(void);
void    SPIEndCH394Q(void);
#endif
