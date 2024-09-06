/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/04/23
 * Description        : CH394Q application program header file
 *******************************************************************************/
#ifndef  _CH394Q_H_
#define  _CH394Q_H_
#include "stdint.h"

#define SOCKET_NUM 8

#define MODE                        (0x000000)  /*模式寄存器*/
#define GWIP0                       (0x000100)  /*网关地址寄存器*/
#define GWIP1                       (0x000200)
#define GWIP2                       (0x000300)
#define GWIP3                       (0x000400)

#define SMIP0                       (0x000500)  /*子网掩码寄存器*/
#define SMIP1                       (0x000600)
#define SMIP2                       (0x000700)
#define SMIP3                       (0x000800)

#define MAC0                        (0x000900)  /*本地MAC地址寄存器*/
#define MAC1                        (0x000A00)
#define MAC2                        (0x000B00)
#define MAC3                        (0x000C00)
#define MAC4                        (0x000D00)
#define MAC5                        (0x000E00)

#define IP0                         (0x000F00)  /*本地IP地址寄存器*/
#define IP1                         (0x001000)
#define IP2                         (0x001100)
#define IP3                         (0x001200)

#define IIT0                        (0x001300)  /*中断间隔时间寄存器*/
#define IIT1                        (0x001400)
#define GINT                        (0x001500)  /*全局中断寄存器*/
#define GINTE                       (0x001600)  /*全局中断使能寄存器*/
#define SINT                        (0x001700)  /*socket中断寄存器*/
#define SINTE                       (0x001800)  /*socket中断使能寄存器*/
#define RTIME0                      (0x001900)  /*重传超时时间寄存器(单位数值为100us)*/
#define RTIME1                      (0x001A00)
#define RCNT                        (0x001B00)  /*重传次数寄存器*/

#define UNIP0                       (0x002800)  /*无法抵达IP地址寄存器*/
#define UNIP1                       (0x002900)
#define UNIP2                       (0x002A00)
#define UNIP3                       (0x002B00)

#define UNPORT0                     (0x002C00)  /*无法抵达端口寄存器*/
#define UNPORT1                     (0x002D00)
#define PHY_CFG                     (0x002E00)  /*PHY配置寄存器*/
#define CHIPV                       (0x003900)  /*芯片版本寄存器*/
#define Sn_MODE(s)                  (0x000008 + (s<<5))  /*Socket n 模式寄存器*/
#define Sn_CTRL(s)                  (0x000108 + (s<<5))  /*Socket n 配置寄存器*/
#define Sn_INT(s)                   (0x000208 + (s<<5))  /*Socket n 中断寄存器*/
#define Sn_STA(s)                   (0x000308 + (s<<5))  /*Socket n 状态寄存器*/
#define Sn_PORT0(s)                 (0x000408 + (s<<5))  /*Socket n 源端口寄存器*/
#define Sn_PORT1(s)                 (0x000508 + (s<<5))

#define Sn_DMAC0(s)                 (0x000608 + (s<<5))  /*Socket n 目的 MAC地址寄存器*/
#define Sn_DMAC1(s)                 (0x000708 + (s<<5))
#define Sn_DMAC2(s)                 (0x000808 + (s<<5))
#define Sn_DMAC3(s)                 (0x000908 + (s<<5))
#define Sn_DMAC4(s)                 (0x000A08 + (s<<5))
#define Sn_DMAC5(s)                 (0x000B08 + (s<<5))

#define Sn_DIP0(s)                  (0x000C08 + (s<<5))  /*Socket 目标IP地址寄存器*/
#define Sn_DIP1(s)                  (0x000D08 + (s<<5))
#define Sn_DIP2(s)                  (0x000E08 + (s<<5))
#define Sn_DIP3(s)                  (0x000F08 + (s<<5))

