#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define _W5100S_SPI_READ_               (0x0F)
#define _W5100S_SPI_WRITE_              (0xF0)

#define _W5100S_IO_BASE_    			(0x0000)
#define _W5100S_SN_BASE_  				(0x0400)
#define _W5100S_SN_SIZE_  				(0x0100)
#define _W5100S_IO_TXBUF_ 				(0x4000) /* Internal Tx buffer address of the iinchip */
#define _W5100S_IO_RXBUF_ 				(0x6000) /* Internal Rx buffer address of the iinchip */

#define W5100S_CREG_BLOCK      	      	0x00   ///< Common register block
#define W5100S_SREG_BLOCK(N)    	   	(_W5100S_SN_BASE_+ _W5100S_SN_SIZE_*N) ///< Socket N register block

#define W5100S_OFFSET_INC(ADDR, N)    	(ADDR + N) ///< Increase offset address

#define W5100S_IDM_OR             		((_W5100S_IO_BASE  + 0x0000))
#define W5100S_IDM_AR0            		((_W5100S_IO_BASE_ + 0x0001))
#define W5100S_IDM_AR1            		((_W5100S_IO_BASE_ + 0x0002))
#define W5100S_IDM_DR             		((_W5100S_IO_BASE_ + 0x0003))


//----------------------------- W5100S Common Registers IOMAP -----------------------------
/**
 * @ingroup Common_register_group_W5100S
 * @brief Mode Register address(R/W)\n
 * \ref MR is used for S/W reset, ping block mode, PPPoE mode and etc.
 * @details Each bit of \ref MR defined as follows.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>RST</td> <td>Reserved</td> <td>WOL</td> <td>PB</td> <td>PPPoE</td> <td>Reserved</td> <td>AI</td> <td>IND</td> </tr>
 * </table>
 * - \ref MR_RST		 	: Reset
 * - \ref MR_PB         	: Ping block
 * - \ref MR_PPPOE      	: PPPoE mode
 * - \ref MR_AI         	: Address Auto-Increment in Indirect Bus Interface
 * - \ref MR_IND         	: Indirect Bus Interface mode
 */
#define W5100S_REG_MR					(_W5100S_IO_BASE_ + (0x0000))  // Mode


/**
 * @ingroup Common_register_group_W5100S
 * @brief Gateway IP Register address(R/W)
 * @details \ref GAR configures the default gateway address.
 */
#define W5100S_REG_GAR     			(_W5100S_IO_BASE_ + (0x0001))  // GW Address

/**
 * @ingroup Common_register_group_W5100S
 * @brief Subnet mask Register address(R/W)
 * @details \ref SUBR configures the subnet mask address.
 */
#define W5100S_REG_SUBR    			(_W5100S_IO_BASE_ + (0x0005)) // SN Mask Address

/**
 * @ingroup Common_register_group_W5100S
 * @brief Source MAC Register address(R/W)
 * @details \ref SHAR configures the source hardware address.
 */
#define W5100S_REG_SHAR    			(_W5100S_IO_BASE_ + (0x0009)) // Source Hardware Address

/**
 * @ingroup Common_register_group_W5100S
 * @brief Source IP Register address(R/W)
 * @details \ref SIPR configures the source IP address.
 */
#define W5100S_REG_SIPR    			(_W5100S_IO_BASE_ + (0x000F)) // Source IP Address

#define	W5100S_REG_INTPTMR	 	    (_W5100S_IO_BASE_ + (0x0013))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Interrupt Register(R/W)
 * @details \ref IR indicates the interrupt status. Each bit of \ref IR will be still until the bit will be written to by the host.
 * If \ref IR is not equal to x00 INTn PIN is asserted to low until it is x00\n\n
 * Each bit of \ref IR defined as follows.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>CONFLICT</td> <td>UNREACH</td> <td>PPPoE</td> <td>Reserved</td> <td>S3_INT</td> <td>S2_INT</td> <td>S1_INT</td> <td>S0_INT</td> </tr>
 * </table>
 * - \ref IR_CONFLICT : IP conflict
 * - \ref IR_UNREACH  : Destination unreachable
 * - \ref IR_PPPoE	 : PPPoE connection close
 * - \ref IR_SOCK(3)  : SOCKET 3 Interrupt
 * - \ref IR_SOCK(2)  : SOCKET 2 Interrupt
 * - \ref IR_SOCK(1)  : SOCKET 1 Interrupt
 * - \ref IR_SOCK(0)  : SOCKET 0 Interrupt
 */
#define W5100S_REG_IR					(_W5100S_IO_BASE_ + (0x0015)) // Interrupt

/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket Interrupt Mask Register(R/W)
 * @details Each bit of \ref _IMR_ corresponds to each bit of \ref IR.
 * When a bit of \ref _IMR_ is and the corresponding bit of \ref IR is set, Interrupt will be issued.
 */
#define W5100S_REG_IMR    			(_W5100S_IO_BASE_ + (0x0016)) // Socket Interrupt Mask

/**
 * @ingroup Common_register_group_W5100S
 * @brief Timeout register address( 1 is 100us )(R/W)
 * @details \ref _RTR_ configures the retransmission timeout period. The unit of timeout period is 100us and the default of \ref _RTR_ is x07D0or 000
 * And so the default timeout period is 200ms(100us X 2000). During the time configured by \ref _RTR_, W5100S waits for the peer response
 * to the packet that is transmitted by \ref Sn_CR (CONNECT, DISCON, CLOSE, SEND, SEND_MAC, SEND_KEEP command).
 * If the peer does not respond within the \ref _RTR_ time, W5100S retransmits the packet or issues timeout.
 */
#define W5100S_REG_RTR     			(_W5100S_IO_BASE_ + (0x0017)) // Retry Time

/**
 * @ingroup Common_register_group_W5100S
 * @brief Retry count register(R/W)
 * @details \ref _RCR_ configures the number of time of retransmission.
 * When retransmission occurs as many as ref _RCR_+1 Timeout interrupt is issued (\ref Sn_IR_TIMEOUT = '1').
 */
#define W5100S_REG_RCR				(_W5100S_IO_BASE_ + (0x0019)) // Retry Count

/**
 * @ingroup Common_register_group_W5100S
 * @brief Receive Memory Size Register
 * @details \ref RMSR register configures RX bufffer Size of the SOCKET
 * The sum of the RX buffers can not exceed 8kB.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>S3-1</td> <td>S3-0</td> <td>S2-1</td> <td>S2-0</td> <td>S1-1</td> <td>S1-0</td> <td>S0-1</td> <td>S0-0</td> </tr>
 * </table>
 * <table>
 * 		<tr> <td>Memory Size</td><td>Sn-1</td><td>Sn-0</td> </tr>
 * 		<tr> <td>1KB</td><td>0</td><td>0</td> </tr>
 * 		<tr> <td>2KB</td><td>0</td><td>1</td> </tr>
 * 		<tr> <td>4KB</td><td>1</td><td>0</td> </tr>
 * 		<tr> <td>8KB</td><td>1</td><td>1</td> </tr>
 * </table>
 */
#define W5100S_REG_RMSR				(_W5100S_IO_BASE_ + (0x001A)) // Receive Memory Size

/**
 * @ingroup Common_register_group_W5100S
 * @brief Transmit Memory Size Register
 * @details \ref TMSR register configures TX bufffer Size of the SOCKET
 * The sum of the TX buffers can not exceed 8kB.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>S3-1</td> <td>S3-0</td> <td>S2-1</td> <td>S2-0</td> <td>S1-1</td> <td>S1-0</td> <td>S0-1</td> <td>S0-0</td> </tr>
 * </table>
 * <table>
 * 		<tr> <td>Memory Size</td><td>Sn-1</td><td>Sn-0</td> </tr>
 * 		<tr> <td>1KB</td><td>0</td><td>0</td> </tr>
 * 		<tr> <td>2KB</td><td>0</td><td>1</td> </tr>
 * 		<tr> <td>4KB</td><td>1</td><td>0</td> </tr>
 * 		<tr> <td>8KB</td><td>1</td><td>1</td> </tr>
 * </table>
 */
