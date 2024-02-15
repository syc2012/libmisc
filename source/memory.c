#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libmisc.h"


/**
* Flexible memory initial.
* @param [in]  pMem  A @ref tFlexMem object.
*/
void fmem_init(tFlexMem *pMem)
{
    memset(pMem, 0, sizeof( tFlexMem ));
}

/**
* Flexible memory un-initial.
* @param [in]  pMem  A @ref tFlexMem object.
*/
void fmem_uninit(tFlexMem *pMem)
{
    if (NULL != pMem->pBuf)
    {
        free( pMem->pBuf );
        pMem->pBuf = NULL;
    }
    pMem->len = pMem->lenMax = 0;
}

/**
* Flexible memory resize.
* @param [in]  pMem   A @ref tFlexMem object.
* @param [in]  bytes  Memory buffer size.
* @returns  Memory buffer.
*/
void *fmem_resize(tFlexMem *pMem, unsigned int bytes)
{
    if (NULL == pMem->pBuf)
    {
        pMem->pBuf = malloc( bytes );
        pMem->len = pMem->lenMax = bytes;
        return pMem->pBuf;
    }

    if (bytes > pMem->lenMax)
    {
        free( pMem->pBuf );
        pMem->pBuf = malloc( bytes );
        pMem->len = pMem->lenMax = bytes;
        return pMem->pBuf;
    }

    pMem->len = bytes;
    return pMem->pBuf;
}

