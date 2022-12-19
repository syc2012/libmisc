#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
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
 * Generate a random between min. and max. value.
 * @param [in]  min  Min. value.
 * @param [in]  max  Max. value.
 * @returns  Random number.
 */
int rand_num(int min, int max)
{
    struct timeval tv;
    int val;

    gettimeofday(&tv, NULL);

    srand( (unsigned int)tv.tv_usec );
    val = ((rand() % (max - min + 1)) + min);

    return val;
}

/**
 * Get time stamp.
 * @returns  Time stamp string.
 */
char *time_stamp(void)
{
    static char _buffer[4][16];
    static int  _index = 0;
    struct tm *pTM;
    time_t t;

    t = time( NULL );
    pTM = localtime( &t );

    _index = ((_index + 1) & 0x3);

    sprintf(
        _buffer[_index],
        "%04d%02d%02d%02d%02d%02d",
        (pTM->tm_year + 1900),
        (pTM->tm_mon + 1),
        (pTM->tm_mday),
        (pTM->tm_hour),
        (pTM->tm_min),
        (pTM->tm_sec)
    );

    return _buffer[_index];
}

