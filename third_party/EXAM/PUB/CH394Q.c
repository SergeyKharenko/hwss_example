/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/04/23
 * Description        : CH394Q application program file
 *******************************************************************************/
#include <CH394Q.h>
#include <CH394Q_SPI.h>
#include "debug.h"

extern uint8_t  TX_BUFF[];
extern uint8_t  RX_BUFF[];
/********************************************************************************
 * Function Name  : CH394Q_SetMODE
 * Description    : 设置CH394Q模式
 * Input          : mode：设置模式
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetMODE(uint8_t mode)
{
    CH394Q_Write(MODE,mode);
}

/********************************************************************************
 * Function Name  : CH394Q_SetGWIP
 * Description    : 设置CH394Q网关地址
 * Input          : gwip：网关地址
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetGWIP(uint8_t * gwip )
{
    CH394Q_Write(GWIP0,gwip[0]);
    CH394Q_Write(GWIP1,gwip[1]);
    CH394Q_Write(GWIP2,gwip[2]);
    CH394Q_Write(GWIP3,gwip[3]);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSMIP
 * Description    : 设置CH394Q子网掩码地址
 * Input          : smip：子网掩码地址
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSMIP(uint8_t * smip)
{
    CH394Q_Write(SMIP0,smip[0]);
    CH394Q_Write(SMIP1,smip[1]);
    CH394Q_Write(SMIP2,smip[2]);
    CH394Q_Write(SMIP3,smip[3]);
}

/********************************************************************************
 * Function Name  : CH394Q_SetMAC
 * Description    : 设置CH394Q的MAC地址
 * Input          : mac:MAC地址
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetMAC(uint8_t * mac)
{
    CH394Q_Write(MAC0,mac[0]);
    CH394Q_Write(MAC1,mac[1]);
    CH394Q_Write(MAC2,mac[2]);
    CH394Q_Write(MAC3,mac[3]);
    CH394Q_Write(MAC4,mac[4]);
    CH394Q_Write(MAC5,mac[5]);
}

/********************************************************************************
 * Function Name  : CH394Q_SetIP
 * Description    : 设置CH394Q源IP地址
 * Input          : ip：IP地址
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetIP(uint8_t * ip)
{
    CH394Q_Write(IP0,ip[0]);
    CH394Q_Write(IP1,ip[1]);
    CH394Q_Write(IP2,ip[2]);
    CH394Q_Write(IP3,ip[3]);
}

/********************************************************************************
 * Function Name  : CH394Q_SetIIT
 * Description    : 中断间隔时间寄存器
 * Input          : iit：等待时间
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetIIT(uint16_t iit)
{
    CH394Q_Write(IIT0,(uint8_t)((iit & 0xff00) >> 8));
    CH394Q_Write(IIT1,(uint8_t)(iit & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetGINT
 * Description    : 清除全局中断标志
 * Input          : gint：清除某个全局中断标志
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetGINT(uint8_t gint)
{
    CH394Q_Write(GINT,gint);
}

/********************************************************************************
 * Function Name  : CH394Q_SetGINTE
 * Description    : 使能全局中断
 * Input          : ginte：使能某个全局中断
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetGINTE(uint8_t ginte)
{
    CH394Q_Write(GINTE,ginte);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSINTE
 * Description    : 使能socket中断
 * Input          : sinte：使能某个socket中断
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSINTE(uint8_t sinte)
{
    CH394Q_Write(SINTE,sinte);
}

/********************************************************************************
 * Function Name  : CH394Q_SetRTIME
 * Description    : 设置TCP重传时间
 * Input          : rtime：TCP重传时间，单位时间为100us
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetRTIME(uint16_t rtime)
{
    CH394Q_Write(RTIME0,(uint8_t)((rtime & 0xff00) >> 8));
    CH394Q_Write(RTIME1,(uint8_t)(rtime & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetRCNT
 * Description    : 设置TCP重传次数
 * Input          : rcnt：TCP重传次数
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetRCNT(uint8_t rcnt)
{
    CH394Q_Write(RCNT,rcnt);
}

/********************************************************************************
 * Function Name  : CH394Q_SetPHY_CFG
 * Description    : 设置PHY配置
 * Input          : phycfg：相关配置模式
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetPHY_CFG(uint8_t phycfg)
{
    CH394Q_Write(PHY_CFG,phycfg);
    Delay_Ms(2);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_MODE
 * Description    : 设置socket模式
 * Input          : sockindex：socket number
                    mode：模式
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_MODE(uint8_t sockindex, uint8_t mode)
{
    CH394Q_Write(Sn_MODE(sockindex), mode);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_CTRL
 * Description    : 设置socket控制命令
 * Input          : sockindex：socket number
                    ctrl：控制命令
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_CTRL(uint8_t sockindex, uint8_t ctrl)
{
    CH394Q_Write(Sn_CTRL(sockindex), ctrl);
    Delay_Us(2);//控制命令需执行时间
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_INT
 * Description    : 清除socket n 的中断标志
 * Input          : sockindex：socket number
                    val：清除socket n 的某个中断标志
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_INT(uint8_t sockindex, uint8_t val)
{
    CH394Q_Write(Sn_INT(sockindex), val);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_PORT
 * Description    : 设置socket本地端口
 * Input          : sockindex：socket number
                    port：本地端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_PORT(uint8_t sockindex, uint16_t port )
{
    CH394Q_Write( Sn_PORT0(sockindex), (uint8_t)((port & 0xff00) >> 8));
    CH394Q_Write( Sn_PORT1(sockindex), (uint8_t)(port & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_DMAC
 * Description    : 设置socket目的MAC地址
 * Input          : sockindex：socket number
                    dmac：目的MAC地址
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_DMAC(uint8_t sockindex, uint8_t * dmac)
{
    CH394Q_Write(Sn_DMAC0(sockindex),dmac[0]);
    CH394Q_Write(Sn_DMAC1(sockindex),dmac[1]);
    CH394Q_Write(Sn_DMAC2(sockindex),dmac[2]);
    CH394Q_Write(Sn_DMAC3(sockindex),dmac[3]);
    CH394Q_Write(Sn_DMAC4(sockindex),dmac[4]);
    CH394Q_Write(Sn_DMAC5(sockindex),dmac[5]);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_DIP
 * Description    : 设置socket目的IP地址
 * Input          : sockindex：socket number
                    dip：目的IP地址
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_DIP(uint8_t sockindex, uint8_t * dip)
{
    CH394Q_Write(Sn_DIP0(sockindex),dip[0]);
    CH394Q_Write(Sn_DIP1(sockindex),dip[1]);
    CH394Q_Write(Sn_DIP2(sockindex),dip[2]);
    CH394Q_Write(Sn_DIP3(sockindex),dip[3]);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_DPORT
 * Description    : 设置socket目的端口
 * Input          : sockindex：socket number
                    dport：目的端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_DPORT(uint8_t sockindex, uint16_t dport )
{
    CH394Q_Write( Sn_DPORT0(sockindex), (uint8_t)((dport & 0xff00) >> 8));
    CH394Q_Write( Sn_DPORT1(sockindex), (uint8_t)(dport & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_MTU
 * Description    : 设置socket最大分段寄存器
 * Input          : sockindex：socket number
                    mtu：最大分段大小
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_MTU(uint8_t sockindex, uint16_t mtu)
{
    CH394Q_Write( Sn_MTU0(sockindex), (uint8_t)((mtu & 0xff00) >> 8));
    CH394Q_Write( Sn_MTU1(sockindex), (uint8_t)(mtu & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TOS
 * Description    : 设置socketIP服务类型
 * Input          : sockindex：socket number
                    tos：服务类型
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TOS(uint8_t sockindex, uint8_t tos)
{
    CH394Q_Write(Sn_TOS(sockindex), tos);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TTL
 * Description    : 设置socket IP生存时间
 * Input          : sockindex：socket number
                    ttl：生存时间
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TTL(uint8_t sockindex, uint8_t ttl)
{
    CH394Q_Write( Sn_TTL(sockindex) , ttl);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_RXBUF_SIZE
 * Description    : 设置socket接收缓存区大小
 * Input          : sockindex：socket number
                    rxbuf：接收缓存区大小
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_RXBUF_SIZE(uint8_t sockindex, uint8_t rxbuf)
{
    CH394Q_Write( Sn_RXBUF_SIZE(sockindex) , rxbuf);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TXBUF_SIZE
 * Description    : 设置socket发送缓存器大小
 * Input          : sockindex：socket number
                    txbuf：发送缓存器大小
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TXBUF_SIZE(uint8_t sockindex, uint8_t txbuf)
{
    CH394Q_Write( Sn_TXBUF_SIZE(sockindex) , txbuf);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TX_WR
 * Description    : 设置socket发送写指针
 * Input          : sockindex：socket number
                    txwr：发送数据长度
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TX_WR(uint8_t sockindex, uint16_t txwr)
{
    CH394Q_Write( Sn_TX_WR0(sockindex), (uint8_t)((txwr & 0xff00) >> 8));
    CH394Q_Write( Sn_TX_WR1(sockindex), (uint8_t)(txwr & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_RX_RD
 * Description    : 设置socket接收读指针
 * Input          : sockindex：socket number
                    rxrd：接收数据长度
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_RX_RD(uint8_t sockindex, uint16_t rxrd)
{
    CH394Q_Write( Sn_RX_RD0(sockindex), (uint8_t)((rxrd & 0xff00) >> 8));
    CH394Q_Write( Sn_RX_RD1(sockindex), (uint8_t)(rxrd & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_INTE
 * Description    : 使能socket中断
 * Input          : sockindex：socket number
                    inte：使能socket某个中断
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_INTE(uint8_t sockindex, uint8_t inte)
{
    CH394Q_Write( Sn_INTE(sockindex) , inte);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_IPF
 * Description    : 设置socketIP层中IP报头的分段
 * Input          : sockindex：socket number
                    ipf：IP分段
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_IPF(uint8_t sockindex, uint16_t ipf)
{
    CH394Q_Write( Sn_IPF0(sockindex), (uint8_t)((ipf & 0xff00) >> 8));
    CH394Q_Write( Sn_IPF1(sockindex), (uint8_t)(ipf & 0x00ff));
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_KEEPALIVE
 * Description    : 设置KEEPALIVE时间间隔
 * Input          : sockindex：socket number
                    val：时间间隔(单位时间为5s)
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_KEEPALIVE(uint8_t sockindex,uint8_t val)
{
    CH394Q_Write(Sn_KEEPALIVE(sockindex),val);
}

/********************************************************************************
 * Function Name  : CH394Q_GetMODE
 * Description    : 读取MODE模式
 * Input          : None
 * Output         : None
 * Return         : MODE模式
 *******************************************************************************/