#define Sn_DPORT0(s)                (0x001008 + (s<<5))  /*Socket n 目标端口寄存器*/
#define Sn_DPORT1(s)                (0x001108 + (s<<5))
#define Sn_MTU0(s)                  (0x001208 + (s<<5))  /*Socket n 最大分段寄存器*/
#define Sn_MTU1(s)                  (0x001308 + (s<<5))
#define Sn_TOS(s)                   (0x001508 + (s<<5))  /*Socket IP服务类型寄存器*/
#define Sn_TTL(s)                   (0x001608 + (s<<5))  /*Socket IP生存时间寄存器 */
#define Sn_RXBUF_SIZE(s)            (0x001E08 + (s<<5))  /*Socket n 接收缓存大小寄存器*/
#define Sn_TXBUF_SIZE(s)            (0x001F08 + (s<<5))  /*Socket n 发送缓存大小寄存器*/
#define Sn_TX_FS0(s)                (0x002008 + (s<<5))  /*Socket n 空闲发送缓存寄存器*/
#define Sn_TX_FS1(s)                (0x002108 + (s<<5))
#define Sn_TX_RD0(s)                (0x002208 + (s<<5))  /*Socket n 发送读指针寄存器*/
#define Sn_TX_RD1(s)                (0x002308 + (s<<5))
#define Sn_TX_WR0(s)                (0x002408 + (s<<5))  /*Socket n 发送写指针寄存器*/
#define Sn_TX_WR1(s)                (0x002508 + (s<<5))
#define Sn_RX_RS0(s)                (0x002608 + (s<<5))  /*Socket n 空闲接收缓存寄存器*/
#define Sn_RX_RS1(s)                (0x002708 + (s<<5))
#define Sn_RX_RD0(s)                (0x002808 + (s<<5))  /*Socket n 接收读指针寄存器*/
#define Sn_RX_RD1(s)                (0x002908 + (s<<5))
#define Sn_RX_WR0(s)                (0x002A08 + (s<<5))  /*Socket n 接收写指针寄存器*/
#define Sn_RX_WR1(s)                (0x002B08 + (s<<5))
#define Sn_INTE(s)                  (0x002C08 + (s<<5))  /*Socket n 中断使能寄存器*/
#define Sn_IPF0(s)                  (0x002D08 + (s<<5))  /*Socket n 分段寄存器*/
#define Sn_IPF1(s)                  (0x002E08 + (s<<5))
#define Sn_KEEPALIVE(s)             (0x002F08 + (s<<5))  /*Socket 在线时间寄存器*/

/* MODE模式寄存器 */
#define MODE_RST                     0x80     /* 内部寄存器复位*/
#define MODE_WOL                     0x20     /* 网络唤醒 */
#define MODE_PB                      0x10     /* Ping包屏蔽使能*/
#define MODE_UDP_FARP                0x02     /* 开启 强制ARP*/
/* GINT中断寄存器 */
#define GINT_IP_CONFLI               0x80     /* IP冲突 */
#define GINT_UNREACH                 0x40     /* 目的端口不可达 */
#define GINT_MP                      0x10     /* 魔法包中断 */
/* Socket n 模式寄存器 */
#define Sn_MODE_CLOSE                0x00     /* 未使用socket*/
#define Sn_MODE_TCP                  0x01     /* TCP模式 */
#define Sn_MODE_UDP                  0x02     /* UDP模式 */
#define Sn_MODE_MACRAW               0x04     /* MACRAW模式 */
#define Sn_MODE_UCASTB               0x10     /* 单播过滤*/
#define Sn_MODE_MIP6B                0x10     /* MACRAW模式下开启IPv6包过滤 */
#define Sn_MODE_NA                   0x20     /* 开启无延时 ACK*/
#define Sn_MODE_MV                   0x20     /* 多播使用IGMP版本1*/
#define Sn_MODE_MM                   0x20     /* MACRAW模式下开启多播过滤*/
#define Sn_MODE_BCASTB               0x40     /* 开启广播过滤 */
#define Sn_MODE_MUL                  0x80     /* 开启多播 */
#define Sn_MODE_MFEN                 0x80     /* MACRAW模式下启用MAC地址过滤*/
/* Socket n 配置寄存器 */
#define Sn_CTRL_OPEN                 0x01     /* 打开socket n*/
#define Sn_CTRL_LISTEN               0x02     /* 监听，作为TCP服务器模式等待连接 */
#define Sn_CTRL_CONNECT              0x04     /* 连接，作为TCP客户端模式发起连接请求 */
#define Sn_CTRL_DISCONNECT           0x08     /* 断开连接 */
#define Sn_CTRL_CLOSE                0x10     /* 关闭socket n*/
#define Sn_CTRL_SEND                 0x20     /* 更新发送缓存指针，发送数据 */
#define Sn_CTRL_SEND_MAC             0x21     /* 发送数据时自动使用设置的对端MAC地址 ，无需ARP获取对端MAC*/
#define Sn_CTRL_SEND_KEEP            0x22     /* 发送1字节在线心跳包检查连接状态 */
#define Sn_CTRL_RECV                 0x40     /* 更新接收缓存指针，接收数据 */
/* Socket n 中断寄存器*/
#define Sn_INT_SEND_SUC              0x10     /* 发送 */
#define Sn_INT_TIMEOUT               0x08     /* 超时 */
#define Sn_INT_RECV                  0x04     /* 接收 */
#define Sn_INT_DISCONNECT            0x02     /* 断开连接 */
#define Sn_INT_CONNECT               0x01     /* 建立连接 */
/* Sn_STA Socket n 状态寄存器 */
#define SOCK_CLOSE                   0x00     /* 关闭状态 */
#define SOCK_INIT                    0x13     /* 初始化状态 */
#define SOCK_LISTEN                  0x14     /* 监听状态 */
#define SOCK_SYN_SENT                0x15     /* 发送连接请求到对方 */
#define SOCK_SYN_RECV                0x16     /* 收到连接请求包 */
#define SOCK_TCP_ESTABLISHED         0x17     /* 建立连接成功 */
#define SOCK_FIN_WAIT                0x18
#define SOCK_CLOSING                 0x1A
#define SOCK_TIME_WAIT               0x1B
#define SOCK_CLOSE_WAIT              0x1C
#define SOCK_LAST_ACK                0x1D
#define SOCK_UDP                     0x22     /* 处于UDP模式 */
#define SOCK_MACRAW                  0x42     /* 处于MACRAW模式 */
/*********************************************************
* CH394 access function
*********************************************************/
void CH394Q_SetMODE(uint8_t mode);

