#pragma once

#include "hwss_hso.h"
#include "hwss_hnet.h"


typedef struct hwss_dhcpc_s hwss_dhcpc_t;

typedef struct{
    hwss_eth_sockid_t id;
    
}hwss_dhcpc_config_t;

struct hwss_dhcpc_s {
    hwss_hso_t      *hso;
    hwss_hnet_t     *hnet;

    esp_err_t       (*init)(hwss_dhcpc_t *dhcpc);
    esp_err_t       (*deinit)(hwss_dhcpc_t *dhcpc);
};



