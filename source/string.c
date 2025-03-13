#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
 * Remove CR, LF in the end of string.
 * @param [in,out]  pStr  String buffer.
 */
void removeCRLF(char *pStr)
{
    int i;

    for (i=(strlen(pStr)-1); i>=0; i--)
    {
        switch ( pStr[i] )
        {
            case '\r': case '\n':
                pStr[i] = 0;
                break;
            default:
                return;
        }
    }
}

/**
 * Convert HEX to DEC.
 * @param [in]  ch  HEX character (0 ~ F).
 * @returns  Integer 0 ~ 15.
 */
unsigned char hex2dec(char ch)
{
    unsigned char val = 0;

    switch ( ch )
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            val = (ch - '0');
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            val = (ch - 'a');
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            val = (ch - 'A');
            break;
        default:
            break;
    }

    return val;
}


/**
 * Convert IP address data to string.
 * @param [in]   ver      IP version (4 or 6).
 * @param [in]   pAddr    IP address data (byte array).
 * @param [out]  pBuf     IP address string buffer.
 * @param [in]   bufSize  IP address string buffer size.
 * @returns  String length.
 */
int ip2str(int ver, void *pAddr, char *pBuf, int bufSize)
{
    unsigned char *pIp = pAddr;
    int retval = 0;

    pBuf[0] = 0x00;

    if (4 == ver)
    {
        retval = snprintf(
                    pBuf,
                    bufSize,
                    "%u.%u.%u.%u",
                    pIp[0],
                    pIp[1],
                    pIp[2],
                    pIp[3]
                );
    }
    else if (6 == ver)
    {
        retval = snprintf(
                     pBuf,
                     bufSize,
                     "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
                     pIp[0], pIp[1],
                     pIp[2], pIp[3],
                     pIp[4], pIp[5],
                     pIp[6], pIp[7],
                     pIp[8], pIp[9],
                     pIp[10], pIp[11],
                     pIp[12], pIp[13],
                     pIp[14], pIp[15]
                 );
    }

    return retval;
}

/**
 * Parse IPv6 address string.
 * @param [in]   src  IPv6 address string.
 * @param [out]  dst  IPv6 address buffer.
 * @returns  Success(0) or failure(-1).
 */
static int _parseIpv6Str(const char *src, unsigned char *dst)
{
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ    2
    static const char xdigits_l[] = "0123456789abcdef",
                      xdigits_u[] = "0123456789ABCDEF";
    unsigned char tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
    const char *xdigits;
    int ch, seen_xdigits;
    int val;

    memset((tp = tmp), '\0', NS_IN6ADDRSZ);
    endp = tp + NS_IN6ADDRSZ;
    colonp = NULL;

    /* Leading :: requires some special handling. */
    if (*src == ':')
    {
        if (*++src != ':')
        {
            return -1;
        }
    }

    seen_xdigits = 0;
    val = 0;
    while ((ch = *src++) != '\0')
    {
        const char *pch;
        
        if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
        {
            pch = strchr((xdigits = xdigits_u), ch);
        }
        if (pch != NULL)
        {
            val <<= 4;
            val |= (pch - xdigits);
            if (++seen_xdigits > 4)
            {
                return -1;
            }
            continue;
        }
        if (ch == ':')
        {
            if (!seen_xdigits)
            {
                if (colonp)
                        return -1;
                colonp = tp;
                continue;
            }
            else if (*src == '\0')
            {
                return -1;
            }
            if (tp + NS_INT16SZ > endp)
            {
                return -1;
            }
            *tp++ = (unsigned char) (val >> 8) & 0xff;
            *tp++ = (unsigned char) val & 0xff;
            seen_xdigits = 0;
            val = 0;
            continue;
        }
        return -1;
    }
    if (seen_xdigits)
    {
        if (tp + NS_INT16SZ > endp)
        {
            return -1;
        }
        *tp++ = (unsigned char) (val >> 8) & 0xff;
        *tp++ = (unsigned char) val & 0xff;
    }
    if (colonp != NULL)
    {
        /*
         * Since some memmove()'s erroneously fail to handle
         * overlapping regions, we'll do the shift by hand.
         */
        const int n = tp - colonp;
        int i;
        
        if (tp == endp)
        {
            return -1;
        }
        for (i = 1; i <= n; i++)
        {
            endp[- i] = colonp[n - i];
            colonp[n - i] = 0;
        }
        tp = endp;
    }
    if (tp != endp)
    {
        return -1;
    }

    memcpy(dst, tmp, NS_IN6ADDRSZ);
    return 0;
}

