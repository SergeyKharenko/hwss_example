#pragma once

#include "sdkconfig.h"
#include "hwss_type.h"
#include "hwss_ip_addr.h"

#define HWSS_NETIF_MAX_HWADDR_LEN       6

#if CONFIG_HWSS_MODE_COMPATIBLE
#include "esp_netif.h"

#define HWSS_NETIF_DNS_MAIN             ESP_NETIF_DNS_MAIN     
#define HWSS_NETIF_DNS_BACKUP           ESP_NETIF_DNS_BACKUP   
#define HWSS_NETIF_DNS_FALLBACK         ESP_NETIF_DNS_FALLBACK 
#define HWSS_NETIF_DNS_MAX              ESP_NETIF_DNS_MAX      
#define hwss_netif_dns_type_t           esp_netif_dns_type_t

#define hwss_netif_dns_info_t           esp_netif_dns_info_t

#define HWSS_NETIF_DHCP_INIT            ESP_NETIF_DHCP_INIT        
#define HWSS_NETIF_DHCP_STARTED         ESP_NETIF_DHCP_STARTED     
#define HWSS_NETIF_DHCP_STOPPED         ESP_NETIF_DHCP_STOPPED     
#define HWSS_NETIF_DHCP_STATUS_MAX      ESP_NETIF_DHCP_STATUS_MAX  
#define hwss_netif_dhcp_status_t        esp_netif_dhcp_status_t

#define HWSS_NETIF_OP_START             ESP_NETIF_OP_START     
#define HWSS_NETIF_OP_SET               ESP_NETIF_OP_SET       
#define HWSS_NETIF_OP_GET               ESP_NETIF_OP_GET       
#define HWSS_NETIF_OP_MAX               ESP_NETIF_OP_MAX
#define hwss_netif_dhcp_option_mode_t   esp_netif_dhcp_option_mode_t

#define HWSS_NETIF_SUBNET_MASK                  ESP_NETIF_SUBNET_MASK                   
#define HWSS_NETIF_DOMAIN_NAME_SERVER           ESP_NETIF_DOMAIN_NAME_SERVER            
#define HWSS_NETIF_ROUTER_SOLICITATION_ADDRESS  ESP_NETIF_ROUTER_SOLICITATION_ADDRESS   
#define HWSS_NETIF_REQUESTED_IP_ADDRESS         ESP_NETIF_REQUESTED_IP_ADDRESS          
#define HWSS_NETIF_IP_ADDRESS_LEASE_TIME        ESP_NETIF_IP_ADDRESS_LEASE_TIME         
#define HWSS_NETIF_IP_REQUEST_RETRY_TIME        ESP_NETIF_IP_REQUEST_RETRY_TIME         
#define HWSS_NETIF_VENDOR_CLASS_IDENTIFIER      ESP_NETIF_VENDOR_CLASS_IDENTIFIER       
#define HWSS_NETIF_VENDOR_SPECIFIC_INFO         ESP_NETIF_VENDOR_SPECIFIC_INFO       
#define hwss_netif_dhcp_option_id_t             esp_netif_dhcp_option_id_t

#define hwss_netif_ip_info_t                    esp_netif_ip_info_t
#define hwss_netif_ip6_info_t                   esp_netif_ip6_info_t

#define HWSS_NETIF_DHCP_CLIENT                  ESP_NETIF_DHCP_CLIENT 
#define HWSS_NETIF_DHCP_SERVER                  ESP_NETIF_DHCP_SERVER 
#define HWSS_NETIF_FLAG_AUTOUP                  ESP_NETIF_FLAG_AUTOUP 
#define HWSS_NETIF_FLAG_GARP                    ESP_NETIF_FLAG_GARP   
#define HWSS_NETIF_FLAG_EVENT_IP_MODIFIED       ESP_NETIF_FLAG_EVENT_IP_MODIFIED 
#define HWSS_NETIF_FLAG_IS_PPP                  ESP_NETIF_FLAG_IS_PPP 
#define HWSS_NETIF_FLAG_IS_BRIDGE               ESP_NETIF_FLAG_IS_BRIDGE 
#define HWSS_NETIF_FLAG_MLDV6_REPORT            ESP_NETIF_FLAG_MLDV6_REPORT
#define hwss_netif_flags_t                      esp_netif_flags_t

#define hwss_netif_pair_mac_ip_t                esp_netif_pair_mac_ip_t

#define hwss_netif_dns_param_t                  esp_netif_dns_param_t
#define hwss_netif_ip_lost_timer_t              esp_netif_ip_lost_timer_t

#else

#endif

typedef struct hwss_netif_inherent_config {
    hwss_netif_flags_t flags;               /*!< flags that define esp-netif behavior */
    uint8_t mac[HWSS_NETIF_MAX_HWADDR_LEN]; /*!< initial mac address for this interface */
    const hwss_netif_ip_info_t* ip_info;    /*!< initial ip address for this interface */
    uint32_t get_ip_event;           /*!< event id to be raised when interface gets an IP */
    uint32_t lost_ip_event;          /*!< event id to be raised when interface losts its IP */
    const char * if_key;             /*!< string identifier of the interface */
    const char * if_desc;            /*!< textual description of the interface */
} hwss_netif_inherent_config_t;

typedef struct esp_netif_config {
    hwss_netif_inherent_config_t *base;

}hwss_netif_config_t;

enum hwss_netif_types {
    COMMON_HWSS_NETIF,
    PPP_HWSS_NETIF,
};

typedef struct hwss_netif_related_data {
    bool is_point2point;
    enum hwss_netif_types netif_type;
} hwss_netif_related_data_t;

// typedef struct {
//     esp_netif_t *esp_netif;          /*!< Pointer to corresponding esp-netif object */
//     esp_netif_ip_info_t ip_info;     /*!< IP address, netmask, gatway IP address */
//     bool ip_changed;                 /*!< Whether the assigned IP has changed or not */
// } ip_event_got_ip_t;


// typedef struct {
//     esp_netif_t *esp_netif;          /*!< Pointer to corresponding esp-netif object */
//     esp_netif_ip6_info_t ip6_info;   /*!< IPv6 address of the interface */
//     int ip_index;                    /*!< IPv6 address index */
// } ip_event_got_ip6_t;


// typedef struct {
//     esp_ip6_addr_t addr;            /*!< The address to be added to the interface */
//     bool preferred;                 /*!< The default preference of the address */
// } ip_event_add_ip6_t;


// typedef struct {
//     esp_netif_t *esp_netif; /*!< Pointer to the associated netif handle */
//     esp_ip4_addr_t ip; /*!< IP address which was assigned to the station */
//     uint8_t mac[6];    /*!< MAC address of the connected client */
// } ip_event_ap_staipassigned_t;