#define W5100S_REG_TMSR				(_W5100S_IO_BASE_ + (0x001B)) // Transmit Memory Size

/**
 * @ingroup Common_register_group_W5100S
 * @brief Interrupt register 2
 * @details \ref IR2 indicates the interrupt status.
 * Each bit of IR2 will be still until the bit will be written to by the host.
 * <table>
 * 		<tr>  <td>7:1</td> <td>0</td>   </tr>
 * 		<tr>  <td>Reserved</td> <td>WOL</td> </tr>
 * </table>
 * - \ref IR2_WOL : WOL MAGIC PACKET Interrupt Mask
 */
#define W5100S_REG_IR2					(_W5100S_IO_BASE_ + (0x0020))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Interrupt mask register 2
 * @details \ref IMR2 Each bit of IMR2 corresponds to each bit of IR2.
 * When a bit of IMR2 is and the corresponding bit of IR2 is set, Interrupt will be issued.
 */
#define W5100S_REG_IMR2				(_W5100S_IO_BASE_ + (0x0021))


/**
 * @ingroup Common_register_group_W5100S
 * @brief PPP LCP Request Timer register  in PPPoE mode(R)
 * @details \ref PTIMER configures the time for sending LCP echo request. The unit of time is 25ms.
 */
#define W5100S_REG_PTIMER				(_W5100S_IO_BASE_ + (0x0028)) // PPP LCP RequestTimer

/**
 * @ingroup Common_register_group_W5100S
 * @brief PPP LCP Magic number register  in PPPoE mode(R)
 * @details \ref PMAGIC configures the 4bytes magic number to be used in LCP negotiation.
 */
#define W5100S_REG_PMAGIC				(_W5100S_IO_BASE_ + (0x0029)) // PPP LCP Magic number

/**
 * @ingroup Common_register_group_W5100S
 * @brief Unreachable IP address register
 * @details \ref
 */
#define W5100S_REG_UIPR				(_W5100S_IO_BASE_ + (0x002A))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Unreachable Port register
 * @details \ref
 */
#define W5100S_REG_UPORTR				(_W5100S_IO_BASE_  + (0x002E))

/* register for W5100S only */

/*------------------------------------------ Common registers ------------------------------------------*/

/**
 * @ingroup Common_register_group_W5100S
 * @brief MR2 Mode register 2
 * @details \reg
 */
#define W5100S_REG_MR2					(_W5100S_IO_BASE_ + (0x0030))


/**
 * @ingroup Common_register_group_W5100S
 * @brief Destination Hardware address in PPPoE
 * @details \reg
 */
#define W5100S_REG_PHAR				(_W5100S_IO_BASE_ + (0x0032))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Session ID in PPPoE
 * @details \reg
 */
#define W5100S_REG_PSIDR				(_W5100S_IO_BASE_ + (0x0038))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Maximum receive Unit in PPPoE
 * @details \reg
 */
#define W5100S_REG_PMRUR				(_W5100S_IO_BASE_ + (0x003A))


/*------------------------------------------ PHY registers ------------------------------------------*/

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY status register
 * @details \reg
 */
#define W5100S_REG_PHYSR				(_W5100S_IO_BASE_ + (0x003C))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY status register(hidden)
 * @details \reg
 */
#define W5100S_REG_PHYSR1				(_W5100S_IO_BASE_ + (0x003D))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Address value
 * @details \reg
 */
#define W5100S_REG_PHYAR				(_W5100S_IO_BASE_ + (0x003E))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Register address
 * @details \reg
 */
#define W5100S_REG_PHYRAR				(_W5100S_IO_BASE_ + (0x003F))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Data input register
 * @details \reg
 */
#define W5100S_REG_PHYDIR				(_W5100S_IO_BASE_ + (0x0040))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY data output register
 * @details \reg
 */
#define W5100S_REG_PHYDOR				(_W5100S_IO_BASE_ + (0x0042))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Action register
 * @details \reg
 */
#define W5100S_REG_PHYACR				(_W5100S_IO_BASE_ + (0x0044))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Division register
 * @details \reg
 */
#define W5100S_REG_PHYDIVR				(_W5100S_IO_BASE_ + (0x0045))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Control register 0
 * @details \reg
 */
#define W5100S_REG_PHYCR0					(_W5100S_IO_BASE_ + (0x0046))
/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Control register 1
 * @details \reg
 */
#define W5100S_REG_PHYCR1					(_W5100S_IO_BASE_ + (0x0047))

/*------------------------------------------ Socket Less registers ------------------------------------------*/

/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket-less control register
 * @details \reg
 */
#define W5100S_REG_SLCR				(_W5100S_IO_BASE_ + (0x004C))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket-less retry time register
 * @details \reg
 */
#define W5100S_REG_SLRTR				(_W5100S_IO_BASE_ + (0x004D))


/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket-less retry count register
 * @details \reg
 */
#define W5100S_REG_SLRCR				(_W5100S_IO_BASE_ + (0x004F))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket-less peer IP address register
 * @details \reg
 */
#define W5100S_REG_SLPIPR				(_W5100S_IO_BASE_ + (0x0050))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket-less peer hardware address register
 * @details \reg
 */
#define W5100S_REG_SLPHAR				(_W5100S_IO_BASE_ + (0x0054))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Ping sequence number register
 * @details \reg
 */
#define W5100S_REG_PINGSEQR				(_W5100S_IO_BASE_ + (0x005A))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Ping ID register
 * @details \reg
 */
#define W5100S_REG_PINGIDR				(_W5100S_IO_BASE_ + (0x005C))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Socket-less interrupt mask register
 * @details \reg
 */
#define W5100S_REG_SLIMR				(_W5100S_IO_BASE_ + (0x005E))

/**
 * @ingroup Common_register_group_W5100S
 * @brief  Socket-less interrupt register
 * @details \reg
 */
#define W5100S_REG_SLIR				(_W5100S_IO_BASE_ + (0x005F))

/**
 * @ingroup Common_register_group_W5100S
 * @brief DBGOUT(hidden)
 * @details \reg
 */
#define W5100S_REG_DBGOUT				(_W5100S_IO_BASE_ + (0x0060))

/**
 * @ingroup Common_register_group_W5100S
 * @brief NICMAXCOLR(hidden)
 * @details \reg
 */
#define W5100S_REG_NICMAXCOLR			(_W5100S_IO_BASE_ + (0x0063))
/*------------------------------------------ CFG registers ------------------------------------------*/

/**
 * @ingroup Common_register_group_W5100S
 * @brief Chip Configuration locking register
 * @details \reg
 */
#define W5100S_REG_CHIPLCKR				(_W5100S_IO_BASE_ + (0x0070))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Network Configuration locking register
 * @details \reg
 */
#define W5100S_REG_NETLCKR				(_W5100S_IO_BASE_ + (0x0071))

/**
 * @ingroup Common_register_group_W5100S
 * @brief PHY Configuration locking register
 * @details \reg
 */
#define W5100S_REG_PHYLCKR				(_W5100S_IO_BASE_ + (0x0072))

/**
 * @ingroup Common_register_group_W5100S
 * @brief version register
 * @details \reg
 */
#define W5100S_REG_VERR				(_W5100S_IO_BASE_ + (0x0080))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Core 100us Counter register
 * @details \reg
 */
#define W5100S_REG_TCNTR				(_W5100S_IO_BASE_ + (0x0082))

/**
 * @ingroup Common_register_group_W5100S
 * @brief Core 100us Counter clear register
 * @details \reg
 */
#define W5100S_REG_TCNTCLKR			(_W5100S_IO_BASE_ + (0x0088))

//----------------------------- W5100S Socket Registers -----------------------------

//--------------------------- For Backward Compatibility ---------------------------

