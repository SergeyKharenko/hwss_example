#pragma once
#include "hwss_io.h"

#ifdef __cplusplus
extern "C" {
#endif

hwss_io_t *hwss_io_new_w5500(hwss_io_type_t type, void *io_config);

#ifdef __cplusplus
}
#endif
