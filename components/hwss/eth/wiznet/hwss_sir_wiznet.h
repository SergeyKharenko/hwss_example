#pragma once

#include "hwss_sir.h"

hwss_sir_t *hwss_sir_new_w5500(esp_event_loop_handle_t elp_hdl, hwss_io_t *io, const hwss_sir_config_t *config);