uint8_t CH394Q_GetMODE( void )
{
    return CH394Q_Read(MODE);
}

/********************************************************************************
 * Function Name  : CH394Q_GetGWIP
 * Description    : 读取CH394Q网关地址
 * Input          : gwip：网关地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetGWIP(uint8_t * gwip)
{
    gwip[0] = CH394Q_Read(GWIP0);
    gwip[1] = CH394Q_Read(GWIP1);
    gwip[2] = CH394Q_Read(GWIP2);
    gwip[3] = CH394Q_Read(GWIP3);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSMIP
 * Description    : 读取CH394Q子网掩码地址
 * Input          : smip：子网掩码地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetSMIP(uint8_t * smip)
{
    smip[0] = CH394Q_Read(SMIP0);
    smip[1] = CH394Q_Read(SMIP1);
    smip[2] = CH394Q_Read(SMIP2);
    smip[3] = CH394Q_Read(SMIP3);
}

/********************************************************************************
 * Function Name  : CH394Q_GetMAC
 * Description    : 读取CH394Q MAC地址
 * Input          : mac：MAC地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetMAC(uint8_t * mac)
{
    mac[0] = CH394Q_Read(MAC0);
    mac[1] = CH394Q_Read(MAC1);
    mac[2] = CH394Q_Read(MAC2);
    mac[3] = CH394Q_Read(MAC3);
    mac[4] = CH394Q_Read(MAC4);
    mac[5] = CH394Q_Read(MAC5);
}

/********************************************************************************
 * Function Name  : CH394Q_GetIP
 * Description    : 读取CH394Q源IP地址
 * Input          : ip：源IP地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetIP(uint8_t * ip)
{
    ip[0] = CH394Q_Read(IP0);
    ip[1] = CH394Q_Read(IP1);
    ip[2] = CH394Q_Read(IP2);
    ip[3] = CH394Q_Read(IP3);
}

/********************************************************************************
 * Function Name  : CH394Q_GetIIT
 * Description    : 读取低电平中断定时时间
 * Input          : None
 * Output         : None
 * Return         : 低电平中断定时时间
 *******************************************************************************/
