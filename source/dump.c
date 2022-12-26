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
 */
void mem_dump(char *pDesc, void *pAddr, unsigned int size)
{
    unsigned char *pByte = pAddr;
    unsigned int i = 0;

    if (NULL == pAddr)
    {
        fprintf(stderr, "%s (NULL)\n\n", pDesc);
        return;
    }

    fprintf(stderr, "%s\n", pDesc);
    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            fprintf(stderr, "\n");
        }
        fprintf(stderr, " %02X", pByte[i]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, " (%u bytes)\n\n", size);
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

    fprintf(stderr, "%s\n", pDesc);
    for (i=0; i<len; i++)
    {
        #if 1
        fprintf(stderr, " %+.6f %+.6fi\n", pSeq[i].real, pSeq[i].imag);
        #else
        fprintf(stderr, " %+e %+ei\n", pSeq[i].real, pSeq[i].imag);
        #endif
    }
    fprintf(stderr, " (%u points)\n\n", len);
}

