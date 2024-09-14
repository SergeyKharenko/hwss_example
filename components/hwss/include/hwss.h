#pragma once

#include <stdint.h>

#include "hwss_io.h"
#include "hwss_phy.h"
#include "hwss_mac.h"
#include "hwss_hnet.h"
#include "hwss_hso.h"

typedef void *hwss_handle_t;

typedef struct{
    gpio_num_t rst_gpio_num;
    gpio_num_t irq_gpio_num;
    

}hwss_config_t;

typedef struct hwss_driver_s hwss_driver_t;

struct hwss_driver_s{
    hwss_io_t *io;
    hwss_phy_t *phy;
    hwss_mac_t *mac;
    hwss_hnet_t *hnet;
    hwss_hso_t *hso;

    
};

