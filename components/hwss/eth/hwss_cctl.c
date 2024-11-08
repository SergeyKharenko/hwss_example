#include "hwss_cctl_wiznet.h"
#include "hwss_cctl.h"

hwss_cctl_t *hwss_cctl_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_cctl_config_t *config){
    switch (sku){
        case HWSS_ETH_SKU_W5100S: return hwss_cctl_new_w5100s(io, config);
        case HWSS_ETH_SKU_W5500: return hwss_cctl_new_w5500(io,config);
        
        default: return NULL;
    }
}