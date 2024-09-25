#include "hwss_hsl_wiznet.h"
#include "hwss_hsl.h"

hwss_hsl_t *hwss_hsl_new(hwss_sku_t sku,esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hsl_config_t *config){
    switch (sku)
    {
    case HWSS_SKU_W5100S: return hwss_hsl_new_w5100s(elp_hdl,io,config);
    default: return NULL;
    }
}
