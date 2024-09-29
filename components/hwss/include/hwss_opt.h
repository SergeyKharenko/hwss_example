#pragma once

#define HWSS_SOCKET_BITS                    8
#define HWSS_DEVID_BITS                     4
#define HWSS_SOCKID_BITS                    4

#define HWSS_ETH_SOCKNUM_MAX                16

#define HWSS_ETH_NAMELEN_MAX                16

#define HWSS_IO_CACHE_SIZE                  8
#define HWSS_IO_LOCK_TIMEOUT_MS             20

#if HWSS_DEVID_BITS+HWSS_SOCKID_BITS!=HWSS_SOCKET_BITS
    #error HWSS_SOCKET_BITS should be equal to HWSS_DEVID_BITS+HWSS_SOCKID_BITS!
#endif

