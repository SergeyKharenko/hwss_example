/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_SPI.C
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/04/23
 * Description        : Hardware abstraction layer for CH39Q SPI V1.0.0
 *******************************************************************************/
#include <CH394Q_SPI.h>
#include "stm32f10x.h"
/*******************************************************************************
 * Function Name  : CH394Q_SPIPort_Init
 * Description    : SPI端口初始化
 * Input          : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SPIPort_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    //CS -> PA4
	  GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //CLK -> PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //MISO -> PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //MOSI -> PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
 * Function Name  : SPI1_WriteRead
 * Description    : SPI读取和写一个字节
 * Input          : uint8_t Data
 * Return         : None
 *******************************************************************************/
uint8_t SPI1_WriteRead(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

/******************************************************************************
 * Function Name  : SPIWriteCH394QData
 * Description    : SPI写数据
 * Input          : uint8_t mdata
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPIWriteCH394QData(uint8_t mdata)
{
    SPI1_WriteRead(mdata);
}

/*******************************************************************************
 * Function Name  : SPIReadCH394QData
 * Description    : SPI读数据
 * Input          : None
 * Output         : None
 * Return         : uint8_t i
 *******************************************************************************/
uint8_t SPIReadCH394QData(void)
{
    uint8_t i;
    i = SPI1_WriteRead(0x00);
    return i;
}

/*******************************************************************************
 * Function Name  : SPICH394QStart
 * Description    : 片选拉低
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPICH394QStart(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
}

/*******************************************************************************
 * Function Name  : SPIEndCH394Q
 * Description    : 片选拉高
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPIEndCH394Q(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
}