/**
 * @ingroup Socket_register_group_W5100S
 * @brief socket Mode register(R/W)
 * @details \ref Sn_MR configures the option or protocol type of Socket n.\n\n
 * Each bit of \ref Sn_MR defined as the following.
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>MULTI</td> <td>MF</td> <td>ND/MC</td> <td>Reserved</td> <td>Protocol[3]</td> <td>Protocol[2]</td> <td>Protocol[1]</td> <td>Protocol[0]</td> </tr>
 * </table>
 * - \ref Sn_MR_MULTI	: Support UDP Multicasting
 * - \ref Sn_MR_MF	   : Support MACRAW
 * - \ref Sn_MR_ND		: No Delayed Ack(TCP) flag
 * - \ref Sn_MR_MC   	: IGMP version used <b>in UDP mulitcasting</b>
 * - <b>Protocol</b>
 * <table>
 * 		<tr>   <td><b>Protocol[3]</b></td> <td><b>Protocol[2]</b></td> <td><b>Protocol[1]</b></td> <td><b>Protocol[0]</b></td> <td>@b Meaning</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>0</td> <td>Closed</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>0</td> <td>1</td> <td>TCP</td>   </tr>
 * 		<tr>   <td>0</td> <td>0</td> <td>1</td> <td>0</td> <td>UDP</td>   </tr>
 * 		<tr>   <td>0</td> <td>1</td> <td>0</td> <td>0</td> <td>MACRAW</td>   </tr>
 * </table>
 * - <b>In case of Socket 0</b>
 *  <table>
 * 		<tr>   <td><b>Protocol[3]</b></td> <td><b>Protocol[2]</b></td> <td><b>Protocol[1]</b></td> <td><b>Protocol[0]</b></td> <td>@b Meaning</td>   </tr>
 * 		<tr>   <td>0</td> <td>1</td> <td>0</td> <td>0</td> <td>MACRAW</td>   </tr>
 * 		<tr>   <td>0</td> <td>1</td> <td>0</td> <td>1</td> <td>PPPoE</td>   </tr>
 * </table>
 *	 - \ref Sn_MR_MACRAW	: MAC LAYER RAW SOCK \n
 *  - \ref Sn_MR_UDP		: UDP
 *  - \ref Sn_MR_TCP		: TCP
 *  - \ref Sn_MR_CLOSE	: Unused socket
 *  @note MACRAW mode should be only used in Socket 0.
 */
#define W5100S_REG_Sn_MR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0000)) // socket Mode register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket command register(R/W)
 * @details This is used to set the command for Socket n such as OPEN, CLOSE, CONNECT, LISTEN, SEND, and RECEIVE.\n
 * After W5100S accepts the command, the \ref Sn_CR register is automatically cleared to 0x00.
 * Even though \ref Sn_CR is cleared to 0x00, the command is still being processed.\n
 * To check whether the command is completed or not, please check the \ref Sn_IR or \ref Sn_SR.
 * - \ref Sn_CR_OPEN 		: Initialize or open socket.
 * - \ref Sn_CR_LISTEN 		: Wait connection request in TCP mode(<b>Server mode</b>)
 * - \ref Sn_CR_CONNECT 	: Send connection request in TCP mode(<b>Client mode</b>)
 * - \ref Sn_CR_DISCON 		: Send closing request in TCP mode.
 * - \ref Sn_CR_CLOSE   	: Close socket.
 * - \ref Sn_CR_SEND    	: Update TX buffer pointer and send data.
 * - \ref Sn_CR_SEND_MAC	: Send data with MAC address, so without ARP process.
 * - \ref Sn_CR_SEND_KEEP 	: Send keep alive message.
 * - \ref Sn_CR_RECV		: Update RX buffer pointer and receive data.
 * - <b>In case of S0_MR(P3:P0) = S0_MR_PPPoE</b>
 *  <table>
 * 		<tr>   <td><b>Value</b></td> <td><b>Symbol</b></td> <td><b>Description</b></td></tr>
 * 		<tr>   <td>0x23</td> <td>PCON</td> <td>PPPoE connection begins by transmitting PPPoE discovery packet</td>  </tr>
 * 		<tr>   <td>0x24</td> <td>PDISCON</td> <td>Closes PPPoE connection</td>  </tr>
 * 		<tr>   <td>0x25</td> <td>PCR</td> <td>In each phase, it transmits REQ message.</td> </tr>
 * 		<tr>   <td>0x26</td> <td>PCN</td> <td>In each phase, it transmits NAK message.</td> </tr>
 * 		<tr>   <td>0x27</td> <td>PCJ</td> <td>In each phase, it transmits REJECT message.</td> </tr>
 * </table>
 */
#define W5100S_REG_Sn_CR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0001)) // channel Sn_CR register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket interrupt register(R)
 * @details \ref Sn_IR indicates the status of Socket Interrupt such as establishment, termination, receiving data, timeout).\n
 * When an interrupt occurs and the corresponding bit \ref IR_SOCK(N) in \ref _IMR_ are set, \ref IR_SOCK(N) in \ref IR becomes '1'.\n
 * In order to clear the \ref Sn_IR bit, the host should write the bit to \n
 * <table>
 * 		<tr>  <td>7</td> <td>6</td> <td>5</td> <td>4</td> <td>3</td> <td>2</td> <td>1</td> <td>0</td>   </tr>
 * 		<tr>  <td>PRECV</td> <td>PFAIL</td> <td>PNEXT</td> <td>SEND_OK</td> <td>TIMEOUT</td> <td>RECV</td> <td>DISCON</td> <td>CON</td> </tr>
 * </table>
 * - \ref Sn_IR_PRECV : <b>PPP Receive Interrupt</b>
 * - \ref Sn_IR_PFAIL : <b>PPP Fail Interrupt</b>
 * - \ref Sn_IR_PNEXT : <b>PPP Next Phase Interrupt</b>
 * - \ref Sn_IR_SENDOK : <b>SEND_OK Interrupt</b>
 * - \ref Sn_IR_TIMEOUT : <b>TIMEOUT Interrupt</b>
 * - \ref Sn_IR_RECV : <b>RECV Interrupt</b>
 * - \ref Sn_IR_DISCON : <b>DISCON Interrupt</b>
 * - \ref Sn_IR_CON : <b>CON Interrupt</b>
 */
#define W5100S_REG_Sn_IR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0002)) // channel interrupt register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket status register(R)
 * @details \ref Sn_SR indicates the status of Socket n.\n
 * The status of Socket n is changed by \ref Sn_CR or some special control packet as SYN, FIN packet in TCP.
 * @par Normal status
 * - \ref SOCK_CLOSED 		: Closed
 * - \ref SOCK_INIT   		: Initiate state
 * - \ref SOCK_LISTEN    	: Listen state
 * - \ref SOCK_ESTABLISHED 	: Success to connect
 * - \ref SOCK_CLOSE_WAIT   : Closing state
 * - \ref SOCK_UDP   		: UDP socket
 * - \ref SOCK_MACRAW  		: MAC raw mode socket
 *@par Temporary status during changing the status of Socket n.
 * - \ref SOCK_SYNSENT   	: This indicates Socket n sent the connect-request packet (SYN packet) to a peer.
 * - \ref SOCK_SYNRECV    	: It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.
 * - \ref SOCK_FIN_WAIT		: Connection state
 * - \ref SOCK_CLOSING		: Closing state
 * - \ref SOCK_TIME_WAIT	: Closing state
 * - \ref SOCK_LAST_ACK 	: Closing state
 */
#define W5100S_REG_Sn_SR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0003)) // channel status register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief source port register(R/W)
 * @details \ref Sn_PORT configures the source port number of Socket n.
 * It is valid when Socket n is used in TCP/UDP mode. It should be set before OPEN command is ordered.
*/
#define W5100S_REG_Sn_PORT(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0004)) // source port register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Peer MAC register address(R/W)
 * @details \ref Sn_DHAR configures the destination hardware address of Socket n when using SEND_MAC command in UDP mode or
 * it indicates that it is acquired in ARP-process by CONNECT/SEND command.
 */
#define W5100S_REG_Sn_DHAR(sn)     (_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0006)) // Peer MAC register address

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Peer IP register address(R/W)
 * @details \ref Sn_DIPR configures or indicates the destination IP address of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP client mode, it configures an IP address of TCP server before CONNECT command.
 * In TCP server mode, it indicates an IP address of TCP client after successfully establishing connection.
 * In UDP mode, it configures an IP address of peer to be received the UDP packet by SEND or SEND_MAC command.
 */
