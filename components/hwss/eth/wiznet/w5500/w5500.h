#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define W5500_SOCKNUM                   8
#define W5500_SOCK_CACHE_SIZE_KB        16

#define _W5500_IO_BASE_                 0x00000000

#define _W5500_SPI_READ_			    (0x00 << 2) //< SPI interface Read operation in Control Phase
#define _W5500_SPI_WRITE_			    (0x01 << 2) //< SPI interface Write operation in Control Phase

#define _W5500_SPI_VDM_OP_              0x00

#define W5500_CREG_BLOCK                0x00 	//< Common register block
#define W5500_SREG_BLOCK(N)             (1+4*N) //< Socket N register block
#define W5500_TXBUF_BLOCK(N)            (2+4*N) //< Socket N Tx buffer address block
#define W5500_RXBUF_BLOCK(N)            (3+4*N) //< Socket N Rx buffer address block

//------------------------------- defgroup end --------------------------------------------
//----------------------------- W5500 Common Registers IOMAP -----------------------------
/**
 * @ingroup Common_register_group
 * @brief Mode Register address(R/W)\n
 * @ref MR is used for S/W reset, ping block mode, PPPoE mode and etc.
 * @details Each bit of @ref MR defined as follows.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>RST</td> <td>Reserved</td> <td>WOL</td> <td>PB</td> <td>PPPoE</td> <td>Reserved</td> <td>FARP</td> <td>Reserved</td> </tr>
 * </table>
 * - \ref MR_RST		 	: Reset
 * - \ref MR_WOL       	: Wake on LAN
 * - \ref MR_PB         : Ping block
 * - \ref MR_PPPOE      : PPPoE mode
 * - \ref MR_FARP			: Force ARP mode
 */
#define W5500_REG_MR                 (_W5500_IO_BASE_ + (0x0000 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Gateway IP Register address(R/W)
 * @details @ref GAR configures the default gateway address.
 */
#define W5500_REG_GAR                (_W5500_IO_BASE_ + (0x0001 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Subnet mask Register address(R/W)
 * @details @ref SUBR configures the subnet mask address.
 */
#define W5500_REG_SUBR               (_W5500_IO_BASE_ + (0x0005 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Source MAC Register address(R/W)
 * @details @ref SHAR configures the source hardware address.
 */
#define W5500_REG_SHAR               (_W5500_IO_BASE_ + (0x0009 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Source IP Register address(R/W)
 * @details @ref SIPR configures the source IP address.
 */
#define W5500_REG_SIPR               (_W5500_IO_BASE_ + (0x000F << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Set Interrupt low level timer register address(R/W)
 * @details @ref INTLEVEL configures the Interrupt Assert Time.
 */
#define W5500_REG_INTLEVEL           (_W5500_IO_BASE_ + (0x0013 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Interrupt Register(R/W)
 * @details @ref IR indicates the interrupt status. Each bit of @ref IR will be still until the bit will be written to by the host.
 * If @ref IR is not equal to x00 INTn PIN is asserted to low until it is x00\n\n
 * Each bit of @ref IR defined as follows.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>CONFLICT</td> <td>UNREACH</td> <td>PPPoE</td> <td>MP</td> <td>Reserved</td> <td>Reserved</td> <td>Reserved</td> <td>Reserved</td> </tr>
 * </table>
 * - \ref IR_CONFLICT : IP conflict
 * - \ref IR_UNREACH  : Destination unreachable
 * - \ref IR_PPPoE	  : PPPoE connection close
 * - \ref IR_MP		  : Magic packet
 */
#define W5500_REG_IR                 (_W5500_IO_BASE_ + (0x0015 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Interrupt mask register(R/W)
 * @details @ref _IMR_ is used to mask interrupts. Each bit of @ref _IMR_ corresponds to each bit of @ref IR.
 * When a bit of @ref _IMR_ is and the corresponding bit of @ref IR is  an interrupt will be issued. In other words,
 * if a bit of @ref _IMR_ is  an interrupt will not be issued even if the corresponding bit of @ref IR is \n\n
 * Each bit of @ref _IMR_ defined as the following.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>IM_IR7</td> <td>IM_IR6</td> <td>IM_IR5</td> <td>IM_IR4</td> <td>Reserved</td> <td>Reserved</td> <td>Reserved</td> <td>Reserved</td> </tr>
 * </table>
 * - \ref IM_IR7 : IP Conflict Interrupt Mask
 * - \ref IM_IR6 : Destination unreachable Interrupt Mask
 * - \ref IM_IR5 : PPPoE Close Interrupt Mask
 * - \ref IM_IR4 : Magic Packet Interrupt Mask
 */
//M20150401 : Rename SYMBOE ( Re-define error in a compile) 
//#define IMR                (_W5500_IO_BASE_ + (0x0016 << 8) + (W5500_CREG_BLOCK << 3))
#define W5500_REG_IMR                (_W5500_IO_BASE_ + (0x0016 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Socket Interrupt Register(R/W)
 * @details @ref SIR indicates the interrupt status of Socket.\n
 * Each bit of @ref SIR be still until @ref Sn_IR is cleared by the host.\n
 * If @ref Sn_IR is not equal to x00 the n-th bit of @ref SIR is and INTn PIN is asserted until @ref SIR is x00 */
#define W5500_REG_SIR                (_W5500_IO_BASE_ + (0x0017 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Socket Interrupt Mask Register(R/W)
 * @details Each bit of @ref SIMR corresponds to each bit of @ref SIR.
 * When a bit of @ref SIMR is and the corresponding bit of @ref SIR is  Interrupt will be issued.
 * In other words, if a bit of @ref SIMR is  an interrupt will be not issued even if the corresponding bit of @ref SIR is 
 */
#define W5500_REG_SIMR               (_W5500_IO_BASE_ + (0x0018 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Timeout register address( 1 is 100us )(R/W)
 * @details @ref _RTR_ configures the retransmission timeout period. The unit of timeout period is 100us and the default of @ref _RTR_ is x07D0.
 * And so the default timeout period is 200ms(100us X 2000). During the time configured by @ref _RTR_, W5500 waits for the peer response
 * to the packet that is transmitted by \ref Sn_CR (CONNECT, DISCON, CLOSE, SEND, SEND_MAC, SEND_KEEP command).
 * If the peer does not respond within the @ref _RTR_ time, W5500 retransmits the packet or issues timeout.
 */
