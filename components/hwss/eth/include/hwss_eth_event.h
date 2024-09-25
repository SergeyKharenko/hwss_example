#pragma once

#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(HWSS_PHY_EVENT);
ESP_EVENT_DECLARE_BASE(HWSS_HIR_EVENT);
ESP_EVENT_DECLARE_BASE(HWSS_HNET_EVENT);
ESP_EVENT_DECLARE_BASE(HWSS_HSO_EVENT);
ESP_EVENT_DECLARE_BASE(HWSS_HSL_EVENT);

typedef enum{
    HWSS_PHY_EVENT_CONNECT=0x01,
    HWSS_PHY_EVENT_DISCONN=0x02,
}hwss_phy_event;

typedef enum{
    HWSS_HIR_EVENT_TRIGGER=0x03
}hwss_hir_event;

typedef enum{
    HWSS_HNET_EVENT_IP_CONFLICT=0x04,
    HWSS_HNET_EVENT_DEST_UNREACH=0x05,
    HWSS_HNET_EVENT_PPPOE_CLOSE=0x06,
    HWSS_HNET_EVENT_MAGIC_PACK=0x07,
}hwss_hnet_event;
    
typedef enum{
    HWSS_HSO_EVENT_SEND_OK=0x08,
    HWSS_HSO_EVENT_TIMEOUT=0x09,
    HWSS_HSO_EVENT_RECV=0x0A,
    HWSS_HSO_EVENT_DISCONN=0x0B,
    HWSS_HSO_EVENT_CONNECT=0x0C,
}hwss_hso_event;

typedef enum{
    HWSS_HSL_EVENT_TIMEOUT=0x0D,
    HWSS_HSL_EVENT_ARP=0x0E,
    HWSS_HSL_EVENT_PING=0x0F,
}hwss_hsl_event;

