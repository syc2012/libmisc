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

#define MAX_TSPEC_NUM 16


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

static struct timespec _tspec[MAX_TSPEC_NUM][2];


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
 * Sleep in milli-second(s).
 * @param [in]  ms  milli-second.
 */
void msleep(unsigned int ms)
{
    struct timeval tout;

    tout.tv_sec  = ms / 1000;
    tout.tv_usec = (ms % 1000) * 1000;

    while (select(0, 0, 0, 0, &tout) < 0);
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

/**
 * Set time mark point A.
 * @param [in]  id  Mark ID (0 ~ 15).
 */
void time_markA(int id)
{
    id = (id & (MAX_TSPEC_NUM - 1));
    _tspec[id][1].tv_sec = 0;
    _tspec[id][1].tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &(_tspec[id][0]));
}

/**
 * Set time mark point B.
 * @param [in]  id  Mark ID (0 ~ 15).
 */
void time_markB(int id)
{
    id = (id & (MAX_TSPEC_NUM - 1));
    clock_gettime(CLOCK_MONOTONIC, &(_tspec[id][1]));
}

/**
 * Get time elapsed from point A to B.
 * @param [in]  id  Mark ID (0 ~ 15).
 * @returns  Time elapsed in nsec.
 */
long time_elapse(int id)
{
    long nsec;

    id = (id & (MAX_TSPEC_NUM - 1));
    nsec = ((_tspec[id][1].tv_sec * 1000000000L) + _tspec[id][1].tv_nsec) -
           ((_tspec[id][0].tv_sec * 1000000000L) + _tspec[id][0].tv_nsec);

    return nsec;
}