//M20150401 : Rename SYMBOE ( Re-define error in a compile)  
//#define RTR                (_W5500_IO_BASE_ + (0x0019 << 8) + (W5500_CREG_BLOCK << 3))
#define W5500_REG_RTR                (_W5500_IO_BASE_ + (0x0019 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Retry count register(R/W)
 * @details @ref _RCR_ configures the number of time of retransmission.
 * When retransmission occurs as many as ref _RCR_+1 Timeout interrupt is issued (@ref Sn_IR_TIMEOUT = '1').
 */
//M20150401 : Rename SYMBOE ( Re-define error in a compile)
//#define RCR                (_W5500_IO_BASE_ + (0x001B << 8) + (W5500_CREG_BLOCK << 3))  
#define W5500_REG_RCR                (_W5500_IO_BASE_ + (0x001B << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP LCP Request Timer register  in PPPoE mode(R/W)
 * @details @ref PTIMER configures the time for sending LCP echo request. The unit of time is 25ms.
 */
#define W5500_REG_PTIMER             (_W5500_IO_BASE_ + (0x001C << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP LCP Magic number register  in PPPoE mode(R/W)
 * @details @ref PMAGIC configures the 4bytes magic number to be used in LCP negotiation.
 */
#define W5500_REG_PMAGIC             (_W5500_IO_BASE_ + (0x001D << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP Destination MAC Register address(R/W)
 * @details @ref PHAR configures the PPPoE server hardware address that is acquired during PPPoE connection process.
 */
#define W5500_REG_PHAR                (_W5500_IO_BASE_ + (0x001E << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP Session Identification Register(R/W)
 * @details @ref PSID configures the PPPoE sever session ID acquired during PPPoE connection process.
 */
#define W5500_REG_PSID               (_W5500_IO_BASE_ + (0x0024 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PPP Maximum Segment Size(MSS) register(R/W)
 * @details @ref PMRU configures the maximum receive unit of PPPoE.
 */
#define W5500_REG_PMRU               (_W5500_IO_BASE_ + (0x0026 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Unreachable IP register address in UDP mode(R)
 * @details W5500 receives an ICMP packet(Destination port unreachable) when data is sent to a port number
 * which socket is not open and @ref IR_UNREACH bit of @ref IR becomes and @ref UIPR & @ref UPORTR indicates
 * the destination IP address & port number respectively.
 */
#define W5500_REG_UIPR               (_W5500_IO_BASE_ + (0x0028 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief Unreachable Port register address in UDP mode(R)
 * @details W5500 receives an ICMP packet(Destination port unreachable) when data is sent to a port number
 * which socket is not open and @ref IR_UNREACH bit of @ref IR becomes and @ref UIPR & @ref UPORTR
 * indicates the destination IP address & port number respectively.
 */
#define W5500_REG_UPORTR              (_W5500_IO_BASE_ + (0x002C << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief PHY Status Register(R/W)
 * @details @ref PHYCFGR configures PHY operation mode and resets PHY. In addition, @ref PHYCFGR indicates the status of PHY such as duplex, Speed, Link.
 */
#define W5500_REG_PHYCFGR            (_W5500_IO_BASE_ + (0x002E << 8) + (W5500_CREG_BLOCK << 3))

// Reserved			         (_W5500_IO_BASE_ + (0x002F << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0030 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0031 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0032 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0033 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0034 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0035 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0036 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0037 << 8) + (W5500_CREG_BLOCK << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0038 << 8) + (W5500_CREG_BLOCK << 3))

/**
 * @ingroup Common_register_group
 * @brief chip version register address(R)
 * @details @ref VERSIONR always indicates the W5500 version as @b 0x04.
 */
#define W5500_REG_VERSIONR           (_W5500_IO_BASE_ + (0x0039 << 8) + (W5500_CREG_BLOCK << 3))


//----------------------------- W5500 Socket Registers IOMAP -----------------------------
/**
 * @ingroup Socket_register_group
 * @brief socket Mode register(R/W)
 * @details @ref Sn_MR configures the option or protocol type of Socket n.\n\n
 * Each bit of @ref Sn_MR defined as the following.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>MULTI/MFEN</td> <td>BCASTB</td> <td>ND/MC/MMB</td> <td>UCASTB/MIP6B</td> <td>Protocol[3]</td> <td>Protocol[2]</td> <td>Protocol[1]</td> <td>Protocol[0]</td> </tr>
 * </table>
 * - @ref Sn_MR_MULTI	: Support UDP Multicasting
 * - @ref Sn_MR_BCASTB	: Broadcast block <b>in UDP Multicasting</b>
 * - @ref Sn_MR_ND		: No Delayed Ack(TCP) flag
 * - @ref Sn_MR_MC   	: IGMP version used <b>in UDP mulitcasting</b>
 * - @ref Sn_MR_MMB    	: Multicast Blocking <b>in @ref Sn_MR_MACRAW mode</b>
 * - @ref Sn_MR_UCASTB	: Unicast Block <b>in UDP Multicating</b>
 * - @ref Sn_MR_MIP6B   : IPv6 packet Blocking <b>in @ref Sn_MR_MACRAW mode</b>
 * - <b>Protocol</b>
 * <table>
 * 		<tr>   <td><b>Protocol[3]</b></td> <td><b>Protocol[2]</b></td> <td><b>Protocol[1]</b></td> <td><b>Protocol[0]</b></td> <td>@b Meaning</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>0</td> <td>Closed</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>1</td> <td>TCP</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>1</td> <td>0</td> <td>UDP</td>   </tr>
 * 		<tr>   <td>0</td> <td>1</td> <td>0</td> <td>0</td> <td>MACRAW</td>   </tr>
 * </table>
 *	- @ref Sn_MR_MACRAW	: MAC LAYER RAW SOCK \n
 *  - @ref Sn_MR_UDP		: UDP
 *  - @ref Sn_MR_TCP		: TCP
 *  - @ref Sn_MR_CLOSE	: Unused socket
 *  @note MACRAW mode should be only used in Socket 0.
 */
