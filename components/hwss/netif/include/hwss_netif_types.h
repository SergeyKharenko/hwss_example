#pragma once
#include "hwss_type.h"

/** @brief Type of DNS server */
typedef enum {
    HWSS_NETIF_DNS_MAIN= 0,       /**< DNS main server address*/
    HWSS_NETIF_DNS_BACKUP,        /**< DNS backup server address (Wi-Fi STA and Ethernet only) */
    HWSS_NETIF_DNS_FALLBACK,      /**< DNS fallback server address (Wi-Fi STA and Ethernet only) */
    HWSS_NETIF_DNS_MAX
} hwss_netif_dns_type_t;

/** @brief DNS server info */
typedef struct {
    hwss_ip_addr_t ip; /**< IPV4 address of DNS server */
} hwss_netif_dns_info_t;

/** @brief Status of DHCP client or DHCP server */
typedef enum {
    HWSS_NETIF_DHCP_INIT = 0,    /**< DHCP client/server is in initial state (not yet started) */
    HWSS_NETIF_DHCP_STARTED,     /**< DHCP client/server has been started */
    HWSS_NETIF_DHCP_STOPPED,     /**< DHCP client/server has been stopped */
    HWSS_NETIF_DHCP_STATUS_MAX
} hwss_netif_dhcp_status_t;


/** @brief Mode for DHCP client or DHCP server option functions */
typedef enum{
    HWSS_NETIF_OP_START = 0,
    HWSS_NETIF_OP_SET,           /**< Set option */
    HWSS_NETIF_OP_GET,           /**< Get option */
    HWSS_NETIF_OP_MAX
} hwss_netif_dhcp_option_mode_t;

/** @brief Supported options for DHCP client or DHCP server */
typedef enum{
    HWSS_NETIF_SUBNET_MASK                   = 1,    /**< Network mask */
    HWSS_NETIF_DOMAIN_NAME_SERVER            = 6,    /**< Domain name server */
    HWSS_NETIF_ROUTER_SOLICITATION_ADDRESS   = 32,   /**< Solicitation router address */
    HWSS_NETIF_REQUESTED_IP_ADDRESS          = 50,   /**< Request specific IP address */
    HWSS_NETIF_IP_ADDRESS_LEASE_TIME         = 51,   /**< Request IP address lease time */
    HWSS_NETIF_IP_REQUEST_RETRY_TIME         = 52,   /**< Request IP address retry counter */
    HWSS_NETIF_VENDOR_CLASS_IDENTIFIER       = 60,   /**< Vendor Class Identifier of a DHCP client */
    HWSS_NETIF_VENDOR_SPECIFIC_INFO          = 43,   /**< Vendor Specific Information of a DHCP server */
} hwss_netif_dhcp_option_id_t;