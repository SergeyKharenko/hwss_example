#pragma once

#include "hwss_hso.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    bool macraw_mac_filter;
    bool macraw_broadcast_block;
    bool macraw_multicast_block;
    bool macraw_ipv6_block;
    bool udp_multicast;
    bool udp_broadcast_block;
    bool udp_unicast_block;
    bool tcp_force_psh;
    bool nodelay_ack;
    bool igmp_v2;
}hwss_hso_wiznet_sockmode_opt_t;

hwss_hso_t *hwss_hso_new_w5500(hwss_io_t *io, const hwss_hso_config_t *config);

#ifdef __cplusplus
}
#endif
