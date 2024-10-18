#pragma once

#include "hwss_phy.h"

#ifdef __cplusplus
extern "C" {
#endif

hwss_phy_t *hwss_phy_new_ch394(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_phy_config_t *phy_config);

#ifdef __cplusplus
}
#endif