#define W5500_REG_Sn_MR(N)           (_W5500_IO_BASE_ + (0x0000 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Socket command register(R/W)
 * @details This is used to set the command for Socket n such as OPEN, CLOSE, CONNECT, LISTEN, SEND, and RECEIVE.\n
 * After W5500 accepts the command, the @ref Sn_CR register is automatically cleared to 0x00.
 * Even though @ref Sn_CR is cleared to 0x00, the command is still being processed.\n
 * To check whether the command is completed or not, please check the @ref Sn_IR or @ref Sn_SR.
 * - @ref Sn_CR_OPEN 		: Initialize or open socket.
 * - @ref Sn_CR_LISTEN 		: Wait connection request in TCP mode(<b>Server mode</b>)
 * - @ref Sn_CR_CONNECT 	: Send connection request in TCP mode(<b>Client mode</b>)
 * - @ref Sn_CR_DISCON 		: Send closing request in TCP mode.
 * - @ref Sn_CR_CLOSE   	: Close socket.
 * - @ref Sn_CR_SEND    	: Update TX buffer pointer and send data.
 * - @ref Sn_CR_SEND_MAC	: Send data with MAC address, so without ARP process.
 * - @ref Sn_CR_SEND_KEEP 	: Send keep alive message.
 * - @ref Sn_CR_RECV		: Update RX buffer pointer and receive data.
 */
#define W5500_REG_Sn_CR(N)           (_W5500_IO_BASE_ + (0x0001 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Socket interrupt register(R)
 * @details @ref Sn_IR indicates the status of Socket Interrupt such as establishment, termination, receiving data, timeout).\n
 * When an interrupt occurs and the corresponding bit of @ref Sn_IMR is  the corresponding bit of @ref Sn_IR becomes \n
 * In order to clear the @ref Sn_IR bit, the host should write the bit to \n
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>Reserved</td> <td>Reserved</td> <td>Reserved</td> <td>SEND_OK</td> <td>TIMEOUT</td> <td>RECV</td> <td>DISCON</td> <td>CON</td> </tr>
 * </table>
 * - \ref Sn_IR_SENDOK : <b>SEND_OK Interrupt</b>
 * - \ref Sn_IR_TIMEOUT : <b>TIMEOUT Interrupt</b>
 * - \ref Sn_IR_RECV : <b>RECV Interrupt</b>
 * - \ref Sn_IR_DISCON : <b>DISCON Interrupt</b>
 * - \ref Sn_IR_CON : <b>CON Interrupt</b>
 */
#define W5500_REG_Sn_IR(N)           (_W5500_IO_BASE_ + (0x0002 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Socket status register(R)
 * @details @ref Sn_SR indicates the status of Socket n.\n
 * The status of Socket n is changed by @ref Sn_CR or some special control packet as SYN, FIN packet in TCP.
 * @par Normal status
 * - @ref SOCK_CLOSED 		: Closed
 * - @ref SOCK_INIT   		: Initiate state
 * - @ref SOCK_LISTEN    	: Listen state
 * - @ref SOCK_ESTABLISHED 	: Success to connect
 * - @ref SOCK_CLOSE_WAIT   : Closing state
 * - @ref SOCK_UDP   		: UDP socket
 * - @ref SOCK_MACRAW  		: MAC raw mode socket
 *@par Temporary status during changing the status of Socket n.
 * - @ref SOCK_SYNSENT   	: This indicates Socket n sent the connect-request packet (SYN packet) to a peer.
 * - @ref SOCK_SYNRECV    	: It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.
 * - @ref SOCK_FIN_WAIT		: Connection state
 * - @ref SOCK_CLOSING		: Closing state
 * - @ref SOCK_TIME_WAIT	: Closing state
 * - @ref SOCK_LAST_ACK 	: Closing state
 */
#define W5500_REG_Sn_SR(N)           (_W5500_IO_BASE_ + (0x0003 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief source port register(R/W)
 * @details @ref Sn_PORT configures the source port number of Socket n.
 * It is valid when Socket n is used in TCP/UDP mode. It should be set before OPEN command is ordered.
 */
#define W5500_REG_Sn_PORT(N)         (_W5500_IO_BASE_ + (0x0004 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Peer MAC register address(R/W)
 * @details @ref Sn_DHAR configures the destination hardware address of Socket n when using SEND_MAC command in UDP mode or
 * it indicates that it is acquired in ARP-process by CONNECT/SEND command.
 */
#define W5500_REG_Sn_DHAR(N)         (_W5500_IO_BASE_ + (0x0006 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Peer IP register address(R/W)
 * @details @ref Sn_DIPR configures or indicates the destination IP address of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP client mode, it configures an IP address of TCP serverbefore CONNECT command.
 * In TCP server mode, it indicates an IP address of TCP clientafter successfully establishing connection.
 * In UDP mode, it configures an IP address of peer to be received the UDP packet by SEND or SEND_MAC command.
 */
#define W5500_REG_Sn_DIPR(N)         (_W5500_IO_BASE_ + (0x000C << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Peer port register address(R/W)
 * @details @ref Sn_DPORT configures or indicates the destination port number of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP clientmode, it configures the listen port number of TCP serverbefore CONNECT command.
 * In TCP Servermode, it indicates the port number of TCP client after successfully establishing connection.
 * In UDP mode, it configures the port number of peer to be transmitted the UDP packet by SEND/SEND_MAC command.
 */
#define W5500_REG_Sn_DPORT(N)        (_W5500_IO_BASE_ + (0x0010 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Maximum Segment Size(Sn_MSSR0) register address(R/W)
 * @details @ref Sn_MSSR configures or indicates the MTU(Maximum Transfer Unit) of Socket n.
 */
#define W5500_REG_Sn_MSSR(N)         (_W5500_IO_BASE_ + (0x0012 << 8) + (W5500_SREG_BLOCK(N) << 3))

