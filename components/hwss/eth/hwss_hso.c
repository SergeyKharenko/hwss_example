#include "hwss_hso_wiznet.h"
#include "hwss_hso.h"

hwss_hso_t *hwss_hso_new(hwss_eth_sku_t sku, hwss_io_t *io, hwss_hir_t* hir, const hwss_hso_config_t *config){
    switch (sku)
    {
        case HWSS_ETH_SKU_W5100S: return hwss_hso_new_w5100s(io,hir,config);
        case HWSS_ETH_SKU_W5500: return hwss_hso_new_w5500(io,hir,config);
        
        default: return NULL;
    }
}