#define W5100S_REG_Sn_DIPR(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x000C)) // Peer IP register address

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Peer port register address(R/W)
 * @details \ref Sn_DPORT configures or indicates the destination port number of Socket n. It is valid when Socket n is used in TCP/UDP mode.
 * In TCP clientmode, it configures the listen port number of TCP server before CONNECT command.
 * In TCP Servermode, it indicates the port number of TCP client after successfully establishing connection.
 * In UDP mode, it configures the port number of peer to be transmitted the UDP packet by SEND/SEND_MAC command.
 */
#define W5100S_REG_Sn_DPORT(sn)    (_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0010)) // Peer port register address

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Maximum Segment Size(Sn_MSSR0) register address(R/W)
 * @details \ref Sn_MSSR configures or indicates the MTU(Maximum Transfer Unit) of Socket n.
 */
#define W5100S_REG_Sn_MSSR(sn)	   (_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0012)) // Maximum Segment Size(Sn_MSSR0) register address

/**
 * @ingroup Socket_register_group_W5100S
 * @brief IP Protocol(PROTO) Register(R/W)
 * @details \ref Sn_PROTO that sets the protocol number field of the IP header at the IP layer. It is
 * valid only in IPRAW mode, and ignored in other modes.
 */
#define W5100S_REG_Sn_PROTO(sn)	   (_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0014)) // Protocol of IP Header field register in IP raw mode

/**
 * @ingroup Socket_register_group_W5100S
 * @brief IP Type of Service(TOS) Register(R/W)
 * @details \ref Sn_TOS configures the TOS(Type Of Service field in IP Header) of Socket n.
 * It is set before OPEN command.
 */
#define W5100S_REG_Sn_TOS(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + 0x0015) // IP Type of Service(TOS) Register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief IP Time to live(TTL) Register(R/W)
 * @details \ref Sn_TTL configures the TTL(Time To Live field in IP header) of Socket n.
 * It is set before OPEN command.
 */
#define W5100S_REG_Sn_TTL(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0016)) // IP Time to live(TTL) Register

// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0017))
// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0018))
// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0019))
// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x001A))
// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x001B))
// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x001C))
// Reserved					(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x001D))



/**
 * @ingroup Socket_register_group_W5100S
 * @brief Receive memory size register(R/W)
 * @details @ref Sn_RXBUF_SIZE configures the RX buffer block size of Socket n.
 * Socket n RX Buffer Block size can be configured with 1,2,4 and 8Kbytes.
 * If a different size is configured, the data cannot be normally received from a peer.
 * Although Socket n RX Buffer Block size is initially configured to 2Kbytes,
 * user can re-configure its size using @ref Sn_RXBUF_SIZE. The total sum of @ref Sn_RXBUF_SIZE can not be exceed 8Kbytes.
 * When exceeded, the data reception error is occurred.
 */
#define W5100S_REG_Sn_RXBUF_SIZE(sn)   (_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x001E))

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Transmit memory size register(R/W)
 * @details @ref Sn_TXBUF_SIZE configures the TX buffer block size of Socket n. Socket n TX Buffer Block size can be configured with 1,2,4 and 8Kbytes.
 * If a different size is configured, the data cannot be normally transmitted to a peer.
 * Although Socket n TX Buffer Block size is initially configured to 2Kbytes,
 * user can be re-configure its size using @ref Sn_TXBUF_SIZE. The total sum of @ref Sn_TXBUF_SIZE can not be exceed 8Kbytes.
 * When exceeded, the data transmission error is occurred.
 */
#define W5100S_REG_Sn_TXBUF_SIZE(sn)   (_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x001F))


/**
 * @ingroup Socket_register_group_W5100S
 * @brief Transmit free memory size register(R)
 * @details \ref Sn_TX_FSR indicates the free size of Socket n TX Buffer Block. It is initialized to the configured size by \ref Sn_TXMEM_SIZE.
 * Data bigger than \ref Sn_TX_FSR should not be saved in the Socket n TX Buffer because the bigger data overwrites the previous saved data not yet sent.
 * Therefore, check before saving the data to the Socket n TX Buffer, and if data is equal or smaller than its checked size,
 * transmit the data with SEND/SEND_MAC command after saving the data in Socket n TX buffer. But, if data is bigger than its checked size,
 * transmit the data after dividing into the checked size and saving in the Socket n TX buffer.
 */
#define W5100S_REG_Sn_TX_FSR(sn)	(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0020)) // Transmit free memory size register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Transmit memory read pointer register address(R)
 * @details \ref Sn_TX_RD is initialized by OPEN command. However, if Sn_MR(P[3:0]) is TCP mode(001), it is re-initialized while connecting with TCP.
 * After its initialization, it is auto-increased by SEND command.
 * SEND command transmits the saved data from the current \ref Sn_TX_RD to the \ref Sn_TX_WR in the Socket n TX Buffer.
 * After transmitting the saved data, the SEND command increases the \ref Sn_TX_RD as same as the \ref Sn_TX_WR.
 * If its increment value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.
 */
#define W5100S_REG_Sn_TX_RD(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0022)) // Transmit memory read pointer register address

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Transmit memory write pointer register address(R/W)
 * @details \ref Sn_TX_WR is initialized by OPEN command. However, if Sn_MR(P[3:0]) is TCP mode(001), it is re-initialized while connecting with TCP.\n
 * It should be read or be updated like as follows.\n
 * 1. Read the starting address for saving the transmitting data.\n
 * 2. Save the transmitting data from the starting address of Socket n TX buffer.\n
 * 3. After saving the transmitting data, update \ref Sn_TX_WR to the increased value as many as transmitting data size.
 * If the increment value exceeds the maximum value 0xFFFF(greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.\n
 * 4. Transmit the saved data in Socket n TX Buffer by using SEND/SEND command
 */
#define W5100S_REG_Sn_TX_WR(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0024)) // Transmit memory write pointer register address

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Received data size register(R)
 * @details \ref Sn_RX_RSR indicates the data size received and saved in Socket n RX Buffer.
 * \ref Sn_RX_RSR does not exceed the \ref Sn_RXMEM_SIZE and is calculated as the difference between
 * Socket n RX Write Pointer (\ref Sn_RX_WR)and Socket n RX Read Pointer (\ref Sn_RX_RD)
 */
#define W5100S_REG_Sn_RX_RSR(sn)	(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0026)) // Received data size register

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Read point of Receive memory(R/W)
 * @details \ref Sn_RX_RD is initialized by OPEN command. Make sure to be read or updated as follows.\n
 * 1. Read the starting save address of the received data.\n
 * 2. Read data from the starting address of Socket n RX Buffer.\n
 * 3. After reading the received data, Update \ref Sn_RX_RD to the increased value as many as the reading size.
 * If the increment value exceeds the maximum value 0xFFFF, that is, is greater than 0x10000 and the carry bit occurs,
 * update with the lower 16bits value ignored the carry bit.\n
 * 4. Order RECV command is for notifying the updated \ref Sn_RX_RD to W5100S.
 */
#define W5100S_REG_Sn_RX_RD(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0028)) // Read point of Receive memory

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Write point of Receive memory(R)
 * @details \ref Sn_RX_WR is initialized by OPEN command and it is auto-increased by the data reception.
 * If the increased value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs),
 * then the carry bit is ignored and will automatically update with the lower 16bits value.
 */
#define W5100S_REG_Sn_RX_WR(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x002A)) // Write point of Receive memory


//todo
/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket interrupt mask register
 * @details Register address to configure the interrupt mask of the socket
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 *
 */
#define W5100S_REG_Sn_IMR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x002C))

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket fragment field register
 * @details Register to configure the Fragment field of IP Header
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 */
#define W5100S_REG_Sn_FRAGR(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x002D)) // and +1

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket Mode register 2
 * @details Register to set mode 2
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 */
#define W5100S_REG_Sn_MR2(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x002F))

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket n Keep Alive Timer Register
 * @details Register to set the transmission period of keep alive packet.
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 */
#define W5100S_REG_Sn_KPALVTR(sn)		(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0030))

