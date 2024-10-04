#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <machine/endian.h>

typedef enum{
    HWSS_IO_SPI =           0x00,
    HWSS_IO_SPI_PRO =       0x01,
    
    HWSS_IO_PARALLEL_8 =    0x02,
    HWSS_IO_PARALLEL_16 =   0x03,
    HWSS_IO_FAST_PARALLEL_8 =   0x04,
    HWSS_IO_FAST_PARALLEL_16 =  0x05,

    HWSS_IO_UART =          0x06,

    HWSS_IO_TYPE_END =      0xFF
}hwss_io_type_t;

typedef hwss_io_type_t      (hwss_io_types_t)[];
static inline bool hwss_io_type_supported(const hwss_io_types_t list, hwss_io_type_t type){
    uint8_t idx=0;
    while(list[idx]!=HWSS_IO_TYPE_END){
        if(type == *list)
            return true;
        idx++;
    }
    return false;
}

typedef enum{
    HWSS_TRIGGER_POSEDGE,
    HWSS_TRIGGER_NEGEDGE
}hwss_trigger_t;

typedef enum {
    HWSS_PHY_AUTONEGO_RESTART,
    HWSS_PHY_AUTONEGO_EN,
    HWSS_PHY_AUTONEGO_DIS,
    HWSS_PHY_AUTONEGO_G_STAT,
} hwss_phy_autoneg_cmd_t;

typedef enum {
    HWSS_LINK_UP,  
    HWSS_LINK_DOWN 
} hwss_link_t;

typedef enum {
    HWSS_SPEED_10M, 
    HWSS_SPEED_100M,
    HWSS_SPEED_MAX  
} hwss_speed_t;

typedef enum {
    HWSS_DUPLEX_HALF,
    HWSS_DUPLEX_FULL,
} hwss_duplex_t;

typedef enum{
    HWSS_PROTO_CLOSE =     0x00,
    HWSS_PROTO_TCP =       0x01,
    HWSS_PROTO_UDP =       0x02,
    HWSS_PROTO_MACRAW =    0x03,
    
    HWSS_PROTO_UNKNOW =    0xFF
} hwss_proto_t;

#define HWSS_ETH_MAC_ADDR_LEN           6
#define HWSS_ETH_IP4_ADDR_LEN           4
#define HWSS_ETH_IP6_ADDR_LEN           8

typedef uint8_t (hwss_eth_mac_addr_t)[HWSS_ETH_MAC_ADDR_LEN];
typedef uint8_t (hwss_eth_ip4_addr_t)[HWSS_ETH_IP4_ADDR_LEN];
typedef uint16_t (hwss_eth_ip6_addr_t)[HWSS_ETH_IP6_ADDR_LEN];

typedef uint8_t                         hwss_eth_sockid_t;
typedef uint16_t                        hwss_eth_port_t;

typedef enum{
    HWSS_ETH_PACK_HEADER_UDP,
    HWSS_ETH_PACK_HEADER_MACRAW,
}hwss_eth_pack_header_t;

typedef struct{
    hwss_eth_ip4_addr_t     addr;
    hwss_eth_port_t         port;
    uint16_t                len;
}hwss_eth_udp_header_t;

typedef struct{
    hwss_eth_mac_addr_t     s_addr;
    hwss_eth_mac_addr_t     d_addr;
    uint16_t                len;
    uint16_t                type;
}hwss_eth_macraw_header_t;

typedef enum{
    HWSS_SOCKACT_ACTIVE,
    HWSS_SOCKACT_IDLE,
    HWSS_SOCKACT_GENERIC
}hwss_sockact_sta_t;

#define HWSS_SOCK_INTR_SEND_OK           1<<0
#define HWSS_SOCK_INTR_TIMEOUT           1<<1
#define HWSS_SOCK_INTR_RECV              1<<2
#define HWSS_SOCK_INTR_DISCONN           1<<3
#define HWSS_SOCK_INTR_CONNECT           1<<4

typedef uint16_t hwss_chipver_t;

#if _BYTE_ORDER == _LITTLE_ENDIAN
#define hwss_eth_htons(x)                       __builtin_bswap16(x)
#define hwss_eth_ntohs(x)                       __builtin_bswap16(x)

#else
#define hwss_eth_htons(x)                       (uint16_t)(x)
#define hwss_eth_ntohs(x)                       (uint16_t)(x)
#endif