uint16_t CH394Q_GetIIT(void)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(IIT0) << 8) & 0xff00;
    i |= CH394Q_Read(IIT1) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetGINT
 * Description    : 读取全局中断状态
 * Input          : None
 * Output         : None
 * Return         : 全局中断状态
 *******************************************************************************/
uint8_t CH394Q_GetGINT( void )
{
    return CH394Q_Read(GINT);
}

/********************************************************************************
 * Function Name  : CH394Q_GetGINTE
 * Description    : 读取使能中断状态
 * Input          : None
 * Output         : None
 * Return         : 使能中断状态
 *******************************************************************************/
uint8_t CH394Q_GetGINTE( void )
{
    return CH394Q_Read(GINTE);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSINT
 * Description    : 读取socket中断状态
 * Input          : None
 * Output         : None
 * Return         : socket中断状态
 *******************************************************************************/
uint8_t CH394Q_GetSINT( void )
{
    return CH394Q_Read(SINT);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSINTE
 * Description    : 读取使能socket中断状态
 * Input          : None
 * Output         : None
 * Return         : 使能socket中断状态
 *******************************************************************************/
uint8_t CH394Q_GetSINTE( void )
{
    return CH394Q_Read(SINTE);
}

/********************************************************************************
 * Function Name  : CH394Q_GetRTIME
 * Description    : 读取重传超时时间（单位时间为100us）
 * Input          : None
 * Output         : None
 * Return         : 重传超时时间
 *******************************************************************************/
uint16_t CH394Q_GetRTIME(void)
{
    uint16_t i = 0;
    i = (CH394Q_Read(RTIME0) << 8) & 0xff00;
    i |= CH394Q_Read(RTIME1) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetRCNT
 * Description    : 读取重传次数
 * Input          : None
 * Output         : None
 * Return         : 重传次数
 *******************************************************************************/
uint8_t CH394Q_GetRCNT( void )
{
    return CH394Q_Read(RCNT);
}

/********************************************************************************
 * Function Name  : CH394Q_GetUNIP
 * Description    : 读取无法抵达IP地址
 * Input          : unip：无法抵达IP地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetUNIP(uint8_t * unip)
{
    unip[0] = CH394Q_Read(UNIP0);
    unip[1] = CH394Q_Read(UNIP1);
    unip[2] = CH394Q_Read(UNIP2);
    unip[3] = CH394Q_Read(UNIP3);
}

/********************************************************************************
 * Function Name  : CH394Q_GetUNPORT
 * Description    : 读取无法抵达端口
 * Input          : None
 * Output         : None
 * Return         : 无法抵达端口
 *******************************************************************************/
uint16_t CH394Q_GetUNPORT( void )
{
    uint16_t i = 0;
    i |= (CH394Q_Read(UNPORT0) << 8) & 0xff00;
    i |= CH394Q_Read(UNPORT1) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetPHYStatus
 * Description    : 读取PHY状态
 * Input          : None
 * Output         : None
 * Return         : PHY状态
 *******************************************************************************/
uint8_t CH394Q_GetPHYStatus( void )
{
    return CH394Q_Read(PHY_CFG);
}

/********************************************************************************
 * Function Name  : CH394Q_GetCHIPV
 * Description    : 读取CH394Q版本号
 * Input          : None
 * Output         : None
 * Return         : 版本号
 *******************************************************************************/
uint8_t CH394Q_GetCHIPV(void)
{
    return CH394Q_Read(CHIPV);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_MODE
 * Description    : 读取socket模式
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket模式
 *******************************************************************************/
uint8_t CH394Q_GetSn_MODE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_MODE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_CTRL
 * Description    : 读取socket控制命令状态
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket控制命令状态
 *******************************************************************************/
uint8_t CH394Q_GetSn_CTRL(uint8_t sockindex)
{
    return CH394Q_Read(Sn_CTRL(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_INT
 * Description    : 读取socket n 的中断状态
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket n 的中断状态
 *******************************************************************************/
uint8_t CH394Q_GetSn_INT(uint8_t sockindex)
{
    return CH394Q_Read(Sn_INT(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_STA
 * Description    : 读取socket的状态
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket的状态
 *******************************************************************************/
uint8_t CH394Q_GetSn_STA(uint8_t sockindex)
{
    return CH394Q_Read(Sn_STA(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_PORT
 * Description    : 读取socket本地端口
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket本地端口
 *******************************************************************************/
uint16_t CH394Q_GetSn_PORT(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_PORT0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_PORT1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_DMAC
 * Description    : 读取socket目的MAC地址
 * Input          : sockindex：socket number
                    dmac：目的MAC地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetSn_DMAC(uint8_t sockindex, uint8_t * dmac)
{
    dmac[0] = CH394Q_Read(Sn_DMAC0(sockindex));
    dmac[1] = CH394Q_Read(Sn_DMAC1(sockindex));
    dmac[2] = CH394Q_Read(Sn_DMAC2(sockindex));
    dmac[3] = CH394Q_Read(Sn_DMAC3(sockindex));
    dmac[4] = CH394Q_Read(Sn_DMAC4(sockindex));
    dmac[5] = CH394Q_Read(Sn_DMAC5(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_DIP
 * Description    : 读取socket目的IP
 * Input          : sockindex：socket number
                    dip：目的IP地址指针
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GetSn_DIP(uint8_t sockindex, uint8_t * dip)
{
    dip[0] = CH394Q_Read(Sn_DIP0(sockindex));
    dip[1] = CH394Q_Read(Sn_DIP1(sockindex));
    dip[2] = CH394Q_Read(Sn_DIP2(sockindex));
    dip[3] = CH394Q_Read(Sn_DIP3(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_DPORT
 * Description    : 读取socket目的端口
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket目的端口
 *******************************************************************************/
uint16_t CH394Q_GetSn_DPORT(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_DPORT0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_DPORT1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_MTU
 * Description    : 读取socket最大分段
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket最大分段
 *******************************************************************************/
uint16_t CH394Q_GetSn_MTU(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_MTU0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_MTU1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TOS
 * Description    : 读取socket服务类型
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket服务类型
 *******************************************************************************/
uint8_t CH394Q_GetSn_TOS(uint8_t sockindex)
{
    return CH394Q_Read(Sn_TOS(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TTL
 * Description    : 读取socket IP生存时间
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket IP生存时间
 *******************************************************************************/
uint8_t CH394Q_GetSn_TTL(uint8_t sockindex)
{
    return CH394Q_Read(Sn_TTL(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_RXBUF_SIZE
 * Description    : 读取socket接收缓存大小
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket接收缓存大小
 *******************************************************************************/
uint8_t CH394Q_GetSn_RXBUF_SIZE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_RXBUF_SIZE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TXBUF_SIZE
 * Description    : 读取socket发送缓存大小
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket发送缓存大小
 *******************************************************************************/
uint8_t CH394Q_GetSn_TXBUF_SIZE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_TXBUF_SIZE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TX_FS
 * Description    : 读取socket空闲发送缓存
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket空闲发送缓存
 *******************************************************************************/
uint16_t CH394Q_GetSn_TX_FS(uint8_t sockindex)
{
    uint16_t i = 0, t = 0;
    do
    {
        t = (CH394Q_Read(Sn_TX_FS0(sockindex)) << 8) & 0xff00;
        t |= CH394Q_Read(Sn_TX_FS1(sockindex)) & 0x00ff;
        if (t != 0)
        {
            i = (CH394Q_Read(Sn_TX_FS0(sockindex)) << 8) & 0xff00;
            i |= CH394Q_Read(Sn_TX_FS1(sockindex)) & 0x00ff;
        }
    }while (i != t);
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_RX_RS
 * Description    : 读取socket空闲接收缓存
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket空闲接收缓存
 *******************************************************************************/
uint16_t CH394Q_GetSn_RX_RS(uint8_t sockindex)
{
    uint16_t i = 0, t = 0;
    do
    {
        t = (CH394Q_Read(Sn_RX_RS0(sockindex)) << 8) & 0xff00;
        t |= CH394Q_Read(Sn_RX_RS1(sockindex)) & 0x00ff;
        if (t != 0)
        {
            i = (CH394Q_Read(Sn_RX_RS0(sockindex)) << 8) & 0xff00;
            i |= CH394Q_Read(Sn_RX_RS1(sockindex)) & 0x00ff;
        }
    }while (i != t);
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TX_RD
 * Description    : 读取socket发送读指针
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket发送读指针
 *******************************************************************************/
uint16_t CH394Q_GetSn_TX_RD(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_TX_RD0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_TX_RD1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TX_WR
 * Description    : 读取socket发送写指针
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket发送写指针
 *******************************************************************************/
uint16_t CH394Q_GetSn_TX_WR(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_TX_WR0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_TX_WR1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_RX_RD
 * Description    : 读取socket接收读指针
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket接收读指针
 *******************************************************************************/
uint16_t CH394Q_GetSn_RX_RD(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_RX_RD0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_RX_RD1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_RX_WR
 * Description    : 读取socket接收写指针
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket接收写指针
 *******************************************************************************/
uint16_t CH394Q_GetSn_RX_WR(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_RX_WR0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_RX_WR1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_INTE
 * Description    : 读取socket n 使能中断状态
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket n 使能中断状态
 *******************************************************************************/
uint8_t CH394Q_GetSn_INTE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_INTE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_IPF
 * Description    : 读取socket分段状态
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket分段状态
 *******************************************************************************/
uint16_t CH394Q_GetSn_IPF(uint8_t sockindex)
{
    uint16_t i = 0;
    i |= (CH394Q_Read(Sn_IPF0(sockindex)) << 8) & 0xff00;
    i |= CH394Q_Read(Sn_IPF1(sockindex)) & 0x00ff;
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_KEEPALIVE
 * Description    : 读取socket心跳包传输时间（单位时间为5s）
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket心跳包传输时间
 *******************************************************************************/
uint8_t CH394Q_GetSn_KEEPALIVE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_KEEPALIVE(sockindex));
}

/******************************************************************************
 * Function Name  : CH394Q_Write
 * Description    : 写入一个字节数据到CH394Q
 * Input          : addr:写入数据的地址
                    data:写入的8位数据
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Write(uint32_t addr,uint8_t data)
{
    SPICH394QStart();
    SPIWriteCH394QData( (addr & 0x00ff0000)>>16);
    SPIWriteCH394QData( (addr & 0x0000ff00)>> 8);
    SPIWriteCH394QData( (addr & 0x000000f8) + 4);
    SPIWriteCH394QData(data);
    SPIEndCH394Q();
}

/******************************************************************************
 * Function Name  : CH394Q_Read
 * Description    : 从CH394Q读一个字节数据
 * Input          : addr:读取数据的地址
 * Output         : None
 * Return         : data
 *******************************************************************************/
uint8_t CH394Q_Read(uint32_t addr)
{
    uint8_t data = 0;
    SPICH394QStart();
    SPIWriteCH394QData( (addr & 0x00ff0000)>>16);
    SPIWriteCH394QData( (addr & 0x0000ff00)>> 8);
    SPIWriteCH394QData( (addr & 0x000000f8))    ;
    data = SPIReadCH394QData();
    SPIEndCH394Q();
    return data;
}

/******************************************************************************
 * Function Name  : CH394Q_WriteBuf
 * Description    : 写入len字节数据到CH394Q
 * Input          : addr:写入数据的地址
                    buf:写入字符串
                    len：长度
 * Output         : None
 * Return         : len
 *******************************************************************************/
uint16_t CH394Q_WriteBuf(uint32_t addr,uint8_t* buf,uint16_t len)
{
    uint16_t i = 0;
    SPICH394QStart();
    SPIWriteCH394QData( (addr & 0x00ff0000)>>16);
    SPIWriteCH394QData( (addr & 0x0000ff00)>> 8);
    SPIWriteCH394QData( (addr & 0x000000f8) + 4);
    for(i = 0; i < len; i++)
    {
        while((SPI1->SR & SPI_I2S_FLAG_TXE) == 0);
        SPI1->DR = buf[i];
        while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);
        (void)(SPI1->DR);
    }
    SPIEndCH394Q();
    return len;
}

/******************************************************************************
 * Function Name  : CH394Q_ReadBuf
 * Description    : 从CH394Q读len字节数据
 * Input          : addr:读取数据的地址
                    buf:存放读取数据
                    len：长度
 * Output         : None
 * Return         : len
 *******************************************************************************/
uint16_t CH394Q_ReadBuf(uint32_t addr, uint8_t* buf,uint16_t len)
{
    uint16_t i = 0;
    SPICH394QStart();
    SPIWriteCH394QData( (addr & 0x00ff0000)>>16);
    SPIWriteCH394QData( (addr & 0x0000ff00)>> 8);
    SPIWriteCH394QData( (addr & 0x000000f8));
    for(i = 0; i < len; i++)
    {
        while((SPI1->SR & SPI_I2S_FLAG_TXE) == 0);
        SPI1->DR = 0;
        while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);
        buf[i] = SPI1->DR;
    }
    SPIEndCH394Q();
    return len;
}

/********************************************************************************
 * Function Name  : CH394Q_SocketSendData
 * Description    : socket发送数据
 * Input          : sockindex:socket number
                    buf:发送数据缓存区
                    len：数据长度
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SocketSendData(uint8_t sockindex, uint8_t *buf, uint16_t len)
{
    uint16_t i =0;
    uint32_t address =0;
    i = CH394Q_GetSn_TX_WR(sockindex);
    address = (uint32_t)(i<<8) + (sockindex<<5) + 0x10;
    CH394Q_WriteBuf(address, buf, len);
    i += len;
    CH394Q_SetSn_TX_WR(sockindex, i);
}

/********************************************************************************
 * Function Name  : CH394Q_SocketRecvData
 * Description    : socket接收数据
 * Input          : sockindex:socket number
                    buf:接收数据缓存区
                    len：数据长度
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SocketRecvData(uint8_t sockindex, uint8_t *buf, uint16_t len)
{
    uint16_t i = 0;
    uint32_t address = 0;
    i = CH394Q_GetSn_RX_RD(sockindex);
    address = (uint32_t)(i<<8) + (sockindex<<5) + 0x18;
    CH394Q_ReadBuf(address, buf, len);
    i += len;
    CH394Q_SetSn_RX_RD(sockindex, i);
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_RECV);
    while( CH394Q_GetSn_CTRL(sockindex) );
}

/********************************************************************************
 * Function Name  : CH394Q_SocketTCPSend
 * Description    : TCP模式发送数据
 * Input          : sockindex：socket number
                    buf: 发送数据缓存区
                    len：数据长度
 * Output         : None
 * Return         : 成功返回数据长度，失败返回0
 *******************************************************************************/
uint16_t CH394Q_SocketTCPSend(uint8_t sockindex, const uint8_t * buf, uint16_t len)
{
    uint16_t i = 0;
    uint16_t ferrbuf = 0;
    uint8_t  Sn_STA_state = CH394Q_GetSn_STA(sockindex);
    if (len > CH394Q_GetTxMax(sockindex))
    {
        i = CH394Q_GetTxMax(sockindex);
    }
    else i = len;
    while (ferrbuf < i)
    {
        ferrbuf = CH394Q_GetSn_TX_FS(sockindex);
        Sn_STA_state =  CH394Q_GetSn_STA(sockindex);
    }
    CH394Q_SocketSendData(sockindex, (uint8_t *)buf, i);
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_SEND);
    while( CH394Q_GetSn_CTRL(sockindex) );
    while ( (CH394Q_GetSn_INT(sockindex) & Sn_INT_SEND_SUC) != Sn_INT_SEND_SUC )
    {
        Sn_STA_state = CH394Q_GetSn_STA(sockindex);
        if ((Sn_STA_state != SOCK_TCP_ESTABLISHED) && (Sn_STA_state != SOCK_CLOSE_WAIT) )
        {
            printf(" Sn_STA_state :%x\r\n",Sn_STA_state);
            printf(" send err\r\n");
            return 0;
        }
    }
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_SocketTCPRecv
 * Description    : TCP模式接收数据
 * Input          : sockindex：socket number
                    buf: 接收数据缓存区
                    len：数据长度
 * Output         : None
 * Return         : 返回数据长度
 *******************************************************************************/
uint16_t CH394Q_SocketTCPRecv(uint8_t sockindex, uint8_t * buf, uint16_t len)
{
    uint16_t i = 0;
    if ( len > 0 )
    {
        CH394Q_SocketRecvData(sockindex, buf, len);
        i = len;
    }
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_SocketUDPSendTo
 * Description    : UDP模式发送数据
 * Input          : sockindex：socket number
                    buf：发送数据缓存区
                    len：数据长度
                    desip：对端IP地址
                    desport：对端端口
 * Output         : None
 * Return         : 成功返回数据长度，失败返回0
 *******************************************************************************/
uint16_t CH394Q_SocketSendTo(uint8_t sockindex, const uint8_t * buf, uint16_t len, uint8_t * desip, uint16_t desport)
{
    uint16_t i = 0;
    if (len > CH394Q_GetTxMax(sockindex))
    {
        i = CH394Q_GetTxMax(sockindex);
    }
    else i = len;
    CH394Q_SetSn_DIP(sockindex, desip);
    CH394Q_SetSn_DPORT(sockindex, desport);
    CH394Q_SocketSendData(sockindex, (uint8_t *)buf, i);
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_SEND);
    while(  CH394Q_GetSn_CTRL(sockindex)  ) ;
    while( ( CH394Q_GetSn_INT(sockindex)  & Sn_INT_SEND_SUC) != Sn_INT_SEND_SUC )
    {
        CH394Q_SetSn_INT( sockindex, (Sn_INT_SEND_SUC));
        return 0;
    }
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_UDP_Recv
 * Description    : UDP模式接收数据
 * Input          : sockindex：socket number
                    buf: 接收数据缓存区
                    len：数据长度
                    desip：对端IP地址
                    desport：对端端口
 * Output         : None
 * Return         : 返回数据长度
 *******************************************************************************/
uint16_t CH394Q_Socket_UDP_Recv(uint8_t sockindex, uint8_t * buf, uint16_t len, uint8_t * desip, uint16_t *desport)
{
    uint8_t msg[8];
    uint16_t data_len = 0;
    if ( len > 0 )
    {
        CH394Q_SocketRecvData( sockindex,  msg,  0x08);
        desip[0] = msg[0];
        desip[1] = msg[1];
        desip[2] = msg[2];
        desip[3] = msg[3];
        *desport = msg[4];
        *desport = (*desport << 8) + msg[5];
        data_len = msg[6];
        data_len = (data_len << 8) + msg[7];
        CH394Q_SocketRecvData( sockindex,  buf,  data_len);
    }
    return data_len;
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_MACRAW_Send
 * Description    : MACRAW模式发送数据
 * Input          : sockindex：socket number
                    buf:发送数据缓存区
                    len：数据长度
 * Output         : None
 * Return         : 成功返回数据长度，失败返回0
 *******************************************************************************/
uint16_t CH394Q_Socket_MACRAW_Send(uint8_t sockindex, const uint8_t * buf, uint16_t len)
{
    uint16_t i = 0;
    if (len > CH394Q_GetTxMax(sockindex))
    {
        i = CH394Q_GetTxMax(sockindex);
    }
    else i = len;
    CH394Q_SocketSendData(sockindex, (uint8_t *)buf, i);
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_SEND);
    while(  CH394Q_GetSn_CTRL(sockindex)  ) ;
    while( ( CH394Q_GetSn_INT(sockindex)  & Sn_INT_SEND_SUC) != Sn_INT_SEND_SUC )
    {
        CH394Q_SetSn_INT( sockindex, (Sn_INT_SEND_SUC));
        return 0;
    }
    return i;
}
/********************************************************************************
 * Function Name  : CH394Q_Socket_MACRAW_Recv
 * Description    : MACRAW模式接收数据
 * Input          : sockindex：socket number
                    buf: 接收数据缓存区
                    len：数据长度
 * Output         : None
 * Return         : 接收数据成功返回数据长度，失败返回0
 *******************************************************************************/
uint16_t CH394Q_Socket_MACRAW_Recv(uint8_t sockindex, uint8_t * buf, uint16_t len)
{
    uint8_t msg[2];
    uint16_t data_len = 0;
    uint8_t socket_mode = CH394Q_GetSn_MODE(sockindex);
    if ( len > 0 )
    {
        if (socket_mode & Sn_MODE_MACRAW)
        {
            CH394Q_SocketRecvData( sockindex,  msg,  0x02);
            data_len = msg[0];
            data_len = (data_len << 8) + msg[1] - 2;
            if (data_len > 1514)
            {
                CH394Q_SocketRecvData( sockindex,  buf,  len - 2);
                printf(" The received data length is more than 1514\r\n");
                return 0;
            }
            CH394Q_SocketRecvData( sockindex,  buf,  data_len);
        }
    }
    return data_len;
}

/********************************************************************************
 * Function Name  : CH394Q_SocketSendMAC
 * Description    : 使用send_mac指令时发送数据
 * Input          : sockindex：socket number
                    buf: 发送数据缓存区
                    len：数据长度
                    desmac：对端mac地址
                    desip：对端IP地址
                    desport：对端端口
 * Output         : None
 * Return         : 成功返回数据长度，失败返回0
 *******************************************************************************/
uint16_t CH394Q_SocketSendMAC(uint8_t sockindex, const uint8_t * buf, uint16_t len, uint8_t * desmac, uint8_t * desip, uint16_t desport)
{
    uint16_t i = 0;
    if (len > CH394Q_GetTxMax(sockindex))
    {
        i = CH394Q_GetTxMax(sockindex);
    }
    else i = len;
    CH394Q_SetSn_DMAC(sockindex, desmac);
    CH394Q_SetSn_DIP(sockindex, desip);
    CH394Q_SetSn_DPORT(sockindex, desport);
    CH394Q_SocketSendData(sockindex, (uint8_t *)buf, i);
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_SEND_MAC);
    while(  CH394Q_GetSn_CTRL(sockindex)  ) ;
    while( ( CH394Q_GetSn_INT(sockindex)  & Sn_INT_SEND_SUC) != Sn_INT_SEND_SUC )
    {
        CH394Q_SetSn_INT( sockindex, (Sn_INT_SEND_SUC));
        return 0;
    }
    return i;
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_Init
 * Description    : CH394Q socket初始化
 * Input          : sockindex：socket number
                    mode：socket模式
                    port：本地端口
 * Output         : None
 * Return         : None
 *******************************************************************************/

void CH394Q_Socket_Init(uint8_t sockindex, uint8_t mode, uint16_t port)
{
    uint16_t t;
    CH394Q_Socket_Close(sockindex);
    CH394Q_SetSn_MODE(sockindex, mode );
    if (port != 0)
    {
        CH394Q_SetSn_PORT(sockindex, port);
    } else
    {
        t = 8000 + sockindex;
        CH394Q_SetSn_PORT(sockindex, t );
    }
    CH394Q_Socket_Open(sockindex);
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_Open
 * Description    : 打开socket
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Socket_Open(uint8_t sockindex)
{
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_OPEN);
    while( CH394Q_GetSn_CTRL(sockindex) );
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_Close
 * Description    : 关闭socket
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Socket_Close(uint8_t sockindex)
{
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_CLOSE);
    while( CH394Q_GetSn_CTRL(sockindex) );
    CH394Q_SetSn_INT(sockindex, 0xFF);
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_Listen
 * Description    : 打开监听
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Socket_Listen(uint8_t sockindex)
{
    if (CH394Q_GetSn_STA(sockindex) == SOCK_INIT)
    {
        CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_LISTEN);
        while( CH394Q_GetSn_CTRL(sockindex) );
    }
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_Connect
 * Description    : 建立连接
 * Input          : sockindex：socket number
                    desip：对端服务器地址
                    desport：对端服务器端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Socket_Connect(uint8_t sockindex, uint8_t * desip, uint16_t desport)
{
    CH394Q_SetSn_DIP(sockindex, desip);
    CH394Q_SetSn_DPORT(sockindex, desport);
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_CONNECT);
    while ( CH394Q_GetSn_CTRL(sockindex) ) ;
}

/********************************************************************************
 * Function Name  : CH394Q_Socket_Disconnect
 * Description    : socket断开连接
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Socket_Disconnect(uint8_t sockindex)
{
    CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_DISCONNECT);
    while( CH394Q_GetSn_CTRL(sockindex) );
}

/********************************************************************************
 * Function Name  : CH394Q_SocketBuf_Init
 * Description    : 初始化socket接收和发送缓存区大小
 * Input          : tx_size:发送缓存区大小设置
                    rx_size:接收缓存区大小设置
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SocketBuf_Init( uint8_t * tx_size, uint8_t * rx_size  )
{
    uint16_t i;
    for (i = 0 ; i < SOCKET_NUM; i++)
    {
        CH394Q_SetSn_TXBUF_SIZE(i, tx_size[i]);
        CH394Q_SetSn_RXBUF_SIZE(i, rx_size[i]);
    }
    Delay_Ms(1);
}

/********************************************************************************
 * Function Name  : CH394Q_GetRxMax
 * Description    : 读取socket n 最大接收缓存
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : 该socket n 最大接收缓存
 *******************************************************************************/
uint16_t CH394Q_GetRxMax(uint8_t sockindex)
{
    return RX_BUFF[sockindex]*1024;
}

/********************************************************************************
 * Function Name  : CH394Q_GetTxMax
 * Description    : 读取socket n 最大发送缓存
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : socket n 最大发送缓存
 *******************************************************************************/
uint16_t CH394Q_GetTxMax(uint8_t sockindex)
{
    return TX_BUFF[sockindex]*1024;
}

/********************************************************************************
 * Function Name  : CH394Q_InfParam
 * Description    : 配置CH394Q网络参数
 * Input          : ip:本地IP
                    smip:本地掩码
                    gwip：本地网关
                    mac：本地MAC
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_InfParam(uint8_t * ip,uint8_t * smip,uint8_t * gwip ,uint8_t * mac)
{
    CH394Q_SetIP(ip);
    CH394Q_SetSMIP(smip);
    CH394Q_SetGWIP(gwip);
    CH394Q_GetMAC(mac);
    printf(" CH394Q MAC地址  : %02x.%02x.%02x.%02x.%02x.%02x\r\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    CH394Q_GetIP (ip);
    printf(" CH394Q IP地址   : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
    CH394Q_GetSMIP(smip);
    printf(" CH394Q 子网掩码 : %d.%d.%d.%d\r\n", smip[0],smip[1],smip[2],smip[3]);
    CH394Q_GetGWIP(gwip);
    printf(" CH394Q 网关     : %d.%d.%d.%d\r\n", gwip[0],gwip[1],gwip[2],gwip[3]);
}

/********************************************************************************
 * Function Name  : CH394Q_PHY_Check
 * Description    : PHY连接检测
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_PHY_Check(void)
{
    uint8_t PHY_state = 0;
    while(1)
    {
        PHY_state = CH394Q_GetPHYStatus();
        if ((PHY_state & 0x01) == 0)
        {
            Delay_Ms(500);
        }
        else
        {
            printf(" Link up..\r\n");
            break;
        }
    }
}

/********************************************************************************
 * Function Name  : CH394Q_ResetSW
 * Description    : CH394Q软件复位
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_ResetSW(void)
{
    CH394Q_SetMODE( MODE_RST );
    Delay_Ms(200);
}
