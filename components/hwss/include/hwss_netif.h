#pragma once
#include "esp_err.h"
#include "hwss_type.h"

typedef struct{

}hwss_netif_config_t;


typedef struct{



}hwss_netif_t;

esp_err_t hwss_netif_init(void);
esp_err_t hwss_netif_deinit(void);

hwss_netif_t *hwss_netif_new(hwss_netif_config_t *hwss_netif_config);
void hwss_netif_destroy(hwss_netif_t *hwss_netif);

esp_err_t hwss_netif_get_devid(hwss_netif_t *hwss_netif, hwss_devid_t *devid);




esp_err_t hwss_netif_sock_alloc(hwss_netif_t *hwss_netif, hwss_sock_t *sock);
esp_err_t hwss_netif_sock_free(hwss_sock_t sock);