/**
 * Convert IP address string to byte array.
 * @param [in]   ver    IP version (4 or 6).
 * @param [in]   pStr   IP address string.
 * @param [out]  pAddr  IP address data buffer (byte array).
 * @returns  Data length.
 */
int str2ip(int ver, char *pStr, void *pAddr)
{
    unsigned char *pIp = pAddr;
    int  buf[16] = {0};
    int  retval = 0;

    if (4 == ver)
    {
        retval = sscanf(
                     pStr,
                     "%u.%u.%u.%u",
                     &(buf[0]),
                     &(buf[1]),
                     &(buf[2]),
                     &(buf[3])
                 );

        pIp[0] = (buf[0] & 0xFF);
        pIp[1] = (buf[1] & 0xFF);
        pIp[2] = (buf[2] & 0xFF);
        pIp[3] = (buf[3] & 0xFF);
    }
    else if (6 == ver)
    {
        retval = _parseIpv6Str(pStr, pIp);
        retval = ((0 == retval) ? 16 : 0);
    }

    return retval;
}


/**
 * Convert byte array data to HEX string.
 * @param [in]   pHex     Byte array data.
 * @param [in]   hexSize  Byte array data size.
 * @param [out]  pBuf     String buffer.
 * @param [in]   bufSize  String buffer size.
 * @returns  String length.
 */
int hex2str(void *pHex, int hexSize, char *pBuf, int bufSize)
{
    unsigned char *pByte = (unsigned char *)pHex;
    char *pStr = pBuf;
    int i;

    if (bufSize <= (hexSize << 1))
    {
        pBuf[0] = 0x00;
        fprintf(
            stderr,
            "%s: un-enough output buffer length %d\n",
            __func__,
            bufSize
        );
        return 0;
    }

    /*
    * Example:
    *   Input   0x11 0x22 0x33 0x44
    *   Output  "11223344"
    */
    for (i=0; i<hexSize; i++)
    {
        sprintf(pStr, "%02X", pByte[i]);
        pStr += 2;
    }

    (*pStr) = 0x00;

    return strlen( pBuf );
}

/**
 * Convert HEX string to byte array.
 * @param [in]   pStr     HEX string.
 * @param [out]  pBuf     Byte array buffer.
 * @param [in]   bufSize  Byte array buffer size.
 * @returns  Data length.
 */
int str2hex(char *pStr, unsigned char *pBuf, int bufSize)
{
    unsigned char  nibbleH;
    unsigned char  nibbleL;
    unsigned char  byte;
    int len;
    int i;
    int j;

    len = strlen( pStr );

    if ((len % 2) != 0)
    {
        memset(pBuf, 0x00, bufSize);
        fprintf(
            stderr,
            "%s: wrong input string length %d\n",
            __func__,
            len
        );
        return 0;
    }

    if (bufSize < (len >> 1))
    {
        memset(pBuf, 0x00, bufSize);
        fprintf(
            stderr,
            "%s: un-enough output buffer length %d\n",
            __func__,
            bufSize
        );
        return 0;
    }

    /*
    * Example:
    *   Input   "11223344"
    *   Output  0x11 0x22 0x33 0x44
    */
    for (i=0, j=0; i<len; i+=2)
    {
        nibbleH = hex2dec( pStr[i  ] );
        nibbleL = hex2dec( pStr[i+1] );

        byte = ((nibbleH << 4) | (nibbleL));

        pBuf[j++] = byte;
    }

    return j;
}


/**
 * Convert byte array data to PLMN ID string.
 * @param [in]   pPlmn     Byte array data.
 * @param [in]   plmnSize  Byte array data size.
 * @param [out]  pBuf      String buffer.
 * @param [in]   bufSize   String buffer size.
 * @returns  String length.
 */
