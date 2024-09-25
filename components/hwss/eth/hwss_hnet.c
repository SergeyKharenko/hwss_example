#include "hwss_hnet_wiznet.h"
#include "hwss_hnet.h"

hwss_hnet_t *hwss_hnet_new(hwss_sku_t sku,esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hnet_config_t *config){
    switch (sku)
    {
    case HWSS_SKU_W5100S: return hwss_hnet_new_w5100s(elp_hdl,io,config);
    case HWSS_SKU_W5500: return hwss_hnet_new_w5500(elp_hdl,io,config);
    
    default: return NULL;
    }
}
