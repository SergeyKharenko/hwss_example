#include "hwss_io_wiznet.h"
#include "hwss_io.h"

hwss_io_t *hwss_io_new(hwss_sku_t sku, hwss_io_type_t io_type, void *io_config){
    switch (sku){
        // case HWSS_SKU_W5100S: return hwss_io_new_w5100s(io_type, io_config);
        case HWSS_SKU_W5500: return hwss_io_new_w5500(io_type, io_config);
        
        default: return NULL;
    }
}