/** todo delete
 * @ingroup Socket_register_group_W5100S
 * @brief Socket n Timer Status Register
 * @details
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 */
//#define Sn_TSR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0031))

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket n Retry Time-value Register
 * @details Register to set the retry time value
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 */
#define W5100S_REG_Sn_RTR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0032))

/**
 * @ingroup Socket_register_group_W5100S
 * @brief Socket n Retry Count-value Register
 * @details Register to set the retry count value
 * @param sn Socket number. It should be <b>0 ~ @ref \_W5100S_SOCK_NUM_</b> expect <b>bit 4</b>.
 */
#define W5100S_REG_Sn_RCR(sn)			(_W5100S_IO_BASE_ + W5100S_SREG_BLOCK(sn) + (0x0034))


/*----------------------------- W5100S Register values  -----------------------------*/

/* MODE register values */
/**
 * @brief Reset
 * @details If this bit is  All internal registers will be initialized. It will be automatically cleared as after S/W reset.
 */
#define W5100S_MR_RST				0x80 ///< reset


/**
 * @brief Ping block
 * @details 0 : Disable Ping block\n
 * 1 : Enable Ping block\n
 * If the bit is  it blocks the response to a ping request.
 */
#define W5100S_MR_PB				0x10 ///< ping block

/**
 * @brief Enable PPPoE
 * @details 0 : DisablePPPoE mode\n
 * 1 : EnablePPPoE mode\n
 * If you use ADSL, this bit should be '1'.
 */
#define W5100S_MR_PPPOE			0x08 ///< enable pppoe

/**
 * @brief Address Auto-Increment in Indirect Bus Interface
 * @details 0 : Disable auto-increment \n
 * 1 : Enable auto-incremente \n
 * At the Indirect Bus Interface mode, if this bit is set as the address will
 * be automatically increased by 1 whenever read and write are performed.
 */
#define W5100S_MR_AI				0x02 ///< auto-increment in indirect mode

/**
 * @brief Indirect Bus Interface mode
 * @details 0 : Disable Indirect bus Interface mode \n
 * 1 : Enable Indirect bus Interface mode \n
 * If this bit is set as Indirect Bus Interface mode is set.
 */
#define W5100S_MR_IND				0x01 ///< enable indirect mode

/* IR register values */
/**
 * @brief Check IP conflict.
 * @details Bit is set as when own source IP address is same with the sender IP address in the received ARP request.
 */
#define W5100S_IR_CONFLICT			0x80 ///< check ip confict

/**
 * @brief Get the destination unreachable message in UDP sending.
 * @details When receiving the ICMP (Destination port unreachable) packet, this bit is set as
 * When this bit is  Destination Information such as IP address and Port number may be checked with the corresponding @ref UIPR & @ref UPORTR.
 */
#define W5100S_IR_UNREACH			0x40 ///< check destination unreachable

/**
 * @brief Get the PPPoE close message.
 * @details When PPPoE is disconnected during PPPoE mode, this bit is set.
 */
#define W5100S_IR_PPPoE			0x20 ///< get the PPPoE close message

/**
 * @brief Socket interrupt bit
 * @details Indicates whether each socket interrupt has occured.
 */
#define W5100S_IR_SOCK(sn)		(0x01 << sn)	///< check socket interrupt

/**
 * @brief IP conflict interrupt mask bit
 * @details If this bit is set, IP conflict interrupt is enabled.
 */
#define W5100S_IMR_CONFLICT		0x80

/**
 * @brief Destination port unreachable interrupt mask bit
 * @details If this bit is set, destination port unreachable interrupt is enabled.
 */
#define W5100S_IMR_UNREACH			0x40

/**
 * @brief PADT/LCPT interrupt mask bit(PPPoE)
 * @details If this bit is set, PADT/LCPT interrupt is enabled.
 */
#define W5100S_IMR_PPPoE			0x20

/**
 * @brief Socket interrupt mask bit
 * @details If this bit is set, each socket interrupt is enabled.
 */
#define W5100S_IMR_SOCK(sn)		(0x01 << sn)

/**
 * @brief Socket-less command register bit
 * @details ARP command
 */
#define W5100S_SLCMD_ARP		(1<<1)

/**
 * @brief Socket-less command register bit
 * @details ARP command
 */
#define W5100S_SLCMD_PING		(1<<0)

/**
 * @brief Socket-less command interrupt and interrupt mask register bit
 * @details Request command time out interrupt and interrupt mask
 */
#define W5100S_SLIR_TIMEOUT		(1<<2)

/**
* @brief Socket less command interrupt and interrupt mask register bit
* @details Socket less command ARP interrupt and interrupt mask
*/
#define W5100S_SLIR_ARP (1<<1)

/**
* @brief Socket less command interrupt and interrupt mask register bit
* @details Socket less command PING interrupt and interruptmask
*/
#define W5100S_SLIR_PING (1<<0)



// Sn_MR values
/* Sn_MR Default values */
/**
 * @brief Unused socket
 * @details This configures the protocol mode of Socket n.
 */
#define W5100S_Sn_MR_CLOSE			0x00 ///< unused socket

/**
 * @brief TCP
 * @details This configures the protocol mode of Socket n.
 */
#define W5100S_Sn_MR_TCP			0x01 ///< TCP

/**
 * @brief UDP
 * @details This configures the protocol mode of Socket n.
 */
#define W5100S_Sn_MR_UDP       0x02 ///< UDP
#define W5100S_Sn_MR_IPRAW     0x03 ///< IP LAYER RAW SOCK

/**
 * @brief MAC LAYER RAW SOCK
 * @details This configures the protocol mode of Socket n.
 * @note MACRAW mode should be only used in Socket 0.
 */
#define W5100S_Sn_MR_MACRAW		0x04 ///< MAC LAYER RAW SOCK

/**
 * @brief PPPoE
 * @details This configures the protocol mode of Socket n.
 * @note PPPoE mode should be only used in Socket 0.
 */
#define W5100S_Sn_MR_PPPoE			0x05 ///< PPPoE

/**
 * @brief No Delayed Ack(TCP), Multicast flag
 * @details 0 : Disable No Delayed ACK option\n
 * 1 : Enable No Delayed ACK option\n
 * This bit is applied only during TCP mode (P[3:0] = 001).\n
 * When this bit is  It sends the ACK packet without delay as soon as a Data packet is received from a peer.\n
 * When this bit is  It sends the ACK packet after waiting for the timeout time configured by \ref _RTR_.
 */
#define W5100S_Sn_MR_ND			0x20 ///< No Delayed Ack(TCP) flag

/**
 * @brief Support UDP Multicasting
 * @details 0 : using IGMP version 2\n
 * 1 : using IGMP version 1\n
 * This bit is applied only during UDP mode(P[3:0] = 010 and MULTI = '1')
 * It configures the version for IGMP messages (Join/Leave/Report).
 */
#define W5100S_Sn_MR_MC			Sn_MR_ND ///< Select IGMP version 1(0) or 2(1)

/**
 * @brief MAC filter enable in @ref Sn_MR_MACRAW mode
 * @details 0 : disable MAC Filtering\n
 * 1 : enable MAC Filtering\n
 * This bit is applied only during MACRAW mode(P[3:0] = 100.\n
 * When set as  W5100S can only receive broadcasting packet or packet sent to itself.
 * When this bit is  W5100S can receive all packets on Ethernet.
 * If user wants to implement Hybrid TCP/IP stack,
 * it is recommended that this bit is set as for reducing host overhead to process the all received packets.
 */
#define W5100S_Sn_MR_MF			0x40 ///< Use MAC filter
#define W5100S_Sn_MR_MFEN      Sn_MR_MF


