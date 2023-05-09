#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "libmisc.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Get network device IP address.
 * @param [in]   pDev  Network device name.
 * @param [out]  pIp   IP address (string).
 * @returns  Success(0) or failure(-1).
 */
int netdev_getIpAddr(char *pDev, char *pIp)
{
    struct ifreq ifreq;
    struct sockaddr_in  *pSaAddr;
    struct sockaddr_in6 *pSaAddr6;
    int err = 0;
    int fd;

    fd = socket(AF_PACKET, SOCK_PACKET, 0);
    if (fd < 0)
    {
        perror ( "socket" );
        return -1;
    }

    strcpy(ifreq.ifr_name, pDev);

    /* Interface index */
    if (ioctl(fd, SIOCGIFINDEX, &ifreq) < 0)
    {
        perror ("SIOCGIFINDEX");
        close( fd );
        return -1;
    }

    /* IP address */
    if (ioctl(fd, SIOCGIFADDR, &ifreq) < 0)
    {
        perror ("SIOCGIFADDR");
        close( fd );
        return -1;
    }

    pSaAddr = (struct sockaddr_in *)&(ifreq.ifr_addr);
    if (pSaAddr->sin_family == AF_INET)
    {
        sprintf(pIp, "%s", inet_ntoa( pSaAddr->sin_addr ));
    }
    else if (pSaAddr->sin_family == AF_INET6)
    {
        pSaAddr6 = (struct sockaddr_in6 *)&(ifreq.ifr_addr);
        sprintf(
            pIp,
            "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
            pSaAddr6->sin6_addr.s6_addr[0],
            pSaAddr6->sin6_addr.s6_addr[1],
            pSaAddr6->sin6_addr.s6_addr[2],
            pSaAddr6->sin6_addr.s6_addr[3],
            pSaAddr6->sin6_addr.s6_addr[4],
            pSaAddr6->sin6_addr.s6_addr[5],
            pSaAddr6->sin6_addr.s6_addr[6],
            pSaAddr6->sin6_addr.s6_addr[7],
            pSaAddr6->sin6_addr.s6_addr[8],
            pSaAddr6->sin6_addr.s6_addr[9],
            pSaAddr6->sin6_addr.s6_addr[10],
            pSaAddr6->sin6_addr.s6_addr[11],
            pSaAddr6->sin6_addr.s6_addr[12],
            pSaAddr6->sin6_addr.s6_addr[13],
            pSaAddr6->sin6_addr.s6_addr[14],
            pSaAddr6->sin6_addr.s6_addr[15]
        );
    }
    else
    {
        pIp[0] = 0;
        err = -1;
    }

    close( fd );
    return err;
}

/**
 * Get network device MAC address.
 * @param [in]   pDev  Network device name.
 * @param [out]  pMac  MAC address (6 bytes).
 * @returns  Success(0) or failure(-1).
 */
int netdev_getMacAddr(char *pDev, unsigned char *pMac)
{
    struct ifreq ifreq;
    int fd;

    fd = socket(AF_PACKET, SOCK_PACKET, 0);
    if (fd < 0)
    {
        perror ( "socket" );
        return -1;
    }

    strcpy(ifreq.ifr_name, pDev);

    /* Interface index */
    if (ioctl(fd, SIOCGIFINDEX, &ifreq) < 0)
    {
        perror ("SIOCGIFINDEX");
        close( fd );
        return -1;
    }

    /* MAC address */
    if (ioctl(fd, SIOCGIFHWADDR, &ifreq) < 0)
    {
        perror ("SIOCGIFHWADDR");
        close( fd );
        return -1;
    }

    memcpy(pMac, (unsigned char *)(ifreq.ifr_hwaddr.sa_data), 6);

    close( fd );
    return 0;
}

/**
 * Get network device MTU.
 * @param [in]   pDev  Network device name.
 * @param [out]  pMtu  MTU size.
 * @returns  Success(0) or failure(-1).
 */
int netdev_getMtu(char *pDev, int *pMtu)
{
    struct ifreq ifreq;
    int fd;

    fd = socket(AF_PACKET, SOCK_PACKET, 0);
    if (fd < 0)
    {
        perror ( "socket" );
        return -1;
    }

    strcpy(ifreq.ifr_name, pDev);

    /* Interface index */
    if (ioctl(fd, SIOCGIFINDEX, &ifreq) < 0)
    {
        perror ("SIOCGIFINDEX");
        close( fd );
        return -1;
    }

    /* MTU */
    if (ioctl(fd, SIOCGIFMTU, &ifreq) < 0)
    {
        perror ("SIOCGIFMTU");
        close( fd );
        return -1;
    }

    *pMtu = ifreq.ifr_mtu;

    close( fd );
    return 0;
}