int plmn2str(void *pPlmn, int plmnSize, char *pBuf, int bufSize)
{
    unsigned char *pByte = pPlmn;

    if (plmnSize != 3)
    {
        pBuf[0] = 0x00;
        fprintf(
            stderr,
            "%s: incorrect PLMN ID length %d\n",
            __func__,
            plmnSize
        );
        return 0;
    }

    if (bufSize < 8)
    {
        pBuf[0] = 0x00;
        fprintf(
            stderr,
            "%s: un-enough output buffer length %d\n",
            __func__,
            bufSize
        );
        return 0;
    }

    if (((pByte[1] & 0xF0) >> 4) == 0xF)
    {
        /*
        * Example:
        *   Input   0x64 0xF6 0x29
        *   Output  "466.92"
        */
        sprintf(
            pBuf,
            "%u%u%u.%u%u",
            ((pByte[0] & 0x0F)     ),
            ((pByte[0] & 0xF0) >> 4),
            ((pByte[1] & 0x0F)     ),
            ((pByte[2] & 0x0F)     ),
            ((pByte[2] & 0xF0) >> 4)
        );
    }
    else
    {
        /*
        * Example:
        *   Input   0x21 0x43 0x65
        *   Output  "123.456"
        */
        sprintf(
            pBuf,
            "%u%u%u.%u%u%u",
            ((pByte[0] & 0x0F)     ),
            ((pByte[0] & 0xF0) >> 4),
            ((pByte[1] & 0x0F)     ),
            ((pByte[1] & 0xF0) >> 4),
            ((pByte[2] & 0x0F)     ),
            ((pByte[2] & 0xF0) >> 4)
        );
    }

    return strlen( pBuf );
}

/**
 * Convert PLMN ID string to byte array.
 * @param [in]   pStr     PLMN ID string.
 * @param [out]  pBuf     Byte array buffer.
 * @param [in]   bufSize  Byte array buffer size.
 * @returns  Data length.
 */
int str2plmn(char *pStr, unsigned char *pBuf, int bufSize)
{
    int len;

    len = strlen( pStr );

    if ((len != 6) && (len != 7))
    {
        memset(pBuf, 0x00, bufSize);
        fprintf(
            stderr,
            "%s: wrong input string length %d\n",
            __func__,
            len
        );
        return 0;
    }

    if (pStr[3] != '.')
    {
        memset(pBuf, 0x00, bufSize);
        fprintf(
            stderr,
            "%s: wrong input string format %s\n",
            __func__,
            pStr
        );
        return 0;
    }

    if (bufSize < 3)
    {
        memset(pBuf, 0x00, bufSize);
        fprintf(
            stderr,
            "%s: un-enough output buffer length %d\n",
            __func__,
            bufSize
        );
        return 0;
    }

    if (6 == len)
    {
        /*
        * Example:
        *   Input   "466.92"
        *   Output  0x64 0xF6 0x29
        */
        pBuf[0]  = (((pStr[0] - '0') & 0xF)     );
        pBuf[0] |= (((pStr[1] - '0') & 0xF) << 4);
        pBuf[1]  = (((pStr[2] - '0') & 0xF)     );
        pBuf[1] |= 0xF0;
        pBuf[2]  = (((pStr[4] - '0') & 0xF)     );
        pBuf[2] |= (((pStr[5] - '0') & 0xF) << 4);
    }
    else
    {
        /*
        * Example:
        *   Input   "123.456"
        *   Output  0x21 0x43 0x65
        */
        pBuf[0]  = (((pStr[0] - '0') & 0xF)     );
        pBuf[0] |= (((pStr[1] - '0') & 0xF) << 4);
        pBuf[1]  = (((pStr[2] - '0') & 0xF)     );
        pBuf[1] |= (((pStr[4] - '0') & 0xF) << 4);
        pBuf[2]  = (((pStr[5] - '0') & 0xF)     );
        pBuf[2] |= (((pStr[6] - '0') & 0xF) << 4);
    }

    return 3;
}

