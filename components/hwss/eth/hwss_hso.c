#include "hwss_hso_wiznet.h"
#include "hwss_hso.h"

hwss_hso_t *hwss_hso_new(hwss_sku_t sku, esp_event_loop_handle_t elp, hwss_io_t *io, hwss_hir_t* hir, const hwss_hso_config_t *config){
    switch (sku)
    {
    // case HWSS_SKU_W5100S: return hwss_hso_new_w5100s(elp,io,hir,config);
    case HWSS_SKU_W5500: return hwss_hso_new_w5500(elp,io,hir,config);
    
    default: return NULL;
    }
}
