#include "hwss_hppe_wiznet.h"

hwss_hppe_t *hwss_hppe_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_hppe_config_t *config){
    switch(sku){
        case HWSS_ETH_SKU_W5100S: return hwss_hppe_new_w5100s(io,config);
        case HWSS_ETH_SKU_W5500: return hwss_hppe_new_w5500(io,config);

        default: return NULL;
    }
}
