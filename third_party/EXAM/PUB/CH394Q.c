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
 * Description    : ����CH394Qģʽ
 * Input          : mode������ģʽ
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetMODE(uint8_t mode)
{
    CH394Q_Write(MODE,mode);
}

/********************************************************************************
 * Function Name  : CH394Q_SetGWIP
 * Description    : ����CH394Q���ص�ַ
 * Input          : gwip�����ص�ַ
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
 * Description    : ����CH394Q���������ַ
 * Input          : smip�����������ַ
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
 * Description    : ����CH394Q��MAC��ַ
 * Input          : mac:MAC��ַ
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
 * Description    : ����CH394QԴIP��ַ
 * Input          : ip��IP��ַ
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
 * Description    : �жϼ��ʱ��Ĵ���
 * Input          : iit���ȴ�ʱ��
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
 * Description    : ���ȫ���жϱ�־
 * Input          : gint�����ĳ��ȫ���жϱ�־
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetGINT(uint8_t gint)
{
    CH394Q_Write(GINT,gint);
}

/********************************************************************************
 * Function Name  : CH394Q_SetGINTE
 * Description    : ʹ��ȫ���ж�
 * Input          : ginte��ʹ��ĳ��ȫ���ж�
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetGINTE(uint8_t ginte)
{
    CH394Q_Write(GINTE,ginte);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSINTE
 * Description    : ʹ��socket�ж�
 * Input          : sinte��ʹ��ĳ��socket�ж�
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSINTE(uint8_t sinte)
{
    CH394Q_Write(SINTE,sinte);
}

/********************************************************************************
 * Function Name  : CH394Q_SetRTIME
 * Description    : ����TCP�ش�ʱ��
 * Input          : rtime��TCP�ش�ʱ�䣬��λʱ��Ϊ100us
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
 * Description    : ����TCP�ش�����
 * Input          : rcnt��TCP�ش�����
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetRCNT(uint8_t rcnt)
{
    CH394Q_Write(RCNT,rcnt);
}

/********************************************************************************
 * Function Name  : CH394Q_SetPHY_CFG
 * Description    : ����PHY����
 * Input          : phycfg���������ģʽ
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
 * Description    : ����socketģʽ
 * Input          : sockindex��socket number
                    mode��ģʽ
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_MODE(uint8_t sockindex, uint8_t mode)
{
    CH394Q_Write(Sn_MODE(sockindex), mode);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_CTRL
 * Description    : ����socket��������
 * Input          : sockindex��socket number
                    ctrl����������
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_CTRL(uint8_t sockindex, uint8_t ctrl)
{
    CH394Q_Write(Sn_CTRL(sockindex), ctrl);
    Delay_Us(2);//����������ִ��ʱ��
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_INT
 * Description    : ���socket n ���жϱ�־
 * Input          : sockindex��socket number
                    val�����socket n ��ĳ���жϱ�־
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_INT(uint8_t sockindex, uint8_t val)
{
    CH394Q_Write(Sn_INT(sockindex), val);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_PORT
 * Description    : ����socket���ض˿�
 * Input          : sockindex��socket number
                    port�����ض˿�
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
 * Description    : ����socketĿ��MAC��ַ
 * Input          : sockindex��socket number
                    dmac��Ŀ��MAC��ַ
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
 * Description    : ����socketĿ��IP��ַ
 * Input          : sockindex��socket number
                    dip��Ŀ��IP��ַ
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
 * Description    : ����socketĿ�Ķ˿�
 * Input          : sockindex��socket number
                    dport��Ŀ�Ķ˿�
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
 * Description    : ����socket���ֶμĴ���
 * Input          : sockindex��socket number
                    mtu�����ֶδ�С
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
 * Description    : ����socketIP��������
 * Input          : sockindex��socket number
                    tos����������
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TOS(uint8_t sockindex, uint8_t tos)
{
    CH394Q_Write(Sn_TOS(sockindex), tos);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TTL
 * Description    : ����socket IP����ʱ��
 * Input          : sockindex��socket number
                    ttl������ʱ��
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TTL(uint8_t sockindex, uint8_t ttl)
{
    CH394Q_Write( Sn_TTL(sockindex) , ttl);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_RXBUF_SIZE
 * Description    : ����socket���ջ�������С
 * Input          : sockindex��socket number
                    rxbuf�����ջ�������С
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_RXBUF_SIZE(uint8_t sockindex, uint8_t rxbuf)
{
    CH394Q_Write( Sn_RXBUF_SIZE(sockindex) , rxbuf);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TXBUF_SIZE
 * Description    : ����socket���ͻ�������С
 * Input          : sockindex��socket number
                    txbuf�����ͻ�������С
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_TXBUF_SIZE(uint8_t sockindex, uint8_t txbuf)
{
    CH394Q_Write( Sn_TXBUF_SIZE(sockindex) , txbuf);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_TX_WR
 * Description    : ����socket����дָ��
 * Input          : sockindex��socket number
                    txwr���������ݳ���
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
 * Description    : ����socket���ն�ָ��
 * Input          : sockindex��socket number
                    rxrd���������ݳ���
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
 * Description    : ʹ��socket�ж�
 * Input          : sockindex��socket number
                    inte��ʹ��socketĳ���ж�
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_INTE(uint8_t sockindex, uint8_t inte)
{
    CH394Q_Write( Sn_INTE(sockindex) , inte);
}

/********************************************************************************
 * Function Name  : CH394Q_SetSn_IPF
 * Description    : ����socketIP����IP��ͷ�ķֶ�
 * Input          : sockindex��socket number
                    ipf��IP�ֶ�
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
 * Description    : ����KEEPALIVEʱ����
 * Input          : sockindex��socket number
                    val��ʱ����(��λʱ��Ϊ5s)
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_SetSn_KEEPALIVE(uint8_t sockindex,uint8_t val)
{
    CH394Q_Write(Sn_KEEPALIVE(sockindex),val);
}

/********************************************************************************
 * Function Name  : CH394Q_GetMODE
 * Description    : ��ȡMODEģʽ
 * Input          : None
 * Output         : None
 * Return         : MODEģʽ
 *******************************************************************************/
