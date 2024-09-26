#include "hwss_mac_wiznet.h"
#include "hwss_mac.h"

hwss_mac_t *hwss_mac_new(hwss_sku_t sku, hwss_io_t *io, const hwss_mac_config_t *config){
    switch (sku)
    {
    // case HWSS_SKU_W5100S: return hwss_mac_new_w5100s(io,config);
    case HWSS_SKU_W5500: return hwss_mac_new_w5500(io,config);
    
    default: return NULL;
    }
}