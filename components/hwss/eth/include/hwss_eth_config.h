#pragma once
#include <string.h>
#include "esp_task.h"
#include "hwss_eth_sku.h"
#include "hwss_opt.h"
#include "hwss_io.h"
#include "hwss_hir.h"
#include "hwss_sir.h"
#include "hwss_cctl.h"
#include "hwss_lpw.h"
#include "hwss_phy.h"
#include "hwss_mac.h"
#include "hwss_hppe.h"
#include "hwss_hnet.h"
#include "hwss_hso.h"
#include "hwss_hsl.h"
#include "hwss_hdc.h"
#include "hwss_sscm.h"

typedef struct{
    hwss_eth_sku_t          sku;

    uint8_t             en_socknum;

    hwss_io_type_t      io_type;
    void                *io_config;

    esp_event_loop_args_t elp_args;

    hwss_cctl_config_t  cctl;
    hwss_hir_config_t   hir;
    hwss_sir_config_t   sir;
    hwss_phy_config_t   phy;
    hwss_mac_config_t   mac;
    hwss_hnet_config_t  hnet;
    hwss_hso_config_t   hso;
    hwss_sscm_config_t  sscm;

    bool has_hppe;
    hwss_hppe_config_t  hppe;

    bool has_hsl;
    hwss_hsl_config_t   hsl;

    bool has_hdc;

}hwss_eth_config_t;

static inline hwss_eth_config_t HWSS_ETH_W5500_DEFAULT_CONFIG(hwss_io_type_t type, void *io_config, gpio_num_t ir, gpio_num_t rst){
    hwss_eth_config_t res={
        .sku=HWSS_ETH_SKU_W5500,
        .en_socknum=1,
        .io_type=type,
        .io_config=io_config, 
        .elp_args={             
            .queue_size=16,     
            .task_core_id=tskNO_AFFINITY,
            .task_name="HWSS ETH LOOP", 
            .task_priority=ESP_TASK_PRIO_MAX-5,          
            .task_stack_size=4096       
        },
        .cctl={
            .hw_rst_ms=10,
            .rst_ionum=rst,
        },                      
        .hir={                  
            .io_num=ir,   
            .tri=HWSS_TRIGGER_NEGEDGE   
        },
        .sir={
            .check_state_period_ms=10,
        },                      
        .phy={                  
            .check_period_ms=10,        
            .reset_timeout_ms=500       
        },                      
        .mac={                  
            .use_burnin_addr=false,
            .addr[0]=0x00,
            .addr[1]=0x08,
            .addr[2]=0xDC,
            .addr[3]=0x04,
            .addr[4]=0x02,
            .addr[5]=0x04,
        },                      
        .hnet={
            .retry_cnt=8,               
            .retry_time_ms=200
        },                      
        .hso={                       
            .tx_buffsize_kb[0]=16,
            .tx_buffsize_kb[1]=0,
            .tx_buffsize_kb[2]=0,
            .tx_buffsize_kb[3]=0,
            .tx_buffsize_kb[4]=0,
            .tx_buffsize_kb[5]=0,
            .tx_buffsize_kb[6]=0,
            .tx_buffsize_kb[7]=0,
            .rx_buffsize_kb[0]=16,
            .rx_buffsize_kb[1]=0,
            .rx_buffsize_kb[2]=0,
            .rx_buffsize_kb[3]=0,
            .rx_buffsize_kb[4]=0,
            .rx_buffsize_kb[5]=0,
            .rx_buffsize_kb[6]=0,
            .rx_buffsize_kb[7]=0,
            .en_socknum=1,
        },
        .sscm={
            .sock_polling_period_ms=5,
            .en_socknum=1,
            .policy=HWSS_SSCM_POLICY_INTR_ONLY,
        },
        .has_hppe=true,
        .hppe={
            .cp_magic_num=0,        
            .cp_request_time_ms=1000
        },                      
        .has_hsl=false,                            
        .has_hdc=false
    };
    return res;
}

static inline hwss_eth_config_t HWSS_ETH_W5100S_DEFAULT_CONFIG(hwss_io_type_t type, void *io_config, gpio_num_t ir, gpio_num_t rst){
    hwss_eth_config_t res={
        .sku=HWSS_ETH_SKU_W5100S,
        .en_socknum=1,
        .io_type=type,
        .io_config=io_config, 
        .elp_args={             
            .queue_size=16,     
            .task_core_id=tskNO_AFFINITY,
            .task_name="HWSS ETH LOOP", 
            .task_priority=ESP_TASK_PRIO_MAX-5,          
            .task_stack_size=4096       
        },
        .cctl={
            .hw_rst_ms=100,
            .rst_ionum=rst,
        },                      
        .hir={                  
            .io_num=ir,   
            .tri=HWSS_TRIGGER_NEGEDGE   
        },
        .sir={
            .check_state_period_ms=10,
        },                      
        .phy={                  
            .check_period_ms=10,        
            .reset_timeout_ms=500       
        },                      
        .mac={                  
            .use_burnin_addr=false,
            .addr[0]=0x00,
            .addr[1]=0x08,
            .addr[2]=0xDC,
            .addr[3]=0x04,
            .addr[4]=0x02,
            .addr[5]=0x05,
        },                      
        .hnet={
            .retry_cnt=8,               
            .retry_time_ms=200
        },                      
        .hso={                       
            .tx_buffsize_kb[0]=8,
            .tx_buffsize_kb[1]=0,
            .tx_buffsize_kb[2]=0,
            .tx_buffsize_kb[3]=0,
            .rx_buffsize_kb[0]=8,
            .rx_buffsize_kb[1]=0,
            .rx_buffsize_kb[2]=0,
            .rx_buffsize_kb[3]=0,
            .en_socknum=1,
        },
        .sscm={
            .sock_active_threshold_ms=1000,
            .sock_polling_period_ms=1,
            .en_socknum=1,
            .policy=HWSS_SSCM_POLICY_INTR_ONLY,
        },
        .has_hppe=true,
        .hppe={
            .cp_magic_num=0,        
            .cp_request_time_ms=1000
        },                      
        .has_hsl=true,          
        .hsl={                  
            .retry_cnt=3,       
            .retry_time_ms=100,
        },                      
        .has_hdc=false
    };
    return res;
}


static inline void hwss_eth_config_set_mac_addr(hwss_eth_config_t *config, hwss_eth_mac_addr_t addr){
    memcpy(config->mac.addr,addr,HWSS_ETH_MAC_ADDR_LEN);
}

static inline void hwss_eth_config_set_tx_rx_buffsize_kb(hwss_eth_config_t *config, uint8_t en_socknum,
            const uint8_t *tx_buffsize_kb, const uint8_t *rx_buffsize_kb){
    memcpy(config->hso.tx_buffsize_kb,tx_buffsize_kb,en_socknum);
    memcpy(config->hso.rx_buffsize_kb,rx_buffsize_kb,en_socknum);

    config->en_socknum=en_socknum;
    config->hso.en_socknum=en_socknum;
    config->sscm.en_socknum=en_socknum;
}
