#include "hwss_hnet_wiznet.h"
#include "hwss_hnet.h"

hwss_hnet_t *hwss_hnet_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_hnet_config_t *config){
    switch (sku){
        case HWSS_ETH_SKU_W5100S: return hwss_hnet_new_w5100s(io,config);
        case HWSS_ETH_SKU_W5500: return hwss_hnet_new_w5500(io,config);
        
        default: return NULL;
    }
}
