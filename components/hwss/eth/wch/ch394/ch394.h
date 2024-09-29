#pragma once

#define CH394_SOCKNUM                   8
#define CH394_SOCK_CACHE_SIZE_KB        16

#define _CH394_SPI_READ_                (0x000000)
#define _CH394_SPI_WRITE_               (0x000004)

#define CH394_REG_MODE                  (0x000000) /*Mode register*/

#define CH394_REG_GWIP0                 (0x000100) /*Gateway address register*/
#define CH394_REG_GWIP1                 (0x000200)
#define CH394_REG_GWIP2                 (0x000300)
#define CH394_REG_GWIP3                 (0x000400)

#define CH394_REG_SMIP0                 (0x000500) /*Subnet mask register*/
#define CH394_REG_SMIP1                 (0x000600)
#define CH394_REG_SMIP2                 (0x000700)
#define CH394_REG_SMIP3                 (0x000800)

#define CH394_REG_MAC0                  (0x000900) /*Local MAC address register*/
#define CH394_REG_MAC1                  (0x000A00)
#define CH394_REG_MAC2                  (0x000B00)
#define CH394_REG_MAC3                  (0x000C00)
#define CH394_REG_MAC4                  (0x000D00)
#define CH394_REG_MAC5                  (0x000E00)

#define CH394_REG_IP0                   (0x000F00) /*Local IP address register*/
#define CH394_REG_IP1                   (0x001000)
#define CH394_REG_IP2                   (0x001100)
#define CH394_REG_IP3                   (0x001200)

#define CH394_REG_IIT0                  (0x001300) /*Interrupt interval time register*/
#define CH394_REG_IIT1                  (0x001400)

#define CH394_REG_GINT                  (0x001500) /*Global interrupt register*/
#define CH394_REG_GINTE                 (0x001600) /*Global interrupt enable register*/
#define CH394_REG_SINT                  (0x001700) /*Socket interrupt register*/
#define CH394_REG_SINTE                 (0x001800) /*socket interrupt enable register*/

#define CH394_REG_RTIME0                (0x001900) /*retransmission timeout register (unit value is 100us)*/
#define CH394_REG_RTIME1                (0x001A00)

#define CH394_REG_RCNT                  (0x001B00) /*retransmission count register*/

#define CH394_REG_UNIP0                 (0x002800) /*unreachable IP address register*/
#define CH394_REG_UNIP1                 (0x002900)
#define CH394_REG_UNIP2                 (0x002A00)
#define CH394_REG_UNIP3                 (0x002B00)

#define CH394_REG_UNPORT0               (0x002C00) /*unreachable port register*/
#define CH394_REG_UNPORT1               (0x002D00)

#define CH394_REG_PHY_CFG               (0x002E00) /*PHY configuration register*/

#define CH394_REG_CHIPV                 (0x003900) /*Chip version register*/

#define CH394_REG_Sn_MODE(s)            (0x000008 + (s<<5)) /*Socket n mode register*/
#define CH394_REG_Sn_CTRL(s)            (0x000108 + (s<<5)) /*Socket n configuration register*/
#define CH394_REG_Sn_INT(s)             (0x000208 + (s<<5)) /*Socket n interrupt register*/
#define CH394_REG_Sn_STA(s)             (0x000308 + (s<<5)) /*Socket n status register*/
#define CH394_REG_Sn_PORT0(s)           (0x000408 + (s<<5)) /*Socket n source port register*/
#define CH394_REG_Sn_PORT1(s)           (0x000508 + (s<<5))

#define CH394_REG_Sn_DMAC0(s)           (0x000608 + (s<<5)) /*Socket n destination MAC address register*/
#define CH394_REG_Sn_DMAC1(s)           (0x000708 + (s<<5))
#define CH394_REG_Sn_DMAC2(s)           (0x000808 + (s<<5))
#define CH394_REG_Sn_DMAC3(s)           (0x000908 + (s<<5))
#define CH394_REG_Sn_DMAC4(s)           (0x000A08 + (s<<5))
#define CH394_REG_Sn_DMAC5(s)           (0x000B08 + (s<<5))

