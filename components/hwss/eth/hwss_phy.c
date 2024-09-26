#include "hwss_phy_wiznet.h"
#include "hwss_phy.h"

hwss_phy_t *hwss_phy_new(hwss_sku_t sku, esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_phy_config_t *phy_config){
    switch (sku)
    {
    // case HWSS_SKU_W5100S: return hwss_phy_new_w5100s(elp_hdl,io,phy_config);
    case HWSS_SKU_W5500: return hwss_phy_new_w5500(elp_hdl,io,phy_config);
    
    default: return NULL;
    }
}
