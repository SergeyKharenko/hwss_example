/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_NET.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/04/23
 * Description        : CH394Q net program body
 *******************************************************************************/
#include <CH394Q_NET.h>
#include <CH394Q.h>
#include "debug.h"

extern uint8_t MyBuffer[][2048];
uint8_t socketmode[8];

/********************************************************************************
 * Function Name  : CH394Q_TCPServerSocketInit
 * Description    : TCP服务器socket初始化函数
 * Input          : sockindex：socket number
                    mode: 设置其他模式
                    sourport：本地端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_TCPServerSocketInit(uint8_t sockindex, uint8_t mode, uint16_t sourport)
{
    CH394Q_Socket_Init(sockindex, mode, sourport);
    CH394Q_Socket_Listen(sockindex);
    socketmode[sockindex] = TCPServer;
}

/********************************************************************************
 * Function Name  : CH394Q_TCPClientSocketInit
 * Description    : TCP客户端socket初始化函数
 * Input          : sockindex：socket number
                    mode: 设置socket模式
                    sourport：本地端口
                    desip：对端IP
                    desport：对端端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_TCPClientSocketInit(uint8_t sockindex, uint8_t mode, uint16_t sourport,uint8_t * desip, uint16_t desport)
{
    CH394Q_Socket_Init(sockindex, mode, sourport);
    CH394Q_Socket_Connect(sockindex, desip, desport);
    socketmode[sockindex] = TCPClient;
}

/********************************************************************************
 * Function Name  : CH394Q_UDPSocketInit
 * Description    : UDP socket初始化函数
 * Input          : sockindex：socket number
                    mode: 设置socket模式
                    sourport：本地端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_UDPSocketInit(uint8_t sockindex, uint8_t mode, uint16_t sourport)
{
    CH394Q_Socket_Init(sockindex, mode, sourport);
    socketmode[sockindex] = UDP;
}

/********************************************************************************
 * Function Name  : CH394Q_UDPMultiSocketInit
 * Description    : UDP组播 socket初始化函数
 * Input          : sockindex：socket number
                    mode: 设置socket模式
                    multiip：组播IP
                    multimac：组播MAC
                    multiport：组播端口
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_UDPMultiSocketInit(uint8_t sockindex, uint8_t mode, uint8_t * multiip, uint8_t * multimac, uint16_t multiport)
{
    CH394Q_SetSn_DIP(sockindex, multiip);
    CH394Q_SetSn_DMAC(sockindex, multimac);
    CH394Q_SetSn_DPORT(sockindex, multiport);
    CH394Q_Socket_Init(sockindex, mode, multiport );
    socketmode[sockindex] = UDP;
}

/********************************************************************************
 * Function Name  : CH394Q_MACRAWSocketInit
 * Description    : MACRAW socket初始化函数
 * Input          : sockindex：socket number
                    mode: 设置socket模式
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_MACRAWSocketInit(uint8_t sockindex, uint8_t mode)
{
    CH394Q_Socket_Init(sockindex, mode, 0);
    socketmode[sockindex] = MACRAW;
}

/********************************************************************************
 * Function Name  : CH394Q_ReopenSocket
 * Description    : 重新打开socket
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_ReopenSocket(uint8_t sockindex)
{
    CH394Q_SetSn_INT(sockindex, 0xFF);
    CH394Q_Socket_Open(sockindex);
    if (socketmode[sockindex] == TCPServer)
    {
        CH394Q_Socket_Listen(sockindex);
    }
    if (socketmode[sockindex] == TCPClient)
    {
        CH394Q_SetSn_CTRL(sockindex, Sn_CTRL_CONNECT);
        while ( CH394Q_GetSn_CTRL(sockindex) ) ;
    }
}
/********************************************************************************
 * Function Name  : CH394Q_SocketInterrupt
 * Description    : socket 中断函数
 * Input          : sockindex：socket number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SocketInterrupt(uint8_t sockindex)
{
    uint16_t len = 0;
    uint16_t udp_real_len = 0;
    uint16_t macraw_real_len = 0;
    uint8_t  udp_desip[4];
    uint16_t udp_desport;
    uint8_t  Sn_STA_state = CH394Q_GetSn_STA(sockindex);
    uint8_t  Sn_INT_state = CH394Q_GetSn_INT(sockindex);
    if (Sn_INT_state & Sn_INT_RECV)
    {
        len = CH394Q_GetSn_RX_RS(sockindex);
        if (len > 0)
        {
            if (Sn_STA_state == SOCK_TCP_ESTABLISHED)
            {
                CH394Q_SocketTCPRecv(sockindex, MyBuffer[sockindex], len);
                CH394Q_SocketTCPSend(sockindex, MyBuffer[sockindex], len);
            }
            else if (Sn_STA_state == SOCK_UDP)
            {
                udp_real_len = CH394Q_Socket_UDP_Recv(sockindex, MyBuffer[sockindex], len, udp_desip, &udp_desport);
                CH394Q_SocketSendTo(sockindex, MyBuffer[sockindex], udp_real_len, udp_desip, udp_desport);
            }
            else if (Sn_STA_state == SOCK_MACRAW)
            {
                macraw_real_len = CH394Q_Socket_MACRAW_Recv(sockindex, MyBuffer[sockindex], len);
                CH394Q_Socket_MACRAW_Send(sockindex, MyBuffer[sockindex], macraw_real_len );
            }
        }
        len = CH394Q_GetSn_RX_RS(sockindex);
        if (len == 0)
        {
            CH394Q_SetSn_INT(sockindex, Sn_INT_RECV);
        }
    }
    if (Sn_INT_state & Sn_INT_DISCONNECT)
    {
        if (Sn_STA_state == SOCK_CLOSE_WAIT)
        {
            CH394Q_Socket_Disconnect(sockindex);
        }
        printf(" DISCONNECT\r\n");
        CH394Q_SetSn_INT(sockindex, Sn_INT_DISCONNECT);
        CH394Q_ReopenSocket(sockindex);
    }
    if (Sn_INT_state & Sn_INT_CONNECT)
    {
        printf(" CONNECT\r\n");
        CH394Q_SetSn_INT(sockindex, Sn_INT_CONNECT);
    }
    if (Sn_INT_state & Sn_INT_SEND_SUC)
    {
        CH394Q_SetSn_INT(sockindex, Sn_INT_SEND_SUC);
    }
    if (Sn_INT_state & Sn_INT_TIMEOUT)
    {
        printf(" TIMEOUT\r\n");
        CH394Q_SetSn_INT(sockindex, Sn_INT_TIMEOUT);
        CH394Q_ReopenSocket(sockindex);
    }
}

/********************************************************************************
 * Function Name  : CH394Q_GlobalInterrupt
 * Description    : 全局中断函数
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_GlobalInterrupt(void)
{
    uint8_t i = 0;
    uint8_t SINT_state = CH394Q_GetSINT();
    uint8_t GINT_state = CH394Q_GetGINT();
    uint8_t SINT_socket[8] ={
            SINT_state_SOCK0, SINT_state_SOCK1, SINT_state_SOCK2, SINT_state_SOCK3,
            SINT_state_SOCK4, SINT_state_SOCK5, SINT_state_SOCK6, SINT_state_SOCK7};
    for (i = 0; i < 8; i ++)
    {
        if (SINT_state & SINT_socket[i])
        {
            CH394Q_SocketInterrupt(i);
        }
    }
    if (GINT_state & GINT_IP_CONFLI)
    {
        printf(" IP CONFLICT\r\n");
        CH394Q_SetGINT(GINT_IP_CONFLI);
    }
    if (GINT_state & GINT_UNREACH)
    {
        printf(" UNREACHABLE\r\n");
        CH394Q_SetGINT(GINT_UNREACH);
    }
    if (GINT_state & GINT_MP)
    {
        printf(" MAGIC\r\n");
        CH394Q_SetGINT(GINT_MP);
    }
}