// Reserved			         (_W5500_IO_BASE_ + (0x0014 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief IP Type of Service(TOS) Register(R/W)
 * @details @ref Sn_TOS configures the TOS(Type Of Service field in IP Header) of Socket n.
 * It is set before OPEN command.
 */
#define W5500_REG_Sn_TOS(N)          (_W5500_IO_BASE_ + (0x0015 << 8) + (W5500_SREG_BLOCK(N) << 3))
/**
 * @ingroup Socket_register_group
 * @brief IP Time to live(TTL) Register(R/W)
 * @details @ref Sn_TTL configures the TTL(Time To Live field in IP header) of Socket n.
 * It is set before OPEN command.
 */
#define W5500_REG_Sn_TTL(N)          (_W5500_IO_BASE_ + (0x0016 << 8) + (W5500_SREG_BLOCK(N) << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0017 << 8) + (W5500_SREG_BLOCK(N) << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x0018 << 8) + (W5500_SREG_BLOCK(N) << 3)) 
// Reserved			         (_W5500_IO_BASE_ + (0x0019 << 8) + (W5500_SREG_BLOCK(N) << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x001A << 8) + (W5500_SREG_BLOCK(N) << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x001B << 8) + (W5500_SREG_BLOCK(N) << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x001C << 8) + (W5500_SREG_BLOCK(N) << 3))
// Reserved			         (_W5500_IO_BASE_ + (0x001D << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Receive memory size register(R/W)
 * @details @ref Sn_RXBUF_SIZE configures the RX buffer block size of Socket n.
 * Socket n RX Buffer Block size can be configured with 1,2,4,8, and 16 Kbytes.
 * If a different size is configured, the data cannot be normally received from a peer.
 * Although Socket n RX Buffer Block size is initially configured to 2Kbytes,
 * user can re-configure its size using @ref Sn_RXBUF_SIZE. The total sum of @ref Sn_RXBUF_SIZE can not be exceed 16Kbytes.
 * When exceeded, the data reception error is occurred.
 */
#define W5500_REG_Sn_RXBUF_SIZE(N)   (_W5500_IO_BASE_ + (0x001E << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit memory size register(R/W)
 * @details @ref Sn_TXBUF_SIZE configures the TX buffer block size of Socket n. Socket n TX Buffer Block size can be configured with 1,2,4,8, and 16 Kbytes.
 * If a different size is configured, the data can�t be normally transmitted to a peer.
 * Although Socket n TX Buffer Block size is initially configured to 2Kbytes,
 * user can be re-configure its size using @ref Sn_TXBUF_SIZE. The total sum of @ref Sn_TXBUF_SIZE can not be exceed 16Kbytes.
 * When exceeded, the data transmission error is occurred.
 */
#define W5500_REG_Sn_TXBUF_SIZE(N)   (_W5500_IO_BASE_ + (0x001F << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit free memory size register(R)
 * @details @ref Sn_TX_FSR indicates the free size of Socket n TX Buffer Block. It is initialized to the configured size by @ref Sn_TXBUF_SIZE.
 * Data bigger than @ref Sn_TX_FSR should not be saved in the Socket n TX Buffer because the bigger data overwrites the previous saved data not yet sent.
 * Therefore, check before saving the data to the Socket n TX Buffer, and if data is equal or smaller than its checked size,
 * transmit the data with SEND/SEND_MAC command after saving the data in Socket n TX buffer. But, if data is bigger than its checked size,
 * transmit the data after dividing into the checked size and saving in the Socket n TX buffer.
 */
