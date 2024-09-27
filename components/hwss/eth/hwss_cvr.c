#include "hwss_cvr_wiznet.h"

hwss_cvr_t *hwss_cvr_new(hwss_eth_sku_t sku, hwss_io_t *io){
    switch (sku)
    {
        case HWSS_ETH_SKU_W5500: return hwss_cvr_new_w5500(io);

        default: return NULL;
    }
}