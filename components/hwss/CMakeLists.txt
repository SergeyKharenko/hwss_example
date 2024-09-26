set(srcs "hwss_event.c"
        "eth/hwss_eth_event.c"
        "eth/hwss_eth.c"
        "eth/hwss_hir.c"
        "eth/hwss_hnet.c"
        "eth/hwss_hsl.c"
        "eth/hwss_sscm.c"
        "eth/hwss_hso.c"
        "eth/hwss_io.c"
        "eth/hwss_mac.c"
        "eth/hwss_phy.c"
)
set(inc_dirs "include" "eth/include")

list(APPEND srcs 
                "eth/wiznet/w5500/hwss_hnet_w5500.c"
                "eth/wiznet/w5500/hwss_io_w5500.c"
                "eth/wiznet/w5500/hwss_mac_w5500.c"
                "eth/wiznet/w5500/hwss_phy_w5500.c"
                "eth/wiznet/w5500/hwss_hso_w5500.c"

                # "eth/wiznet/w5100s/hwss_io_w5100s.c"
                # "eth/wiznet/w5100s/hwss_phy_w5100s.c"
                # "eth/wiznet/w5100s/hwss_mac_w5100s.c"
                # "eth/wiznet/w5100s/hwss_hsl_w5100s.c"
                # "eth/wiznet/w5100s/hwss_hnet_w5100s.c"
                # "eth/wiznet/w5100s/hwss_hso_w5100s.c"
                )
list(APPEND inc_dirs    "eth/wiznet" 
                        "eth/wiznet/w5500" 
                        # "eth/wiznet/w5100s"
)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dirs}
                    REQUIRES driver lwip freertos esp_event esp_timer)