#define W5500_REG_Sn_TX_FSR(N)       (_W5500_IO_BASE_ + (0x0020 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit memory read pointer register address(R)
 * @details @ref Sn_TX_RD is initialized by OPEN command. However, if Sn_MR(P[3:0]) is TCP mode(001, it is re-initialized while connecting with TCP.
 * After its initialization, it is auto-increased by SEND command.
 * SEND command transmits the saved data from the current @ref Sn_TX_RD to the @ref Sn_TX_WR in the Socket n TX Buffer.
 * After transmitting the saved data, the SEND command increases the @ref Sn_TX_RD as same as the @ref Sn_TX_WR.
 * If its increment value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.
 */
#define W5500_REG_Sn_TX_RD(N)        (_W5500_IO_BASE_ + (0x0022 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Transmit memory write pointer register address(R/W)
 * @details @ref Sn_TX_WR is initialized by OPEN command. However, if Sn_MR(P[3:0]) is TCP mode(001, it is re-initialized while connecting with TCP.\n
 * It should be read or be updated like as follows.\n
 * 1. Read the starting address for saving the transmitting data.\n
 * 2. Save the transmitting data from the starting address of Socket n TX buffer.\n
 * 3. After saving the transmitting data, update @ref Sn_TX_WR to the increased value as many as transmitting data size.
 * If the increment value exceeds the maximum value 0xFFFF(greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.\n
 * 4. Transmit the saved data in Socket n TX Buffer by using SEND/SEND command
 */
#define W5500_REG_Sn_TX_WR(N)        (_W5500_IO_BASE_ + (0x0024 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Received data size register(R)
 * @details @ref Sn_RX_RSR indicates the data size received and saved in Socket n RX Buffer.
 * @ref Sn_RX_RSR does not exceed the @ref Sn_RXBUF_SIZE and is calculated as the difference between
 * �Socket n RX Write Pointer (@ref Sn_RX_WR)and �Socket n RX Read Pointer (@ref Sn_RX_RD)
 */
#define W5500_REG_Sn_RX_RSR(N)       (_W5500_IO_BASE_ + (0x0026 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Read point of Receive memory(R/W)
 * @details @ref Sn_RX_RD is initialized by OPEN command. Make sure to be read or updated as follows.\n
 * 1. Read the starting save address of the received data.\n
 * 2. Read data from the starting address of Socket n RX Buffer.\n
 * 3. After reading the received data, Update @ref Sn_RX_RD to the increased value as many as the reading size.
 * If the increment value exceeds the maximum value 0xFFFF, that is, is greater than 0x10000 and the carry bit occurs,
 * update with the lower 16bits value ignored the carry bit.\n
 * 4. Order RECV command is for notifying the updated @ref Sn_RX_RD to W5500.
 */
#define W5500_REG_Sn_RX_RD(N)        (_W5500_IO_BASE_ + (0x0028 << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Write point of Receive memory(R)
 * @details @ref Sn_RX_WR is initialized by OPEN command and it is auto-increased by the data reception.
 * If the increased value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.
 */
#define W5500_REG_Sn_RX_WR(N)        (_W5500_IO_BASE_ + (0x002A << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief socket interrupt mask register(R)
 * @details @ref Sn_IMR masks the interrupt of Socket n.
 * Each bit corresponds to each bit of @ref Sn_IR. When a Socket n Interrupt is occurred and the corresponding bit of @ref Sn_IMR is 
 * the corresponding bit of @ref Sn_IR becomes  When both the corresponding bit of @ref Sn_IMR and @ref Sn_IR are and the n-th bit of @ref IR is 
 * Host is interrupted by asserted INTn PIN to low.
 */
#define W5500_REG_Sn_IMR(N)          (_W5500_IO_BASE_ + (0x002C << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Fragment field value in IP header register(R/W)
 * @details @ref Sn_FRAG configures the FRAG(Fragment field in IP header).
 */
#define W5500_REG_Sn_FRAG(N)         (_W5500_IO_BASE_ + (0x002D << 8) + (W5500_SREG_BLOCK(N) << 3))

/**
 * @ingroup Socket_register_group
 * @brief Keep Alive Timer register(R/W)
 * @details @ref Sn_KPALVTR configures the transmitting timer of �KEEP ALIVE(KA)packet of SOCKETn. It is valid only in TCP mode,
 * and ignored in other modes. The time unit is 5s.
 * KA packet is transmittable after @ref Sn_SR is changed to SOCK_ESTABLISHED and after the data is transmitted or received to/from a peer at least once.
 * In case of '@ref Sn_KPALVTR > 0', W5500 automatically transmits KA packet after time-period for checking the TCP connection (Auto-keepalive-process).
 * In case of '@ref Sn_KPALVTR = 0', Auto-keep-alive-process will not operate,
 * and KA packet can be transmitted by SEND_KEEP command by the host (Manual-keep-alive-process).
 * Manual-keep-alive-process is ignored in case of '@ref Sn_KPALVTR > 0'.
 */
#define W5500_REG_Sn_KPALVTR(N)      (_W5500_IO_BASE_ + (0x002F << 8) + (W5500_SREG_BLOCK(N) << 3))

//#define Sn_TSR(N)          (_W5500_IO_BASE_ + (0x0030 << 8) + (W5500_SREG_BLOCK(N) << 3))


//----------------------------- W5500 Register values  -----------------------------

/* MODE register values */
/**
 * @brief Reset
 * @details If this bit is  All internal registers will be initialized. It will be automatically cleared as after S/W reset.
 */
#define W5500_MR_RST                       0x80

/**
 * @brief Wake on LAN
 * @details 0 : Disable WOL mode\n
 * 1 : Enable WOL mode\n
 * If WOL mode is enabled and the received magic packet over UDP has been normally processed, the Interrupt PIN (INTn) asserts to low.
 * When using WOL mode, the UDP Socket should be opened with any source port number. (Refer to Socket n Mode Register (@ref Sn_MR) for opening Socket.)
 * @note The magic packet over UDP supported by W5500 consists of 6 bytes synchronization stream (xFFFFFFFFFFFF and
 * 16 times Target MAC address stream in UDP payload. The options such like password are ignored. You can use any UDP source port number for WOL mode.
 */
#define W5500_MR_WOL                       0x20

/**
 * @brief Ping block
 * @details 0 : Disable Ping block\n
 * 1 : Enable Ping block\n
 * If the bit is  it blocks the response to a ping request.
 */
#define W5500_MR_PB                        0x10

/**
 * @brief Enable PPPoE
 * @details 0 : DisablePPPoE mode\n
 * 1 : EnablePPPoE mode\n
 * If you use ADSL, this bit should be 
 */
#define W5500_MR_PPPOE                     0x08

/**
 * @brief Enable UDP_FORCE_ARP CHECHK
 * @details 0 : Disable Force ARP mode\n
 * 1 : Enable Force ARP mode\n
 * In Force ARP mode, It forces on sending ARP Request whenever data is sent.
 */
#define W5500_MR_FARP                      0x02

/* IR register values */
/**
 * @brief Check IP conflict.
 * @details Bit is set as when own source IP address is same with the sender IP address in the received ARP request.
 */
#define W5500_IR_CONFLICT                  0x80

/**
 * @brief Get the destination unreachable message in UDP sending.
 * @details When receiving the ICMP (Destination port unreachable) packet, this bit is set as 
 * When this bit is  Destination Information such as IP address and Port number may be checked with the corresponding @ref UIPR & @ref UPORTR.
 */
#define W5500_IR_UNREACH                   0x40

/**
 * @brief Get the PPPoE close message.
 * @details When PPPoE is disconnected during PPPoE mode, this bit is set.
 */
#define W5500_IR_PPPoE                     0x20

/**
 * @brief Get the magic packet interrupt.
 * @details When WOL mode is enabled and receives the magic packet over UDP, this bit is set.
 */
#define W5500_IR_MP                        0x10


/* PHYCFGR register value */
#define W5500_PHYCFGR_RST                  ~(1<<7)  //< For PHY reset, must operate AND mask.
#define W5500_PHYCFGR_OPMD                 (1<<6)   // Configre PHY with OPMDC value
#define W5500_PHYCFGR_OPMDC_ALLA           (7<<3)
#define W5500_PHYCFGR_OPMDC_PDOWN          (6<<3)
#define W5500_PHYCFGR_OPMDC_NA             (5<<3)
#define W5500_PHYCFGR_OPMDC_100FA          (4<<3)
#define W5500_PHYCFGR_OPMDC_100F           (3<<3)
#define W5500_PHYCFGR_OPMDC_100H           (2<<3)
#define W5500_PHYCFGR_OPMDC_10F            (1<<3)
#define W5500_PHYCFGR_OPMDC_10H            (0<<3)           
#define W5500_PHYCFGR_DPX_FULL             (1<<2)
#define W5500_PHYCFGR_DPX_HALF             (0<<2)
#define W5500_PHYCFGR_SPD_100              (1<<1)
#define W5500_PHYCFGR_SPD_10               (0<<1)
#define W5500_PHYCFGR_LNK_ON               (1<<0)
#define W5500_PHYCFGR_LNK_OFF              (0<<0)

/* IMR register values */
/**
 * @brief IP Conflict Interrupt Mask.
 * @details 0: Disable IP Conflict Interrupt\n
 * 1: Enable IP Conflict Interrupt
 */
#define W5500_IM_IR7                  	 0x80

/**
 * @brief Destination unreachable Interrupt Mask.
 * @details 0: Disable Destination unreachable Interrupt\n
 * 1: Enable Destination unreachable Interrupt
 */
#define W5500_IM_IR6                  	 0x40

/**
 * @brief PPPoE Close Interrupt Mask.
 * @details 0: Disable PPPoE Close Interrupt\n
 * 1: Enable PPPoE Close Interrupt
 */
#define W5500_IM_IR5                  	 0x20

/**
 * @brief Magic Packet Interrupt Mask.
 * @details 0: Disable Magic Packet Interrupt\n
 * 1: Enable Magic Packet Interrupt
 */
#define W5500_IM_IR4                  	 0x10

/* Sn_MR Default values */
/**
 * @brief Support UDP Multicasting
 * @details 0 : disable Multicasting\n
 * 1 : enable Multicasting\n
 * This bit is applied only during UDP mode(P[3:0] = 010.\n
 * To use multicasting, @ref Sn_DIPR & @ref Sn_DPORT should be respectively configured with the multicast group IP address & port number
 * before Socket n is opened by OPEN command of @ref Sn_CR.
 */
#define W5500_Sn_MR_MULTI                  0x80

/**
 * @brief Broadcast block in UDP Multicasting.
 * @details 0 : disable Broadcast Blocking\n
 * 1 : enable Broadcast Blocking\n
 * This bit blocks to receive broadcasting packet during UDP mode(P[3:0] = 010.\m
 * In addition, This bit does when MACRAW mode(P[3:0] = 100
 */
#define W5500_Sn_MR_BCASTB                 0x40

/**
 * @brief No Delayed Ack(TCP), Multicast flag
 * @details 0 : Disable No Delayed ACK option\n
 * 1 : Enable No Delayed ACK option\n
 * This bit is applied only during TCP mode (P[3:0] = 001.\n
 * When this bit is  It sends the ACK packet without delay as soon as a Data packet is received from a peer.\n
 * When this bit is  It sends the ACK packet after waiting for the timeout time configured by @ref _RTR_.
 */
#define W5500_Sn_MR_ND                     0x20

/**
 * @brief Unicast Block in UDP Multicasting
 * @details 0 : disable Unicast Blocking\n
 * 1 : enable Unicast Blocking\n
 * This bit blocks receiving the unicast packet during UDP mode(P[3:0] = 010 and MULTI = 
 */
#define W5500_Sn_MR_UCASTB                 0x10

/**
 * @brief MAC LAYER RAW SOCK
 * @details This configures the protocol mode of Socket n.
 * @note MACRAW mode should be only used in Socket 0.
 */
#define W5500_Sn_MR_MACRAW                 0x04

#define W5500_Sn_MR_IPRAW                  0x03     /**< IP LAYER RAW SOCK */

/**
 * @brief UDP
 * @details This configures the protocol mode of Socket n.
 */
#define W5500_Sn_MR_UDP                    0x02

/**
 * @brief TCP
 * @details This configures the protocol mode of Socket n.
 */
#define W5500_Sn_MR_TCP                    0x01

/**
 * @brief Unused socket
 * @details This configures the protocol mode of Socket n.
 */
#define W5500_Sn_MR_CLOSE                  0x00

/* Sn_MR values used with Sn_MR_MACRAW */
/**
 * @brief MAC filter enable in @ref Sn_MR_MACRAW mode
 * @details 0 : disable MAC Filtering\n
 * 1 : enable MAC Filtering\n
 * This bit is applied only during MACRAW mode(P[3:0] = 100.\n
 * When set as  W5500 can only receive broadcasting packet or packet sent to itself.
 * When this bit is  W5500 can receive all packets on Ethernet.
 * If user wants to implement Hybrid TCP/IP stack,
 * it is recommended that this bit is set as for reducing host overhead to process the all received packets.
 */
#define W5500_Sn_MR_MFEN                   Sn_MR_MULTI

/**
 * @brief Multicast Blocking in @ref Sn_MR_MACRAW mode
 * @details 0 : using IGMP version 2\n
 * 1 : using IGMP version 1\n
 * This bit is applied only during UDP mode(P[3:0] = 010 and MULTI = 
 * It configures the version for IGMP messages (Join/Leave/Report).
 */
#define W5500_Sn_MR_MMB                    Sn_MR_ND

/**
 * @brief IPv6 packet Blocking in @ref Sn_MR_MACRAW mode
 * @details 0 : disable IPv6 Blocking\n
 * 1 : enable IPv6 Blocking\n
 * This bit is applied only during MACRAW mode (P[3:0] = 100. It blocks to receiving the IPv6 packet.
 */
#define W5500_Sn_MR_MIP6B                  Sn_MR_UCASTB

/* Sn_MR value used with Sn_MR_UDP & Sn_MR_MULTI */
/**
 * @brief IGMP version used in UDP mulitcasting
 * @details 0 : disable Multicast Blocking\n
 * 1 : enable Multicast Blocking\n
 * This bit is applied only when MACRAW mode(P[3:0] = 100. It blocks to receive the packet with multicast MAC address.
 */
#define W5500_Sn_MR_MC                     Sn_MR_ND

/* Sn_MR alternate values */
/**
 * @brief For Berkeley Socket API
 */
#define W5500_SOCK_STREAM                  Sn_MR_TCP

/**
 * @brief For Berkeley Socket API
 */
#define W5500_SOCK_DGRAM                   Sn_MR_UDP


/* Sn_CR values */
/**
 * @brief Initialize or open socket
 * @details Socket n is initialized and opened according to the protocol selected in Sn_MR(P3:P0).
 * The table below shows the value of @ref Sn_SR corresponding to @ref Sn_MR.\n
 * <table>
 *   <tr>  <td>\b Sn_MR (P[3:0])</td> <td>\b Sn_SR</td>            		 </tr>
 *   <tr>  <td>Sn_MR_CLOSE  (000)</td> <td></td>         	   		 </tr>
 *   <tr>  <td>Sn_MR_TCP  (001)</td> <td>SOCK_INIT (0x13)</td>  		 </tr>
 *   <tr>  <td>Sn_MR_UDP  (010)</td>  <td>SOCK_UDP (0x22)</td>  		 </tr>
 *   <tr>  <td>S0_MR_MACRAW  (100)</td>  <td>SOCK_MACRAW (0x02)</td>  </tr>
 * </table>
 */
#define W5500_Sn_CR_OPEN                   0x01

/**
 * @brief Wait connection request in TCP mode(Server mode)
 * @details This is valid only in TCP mode (\ref Sn_MR(P3:P0) = \ref Sn_MR_TCP).
 * In this mode, Socket n operates as a TCP serverand waits for  connection-request (SYN packet) from any TCP client
 * The @ref Sn_SR changes the state from \ref SOCK_INIT to \ref SOCKET_LISTEN.
 * When a TCP clientconnection request is successfully established,
 * the @ref Sn_SR changes from SOCK_LISTEN to SOCK_ESTABLISHED and the @ref Sn_IR(0) becomes 
 * But when a TCP clientconnection request is failed, @ref Sn_IR(3) becomes and the status of @ref Sn_SR changes to SOCK_CLOSED.
 */
#define W5500_Sn_CR_LISTEN                 0x02

/**
 * @brief Send connection request in TCP mode(Client mode)
 * @details  To connect, a connect-request (SYN packet) is sent to <b>TCP server</b>configured by @ref Sn_DIPR & Sn_DPORT(destination address & port).
 * If the connect-request is successful, the @ref Sn_SR is changed to @ref SOCK_ESTABLISHED and the Sn_IR(0) becomes \n\n
 * The connect-request fails in the following three cases.\n
 * 1. When a @b ARPTO occurs (@ref Sn_IR[3] =  ) because destination hardware address is not acquired through the ARP-process.\n
 * 2. When a @b SYN/ACK packet is not received and @b TCPTO (Sn_IR(3) =  )\n
 * 3. When a @b RST packet is received instead of a @b SYN/ACK packet. In these cases, @ref Sn_SR is changed to @ref SOCK_CLOSED.
 * @note This is valid only in TCP mode and operates when Socket n acts as <b>TCP client</b>
 */
#define W5500_Sn_CR_CONNECT                0x04

/**
 * @brief Send closing request in TCP mode
 * @details Regardless of <b>TCP server</b>or <b>TCP client</b> the DISCON command processes the disconnect-process (b>Active close</b>or <b>Passive close</b>.\n
 * @par Active close
 * it transmits disconnect-request(FIN packet) to the connected peer\n
 * @par Passive close
 * When FIN packet is received from peer, a FIN packet is replied back to the peer.\n
 * @details When the disconnect-process is successful (that is, FIN/ACK packet is received successfully), @ref Sn_SR is changed to @ref SOCK_CLOSED.\n
 * Otherwise, TCPTO occurs (\ref Sn_IR(3)='1') and then @ref Sn_SR is changed to @ref SOCK_CLOSED.
 * @note Valid only in TCP mode.
 */
#define W5500_Sn_CR_DISCON                 0x08

/**
 * @brief Close socket
 * @details Sn_SR is changed to @ref SOCK_CLOSED.
 */
#define W5500_Sn_CR_CLOSE                  0x10

/**
 * @brief Update TX buffer pointer and send data
 * @details SEND transmits all the data in the Socket n TX buffer.\n
 * For more details, please refer to Socket n TX Free Size Register (@ref Sn_TX_FSR), Socket n,
 * TX Write Pointer Register(@ref Sn_TX_WR), and Socket n TX Read Pointer Register(@ref Sn_TX_RD).
 */
#define W5500_Sn_CR_SEND                   0x20

/**
 * @brief Send data with MAC address, so without ARP process
 * @details The basic operation is same as SEND.\n
 * Normally SEND transmits data after destination hardware address is acquired by the automatic ARP-process(Address Resolution Protocol).\n
 * But SEND_MAC transmits data without the automatic ARP-process.\n
 * In this case, the destination hardware address is acquired from @ref Sn_DHAR configured by host, instead of APR-process.
 * @note Valid only in UDP mode.
 */
#define W5500_Sn_CR_SEND_MAC               0x21

/**
 * @brief Send keep alive message
 * @details It checks the connection status by sending 1byte keep-alive packet.\n
 * If the peer can not respond to the keep-alive packet during timeout time, the connection is terminated and the timeout interrupt will occur.
 * @note Valid only in TCP mode.
 */
#define W5500_Sn_CR_SEND_KEEP              0x22

/**
 * @brief Update RX buffer pointer and receive data
 * @details RECV completes the processing of the received data in Socket n RX Buffer by using a RX read pointer register (@ref Sn_RX_RD).\n
 * For more details, refer to Socket n RX Received Size Register (@ref Sn_RX_RSR), Socket n RX Write Pointer Register (@ref Sn_RX_WR),
 * and Socket n RX Read Pointer Register (@ref Sn_RX_RD).
 */
#define W5500_Sn_CR_RECV                   0x40

/* Sn_IR values */
/**
 * @brief SEND_OK Interrupt
 * @details This is issued when SEND command is completed.
 */
#define W5500_Sn_IR_SENDOK                 0x10

/**
 * @brief TIMEOUT Interrupt
 * @details This is issued when ARPTO or TCPTO occurs.
 */
#define W5500_Sn_IR_TIMEOUT                0x08

/**
 * @brief RECV Interrupt
 * @details This is issued whenever data is received from a peer.
 */
#define W5500_Sn_IR_RECV                   0x04

/**
 * @brief DISCON Interrupt
 * @details This is issued when FIN or FIN/ACK packet is received from a peer.
 */
#define W5500_Sn_IR_DISCON                 0x02

/**
 * @brief CON Interrupt
 * @details This is issued one time when the connection with peer is successful and then @ref Sn_SR is changed to @ref SOCK_ESTABLISHED.
 */
#define W5500_Sn_IR_CON                    0x01

/* Sn_SR values */
/**
 * @brief Closed
 * @details This indicates that Socket n is released.\n
 * When DICON, CLOSE command is ordered, or when a timeout occurs, it is changed to @ref SOCK_CLOSED regardless of previous status.
 */
#define W5500_SOCK_CLOSED                  0x00

/**
 * @brief Initiate state
 * @details This indicates Socket n is opened with TCP mode.\n
 * It is changed to @ref SOCK_INIT when @ref Sn_MR(P[3:0]) = 001 and OPEN command is ordered.\n
 * After @ref SOCK_INIT, user can use LISTEN /CONNECT command.
 */
#define W5500_SOCK_INIT                    0x13

/**
 * @brief Listen state
 * @details This indicates Socket n is operating as <b>TCP server</b>mode and waiting for connection-request (SYN packet) from a peer <b>TCP client</b>.\n
 * It will change to @ref SOCK_ESTALBLISHED when the connection-request is successfully accepted.\n
 * Otherwise it will change to @ref SOCK_CLOSED after TCPTO @ref Sn_IR(TIMEOUT) = '1') is occurred.
 */
#define W5500_SOCK_LISTEN                  0x14

/**
 * @brief Connection state
 * @details This indicates Socket n sent the connect-request packet (SYN packet) to a peer.\n
 * It is temporarily shown when @ref Sn_SR is changed from @ref SOCK_INIT to @ref SOCK_ESTABLISHED by CONNECT command.\n
 * If connect-accept(SYN/ACK packet) is received from the peer at SOCK_SYNSENT, it changes to @ref SOCK_ESTABLISHED.\n
 * Otherwise, it changes to @ref SOCK_CLOSED after TCPTO (@ref Sn_IR[TIMEOUT] = '1') is occurred.
 */
#define W5500_SOCK_SYNSENT                 0x15

/**
 * @brief Connection state
 * @details It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.\n
 * If socket n sends the response (SYN/ACK  packet) to the peer successfully,  it changes to @ref SOCK_ESTABLISHED. \n
 * If not, it changes to @ref SOCK_CLOSED after timeout (@ref Sn_IR[TIMEOUT] = '1') is occurred.
 */
#define W5500_SOCK_SYNRECV                 0x16

/**
 * @brief Success to connect
 * @details This indicates the status of the connection of Socket n.\n
 * It changes to @ref SOCK_ESTABLISHED when the <b>TCP SERVER</b>processed the SYN packet from the <b>TCP CLIENT</b>during @ref SOCK_LISTEN, or
 * when the CONNECT command is successful.\n
 * During @ref SOCK_ESTABLISHED, DATA packet can be transferred using SEND or RECV command.
 */
#define W5500_SOCK_ESTABLISHED             0x17

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to @ref SOCK_CLOSED.
 */
#define W5500_SOCK_FIN_WAIT                0x18

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to @ref SOCK_CLOSED.
 */
#define W5500_SOCK_CLOSING                 0x1A

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to @ref SOCK_CLOSED.
 */
#define W5500_SOCK_TIME_WAIT               0x1B

/**
 * @brief Closing state
 * @details This indicates Socket n received the disconnect-request (FIN packet) from the connected peer.\n
 * This is half-closing status, and data can be transferred.\n
 * For full-closing, DISCON command is used. But For just-closing, CLOSE command is used.
 */
#define W5500_SOCK_CLOSE_WAIT              0x1C

/**
 * @brief Closing state
 * @details This indicates Socket n is waiting for the response (FIN/ACK packet) to the disconnect-request (FIN packet) by passive-close.\n
 * It changes to @ref SOCK_CLOSED when Socket n received the response successfully, or when timeout(@ref Sn_IR[TIMEOUT] = '1') is occurred.
 */
#define W5500_SOCK_LAST_ACK                0x1D

/**
 * @brief UDP socket
 * @details This indicates Socket n is opened in UDP mode(@ref Sn_MR(P[3:0]) = '010').\n
 * It changes to SOCK_UDP when @ref Sn_MR(P[3:0]) = '010' and @ref Sn_CR_OPEN command is ordered.\n
 * Unlike TCP mode, data can be transfered without the connection-process.
 */
#define W5500_SOCK_UDP                     0x22

#define W5500_SOCK_IPRAW                   0x32     /**< IP raw mode socket */

/**
 * @brief MAC raw mode socket
 * @details This indicates Socket 0 is opened in MACRAW mode (S0_MR(P[3:0]) = 100and is valid only in Socket 0.\n
 * It changes to SOCK_MACRAW when S0_MR(P[3:0] = 100and OPEN command is ordered.\n
 * Like UDP mode socket, MACRAW mode Socket 0 can transfer a MAC packet (Ethernet frame) without the connection-process.
 */
#define W5500_SOCK_MACRAW                  0x42

//#define SOCK_PPPOE                   0x5F

/* IP PROTOCOL */
#define W5500_IPPROTO_IP                   0        //< Dummy for IP 
#define W5500_IPPROTO_ICMP                 1        //< Control message protocol
#define W5500_IPPROTO_IGMP                 2        //< Internet group management protocol
#define W5500_IPPROTO_GGP                  3        //< Gateway^2 (deprecated)
#define W5500_IPPROTO_TCP                  6        //< TCP
#define W5500_IPPROTO_PUP                  12       //< PUP
#define W5500_IPPROTO_UDP                  17       //< UDP
#define W5500_IPPROTO_IDP                  22       //< XNS idp
#define W5500_IPPROTO_ND                   77       //< UNOFFICIAL net disk protocol
#define W5500_IPPROTO_RAW                  255      //< Raw IP packet

#ifdef __cplusplus
 }
#endif
