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
// }