void CH394Q_SetGWIP(uint8_t * gwip );

void CH394Q_SetSMIP(uint8_t * smip);

void CH394Q_SetMAC(uint8_t * mac);

void CH394Q_SetIP(uint8_t * ip);

void CH394Q_SetIIT(uint16_t iit);

void CH394Q_SetGINT(uint8_t gint);

void CH394Q_SetGINTE(uint8_t ginte);

void CH394Q_SetSINTE(uint8_t sinte);

void CH394Q_SetRTIME(uint16_t rtime);

void CH394Q_SetRCNT(uint8_t rcnt);

void CH394Q_SetPHY_CFG(uint8_t phycfg);

void CH394Q_SetSn_MODE(uint8_t sockindex, uint8_t mode);

void CH394Q_SetSn_CTRL(uint8_t sockindex, uint8_t ctrl);

void CH394Q_SetSn_INT(uint8_t sockindex, uint8_t val);

void CH394Q_SetSn_PORT(uint8_t sockindex, uint16_t port );

void CH394Q_SetSn_DMAC(uint8_t sockindex, uint8_t * dmac);

void CH394Q_SetSn_DIP(uint8_t sockindex, uint8_t * dip);

void CH394Q_SetSn_DPORT(uint8_t sockindex, uint16_t dport );

void CH394Q_SetSn_MTU(uint8_t sockindex, uint16_t mtu);

void CH394Q_SetSn_TOS(uint8_t sockindex, uint8_t tos);

void CH394Q_SetSn_TTL(uint8_t sockindex, uint8_t ttl);

void CH394Q_SetSn_RXBUF_SIZE(uint8_t sockindex, uint8_t rxbuf);

void CH394Q_SetSn_TXBUF_SIZE(uint8_t sockindex, uint8_t txbuf);

void CH394Q_SetSn_TX_WR(uint8_t sockindex, uint16_t txwr);

void CH394Q_SetSn_RX_RD(uint8_t sockindex, uint16_t rxrd);

void CH394Q_SetSn_INTE(uint8_t sockindex, uint8_t inte);

void CH394Q_SetSn_IPF(uint8_t sockindex, uint16_t ipf);

void CH394Q_SetSn_KEEPALIVE(uint8_t sockindex,uint8_t val);

void CH394Q_GetGWIP(uint8_t * gwip);

void CH394Q_GetSMIP(uint8_t * smip);

void CH394Q_GetMAC(uint8_t * mac);

void CH394Q_GetIP(uint8_t * ip);

uint16_t CH394Q_GetIIT(void);

uint8_t  CH394Q_GetGINT( void );

uint8_t  CH394Q_GetGINTE( void );

uint8_t  CH394Q_GetSINT( void );

uint8_t  CH394Q_GetSINTE( void );

uint16_t CH394Q_GetRTIME( void );

uint8_t  CH394Q_GetRCNT( void );

void CH394Q_GetUNIP(uint8_t * unip);

uint16_t CH394Q_GetUNPORT( void );

