#include "esp_task.h"
#include "hwss.h"
#include "hwss_type.h"
#include "hwss_event.h"

static esp_event_loop_handle_t s_hwss_event_loop=NULL;

ESP_EVENT_DEFINE_BASE(HWSS_EVENT);
ESP_EVENT_DEFINE_BASE(HWSS_INTER_NET_EVENT);
ESP_EVENT_DEFINE_BASE(HWSS_INTER_SOCK_EVENT);

esp_err_t hwss_event_loop_create(void){
    if(s_hwss_event_loop)
        return ESP_ERR_INVALID_STATE;
    
    esp_event_loop_args_t loop_args={
        .queue_size=32,
        .task_name="hwss_evt",
        .task_stack_size=ESP_TASKD_EVENT_STACK,
        .task_priority=ESP_TASKD_EVENT_PRIO+1,
        .task_core_id=0
    };
    
    return esp_event_loop_create(&loop_args,&s_hwss_event_loop);
}

esp_err_t hwss_event_loop_delete(void){
    if(!s_hwss_event_loop)
        return ESP_ERR_INVALID_STATE;
    
    esp_err_t err=esp_event_loop_delete(s_hwss_event_loop);

    if(err!=ESP_OK)
        return err;

    s_hwss_event_loop=NULL;

    return ESP_OK;
}