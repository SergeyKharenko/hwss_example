#pragma once

#include <machine/endian.h>
#include "sdkconfig.h"

#if BYTE_ORDER==LITTLE_ENDIAN
#define hwss_htonl(x)                       __builtin_bswap32(x);
#define hwss_ntohl(x)                       __builtin_bswap32(x);
#define hwss_htons(x)                       __builtin_bswap16(x);
#define hwss_ntohs(x)                       __builtin_bswap16(x);
#else
#define hwss_htonl(x)                       (uint32_t)(x)
#define hwss_ntohl(x)                       (uint32_t)(x)
#define hwss_htons(x)                       (uint16_t)(x)
#define hwss_ntohs(x)                       (uint16_t)(x)
#endif

#if CONFIG_HWSS_MODE_COMPATIBLE
#include "esp_netif.h"

#define hwss_ip4_makeu32(a,b,c,d)           esp_netif_ip4_makeu32(a,b,c,d)

#define HWSS_IP6_ADDR_BLOCK1(ip6addr)       ESP_IP6_ADDR_BLOCK1(ip6addr)
#define HWSS_IP6_ADDR_BLOCK2(ip6addr)       ESP_IP6_ADDR_BLOCK2(ip6addr)
#define HWSS_IP6_ADDR_BLOCK3(ip6addr)       ESP_IP6_ADDR_BLOCK3(ip6addr)
#define HWSS_IP6_ADDR_BLOCK4(ip6addr)       ESP_IP6_ADDR_BLOCK4(ip6addr)
#define HWSS_IP6_ADDR_BLOCK5(ip6addr)       ESP_IP6_ADDR_BLOCK5(ip6addr)
#define HWSS_IP6_ADDR_BLOCK6(ip6addr)       ESP_IP6_ADDR_BLOCK6(ip6addr)
#define HWSS_IP6_ADDR_BLOCK7(ip6addr)       ESP_IP6_ADDR_BLOCK7(ip6addr)
#define HWSS_IP6_ADDR_BLOCK8(ip6addr)       ESP_IP6_ADDR_BLOCK8(ip6addr)

#define HWSS_IPSTR                          IPSTR
#define hwss_ip4_addr_get_byte(ipaddr, idx) esp_ip4_addr_get_byte(ipaddr, idx)
#define hwss_ip4_addr1(ipaddr)              esp_ip4_addr1(ipaddr)
#define hwss_ip4_addr2(ipaddr)              esp_ip4_addr2(ipaddr)
#define hwss_ip4_addr3(ipaddr)              esp_ip4_addr3(ipaddr)
#define hwss_ip4_addr4(ipaddr)              esp_ip4_addr4(ipaddr)

#define hwss_ip4_addr1_16(ipaddr)           esp_ip4_addr1_16(ipaddr)
#define hwss_ip4_addr2_16(ipaddr)           esp_ip4_addr2_16(ipaddr)
#define hwss_ip4_addr3_16(ipaddr)           esp_ip4_addr3_16(ipaddr)
#define hwss_ip4_addr4_16(ipaddr)           esp_ip4_addr4_16(ipaddr)

#define HWSS_IP2STR(ipaddr)                 IP2STR(ipaddr)

#define HWSS_IPV6STR                        IPV6STR

#define HWSS_IPV62STR(ipaddr)               IPV62STR(ipaddr)

#define HWSS_IPADDR_TYPE_V4                 ESP_IPADDR_TYPE_V4 
#define HWSS_IPADDR_TYPE_V6                 ESP_IPADDR_TYPE_V6 
#define HWSS_IPADDR_TYPE_ANY                ESP_IPADDR_TYPE_ANY

#define HWSS_IP4TOUINT32(a,b,c,d)           ESP_IP4TOUINT32(a,b,c,d)

#define HWSS_IP4TOADDR(a,b,c,d)             ESP_IP4TOADDR(a,b,c,d)

#define HWSS_IP4ADDR_INIT(a, b, c, d)       ESP_IP4ADDR_INIT(a, b, c, d)
#define HWSS_IP6ADDR_INIT(a, b, c, d)       ESP_IP6ADDR_INIT(a, b, c, d)

#define HWSS_IP4ADDR_STRLEN_MAX             IP4ADDR_STRLEN_MAX

#define hwss_ip4_addr_t                     esp_ip4_addr_t
#define hwss_ip6_addr_t                     esp_ip6_addr_t

#define HWSS_IP6_ADDR_IS_UNKNOWN            ESP_IP6_ADDR_IS_UNKNOWN
#define HWSS_IP6_ADDR_IS_GLOBAL             ESP_IP6_ADDR_IS_GLOBAL
#define HWSS_IP6_ADDR_IS_LINK_LOCAL         ESP_IP6_ADDR_IS_LINK_LOCAL
#define HWSS_IP6_ADDR_IS_SITE_LOCAL         ESP_IP6_ADDR_IS_SITE_LOCAL
#define HWSS_IP6_ADDR_IS_UNIQUE_LOCAL       ESP_IP6_ADDR_IS_UNIQUE_LOCAL
#define HWSS_IP6_ADDR_IS_IPV4_MAPPED_IPV6   ESP_IP6_ADDR_IS_IPV4_MAPPED_IPV6

#define hwss_ip6_addr_type_t                esp_ip6_addr_type_t

#define hwss_netif_ip_addr_copy             esp_netif_ip_addr_copy

#else



#endif


