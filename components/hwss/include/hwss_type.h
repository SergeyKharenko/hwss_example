#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "esp_event_base.h"

typedef enum{
    HWSS_IO_SPI =           0x00,
    HWSS_IO_PARALLEL_8 =    0x01,
    HWSS_IO_PARALLEL_16 =   0x02,
    HWSS_IO_UART =          0x03,

    HWSS_IO_TYPE_END =      0xFF
}hwss_io_type;

#define HWSS_IO_TYPE_LIST(name)                      const hwss_io_type name[]

bool hwss_io_type_supported(hwss_io_type *list, hwss_io_type type);

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
} hwss_proto_t;

#define HWSS_MAC_ADDR_LEN       6
typedef uint8_t*                hwss_mac_addr_t; 

typedef uint8_t                 hwss_devid_t;
typedef uint8_t                 hwss_sockid_t;
typedef uint16_t                hwss_port_t;

#define HWSS_SOCKID_SPEC        0

typedef enum{
    HWSS_EVENT_CONNECTED,
    HWSS_EVENT_DISCONNECTED,


}hwss_event_t;

ESP_EVENT_DECLARE_BASE(HWSS_EVENT);
