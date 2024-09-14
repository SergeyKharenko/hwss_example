#pragma once

#include "hwss_hso.h"

typedef union{
    struct{
        union{
            uint8_t multicast :1;
            uint8_t mac_filter :1;
        };

        uint8_t broadcast_block :1;
        
        union{
            uint8_t nodelay_ack :1;
            uint8_t multicast :1;
            uint8_t multicast_block :1;
        };

        union{
            uint8_t unicast_block :1;
            uint8_t ipv6_block :1;
        };
    };

    uint8_t val;
}hwss_hso_wiznet_sockmode_opt_t;

hwss_hso_t *hwss_hso_new_w5500(hwss_io_t *io, const hwss_hso_config_t *config);