uint8_t CH394Q_GetMODE( void )
{
    return CH394Q_Read(MODE);
}

/********************************************************************************
 * Function Name  : CH394Q_GetGWIP
 * Description    : ��ȡCH394Q���ص�ַ
 * Input          : gwip�����ص�ַָ��
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
 * Description    : ��ȡCH394Q���������ַ
 * Input          : smip�����������ַָ��
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
 * Description    : ��ȡCH394Q MAC��ַ
 * Input          : mac��MAC��ַָ��
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
 * Description    : ��ȡCH394QԴIP��ַ
 * Input          : ip��ԴIP��ַָ��
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
 * Description    : ��ȡ�͵�ƽ�ж϶�ʱʱ��
 * Input          : None
 * Output         : None
 * Return         : �͵�ƽ�ж϶�ʱʱ��
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
 * Description    : ��ȡȫ���ж�״̬
 * Input          : None
 * Output         : None
 * Return         : ȫ���ж�״̬
 *******************************************************************************/
uint8_t CH394Q_GetGINT( void )
{
    return CH394Q_Read(GINT);
}

/********************************************************************************
 * Function Name  : CH394Q_GetGINTE
 * Description    : ��ȡʹ���ж�״̬
 * Input          : None
 * Output         : None
 * Return         : ʹ���ж�״̬
 *******************************************************************************/
