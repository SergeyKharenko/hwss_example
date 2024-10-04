#pragma once

#include "hwss_cctl.h"

hwss_cctl_t *hwss_cctl_new_w5500(hwss_io_t *io, const hwss_cctl_config_t *config);
hwss_cctl_t *hwss_cctl_new_w5100s(hwss_io_t *io, const hwss_cctl_config_t *config);
