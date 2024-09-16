set(srcs "hwss_event.c" "hwss_hso_scm.c" "hwss_type.c")
set(inc_dirs "include")

list(APPEND srcs 
                # "wiznet/hwss_hnet_w5500.c"
                # "wiznet/hwss_hso_w5500.c"
                "wiznet/hwss_io_w5500.c"
                "wiznet/hwss_mac_w5500.c"
                "wiznet/hwss_phy_w5500.c"
                )
list(APPEND inc_dirs "wiznet")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dirs}
                    REQUIRES driver lwip freertos esp_event esp_timer)
