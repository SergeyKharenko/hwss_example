#include "hwss_sku.h"
#include "hwss_sscm_drv_wiznet.h"

hwss_sscm_drv_t *hwss_sscm_drv_new(hwss_sku_t sku, hwss_io_t *io){
    switch (sku)
    {
        case HWSS_SKU_W5500: return hwss_sscm_drv_new_w5500(io);
        
        default: return NULL;
    }
}

