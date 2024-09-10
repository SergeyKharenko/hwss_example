idf_component_register(SRCS "hwss.c" "hwss_type.c"
                    INCLUDE_DIRS "include" "wiznet"
                    REQUIRES driver lwip freertos)