/* Sn_MR Default values */
/**
 * @brief Support UDP Multicasting
 * @details 0 : disable Multicasting\n
 * 1 : enable Multicasting\n
 * This bit is applied only during UDP mode(P[3:0] = 010).\n
 * To use multicasting, \ref Sn_DIPR & \ref Sn_DPORT should be respectively configured with the multicast group IP address & port number
 * before Socket n is opened by OPEN command of \ref Sn_CR.
 */
#define W5100S_Sn_MR_MULTI			0x80 ///< support multicating

/* Sn_CR values */
/**
 * @brief Initialize or open socket
 * @details Socket n is initialized and opened according to the protocol selected in Sn_MR(P3:P0).
 * The table below shows the value of \ref Sn_SR corresponding to \ref Sn_MR.\n
 * <table>
 *   <tr>  <td>\b Sn_MR (P[3:0])</td> <td>\b Sn_SR</td>            		 </tr>
 *   <tr>  <td>Sn_MR_CLOSE  (000)</td> <td>--</td>         	   		 </tr>
 *   <tr>  <td>Sn_MR_TCP  (001)</td> <td>SOCK_INIT (0x13)</td>  		 </tr>
 *   <tr>  <td>Sn_MR_UDP  (010)</td>  <td>SOCK_UDP (0x22)</td>  		 </tr>
 *   <tr>  <td>S0_MR_IPRAW  (011)</td>  <td>SOCK_IPRAW (0x32)</td>  </tr>
 *   <tr>  <td>S0_MR_MACRAW  (100)</td>  <td>SOCK_MACRAW (0x42)</td>  </tr>
 *   <tr>  <td>S0_MR_PPPoE  (101)</td>  <td>SOCK_PPPoE (0x5F)</td>  </tr>
 * </table>
 */
#define W5100S_Sn_CR_OPEN			0x01 ///< initialize or open socket

/**
 * @brief Wait connection request in TCP mode(Server mode)
 * @details This is valid only in TCP mode (Sn_MR(P3:P0) = \ref Sn_MR_TCP).//
 * In this mode, Socket n operates as a 'TCP server' and waits for connection-request (SYN packet) from any 'TCP client'.//
 * The \ref Sn_SR changes the state from SOCK_INIT to SOCKET_LISTEN.//
 * When a 'TCP client' connection request is successfully established,
 * the \ref Sn_SR changes from SOCK_LISTEN to SOCK_ESTABLISHED and the Sn_IR(0) becomes
 * But when a 'TCP client' connection request is failed, Sn_IR(3) becomes and the status of \ref Sn_SR changes to SOCK_CLOSED.
 */
#define W5100S_Sn_CR_LISTEN		0x02 ///< wait connection request in tcp mode(Server mode)

/**
 * @brief Send connection request in TCP mode(Client mode)
 * @details  To connect, a connect-request (SYN packet) is sent to <b>TCP server</b>configured by \ref Sn_DIPR & Sn_DPORT(destination address & port).
 * If the connect-request is successful, the \ref Sn_SR is changed to \ref SOCK_ESTABLISHED and the Sn_IR(0) becomes \n\n
 * The connect-request fails in the following three cases.\n
 * 1. When a @b ARPTO occurs (\ref Sn_IR[3] = '1') because destination hardware address is not acquired through the ARP-process.\n
 * 2. When a @b SYN/ACK packet is not received and @b TCPTO (Sn_IR(3) ='1')\n
 * 3. When a @b RST packet is received instead of a @b SYN/ACK packet. In these cases, \ref Sn_SR is changed to \ref SOCK_CLOSED.
 * @note This is valid only in TCP mode and operates when Socket n acts as <b>TCP client</b>
 */
#define W5100S_Sn_CR_CONNECT		0x04 ///< send connection request in tcp mode(Client mode)

/**
 * @brief Send closing request in TCP mode
 * @details Regardless of <b>TCP server</b>or <b>TCP client</b> the DISCON command processes the disconnect-process (<b>Active close</b>or <b>Passive close</b>.\n
 * @par Active close
 * it transmits disconnect-request(FIN packet) to the connected peer\n
 * @par Passive close
 * When FIN packet is received from peer, a FIN packet is replied back to the peer.\n
 * @details When the disconnect-process is successful (that is, FIN/ACK packet is received successfully), \ref Sn_SR is changed to \ref SOCK_CLOSED.\n
 * Otherwise, TCPTO occurs (Sn_IR(3)='1') and then \ref Sn_SR is changed to \ref SOCK_CLOSED.
 * @note Valid only in TCP mode.
 */
#define W5100S_Sn_CR_DISCON		0x08 ///< send closing reqeuset in tcp mode

/**
 * @brief Close socket
 * @details Sn_SR is changed to \ref SOCK_CLOSED.
 */
#define W5100S_Sn_CR_CLOSE         0x10

/**
 * @brief Update TX buffer pointer and send data
 * @details SEND transmits all the data in the Socket n TX buffer.\n
 * For more details, please refer to Socket n TX Free Size Register (\ref Sn_TX_FSR), Socket n,
 * TX Write Pointer Register(\ref Sn_TX_WR), and Socket n TX Read Pointer Register(\ref Sn_TX_RD).
 */
#define W5100S_Sn_CR_SEND          0x20

/**
 * @brief Send data with MAC address, so without ARP process
 * @details The basic operation is same as SEND.\n
 * Normally SEND transmits data after destination hardware address is acquired by the automatic ARP-process(Address Resolution Protocol).\n
 * But SEND_MAC transmits data without the automatic ARP-process.\n
 * In this case, the destination hardware address is acquired from \ref Sn_DHAR configured by host, instead of APR-process.
 * @note Valid only in UDP mode.
 */
#define W5100S_Sn_CR_SEND_MAC      0x21

/**
 * @brief Send keep alive message
 * @details It checks the connection status by sending 1byte keep-alive packet.\n
 * If the peer can not respond to the keep-alive packet during timeout time, the connection is terminated and the timeout interrupt will occur.
 * @note Valid only in TCP mode.
 */
#define W5100S_Sn_CR_SEND_KEEP     0x22

/**
 * @brief Update RX buffer pointer and receive data
 * @details RECV completes the processing of the received data in Socket n RX Buffer by using a RX read pointer register (\ref Sn_RX_RD).\n
 * For more details, refer to Socket n RX Received Size Register (\ref Sn_RX_RSR), Socket n RX Write Pointer Register (\ref Sn_RX_WR),
 * and Socket n RX Read Pointer Register (\ref Sn_RX_RD).
 */
#define W5100S_Sn_CR_RECV          0x40

/**
 * @brief
 * @details
 */
#define W5100S_Sn_CR_IGMP_JOIN			0x23

/**
 * @brief
 * @details
 */
#define W5100S_Sn_CR_IGMP_LEAVE		0x24


/* Sn_IR values */

/**
 * @brief SEND_OK Interrupt
 * @details This is issued when SEND command is completed.
 */
#define W5100S_Sn_IR_SENDOK		0x10 ///< complete sending

/**
 * @brief TIMEOUT Interrupt
 * @details This is issued when ARPTO or TCPTO occurs.
 */
#define W5100S_Sn_IR_TIMEOUT		0x08 ///< assert timeout

/**
 * @brief RECV Interrupt
 * @details This is issued whenever data is received from a peer.
 */
#define W5100S_Sn_IR_RECV          0x04

/**
 * @brief DISCON Interrupt
 * @details This is issued when FIN or FIN/ACK packet is received from a peer.
 */
#define W5100S_Sn_IR_DISCON        0x02

/**
 * @brief CON Interrupt
 * @details This is issued one time when the connection with peer is successful and then \ref Sn_SR is changed to \ref SOCK_ESTABLISHED.
 */
#define W5100S_Sn_IR_CON           0x01

/* Sn_SR values */
/**
 * @brief Closed
 * @details This indicates that Socket n is released.\n
 * When DICON, CLOSE command is ordered, or when a timeout occurs, it is changed to \ref SOCK_CLOSED regardless of previous status.
 */
#define W5100S_SOCK_CLOSED			0x00 ///< closed

