#pragma once
// #include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_event_base.h"

typedef enum{
    HWSS_IO_SPI =           0x00,
    HWSS_IO_SPI_PRO =       0x01,           // For CH395 only 
    HWSS_IO_PARALLEL_8 =    0x02,
    HWSS_IO_PARALLEL_16 =   0x03,
    HWSS_IO_FAST_PARALLEL_8 =   0x04,
    HWSS_IO_FAST_PARALLEL_16 =  0x05,
    HWSS_IO_UART =          0x06,

    HWSS_IO_TYPE_END =      0xFF
}hwss_io_type_t;

typedef hwss_io_type_t      (hwss_io_types_t)[];
inline bool hwss_io_type_supported(const hwss_io_types_t list, hwss_io_type_t type){
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

#define HWSS_MAC_ADDR_LEN       6
#define HWSS_IP_ADDR_LEN        4

typedef uint8_t (hwss_mac_addr_t)[HWSS_MAC_ADDR_LEN];
typedef uint8_t (hwss_ip_addr_t)[HWSS_IP_ADDR_LEN];

typedef uint8_t                 hwss_devid_t;
typedef uint8_t                 hwss_sockid_t;
typedef uint16_t                hwss_port_t;


// inline bool hwss_same_option_check(uint8_t num, bool opt,...){
//     if(num<2)
//         return true;
    
//     bool cal_res=opt;
//     bool arg;
//     bool res=true;

//     va_list args;
//     va_start(args,opt);

//     if(opt){
//         while(num--){
//             arg=va_arg(args,bool);
//             cal_res=cal_res&&arg;
//             if(!cal_res){
//                 res=false;
//                 break;
//             }
//         }
//     }
//     else{
//         while(num--){
//             arg=va_arg(args,bool);
//             cal_res=cal_res||opt;
//             if(cal_res){
//                 res=false;
//                 break;
//             }
//         }
//     }
//     va_end(args);
//     return res;
// }