#pragma once
#include "esp_err.h"
#include "hwss_type.h"
#include "hwss_eth.h"
#include "hwss_netif_types.h"

typedef struct{
    uint8_t mac[HWSS_NETIF_MAX_HWADDR_LEN];
    hwss_netif_ip_info_t* ip_info;
    hwss_netif_ip_info_t* ip_info_old;

    hwss_eth_t *eth;
    
    hwss_netif_related_data_t *related_data; // holds additional data for specific netifs

    hwss_netif_dhcp_status_t dhcpc_status;
    hwss_netif_dhcp_status_t dhcps_status;
    bool timer_running;

    hwss_netif_flags_t flags;
    char * hostname;

    bool mldv6_report_timer_started;
}hwss_netif_t;



esp_err_t hwss_netif_init(void);
esp_err_t hwss_netif_deinit(void);

hwss_netif_t *hwss_netif_new(hwss_netif_config_t *hwss_netif_config);

// esp_err_t hwss_netif_get_devid(hwss_netif_t *hwss_netif, hwss_devid_t *devid);




// esp_err_t hwss_netif_sock_alloc(hwss_netif_t *hwss_netif, hwss_sock_t *sock);
// esp_err_t hwss_netif_sock_free(hwss_sock_t sock);

