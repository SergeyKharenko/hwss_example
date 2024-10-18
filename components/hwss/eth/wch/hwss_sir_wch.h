#pragma once
#include "hwss_sir.h"

#ifdef __cplusplus
extern "C" {
#endif

hwss_sir_t *hwss_sir_new_ch394(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_sir_config_t *config);

#ifdef __cplusplus
}
#endif
