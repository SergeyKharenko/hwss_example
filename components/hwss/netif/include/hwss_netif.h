#pragma once
#include "esp_err.h"
#include "hwss_type.h"
#include "hwss_netif_types.h"

typedef struct{
    uint8_t mac[HWSS_NETIF_MAX_HWADDR_LEN];
    hwss_netif_ip_info_t* ip_info;
    hwss_netif_ip_info_t* ip_info_old;

    void *driver_handle;
    
    hwss_netif_related_data_t *related_data; // holds additional data for specific netifs

    hwss_netif_dhcp_status_t dhcpc_status;
    hwss_netif_dhcp_status_t dhcps_status;
    bool timer_running;

    hwss_netif_flags_t flags;
    char * hostname;

    bool mldv6_report_timer_started;
}hwss_netif_t;

// esp_err_t hwss_netif_init(void);
// esp_err_t hwss_netif_deinit(void);

hwss_netif_t    *hwss_netif_new(hwss_netif_config_t *hwss_netif_config);
void            hwss_netif_destroy(hwss_netif_t *hwss_netif);

esp_err_t esp_netif_set_mac(esp_netif_t *esp_netif, uint8_t mac[]);
esp_err_t esp_netif_get_mac(esp_netif_t *esp_netif, uint8_t mac[]);
esp_err_t esp_netif_set_hostname(esp_netif_t *esp_netif, const char *hostname);
esp_err_t esp_netif_get_hostname(esp_netif_t *esp_netif, const char **hostname);
bool esp_netif_is_netif_up(esp_netif_t *esp_netif);
esp_err_t esp_netif_get_ip_info(esp_netif_t *esp_netif, esp_netif_ip_info_t *ip_info);
esp_err_t esp_netif_get_old_ip_info(esp_netif_t *esp_netif, esp_netif_ip_info_t *ip_info);
esp_err_t esp_netif_set_ip_info(esp_netif_t *esp_netif, const esp_netif_ip_info_t *ip_info);
esp_err_t esp_netif_set_old_ip_info(esp_netif_t *esp_netif, const esp_netif_ip_info_t *ip_info);


esp_err_t
esp_netif_dhcps_option(esp_netif_t *esp_netif, esp_netif_dhcp_option_mode_t opt_op, esp_netif_dhcp_option_id_t opt_id,
                       void *opt_val, uint32_t opt_len);

esp_err_t
esp_netif_dhcpc_option(esp_netif_t *esp_netif, esp_netif_dhcp_option_mode_t opt_op, esp_netif_dhcp_option_id_t opt_id,
                       void *opt_val, uint32_t opt_len);

esp_err_t esp_netif_dhcpc_start(esp_netif_t *esp_netif);
esp_err_t esp_netif_dhcpc_stop(esp_netif_t *esp_netif);

esp_err_t esp_netif_dhcpc_get_status(esp_netif_t *esp_netif, esp_netif_dhcp_status_t *status);
esp_err_t esp_netif_dhcps_get_status(esp_netif_t *esp_netif, esp_netif_dhcp_status_t *status);

esp_err_t esp_netif_dhcps_start(esp_netif_t *esp_netif);
esp_err_t esp_netif_dhcps_stop(esp_netif_t *esp_netif);
esp_err_t esp_netif_dhcps_get_clients_by_mac(esp_netif_t *esp_netif, int num, esp_netif_pair_mac_ip_t *mac_ip_pair);

esp_err_t esp_netif_set_dns_info(esp_netif_t *esp_netif, esp_netif_dns_type_t type, esp_netif_dns_info_t *dns);
esp_err_t esp_netif_get_dns_info(esp_netif_t *esp_netif, esp_netif_dns_type_t type, esp_netif_dns_info_t *dns);

void esp_netif_set_ip4_addr(esp_ip4_addr_t *addr, uint8_t a, uint8_t b, uint8_t c, uint8_t d);


// esp_err_t hwss_netif_sock_alloc(hwss_netif_t *hwss_netif, hwss_sock_t *sock);
// esp_err_t hwss_netif_sock_free(hwss_sock_t sock);

