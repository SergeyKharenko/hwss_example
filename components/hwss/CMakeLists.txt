set(srcs "hwss_event.c"
        "eth/hwss_cctl.c"
        "eth/hwss_cvr.c"
        "eth/hwss_eth_event.c"
        "eth/hwss_eth.c"
        "eth/hwss_hir.c"
        "eth/hwss_sir.c"
        "eth/hwss_hnet.c"
        "eth/hwss_hsl.c"
        "eth/hwss_sscm.c"
        "eth/hwss_hso.c"
        "eth/hwss_io.c"
        "eth/hwss_hppe.c"
        "eth/hwss_mac.c"
        "eth/hwss_phy.c"
        "eth/hwss_sscm.c"
        "eth/hwss_sscm_drv.c"

        "netif/hwss_netif.c"
)
set(inc_dirs "include" "eth/include" "netif/include")

list(APPEND srcs 
                "eth/wiznet/w5500/hwss_cctl_w5500.c"
                "eth/wiznet/w5500/hwss_cvr_w5500.c"
                "eth/wiznet/w5500/hwss_hnet_w5500.c"
                "eth/wiznet/w5500/hwss_io_w5500.c"
                "eth/wiznet/w5500/hwss_sir_w5500.c"
                "eth/wiznet/w5500/hwss_mac_w5500.c"
                "eth/wiznet/w5500/hwss_phy_w5500.c"
                "eth/wiznet/w5500/hwss_hppe_w5500.c"
                "eth/wiznet/w5500/hwss_hso_w5500.c"
                "eth/wiznet/w5500/hwss_sscm_drv_w5500.c"

                # "eth/wiznet/w5100s/hwss_io_w5100s.c"
                # "eth/wiznet/w5100s/hwss_phy_w5100s.c"
                # "eth/wiznet/w5100s/hwss_mac_w5100s.c"
                # "eth/wiznet/w5100s/hwss_hsl_w5100s.c"
                # "eth/wiznet/w5100s/hwss_hnet_w5100s.c"
                # "eth/wiznet/w5100s/hwss_hso_w5100s.c"

                "eth/wch/ch394/hwss_io_ch394.c"
                )
list(APPEND inc_dirs    "eth/wiznet" 
                        "eth/wiznet/w5500" 
                        # "eth/wiznet/w5100s"

                        "eth/wch"
                        "eth/wch/ch394"
)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dirs}
                    REQUIRES driver freertos esp_event esp_timer esp_netif)
