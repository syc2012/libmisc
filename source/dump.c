#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <execinfo.h>
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
 * Dump memory.
 * @param [in]  pDesc  Description string.
 * @param [in]  pAddr  Memory address.
 * @param [in]  size   Memory size.
 * @param [in]  ascii  Print ASCII (boolean).
 */
void mem_dump(char *pDesc, void *pAddr, unsigned int size, int ascii)
{
    unsigned char *pByte = pAddr;
    unsigned int i = 0;

    if (NULL == pAddr)
    {
        printf("%s (NULL)\n\n", pDesc);
        return;
    }

    printf("%s\n", pDesc);
    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        if ( ascii )
        {
            if ((pByte[i] >= 0x20) && (pByte[i] <= 0x7E))
            {
                printf("  %c", pByte[i]);
            }
            else
            {
                printf(" %02X", pByte[i]);
            }
        }
        else
        {
            printf(" %02X", pByte[i]);
        }
    }
    printf("\n");
    printf(" (%u bytes)\n\n", size);
}

/**
 * Print complex number sequence.
 * @param [in]  pDesc  Description string.
 * @param [in]  pSeq   Complex number sequence.
 * @param [in]  len    Complex number sequence length.
 * @param [in]  eForm  Scientific notation.
*/
void cplx_dump(char *pDesc, tComplex *pSeq, unsigned int len, int eForm)
{
    int i;

    if (NULL == pSeq)
    {
        printf("%s (NULL)\n\n", pDesc);
        return;
    }

    printf("%s\n", pDesc);
    if ( eForm )
    {
        for (i=0; i<len; i++)
        {
            printf(" %+e %+ei\n", pSeq[i].real, pSeq[i].imag);
        }
    }
    else
    {
        for (i=0; i<len; i++)
        {
            printf(" %+.8f %+.8fi\n", pSeq[i].real, pSeq[i].imag);
        }
    }
    printf(" (%u points)\n\n", len);
}

/**
 * Dump stack.
 * @param [in]  pDesc  Description string.
 */
void stack_dump(char *pDesc)
{
    void *pBuf[128];
    char **ppStr;
    int frames;
    int i;

    printf("%s\n", pDesc);
    frames = backtrace(pBuf, 128);
    ppStr = backtrace_symbols(pBuf, frames);
    if ( ppStr )
    {
        for (i=0; i<frames; i++)
        {
            printf("%s\n", ppStr[i]);
        }
        printf("\n");
        free( ppStr );
    }
}

