#include "hwss_type.h"

bool hwss_io_type_supported(hwss_io_type *list, hwss_io_type type){
    while(*list!=HWSS_IO_TYPE_END){
        if(type == *list)
            return true;
        list++;
    }
    return false;
}