#define CH394_REG_Sn_DIP0(s)            (0x000C08 + (s<<5)) /*Socket destination IP address register*/
#define CH394_REG_Sn_DIP1(s)            (0x000D08 + (s<<5))
#define CH394_REG_Sn_DIP2(s)            (0x000E08 + (s<<5))
#define CH394_REG_Sn_DIP3(s)            (0x000F08 + (s<<5))

#define CH394_REG_Sn_DPORT0(s)          (0x001008 + (s<<5)) /*Socket n target port register*/
#define CH394_REG_Sn_DPORT1(s)          (0x001108 + (s<<5))

#define CH394_REG_Sn_MTU0(s)            (0x001208 + (s<<5)) /*Socket n maximum segment register*/
#define CH394_REG_Sn_MTU1(s)            (0x001308 + (s<<5))

#define CH394_REG_Sn_TOS(s)             (0x001508 + (s<<5)) /*Socket IP service type register*/
#define CH394_REG_Sn_TTL(s)             (0x001608 + (s<<5)) /*Socket IP lifetime register*/

#define CH394_REG_Sn_RXBUF_SIZE(s)      (0x001E08 + (s<<5)) /*Socket n receive buffer size register*/
#define CH394_REG_Sn_TXBUF_SIZE(s)      (0x001F08 + (s<<5)) /*Socket n transmit buffer size register*/

#define CH394_REG_Sn_TX_FS0(s)          (0x002008 + (s<<5)) /*Socket n idle transmit buffer register*/
#define CH394_REG_Sn_TX_FS1(s)          (0x002108 + (s<<5))

#define CH394_REG_Sn_TX_RD0(s)          (0x002208 + (s<<5)) /*Socket n Send read pointer register*/
#define CH394_REG_Sn_TX_RD1(s)          (0x002308 + (s<<5))

#define CH394_REG_Sn_TX_WR0(s)          (0x002408 + (s<<5)) /*Socket n send write pointer register*/
#define CH394_REG_Sn_TX_WR1(s)          (0x002508 + (s<<5))

#define CH394_REG_Sn_RX_RS0(s)          (0x002608 + (s<<5)) /*Socket n free receive buffer register*/
#define CH394_REG_Sn_RX_RS1(s)          (0x002708 + (s<<5))

#define CH394_REG_Sn_RX_RD0(s)          (0x002808 + (s<<5)) /*Socket n receive read pointer register*/
#define CH394_REG_Sn_RX_RD1(s)          (0x002908 + (s<<5))

#define CH394_REG_Sn_RX_WR0(s)          (0x002A08 + (s<<5)) /*Socket n receive write pointer register*/
#define CH394_REG_Sn_RX_WR1(s)          (0x002B08 + (s<<5))

#define CH394_REG_Sn_INTE(s)            (0x002C08 + (s<<5)) /*Socket n interrupt enable register*/

#define CH394_REG_Sn_IPF0(s)            (0x002D08 + (s<<5)) /*Socket n segment register*/
#define CH394_REG_Sn_IPF1(s)            (0x002E08 + (s<<5))

#define CH394_REG_Sn_KEEPALIVE(s)       (0x002F08 + (s<<5)) /*Socket online time register*/

#define CH394_TXBUF_BLOCK(s)            (0x000010 + (s<<5))
#define CH394_RXBUF_BLOCK(s)            (0x000018 + (s<<5))

/* MODE mode register */
#define CH394_MODE_RST                  0x80 /* Internal register reset */
#define CH394_MODE_WOL                  0x20 /* Network wake-up */
#define CH394_MODE_PB                   0x10 /* Ping packet mask enable */
#define CH394_MODE_UDP_FARP             0x02 /* Enable forced ARP */

