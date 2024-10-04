#pragma once

#include "hwss_hsl.h"

#ifdef __cplusplus
extern "C" {
#endif

hwss_hsl_t *hwss_hsl_new_w5100s(hwss_io_t *io, const hwss_hsl_config_t *config);

#ifdef __cplusplus
}
#endif
