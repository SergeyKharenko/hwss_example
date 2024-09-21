#pragma once

#include "hwss_hnet.h"

hwss_hnet_t *hwss_hnet_new_w5500(hwss_io_t *io, hwss_hnet_config_t *config);
hwss_hnet_t *hwss_hnet_new_w5100s(hwss_io_t *io, hwss_hnet_config_t *config);
