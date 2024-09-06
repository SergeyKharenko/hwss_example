/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2024/04/23
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include <CH394Q.h>
#include <CH394Q_NET.h>
#include <CH394Q_SPI.h>
#include "debug.h"
#include "stm32f10x.h"
/** CH394Q 引脚接线信息
******************************************************************************
* CH394Q_SCS  -> PA_04 // 片选
* CH394Q_CLK  -> PA_05 // 时钟
* CH394Q_MISO -> PA_06 // 主机输入，从机输出
* CH394Q_MOSI -> PA_07 // 主机输出，从机输入
* CH394Q_INT  -> PB_04 // 中断
* CH394Q_RST  -> PB_05 // 复位
**************************************************************************** */
#define QueryCH394QInterrupt() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
uint8_t  MyBuffer[8][2048];
uint8_t  TX_BUFF[SOCKET_NUM] = {2,2,2,2,2,2,2,2};        /* 默认发送缓存区大小  */
uint8_t  RX_BUFF[SOCKET_NUM] = {2,2,2,2,2,2,2,2};        /* 默认接收缓存区大小  */
/* CH394Q IP信息 */
uint8_t  CH394Q_MACAddr[6];                              /* CH394Q MAC */
uint8_t  CH394Q_IPAddr[4] = {192, 168, 1, 200};          /* CH394Q IP */
uint8_t  CH394Q_GWIPAddr[4] = {192, 168, 1, 1};          /* CH394Q 网关 */
uint8_t  CH394Q_IPMask[4] = {255, 255, 255, 0};          /* CH394Q 掩码 */
uint16_t CH394Q_Port = 1000;                             /* CH394Q 端口 */
/* 对端IP信息 */
uint8_t  DesIP[4] = {192, 168, 1, 100};                  /* 对端 IP */
uint16_t DesPort = 1000;                                 /* 对端端口  */
/* 组播IP信息 */
uint8_t  MultiMAC[6] = {0x01,0x00,0x5e,0x01,0x01,0x12};  /* 组播MAC*/
uint8_t  MultiIP[4] = {224,1,1,18};                      /* 组播IP */
uint16_t MultiPort = 3000;                               /* 组播端口  */
/*******************************************************************************
 * Function Name  : CH394Q_GPIO_Init
 * Description    : 复位和中断引脚初始化
 * Input          : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    /* PB_05 -> RST */
	  GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* PB_04 -> INT */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
 * Function Name  : CH394Q_ResetHW
 * Description    : CH394Q硬件复位
 * Input          : None
 * Return         : None
 *******************************************************************************/
void CH394Q_ResetHW(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    Delay_Ms(500);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    Delay_Ms(500);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    uint16_t i;
    Delay_Init();
    USART_Printf_Init(115200);
    CH394Q_GPIO_Init();
    CH394Q_SPIPort_Init();
    CH394Q_ResetHW();                              // CH394Q硬件复位
    printf("\r\n CH394Q EVT Test Demo \r\n");
    i = CH394Q_GetCHIPV();
    printf(" CH394Q CHIPV : %2x\r\n", i);
    CH394Q_PHY_Check();                            // PHY连接检测
    CH394Q_InfParam(CH394Q_IPAddr, CH394Q_IPMask , CH394Q_GWIPAddr, CH394Q_MACAddr); // 设置CH394Q网络参数
    CH394Q_SocketBuf_Init(TX_BUFF, RX_BUFF);       // 初始化8个Socket的发送接收缓存大小
    CH394Q_SetGINTE(0XFF);                         // 使能中断
    CH394Q_SetSINTE(0XFF);                         // 使能socket中断
    printf(" TCP SERVER......\r\n");
    CH394Q_TCPServerSocketInit( 0, Sn_MODE_TCP, CH394Q_Port);
    while(1)
    {
        if(QueryCH394QInterrupt() == 0)
        {
            CH394Q_GlobalInterrupt();
        }
    }
}

