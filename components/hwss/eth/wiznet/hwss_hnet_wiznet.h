#pragma once

#include "hwss_hnet.h"

#ifdef __cplusplus
extern "C" {
#endif

hwss_hnet_t *hwss_hnet_new_w5500(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hnet_config_t *config);
hwss_hnet_t *hwss_hnet_new_w5100s(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_hnet_config_t *config);

#ifdef __cplusplus
}
#endif
