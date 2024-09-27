#pragma once
// #include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "hwss_ip_addr.h"



// static inline bool hwss_same_option_check(uint8_t num, bool opt,...){
//     if(num<2)
//         return true;
    
//     bool cal_res=opt;
//     bool arg;
//     bool res=true;

//     va_list args;
//     va_start(args,opt);

//     if(opt){
//         while(num--){
//             arg=va_arg(args,bool);
//             cal_res=cal_res&&arg;
//             if(!cal_res){
//                 res=false;
//                 break;
//             }
//         }
//     }
//     else{
//         while(num--){
//             arg=va_arg(args,bool);
//             cal_res=cal_res||opt;
//             if(cal_res){
//                 res=false;
//                 break;
//             }
//         }
//     }
//     va_end(args);
//     return res;
// }


