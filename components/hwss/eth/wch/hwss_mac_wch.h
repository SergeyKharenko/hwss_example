#pragma once

#include "hwss_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

hwss_mac_t *hwss_mac_new_ch394(hwss_io_t *io, const hwss_mac_config_t *config);

#ifdef __cplusplus
}
#endif
