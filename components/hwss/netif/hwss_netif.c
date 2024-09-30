#include "hwss_opt.h"
#include "hwss_netif.h"

// #define HWSS_NETIF_MAX                  1<<(HWSS_DEVID_BITS+1)-1

// #define HWSS_SOCKID(sock)               sock&(1<<(HWSS_SOCKID_BITS+1)-1)
// #define HWSS_DEVID(sock)                sock>>HWSS_SOCKID_BITS    

// static hwss_netif_t *hwss_netifmap[HWSS_NETIF_MAX];

// esp_err_t hwss_netif_sock_alloc(hwss_netif_t *hwss_netif, hwss_sock_t *sock){
//     for(hwss_devid_t i=0;i<HWSS_NETIF_MAX;i++){
//         if(hwss_netifmap[i]==hwss_netif){
            
//         }
//     }
//     return ESP_ERR_NOT_FOUND;
// }

// esp_err_t hwss_netif_sock_free(hwss_sock_t sock){
//     hwss_devid_t devid=HWSS_DEVID(sock);
//     if(devid>=HWSS_NETIF_MAX)
//         return ESP_ERR_INVALID_ARG;

//     hwss_netif_t *netif=hwss_netifmap[devid];
//     if(netif==NULL)
//         return ESP_ERR_NOT_ALLOWED;
    

//     return ESP_OK;
// }hwss_netif_t    *hwss_netif_new(hwss_netif_config_t *hwss_netif_config);


// hwss_netif_t    *hwss_netif_new(hwss_netif_config_t *hwss_netif_config){
//     if (hwss_netif_config == NULL ||
//         hwss_netif_config->base->if_key == NULL ||
//         NULL != esp_netif_get_handle_from_ifkey_unsafe(esp_netif_config->base->if_key)) {
//         ESP_LOGE(TAG, "%s: Failed to configure netif with config=%p (config or if_key is NULL or duplicate key)",
//         __func__,  esp_netif_config);
//         return ESP_FAIL;
//     }
// }

// void            hwss_netif_destroy(hwss_netif_t *hwss_netif);