/**
 * @brief Initiate state
 * @details This indicates Socket n is opened with TCP mode.\n
 * It is changed to \ref SOCK_INIT when Sn_MR(P[3:0]) = 001)and OPEN command is ordered.\n
 * After \ref SOCK_INIT, user can use LISTEN /CONNECT command.
 */
#define W5100S_SOCK_INIT 			0x13 ///< init state

/**
 * @brief Listen state
 * @details This indicates Socket n is operating as <b>TCP server</b>mode and waiting for connection-request (SYN packet) from a peer (<b>TCP client</b>).\n
 * It will change to \ref SOCK_ESTABLISHED when the connection-request is successfully accepted.\n
 * Otherwise it will change to \ref SOCK_CLOSED after TCPTO occurred (Sn_IR(TIMEOUT) = '1').
 */
#define W5100S_SOCK_LISTEN         0x14

/**
 * @brief Connection state
 * @details This indicates Socket n sent the connect-request packet (SYN packet) to a peer.\n
 * It is temporarily shown when \ref Sn_SR is changed from \ref SOCK_INIT to \ref SOCK_ESTABLISHED by CONNECT command.\n
 * If connect-accept(SYN/ACK packet) is received from the peer at SOCK_SYNSENT, it changes to \ref SOCK_ESTABLISHED.\n
 * Otherwise, it changes to \ref SOCK_CLOSED after TCPTO (\ref Sn_IR[TIMEOUT] = '1') is occurred.
 */
#define W5100S_SOCK_SYNSENT        0x15

/**
 * @brief Connection state
 * @details It indicates Socket n successfully received the connect-request packet (SYN packet) from a peer.\n
 * If socket n sends the response (SYN/ACK  packet) to the peer successfully,  it changes to \ref SOCK_ESTABLISHED. \n
 * If not, it changes to \ref SOCK_CLOSED after timeout occurs (\ref Sn_IR[TIMEOUT] = '1').
 */
#define W5100S_SOCK_SYNRECV        0x16

/**
 * @brief Success to connect
 * @details This indicates the status of the connection of Socket n.\n
 * It changes to \ref SOCK_ESTABLISHED when the <b>TCP SERVER</b>processed the SYN packet from the <b>TCP CLIENT</b>during \ref SOCK_LISTEN, or
 * when the CONNECT command is successful.\n
 * During \ref SOCK_ESTABLISHED, DATA packet can be transferred using SEND or RECV command.
 */
#define W5100S_SOCK_ESTABLISHED    0x17

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to \ref SOCK_CLOSED.
 */
#define W5100S_SOCK_FIN_WAIT       0x18

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to \ref SOCK_CLOSED.
 */
#define W5100S_SOCK_CLOSING        0x1A

/**
 * @brief Closing state
 * @details These indicate Socket n is closing.\n
 * These are shown in disconnect-process such as active-close and passive-close.\n
 * When Disconnect-process is successfully completed, or when timeout occurs, these change to \ref SOCK_CLOSED.
 */
#define W5100S_SOCK_TIME_WAIT      0x1B

/**
 * @brief Closing state
 * @details This indicates Socket n received the disconnect-request (FIN packet) from the connected peer.\n
 * This is half-closing status, and data can be transferred.\n
 * For full-closing, DISCON command is used. But For just-closing, @ref Sn_CR_CLOSE command is used.
 */
#define W5100S_SOCK_CLOSE_WAIT     0x1C

/**
 * @brief Closing state
 * @details This indicates Socket n is waiting for the response (FIN/ACK packet) to the disconnect-request (FIN packet) by passive-close.\n
 * It changes to \ref SOCK_CLOSED when Socket n received the response successfully, or when timeout occurs  (\ref Sn_IR[TIMEOUT] = '1').
 */
#define W5100S_SOCK_LAST_ACK       0x1D

/**
 * @brief UDP socket
 * @details This indicates Socket n is opened in UDP mode(Sn_MR(P[3:0]) = 010).\n
 * It changes to SOCK_UDP when Sn_MR(P[3:0]) = 010 and @ref Sn_CR_OPEN command is ordered.\n
 * Unlike TCP mode, data can be transfered without the connection-process.
 */
#define W5100S_SOCK_UDP			0x22 ///< udp socket

/**
 * @brief IP raw mode socket
 * @details TThe socket is opened in IPRAW mode. The SOCKET status is change to SOCK_IPRAW when @ref Sn_MR (P3:P0) is
 * Sn_MR_IPRAW and @ref Sn_CR_OPEN command is used.\n
 * IP Packet can be transferred without a connection similar to the UDP mode.
*/
#define W5100S_SOCK_IPRAW			0x32 ///< ip raw mode socket

/**
 * @brief MAC raw mode socket
 * @details This indicates Socket 0 is opened in MACRAW mode (@ref Sn_MR(P[3:0]) = '100' and n=0) and is valid only in Socket 0.\n
 * It changes to SOCK_MACRAW when @ref Sn_MR(P[3:0]) = '100' and @ref Sn_CR_OPEN command is ordered.\n
 * Like UDP mode socket, MACRAW mode Socket 0 can transfer a MAC packet (Ethernet frame) without the connection-process.
 */
#define W5100S_SOCK_MACRAW			0x42 ///< mac raw mode socket

/**
 * @brief PPPoE mode socket
 * @details It is the status that SOCKET0 is open as PPPoE mode. It is changed to SOCK_PPPoE in case of S0_CR=OPEN and S0_MR
 * (P3:P0)=S0_MR_PPPoE.\n
 * It is temporarily used at the PPPoE
connection.
 */
#define W5100S_SOCK_PPPOE			0x5F ///< pppoe socket

// IP PROTOCOL
#define W5100S_IPPROTO_IP			0 ///< Dummy for IP
#define W5100S_IPPROTO_ICMP		   1 ///< Control message protocol
#define W5100S_IPPROTO_IGMP		   2 ///< Internet group management protocol
#define W5100S_IPPROTO_GGP			3 ///< GW^2 (deprecated)
#define W5100S_IPPROTO_TCP			6 ///< TCP
#define W5100S_IPPROTO_PUP			12 ///< PUP
#define W5100S_IPPROTO_UDP			17 ///< UDP
#define W5100S_IPPROTO_IDP			22 ///< XNS idp
#define W5100S_IPPROTO_ND			77 ///< UNOFFICIAL net disk protocol
#define W5100S_IPPROTO_RAW			255 ///< Raw IP packet



/*----------------------------- W5100S !!Only!! Register values  -----------------------------*/

//todo
/* MODE2 register values */

/**
 * @brief	Clock select bit
 * @details	With this bit, system clock can be selected to be 25Mhz or 100Mhz
 * 1: 25Mhz
 * 0: 100Mhz (default)
 */
#define W5100S_MR2_CLKSEL				(1<<7)

/**
 * @brief	Interrupt pin enable bit
 * @details	This bit enables interrupt.
 * 1: Enable interrupt
 * 0: Disable interrupt
 */
#define W5100S_MR2_G_IEN				(1<<6)


/**
 * @brief	No TCP Reset Packet send
 * @details This bit prevents sending reset packet.
 * 1: Block TCP reset packet send
 * 0: TCP Reset packet send
 */
#define W5100S_MR2_NOTCPRST			(1<<5)

/**
 * @brief	Unreachable Packet Send Block bit
 * @details	This bit prevents sending Destination Port Unreachable Packet.
 * 1: Block Destination Port Unreachable Packet Send
 * 0: Destination Port Unreachable Packet Send
 */
#define W5100S_MR2_UDPURB				(1<<4)

/**
 * @brief Wake On LAN
 * @details This bit enables WOL packet to be received.
 * 1: WOL packet can be received.
 * 0: WOL packet cannot be received.
 */
#define W5100S_MR2_WOL					(1<<3)

/**todo
 * @brief MACRAW No Size Check
 * @details
 */
#define W5100S_MR2_MNOSCHK				(1<<2)

/**
 * @brief	UDP force ARP
 * @details This bit can enables to force ARP for each send command.
 * 1: UDP Force ARP Enable
 * 0: UDP Force ARP Disable.
 *
 */
#define W5100S_MR2_UDPFARP				(1<<1)

