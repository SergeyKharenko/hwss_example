#pragma once

// #include "freertos/event_groups.h"
#include "esp_event.h"

typedef enum{
    HWSS_ETH_EVENT_START,
    HWSS_ETH_EVENT_STOP,
    HWSS_ETH_EVENT_CONNECTED,
    HWSS_ETH_EVENT_DISCONNECTED,
}hwss_eth_event_t;

typedef enum{
    HWSS_IP_EVENT_GOT_IP,
    HWSS_IP_EVENT_LOST_IP
}hwss_ip_event_t;

#define HWSS_EVENT_ANY_ID                       -1
#define HWSS_EVENT_ANY_BASE                     NULL

ESP_EVENT_DECLARE_BASE(HWSS_ETH_EVENT);
ESP_EVENT_DECLARE_BASE(HWSS_IP_EVENT);

