set(srcs "hwss_event.c" "generic/hwss_hso_scm.c")
set(inc_dirs "include" "generic")

list(APPEND srcs 
                "wiznet/hwss_hnet_w5500.c"
                # "wiznet/hwss_hso_w5500.c"
                "wiznet/hwss_io_w5500.c"
                "wiznet/hwss_mac_w5500.c"
                "wiznet/hwss_phy_w5500.c"
                "wiznet/hwss_io_w5100s.c"
                "wiznet/hwss_phy_w5100s.c"
                )
list(APPEND inc_dirs "wiznet")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dirs}
                    REQUIRES driver lwip freertos esp_event esp_timer)
