#include "freertos/semphr.h"
#include "esp_log.h"
#include "hwss_sockets.h"

#define HWSS_SOCKET_MAP_LOCK_TIMEOUT_MS     10
#define HWSS_SOCKET_MAX                     1<<(HWSS_SOCKET_BITS+1)-1

#define HWSS_SOCKET_IDLE                    0x00
#define HWSS_SOCKET_RUNNING                 0xFF

static const char *TAG = "hwss_sockets";

static uint8_t hwss_sockmap[HWSS_SOCKET_MAX];
static uint8_t hwss_sockmap_state[HWSS_SOCKET_MAX];
static SemaphoreHandle_t hwss_sockmap_lock;

static inline bool HWSS_SOCKMAP_LOCK(void){
    return xSemaphoreTake(hwss_sockmap_lock, pdMS_TO_TICKS(HWSS_SOCKET_MAP_LOCK_TIMEOUT_MS)) == pdTRUE; 
}

static inline bool HWSS_SOCKMAP_UNLOCK(void){
    return xSemaphoreGive(hwss_sockmap_lock) == pdTRUE;
}

static esp_err_t hwss_sockmap_init(void){
    esp_err_t ret=ESP_OK;

    memset(hwss_sockmap,0,HWSS_SOCKET_MAX);
    memset(hwss_sockmap_state,HWSS_SOCKET_IDLE,HWSS_SOCKET_MAX);

    hwss_sockmap_lock = xSemaphoreCreateMutex();
    ESP_GOTO_ON_FALSE(hwss_sockmap_lock,NULL,err,TAG,"create map lock failed")
    return ret;

err:
    return ESP_FAIL;
}

static int hwss_sockmap_alloc(void){
    int ret=-1;

    if(!HWSS_SOCKMAP_LOCK())
        return ret;

    for(uint8_t i=0;i<HWSS_SOCKET_MAX;i++){
        if(hwss_sockmap_state[i]==HWSS_SOCKET_IDLE){
            ret=i;
            hwss_sockmap_state[i]=HWSS_SOCKET_RUNNING;
            break;
        }
    }

    HWSS_SOCKMAP_UNLOCK();
    return ret;
}

static esp_err_t hwss_sockmap_free(int s){
    if(s>=HWSS_SOCKET_MAX||s<0)
        return ESP_ERR_INVALID_ARG;
    if(!HWSS_SOCKMAP_LOCK())
        return ESP_FAIL;
    if(hwss_sockmap_state[s]!=HWSS_SOCKET_RUNNING)
        return ESP_ERR_NOT_ALLOWED;
    hwss_sockmap_state[s]=HWSS_SOCKET_IDLE;
    HWSS_SOCKMAP_UNLOCK();
    return ESP_OK;
}

static esp_err_t hwss_sockmap_map(){

}