uint8_t  CH394Q_GetPHYStatus( void );

uint8_t  CH394Q_GetCHIPV(void);

uint8_t  CH394Q_GetSn_MODE(uint8_t sockindex);

uint8_t  CH394Q_GetSn_CTRL(uint8_t sockindex);

uint8_t  CH394Q_GetSn_INT(uint8_t sockindex);

uint8_t  CH394Q_GetSn_STA(uint8_t sockindex);

uint16_t CH394Q_GetSn_PORT(uint8_t sockindex);

void CH394Q_GetSn_DMAC(uint8_t sockindex, uint8_t * dmac);

void CH394Q_GetSn_DIP(uint8_t sockindex, uint8_t * dip);

uint16_t CH394Q_GetSn_DPORT(uint8_t sockindex);

uint16_t CH394Q_GetSn_MTU(uint8_t sockindex);

uint8_t  CH394Q_GetSn_TOS(uint8_t sockindex);

uint8_t  CH394Q_GetSn_TTL(uint8_t sockindex);

uint8_t  CH394Q_GetSn_RXBUF_SIZE(uint8_t sockindex);

uint8_t  CH394Q_GetSn_TXBUF_SIZE(uint8_t sockindex);

uint16_t CH394Q_GetSn_TX_FS(uint8_t sockindex);

uint16_t CH394Q_GetSn_RX_RS(uint8_t sockindex);

uint16_t CH394Q_GetSn_TX_RD(uint8_t sockindex);

uint16_t CH394Q_GetSn_TX_WR(uint8_t sockindex);

uint16_t CH394Q_GetSn_RX_RD(uint8_t sockindex);

uint16_t CH394Q_GetSn_RX_WR(uint8_t sockindex);

uint8_t  CH394Q_GetSn_INTE(uint8_t sockindex);

uint16_t CH394Q_GetSn_IPF(uint8_t sockindex);

uint8_t  CH394Q_GetSn_KEEPALIVE(uint8_t sockindex);

void CH394Q_Write(uint32_t addr,uint8_t data);

uint8_t  CH394Q_Read(uint32_t addr);

uint16_t CH394Q_WriteBuf(uint32_t addr,uint8_t* buf,uint16_t len);

uint16_t CH394Q_ReadBuf(uint32_t addr, uint8_t* buf,uint16_t len);

void CH394Q_SocketSendData(uint8_t sockindex, uint8_t *buf, uint16_t len);

void CH394Q_SocketRecvData(uint8_t sockindex, uint8_t *buf, uint16_t len);

uint16_t CH394Q_SocketTCPSend(uint8_t sockindex, const uint8_t * buf, uint16_t len);

uint16_t CH394Q_SocketTCPRecv(uint8_t sockindex, uint8_t * buf, uint16_t len);

uint16_t CH394Q_SocketSendTo(uint8_t sockindex, const uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port);

uint16_t CH394Q_Socket_UDP_Recv(uint8_t sockindex, uint8_t * buf, uint16_t len, uint8_t * desip, uint16_t *desport);

uint16_t CH394Q_Socket_MACRAW_Send(uint8_t sockindex, const uint8_t * buf, uint16_t len);

uint16_t CH394Q_Socket_MACRAW_Recv(uint8_t sockindex, uint8_t * buf, uint16_t len);

uint16_t CH394Q_SocketSendMAC(uint8_t sockindex, const uint8_t * buf, uint16_t len, uint8_t * desmac, uint8_t * desip, uint16_t desport);

uint16_t CH394Q_GetRxMax(uint8_t sockindex);

uint16_t CH394Q_GetTxMax(uint8_t sockindex);

void CH394Q_Socket_Init(uint8_t sockindex, uint8_t mode, uint16_t port);

void CH394Q_Socket_Open(uint8_t sockindex);

void CH394Q_Socket_Close(uint8_t sockindex);

void CH394Q_Socket_Listen(uint8_t sockindex);

void CH394Q_Socket_Connect(uint8_t sockindex, uint8_t * desip, uint16_t desport);

void CH394Q_Socket_Disconnect(uint8_t sockindex);

void CH394Q_SocketBuf_Init( uint8_t * tx_size, uint8_t * rx_size);

void CH394Q_GPIO_Init(void);

void CH394Q_InfParam(uint8_t * ip,uint8_t * smip,uint8_t * gwip ,uint8_t * mac);

void CH394Q_PHY_Check(void);

void CH394Q_ResetHW(void);

void CH394Q_ResetSW(void);

#endif