/* GINT interrupt register */
#define CH394_GINT_IP_CONFLI            0x80 /* IP conflict */
#define CH394_GINT_UNREACH              0x40 /* Destination port unreachable */
#define CH394_GINT_MP                   0x10 /* Magic packet interrupt */

/* Socket n mode register */
#define CH394_Sn_MODE_CLOSE             0x00 /* Unused socket */
#define CH394_Sn_MODE_TCP               0x01 /* TCP mode */
#define CH394_Sn_MODE_UDP               0x02 /* UDP mode */
#define CH394_Sn_MODE_MACRAW            0x04 /* MACRAW mode */
#define CH394_Sn_MODE_UCASTB            0x10 /* Unicast filtering */
#define CH394_Sn_MODE_MIP6B             0x10 /* Enable IPv6 packet filtering in MACRAW mode */
#define CH394_Sn_MODE_NA                0x20 /* Enable ACK without delay */
#define CH394_Sn_MODE_MV                0x20 /* Multicast uses IGMP version 1 */
#define CH394_Sn_MODE_MM                0x20 /* Enable multicast filtering in MACRAW mode */
#define CH394_Sn_MODE_BCASTB            0x40 /* Enable broadcast filtering */
#define CH394_Sn_MODE_MUL               0x80 /* Enable multicast */
#define CH394_Sn_MODE_MFEN              0x80 /* Enable MAC address filtering in MACRAW mode */

/* Socket n configuration register */
#define CH394_Sn_CTRL_OPEN              0x01 /* Open socket n*/
#define CH394_Sn_CTRL_LISTEN            0x02 /* Listen, wait for connection as TCP server mode */
#define CH394_Sn_CTRL_CONNECT           0x04 /* Connect, initiate connection request as TCP client mode */
#define CH394_Sn_CTRL_DISCONNECT        0x08 /* Disconnect */
#define CH394_Sn_CTRL_CLOSE             0x10 /* Close socket n*/
#define CH394_Sn_CTRL_SEND              0x20 /* Update send buffer pointer, send data */
#define CH394_Sn_CTRL_SEND_MAC          0x21 /* Automatically use the set peer MAC address when sending data, no need to obtain the peer MAC through ARP*/
#define CH394_Sn_CTRL_SEND_KEEP         0x22 /* Send 1-byte online heartbeat packet to check connection status */
#define CH394_Sn_CTRL_RECV              0x40 /* Update the receive buffer pointer and receive data */

/* Socket n interrupt register */
#define CH394_Sn_INT_SEND_SUC           0x10 /* Send */
#define CH394_Sn_INT_TIMEOUT            0x08 /* Timeout */
#define CH394_Sn_INT_RECV               0x04 /* Receive */
#define CH394_Sn_INT_DISCONNECT         0x02 /* Disconnect */
#define CH394_Sn_INT_CONNECT            0x01 /* Establish connection */

/* Sn_STA Socket n status register */
#define CH394_SOCK_CLOSE                0x00 /* Close state */
#define CH394_SOCK_INIT                 0x13 /* Initialization state */
#define CH394_SOCK_LISTEN               0x14 /* Listening state */
#define CH394_SOCK_SYN_SENT             0x15 /* Send connection request to the other party */
#define CH394_SOCK_SYN_RECV             0x16 /* Receive connection request packet */
#define CH394_SOCK_TCP_ESTABLISHED      0x17 /* Connection established successfully */
#define CH394_SOCK_FIN_WAIT             0x18
#define CH394_SOCK_CLOSING              0x1A
#define CH394_SOCK_TIME_WAIT            0x1B
#define CH394_SOCK_CLOSE_WAIT           0x1C
#define CH394_SOCK_LAST_ACK             0x1D
#define CH394_SOCK_UDP                  0x22 /* In UDP mode */
#define CH394_SOCK_MACRAW               0x42 /* In MACRAW mode */
