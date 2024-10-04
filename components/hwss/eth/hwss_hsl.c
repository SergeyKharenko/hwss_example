#include "hwss_hsl_wiznet.h"
#include "hwss_hsl.h"

hwss_hsl_t *hwss_hsl_new(hwss_eth_sku_t sku, hwss_io_t *io, const hwss_hsl_config_t *config){
    switch (sku){
        case HWSS_ETH_SKU_W5100S: return hwss_hsl_new_w5100s(io,config);
        default: return NULL;
    }
}
