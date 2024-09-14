#pragma once

#include "esp_event.h"

typedef enum{
    HWSS_INTER_NET_EVENT_IP_CONFLICT,
    HWSS_INTER_NET_EVENT_DEST_UNREACH,
    HWSS_INTER_NET_EVENT_PPPOE_CLOSE,
    HWSS_INTER_NET_EVENT_MAGIC_PACK
}hwss_inter_net_event;

typedef enum{
    HWSS_INTER_SOCK_EVENT_SEND_OK,
    HWSS_INTER_SOCK_EVENT_TIMEOUT,
    HWSS_INTER_SOCK_EVENT_RECV,
    HWSS_INTER_SOCK_EVENT_DISCONN,
    HWSS_INTER_SOCK_EVENT_CONNECT
}hwss_inter_sock_event;

ESP_EVENT_DEFINE_BASE(HWSS_INTER_NET_EVENT);
ESP_EVENT_DEFINE_BASE(HWSS_INTER_SOCK_EVENT);

esp_err_t hwss_event_handler_register(esp_event_base_t event_base, int32_t event_id,
                                     esp_event_handler_t event_handler, void* event_handler_arg);

esp_err_t hwss_event_handler_instance_register(esp_event_base_t event_base,
                                              int32_t event_id,
                                              esp_event_handler_t event_handler,
                                              void *event_handler_arg,
                                              esp_event_handler_instance_t *context);

esp_err_t hwss_event_handler_unregister(esp_event_base_t event_base, int32_t event_id,
                                       esp_event_handler_t event_handler);

esp_err_t hwss_event_handler_instance_unregister(esp_event_base_t event_base,
                                                int32_t event_id,
                                                esp_event_handler_instance_t context);

esp_err_t hwss_event_post(esp_event_base_t event_base, int32_t event_id,
                         const void* event_data, size_t event_data_size, TickType_t ticks_to_wait);

esp_err_t hwss_event_isr_post(esp_event_base_t event_base, int32_t event_id,
                             const void* event_data, size_t event_data_size, BaseType_t* task_unblocked);
