set(srcs "hwss_event.c" "generic/hwss_hso_scm.c")
set(inc_dirs "include" "generic")

list(APPEND srcs 
                "wiznet/w5500/hwss_hnet_w5500.c"
                # "wiznet/w5500/hwss_hso_w5500.c"
                "wiznet/w5500/hwss_io_w5500.c"
                "wiznet/w5500/hwss_mac_w5500.c"
                "wiznet/w5500/hwss_phy_w5500.c"

                "wiznet/w5100s/hwss_io_w5100s.c"
                "wiznet/w5100s/hwss_phy_w5100s.c"
                "wiznet/w5100s/hwss_mac_w5100s.c"
                "wiznet/w5100s/hwss_hsl_w5100s.c"
                "wiznet/w5100s/hwss_hnet_w5100s.c"
                )
list(APPEND inc_dirs "wiznet" "wiznet/w5500" "wiznet/w5100s")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dirs}
                    REQUIRES driver lwip freertos esp_event esp_timer)
