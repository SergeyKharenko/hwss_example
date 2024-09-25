#pragma once

// #include "freertos/event_groups.h"
#include "esp_event.h"

typedef enum{
    HWSS_EVENT_CONNECTED,
    HWSS_EVENT_DISCONNECTED,
}hwss_event_t;

#define HWSS_EVENT_ANY_ID                       -1
#define HWSS_EVENT_ANY_BASE                     NULL

ESP_EVENT_DECLARE_BASE(HWSS_EVENT);

#define HWSS_PHY_EVENTBIT_CONNECTED             1<<0
#define HWSS_PHY_EVENTBIT_DISCONNECTED          1<<1

#define HWSS_HSL_EVENTBIT_TIMEOUT               1<<2
#define HWSS_HSL_EVENTBIT_ARP                   1<<3
#define HWSS_HSL_EVENTBIT_PING                  1<<4

#define HWSS_HSO_EVENTBIT_SEND_OK               1<<5
#define HWSS_HSO_EVENTBIT_TIMEOUT               1<<6
#define HWSS_HSO_EVENTBIT_RECV                  1<<7
#define HWSS_HSO_EVENTBIT_DISCONN               1<<8
#define HWSS_HSO_EVENTBIT_CONNECT               1<<9

#define HWSS_HNET_EVENTBIT_IP_CONFLICT          1<<10
#define HWSS_HNET_EVENTBIT_DEST_UNREACH         1<<11
#define HWSS_HNET_EVENTBIT_PPPOE_CLOSE          1<<12
#define HWSS_HNET_EVENTBIT_MAGIC_PACK           1<<13

#define HWSS_HIR_EVENTBIT_TRIGGER               1<<14
