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
*/
void cplx_dump(char *pDesc, tComplex *pSeq, unsigned int len)
{
    int i;

    printf("%s\n", pDesc);
    for (i=0; i<len; i++)
    {
        #if 1
        printf(" %+.6f %+.6fi\n", pSeq[i].real, pSeq[i].imag);
        #else
        printf(" %+e %+ei\n", pSeq[i].real, pSeq[i].imag);
        #endif
    }
    printf(" (%u points)\n\n", len);
}

