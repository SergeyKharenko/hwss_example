#pragma once

#include <stdint.h>

#include "hwss_io.h"
#include "hwss_phy.h"
#include "hwss_mac.h"
#include "hwss_hip.h"
#include "hwss_hsock.h"

typedef void *hwss_handle_t;

typedef struct{
    gpio_num_t rst_gpio_num;
    gpio_num_t irq_gpio_num;
    

}hwss_config_t;

typedef struct{
    hwss_io_t *io;
    hwss_phy_t *phy;
    hwss_mac_t *mac;
    hwss_hip_t *hip;
    hwss_hsock_t *hsock;

    
}hwss_driver_t;