/**todo
 * @brief	Skip SRC Hardware Address
 * @details	This bit can enables to receive without checking the hardware address of the peer.
 * 1:
 */
#define W5100S_MR2_SSRCHA				(1<<0)



/* Common interrupt register 2 values */

/**todo
 * @brief magic packet
 * @details
 */
#define W5100S_IR2_MGC				(1<<1)

/**todo
 * @brief Magic packet interrupt mask bit
 * @details If this bit is set, each socket interrupt is enabled.
 */
#define W5100S_IMR2_MGC			(1<<1)

/**todo
 * @brief
 * @details
 */
//#define IR2_MGD				(1<<1) /* Reserved */


/* PHY status register 0 values */

/**todo
 * @brief Ethernet CABLE OFF Signal
 * @details
 */
#define W5100S_PHYSR_CABOFF				(1<<7)

/**todo
 * @brief
 * @details
 */
#define W5100S_PHYSR_CAUTO				(1<<5)

/**todo
 * @brief
 * @details
 */
#define W5100S_PHYSR_CSPD				(1<<4)

/**todo
 * @brief
 * @details
 */
#define W5100S_PHYSR_CDUP				(1<<3)

/**todo
 * @brief
 * @details
 */
#define W5100S_PHYSR_DUP				(1<<2)

/**todo
 * @brief
 * @details
 */
#define W5100S_PHYSR_SPD				(1<<1)

/**todo
 * @brief LINKDONE register
 * @details If 1 Linked successfully, if 0 no link
 */
#define W5100S_PHYSR_LNK				(1<<0)


/* PHY status register 10 values */

/**
 * @brieftodo
 * @details
 */
#define W5100S_PHYSR1_RXPG				(1<<2)

/**
 * @brieftodo
 * @details
 */
#define W5100S_PHYSR1_LPI				(1<<1)

/**
 * @brieftodo
 * @details
 */
#define W5100S_PHYSR1_CLDN				(1<<0)

#define W5100S_PHYCR_AUTONEGO_ENABLE	(0<<2)
#define W5100S_PHYCR_AUTONEGO_DISABLE	(1<<2)

#define W5100S_PHYCR_SPD_10			    (1<<1)
#define W5100S_PHYCR_SPD_100			(0<<1)

#define W5100S_PHYCR_HALF_DUP			(1<<0)
#define W5100S_PHYCR_FULL_DUP			(0<<0)

#define W5100S_PHYCR1_RST				(0<<0)

#define	W5100S_PHYCR1_PWDN_ENABLE		(1<<5)
#define	W5100S_PHYCR1_PWDN_DISABLE		(0<<5)


/* Socket n MODE register 2 values */

/**
 * @brief Broadcast Blocking bit in MACRAW mode
 * @details In MACRAW mode, this bit is set to ????to block the broadcast packet.
 */
#define W5100S_Sn_MR2_MBBLK  			(1<<6)

/**
 * @brief Multicast Blocking bit in MACRAW mode
 * @details In MACRAW mode, this bit is set to ????to block the multicast packet.
 */
#define W5100S_Sn_MR2_MMBLK  			(1<<5)

/**
 * @brief IPv6 packet Blocking bit in MACRAW mode
 * @details In MACRAW mode, this bit is set to ????to block the IPv6 packet.
 */
#define W5100S_Sn_MR2_IPV6BLK  		(1<<4)


/**
 * @brief Broadcast Blocking bit in UDP mode
 * @details In UDP mode, this bit is set to ????to block the broadcast packet.
 */
#define W5100S_Sn_MR2_UBBLK			(1<<1)


/**
 * @brief TCP Force PSH bit
 * @details When the SOCKET transmits data in TCP mode, PSH Flag is set to all packets.
 */
#define W5100S_Sn_MR2_FPSH			Sn_MR2_UBBLK

/**
 * @brief Unicast Blocking bit in UDP mode
 * @details In UDP mode, this bit is set to ????to block the Unicast packet.
 */
#define W5100S_Sn_MR2_UUBLK			(1<<0)

/*----------------------------For PHY Control-------------------------------*/

/********************/
/* Register Address */
/********************/

//Basic mode control register, basic register
#define W5100S_REG_PHYMDIO_BMCR				0x00

//Basic mode status register, basic register
#define W5100S_REG_PHYMDIO_BMSR				0x01

//--------------------------------------Not used-------------------------------------------//
////PHY identifier register 1, extended register
//#define PHY_IDR1				0x02	//not used
//
////PHY identifier register 2, extended register
//#define PHY_IDR2				0x03	//not used
//
////Auto-negotiation advertisement register, extended register
//#define PHY_ANAR				0x04	//not used
//
////Auto-negotiation link partner ability register, extended register
//#define PHY_ANLPAR				0x05	//not used
//
////Auto-negotiation expansion register, extended register
//#define PHY_ANER				0x06	//not used
//
////Auto-negotiation next page transmit
//#define PHY_ANNP				0x07	//not used
//
////Auto-negotiation link partner of the next page receive
//#define PHY_ANLPNP				0x08	//not used
//
////MMD access control register
//#define PHY_REGCR				0x09	//not used
//
////MMD access address data register
//#define PHY_ADDAR				0x0e	//not used
//--------------------------------------Not used-------------------------------------------//

/********************/
/* Bit definitions  */
/********************/

//For BMCR register
#define W5100S_BMCR_RESET				(1<<15)
#define W5100S_BMCR_MLOOPBACK			(1<<14)
#define W5100S_BMCR_SPEED				(1<<13)
#define W5100S_BMCR_AUTONEGO			(1<<12)
#define W5100S_BMCR_PWDN				(1<<11)
#define W5100S_BMCR_ISOLATE				(1<<10)
#define W5100S_BMCR_RSTNEGO				(1<<9)
#define W5100S_BMCR_DUP					(1<<8)
#define W5100S_BMCR_COLTEST				(1<<7)

//For BMSR register
#define W5100S_BMSR_AUTONEGO_COMPL		(1<<5)
#define W5100S_BMSR_REMOTE_FAULT		(1<<4)
#define W5100S_BMSR_LINK_STATUS			(1<<2)
#define W5100S_BMSR_JAB_DETECT			(1<<1)
#define W5100S_EXTENDED_CAPA			(1<<0)

//--------------------------------------Not used-------------------------------------------//
////For ANAR register
//#define ANAR_NP					(1<<15)
//#define ANAR_ACK				(1<<14)
//#define ANAR_RF					(1<<13)
//#define ANAR_ASM				(3<<10)
//#define ANAR_T4					(1<<9)
//#define ANAR_TX_FD				(1<<8)
//#define ANAR_TX_HD				(1<<7)
//#define ANAR_10_FD				(1<<6)
//#define ANAR_10_HD				(1<<5)
//#define ANAR_SELECTOR			(0x1F<<0)
//
////For ANAR register
//#define ANLPAR_NP				(1<<15)
//#define ANLPAR_ACK				(1<<14)
//#define ANLPAR_RF				(1<<13)
//#define ANLPAR_LP_DIR			(1<<11)
//#define ANLPAR_PAUSE			(1<<10)
//#define ANLPAR_T4				(1<<9)
//#define ANLPAR_TX_FD			(1<<8)
//#define ANLPAR_TX_HD			(1<<7)
//#define ANLPAR_10_FD			(1<<6)
//#define ANLPAR_10_HD			(1<<5)
//#define ANLPAR_SELECTOR			(0x1F<<0)

/**/
/* MDIO register*/
//PCS_CTL_1			|		PCS control 1 register
//PCS_STS_1			|		PCS status 1 register
//EEE_ABILITY  		|		EEE capability register
//WAKE_ER_CNTR		|		EEE wake error counter
//EEE_ADVR 			|		EEE Advertisement register
//EEE_LPAR 			|		EEE link partner ability register

//--------------------------------------Not used-------------------------------------------//

/********************/
/*Functions for PHY */
/********************/
//todo move this definition to bit area
#define W5100S_PHYACR_READ			0x02
#define W5100S_PHYACR_WRITE			0x01

#ifdef __cplusplus
}
#endif
