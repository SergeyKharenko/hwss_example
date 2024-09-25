#pragma once
#include <string.h>
#include "hwss_sku.h"
#include "hwss_opt.h"
#include "hwss_io.h"
#include "hwss_hir.h"
#include "hwss_lpw.h"
#include "hwss_phy.h"
#include "hwss_mac.h"
#include "hwss_hnet.h"
#include "hwss_hso.h"
#include "hwss_hsl.h"
#include "hwss_hdc.h"

typedef struct{
    hwss_sku_t          sku;

    hwss_io_type_t      io_type;
    void                *io_config;

    gpio_num_t          rst_ionum;
    uint32_t            rst_timeout_ms;

    esp_event_loop_args_t elp_args;

    hwss_hir_config_t   hir;
    hwss_phy_config_t   phy;
    hwss_mac_config_t   mac;
    hwss_hnet_config_t  hnet;
    hwss_hso_config_t   hso;

    bool has_hsl;
    hwss_hsl_config_t   hsl;

    bool has_hdc;

}hwss_eth_config_t;


#define HWSS_ETH_W5500_DEFAULT_CONFIG(type, config, ir_ionum, rst_ionum)                        \
                                                                    {                           \
                                                                        .sku=HWSS_SKU_W5500     \
                                                                        .io_type=type,          \
                                                                        .io_config=config,      \
                                                                        .rst_ionum=rst_ionum,   \
                                                                        .rst_timeout_ms=10,     \
                                                                        .elp_args={             \
                                                                            .queue_size=16,     \
                                                                            .task_core_id=tskNO_AFFINITY,\
                                                                            .task_name="HWSS ETH LOOP", \
                                                                            .task_priority=10,          \
                                                                            .task_stack_size=4096       \
                                                                        },                      \
                                                                        .hir={                  \
                                                                            .io_num=ir_ionum,   \
                                                                            .tri=HWSS_TRIGGER_NEGEDGE   \
                                                                        },                      \
                                                                        .phy={                  \
                                                                            .check_period_ms=10,        \
                                                                            .reset_timeout_ms=500       \
                                                                        },                      \
                                                                        .mac={                  \
                                                                            .use_burnin_addr=false      \
                                                                            .addr[0]=0x00,      \
                                                                            .addr[1]=0x08,      \
                                                                            .addr[2]=0xDC,      \
                                                                            .addr[3]=0x04,      \
                                                                            .addr[4]=0x02,      \
                                                                            .addr[5]=0x04,      \
                                                                        },                      \
                                                                        .hnet={                 \
                                                                            .check_state_period_ms=10,  \
                                                                            .retry_cnt=8,               \
                                                                            .retry_time_ms=200,         \
                                                                            .ppp_cp_magic_num=0,        \
                                                                            .ppp_cp_request_time_ms=1000\
                                                                        },                      \
                                                                        .hso={                  \
                                                                            .irq_inter_time_us=10,      \
                                                                            .sock_active_threshold_ms=1000,\
                                                                            .sock_polling_period_ms=5   \
                                                                            .en_socknum=2,      \
                                                                            .tx_buffsize_kb[0]=8,\
                                                                            .tx_buffsize_kb[1]=8,\
                                                                            .rx_buffsize_kb[0]=8,\
                                                                            .rx_buffsize_kb[1]=8,\
                                                                        },                      \
                                                                        .has_hsl=true,          \
                                                                        .hsl={                  \
                                                                            .retry_cnt=3,       \
                                                                            .retry_time_ms=100, \
                                                                            .check_state_period_ms=10   \
                                                                        },                      \
                                                                        .has_hdc=false          \
                                                                    };

inline void hwss_eth_config_set_mac_addr(hwss_eth_config_t *config, hwss_mac_addr_t addr){
    memcpy(config->mac.addr,addr,HWSS_MAC_ADDR_LEN);
}

inline void hwss_eth_config_set_tx_rx_buffsize_kb(hwss_eth_config_t *config, uint8_t en_socknum,
            const uint8_t *tx_buffsize_kb, const uint8_t *rx_buffsize_kb){
    memcpy(config->hso.tx_buffsize_kb,tx_buffsize_kb,en_socknum);
    memcpy(config->hso.rx_buffsize_kb,rx_buffsize_kb,en_socknum);
}