uint8_t CH394Q_GetGINTE( void )
{
    return CH394Q_Read(GINTE);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSINT
 * Description    : ��ȡsocket�ж�״̬
 * Input          : None
 * Output         : None
 * Return         : socket�ж�״̬
 *******************************************************************************/
uint8_t CH394Q_GetSINT( void )
{
    return CH394Q_Read(SINT);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSINTE
 * Description    : ��ȡʹ��socket�ж�״̬
 * Input          : None
 * Output         : None
 * Return         : ʹ��socket�ж�״̬
 *******************************************************************************/
uint8_t CH394Q_GetSINTE( void )
{
    return CH394Q_Read(SINTE);
}

/********************************************************************************
 * Function Name  : CH394Q_GetRTIME
 * Description    : ��ȡ�ش���ʱʱ�䣨��λʱ��Ϊ100us��
 * Input          : None
 * Output         : None
 * Return         : �ش���ʱʱ��
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
 * Description    : ��ȡ�ش�����
 * Input          : None
 * Output         : None
 * Return         : �ش�����
 *******************************************************************************/
uint8_t CH394Q_GetRCNT( void )
{
    return CH394Q_Read(RCNT);
}

/********************************************************************************
 * Function Name  : CH394Q_GetUNIP
 * Description    : ��ȡ�޷��ִ�IP��ַ
 * Input          : unip���޷��ִ�IP��ַָ��
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
 * Description    : ��ȡ�޷��ִ�˿�
 * Input          : None
 * Output         : None
 * Return         : �޷��ִ�˿�
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
 * Description    : ��ȡPHY״̬
 * Input          : None
 * Output         : None
 * Return         : PHY״̬
 *******************************************************************************/
uint8_t CH394Q_GetPHYStatus( void )
{
    return CH394Q_Read(PHY_CFG);
}

/********************************************************************************
 * Function Name  : CH394Q_GetCHIPV
 * Description    : ��ȡCH394Q�汾��
 * Input          : None
 * Output         : None
 * Return         : �汾��
 *******************************************************************************/
uint8_t CH394Q_GetCHIPV(void)
{
    return CH394Q_Read(CHIPV);
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_MODE
 * Description    : ��ȡsocketģʽ
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socketģʽ
 *******************************************************************************/
uint8_t CH394Q_GetSn_MODE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_MODE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_CTRL
 * Description    : ��ȡsocket��������״̬
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket��������״̬
 *******************************************************************************/
uint8_t CH394Q_GetSn_CTRL(uint8_t sockindex)
{
    return CH394Q_Read(Sn_CTRL(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_INT
 * Description    : ��ȡsocket n ���ж�״̬
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket n ���ж�״̬
 *******************************************************************************/
uint8_t CH394Q_GetSn_INT(uint8_t sockindex)
{
    return CH394Q_Read(Sn_INT(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_STA
 * Description    : ��ȡsocket��״̬
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket��״̬
 *******************************************************************************/
uint8_t CH394Q_GetSn_STA(uint8_t sockindex)
{
    return CH394Q_Read(Sn_STA(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_PORT
 * Description    : ��ȡsocket���ض˿�
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���ض˿�
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
 * Description    : ��ȡsocketĿ��MAC��ַ
 * Input          : sockindex��socket number
                    dmac��Ŀ��MAC��ַָ��
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
 * Description    : ��ȡsocketĿ��IP
 * Input          : sockindex��socket number
                    dip��Ŀ��IP��ַָ��
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
 * Description    : ��ȡsocketĿ�Ķ˿�
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socketĿ�Ķ˿�
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
 * Description    : ��ȡsocket���ֶ�
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���ֶ�
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
 * Description    : ��ȡsocket��������
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket��������
 *******************************************************************************/
uint8_t CH394Q_GetSn_TOS(uint8_t sockindex)
{
    return CH394Q_Read(Sn_TOS(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TTL
 * Description    : ��ȡsocket IP����ʱ��
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket IP����ʱ��
 *******************************************************************************/
uint8_t CH394Q_GetSn_TTL(uint8_t sockindex)
{
    return CH394Q_Read(Sn_TTL(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_RXBUF_SIZE
 * Description    : ��ȡsocket���ջ����С
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���ջ����С
 *******************************************************************************/
uint8_t CH394Q_GetSn_RXBUF_SIZE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_RXBUF_SIZE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TXBUF_SIZE
 * Description    : ��ȡsocket���ͻ����С
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���ͻ����С
 *******************************************************************************/
uint8_t CH394Q_GetSn_TXBUF_SIZE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_TXBUF_SIZE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_TX_FS
 * Description    : ��ȡsocket���з��ͻ���
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���з��ͻ���
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
 * Description    : ��ȡsocket���н��ջ���
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���н��ջ���
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
 * Description    : ��ȡsocket���Ͷ�ָ��
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���Ͷ�ָ��
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
 * Description    : ��ȡsocket����дָ��
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket����дָ��
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
 * Description    : ��ȡsocket���ն�ָ��
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket���ն�ָ��
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
 * Description    : ��ȡsocket����дָ��
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket����дָ��
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
 * Description    : ��ȡsocket n ʹ���ж�״̬
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket n ʹ���ж�״̬
 *******************************************************************************/
uint8_t CH394Q_GetSn_INTE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_INTE(sockindex));
}

/********************************************************************************
 * Function Name  : CH394Q_GetSn_IPF
 * Description    : ��ȡsocket�ֶ�״̬
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket�ֶ�״̬
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
 * Description    : ��ȡsocket����������ʱ�䣨��λʱ��Ϊ5s��
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket����������ʱ��
 *******************************************************************************/
uint8_t CH394Q_GetSn_KEEPALIVE(uint8_t sockindex)
{
    return CH394Q_Read(Sn_KEEPALIVE(sockindex));
}

/******************************************************************************
 * Function Name  : CH394Q_Write
 * Description    : д��һ���ֽ����ݵ�CH394Q
 * Input          : addr:д�����ݵĵ�ַ
                    data:д���8λ����
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
 * Description    : ��CH394Q��һ���ֽ�����
 * Input          : addr:��ȡ���ݵĵ�ַ
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
 * Description    : д��len�ֽ����ݵ�CH394Q
 * Input          : addr:д�����ݵĵ�ַ
                    buf:д���ַ���
                    len������
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
 * Description    : ��CH394Q��len�ֽ�����
 * Input          : addr:��ȡ���ݵĵ�ַ
                    buf:��Ŷ�ȡ����
                    len������
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
 * Description    : socket��������
 * Input          : sockindex:socket number
                    buf:�������ݻ�����
                    len�����ݳ���
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
 * Description    : socket��������
 * Input          : sockindex:socket number
                    buf:�������ݻ�����
                    len�����ݳ���
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
 * Description    : TCPģʽ��������
 * Input          : sockindex��socket number
                    buf: �������ݻ�����
                    len�����ݳ���
 * Output         : None
 * Return         : �ɹ��������ݳ��ȣ�ʧ�ܷ���0
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
 * Description    : TCPģʽ��������
 * Input          : sockindex��socket number
                    buf: �������ݻ�����
                    len�����ݳ���
 * Output         : None
 * Return         : �������ݳ���
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
 * Description    : UDPģʽ��������
 * Input          : sockindex��socket number
                    buf���������ݻ�����
                    len�����ݳ���
                    desip���Զ�IP��ַ
                    desport���Զ˶˿�
 * Output         : None
 * Return         : �ɹ��������ݳ��ȣ�ʧ�ܷ���0
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
 * Description    : UDPģʽ��������
 * Input          : sockindex��socket number
                    buf: �������ݻ�����
                    len�����ݳ���
                    desip���Զ�IP��ַ
                    desport���Զ˶˿�
 * Output         : None
 * Return         : �������ݳ���
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
 * Description    : MACRAWģʽ��������
 * Input          : sockindex��socket number
                    buf:�������ݻ�����
                    len�����ݳ���
 * Output         : None
 * Return         : �ɹ��������ݳ��ȣ�ʧ�ܷ���0
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
 * Description    : MACRAWģʽ��������
 * Input          : sockindex��socket number
                    buf: �������ݻ�����
                    len�����ݳ���
 * Output         : None
 * Return         : �������ݳɹ��������ݳ��ȣ�ʧ�ܷ���0
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
 * Description    : ʹ��send_macָ��ʱ��������
 * Input          : sockindex��socket number
                    buf: �������ݻ�����
                    len�����ݳ���
                    desmac���Զ�mac��ַ
                    desip���Զ�IP��ַ
                    desport���Զ˶˿�
 * Output         : None
 * Return         : �ɹ��������ݳ��ȣ�ʧ�ܷ���0
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
 * Description    : CH394Q socket��ʼ��
 * Input          : sockindex��socket number
                    mode��socketģʽ
                    port�����ض˿�
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
 * Description    : ��socket
 * Input          : sockindex��socket number
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
 * Description    : �ر�socket
 * Input          : sockindex��socket number
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
 * Description    : �򿪼���
 * Input          : sockindex��socket number
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
 * Description    : ��������
 * Input          : sockindex��socket number
                    desip���Զ˷�������ַ
                    desport���Զ˷������˿�
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
 * Description    : socket�Ͽ�����
 * Input          : sockindex��socket number
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
 * Description    : ��ʼ��socket���պͷ��ͻ�������С
 * Input          : tx_size:���ͻ�������С����
                    rx_size:���ջ�������С����
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
 * Description    : ��ȡsocket n �����ջ���
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : ��socket n �����ջ���
 *******************************************************************************/
uint16_t CH394Q_GetRxMax(uint8_t sockindex)
{
    return RX_BUFF[sockindex]*1024;
}

/********************************************************************************
 * Function Name  : CH394Q_GetTxMax
 * Description    : ��ȡsocket n ����ͻ���
 * Input          : sockindex��socket number
 * Output         : None
 * Return         : socket n ����ͻ���
 *******************************************************************************/
uint16_t CH394Q_GetTxMax(uint8_t sockindex)
{
    return TX_BUFF[sockindex]*1024;
}

/********************************************************************************
 * Function Name  : CH394Q_InfParam
 * Description    : ����CH394Q�������
 * Input          : ip:����IP
                    smip:��������
                    gwip����������
                    mac������MAC
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_InfParam(uint8_t * ip,uint8_t * smip,uint8_t * gwip ,uint8_t * mac)
{
    CH394Q_SetIP(ip);
    CH394Q_SetSMIP(smip);
    CH394Q_SetGWIP(gwip);
    CH394Q_GetMAC(mac);
    printf(" CH394Q MAC��ַ  : %02x.%02x.%02x.%02x.%02x.%02x\r\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    CH394Q_GetIP (ip);
    printf(" CH394Q IP��ַ   : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
    CH394Q_GetSMIP(smip);
    printf(" CH394Q �������� : %d.%d.%d.%d\r\n", smip[0],smip[1],smip[2],smip[3]);
    CH394Q_GetGWIP(gwip);
    printf(" CH394Q ����     : %d.%d.%d.%d\r\n", gwip[0],gwip[1],gwip[2],gwip[3]);
}

/********************************************************************************
 * Function Name  : CH394Q_PHY_Check
 * Description    : PHY���Ӽ��
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
 * Description    : CH394Q�����λ
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_ResetSW(void)
{
    CH394Q_SetMODE( MODE_RST );
    Delay_Ms(200);
}
