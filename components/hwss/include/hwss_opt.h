#pragma once

#define HWSS_SOCKET_BITS                    8
#define HWSS_DEVID_BITS                     4
#define HWSS_SOCKID_BITS                    4

#define HWSS_DHCPS                          1

#if HWSS_DEVID_BITS+HWSS_SOCKID_BITS!=HWSS_SOCKET_BITS
    #error HWSS_SOCKET_BITS should be equal to HWSS_DEVID_BITS+HWSS_SOCKID_BITS!
#endif

