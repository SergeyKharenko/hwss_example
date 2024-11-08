#pragma once

#include "esp_event.h"
#include "hwss_hir.h"
#include "hwss_hso.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    HWSS_HSO_WIZNET_BUFFSIZE_0KB=0,
    HWSS_HSO_WIZNET_BUFFSIZE_1KB=1,
    HWSS_HSO_WIZNET_BUFFSIZE_2KB=2,
    HWSS_HSO_WIZNET_BUFFSIZE_4KB=4,
    HWSS_HSO_WIZNET_BUFFSIZE_8KB=8,
    HWSS_HSO_WIZNET_BUFFSIZE_16KB=16
}hwss_hso_wiznet_buffsize_t;

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

hwss_hso_t *hwss_hso_new_w5500(hwss_io_t *io, hwss_hir_t* hir, const hwss_hso_config_t *config);
hwss_hso_t *hwss_hso_new_w5100s(hwss_io_t *io, hwss_hir_t* hir, const hwss_hso_config_t *config);

#ifdef __cplusplus
}
#endif
