#pragma once

#include "lwip/sockets.h"
#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USOCK_LWIP_SOCK_BIAS                         8
#define USOCK_U2LWIP(s)                             s>>USOCK_LWIP_SOCK_BIAS
#define USOCK_LWIP2U(s)                             s<<USOCK_LWIP_SOCK_BIAS

static inline int uaccept(int s,struct sockaddr *addr,socklen_t *addrlen)
{
#if CONFIG_USOCK_HWSS_SUPPORT
    if(USOCK_U2LWIP(s))
        lwip_accept(USOCK_U2LWIP(s),addr,addrlen);
    else
        void;
#else
    return lwip_accept(s,addr,addrlen);
#endif
}


static inline int ubind(int s,const struct sockaddr *name, socklen_t namelen)
{ 
#if CONFIG_USOCK_HWSS_SUPPORT
    if(USOCK_U2LWIP(s))
        lwip_bind(USOCK_U2LWIP(s),name,namelen);
    else
        void;
#else
    return lwip_bind(s,name,namelen); 
#endif
}

static inline int ushutdown(int s,int how)
{ 
#if CONFIG_USOCK_HWSS_SUPPORT
    if(USOCK_U2LWIP(s))
        lwip_shutdown(USOCK_U2LWIP(s),how);
    else
        void;
#else
    return lwip_shutdown(s,how); 
#endif
}

static inline int ugetpeername(int s,struct sockaddr *name,socklen_t *namelen)
{ 
#if CONFIG_USOCK_HWSS_SUPPORT
    if(USOCK_U2LWIP(s))
        lwip_getpeername(USOCK_U2LWIP(s),name,namelen);
    else
        void;
#else
    return lwip_getpeername(s,name,namelen); 
#endif
}

static inline int getsockname(int s,struct sockaddr *name,socklen_t *namelen)
{ return lwip_getsockname(s,name,namelen); }
static inline int setsockopt(int s,int level,int optname,const void *opval,socklen_t optlen)
{ return lwip_setsockopt(s,level,optname,opval,optlen); }
static inline int getsockopt(int s,int level,int optname,void *opval,socklen_t *optlen)
{ return lwip_getsockopt(s,level,optname,opval,optlen); }
static inline int closesocket(int s)
{ return lwip_close(s); }
static inline int connect(int s,const struct sockaddr *name,socklen_t namelen)
{ return lwip_connect(s,name,namelen); }
static inline int listen(int s,int backlog)
{ return lwip_listen(s,backlog); }
static inline ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{ return lwip_recvmsg(sockfd, msg, flags); }
static inline ssize_t recv(int s,void *mem,size_t len,int flags)
{ return lwip_recv(s,mem,len,flags); }
static inline ssize_t recvfrom(int s,void *mem,size_t len,int flags,struct sockaddr *from,socklen_t *fromlen)
{ return lwip_recvfrom(s,mem,len,flags,from,fromlen); }
static inline ssize_t send(int s,const void *dataptr,size_t size,int flags)
{ return lwip_send(s,dataptr,size,flags); }
static inline ssize_t sendmsg(int s,const struct msghdr *message,int flags)
{ return lwip_sendmsg(s,message,flags); }
static inline ssize_t sendto(int s,const void *dataptr,size_t size,int flags,const struct sockaddr *to,socklen_t tolen)
{ return lwip_sendto(s,dataptr,size,flags,to,tolen); }
static inline int socket(int domain,int type,int protocol)
{ return lwip_socket(domain,type,protocol); }
static inline const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{ return lwip_inet_ntop(af, src, dst, size);  }
static inline int inet_pton(int af, const char *src, void *dst)
{ return lwip_inet_pton(af, src, dst); }















#ifdef __cplusplus
}
#endif
