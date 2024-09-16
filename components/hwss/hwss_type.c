#include "hwss_type.h"

bool hwss_io_type_supported(const hwss_io_types_t list, hwss_io_type_t type){
    uint8_t idx=0;
    while(list[idx]!=HWSS_IO_TYPE_END){
        if(type == *list)
            return true;
        idx++;
    }
    return false;
}
