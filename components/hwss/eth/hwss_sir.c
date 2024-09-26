#include "hwss_sir_wiznet.h"

hwss_sir_t *hwss_sir_new(hwss_sku_t sku, esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_sir_config_t *config){
    switch(sku){
        case HWSS_SKU_W5500: return hwss_sir_new_w5500(elp_hdl,io,config);
        default: return NULL;
    }
}
