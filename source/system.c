#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
 * Test system endian.
 * @returns  Big(1) or litte(0) endian.
 */
int endian(void)
{
    unsigned char data[4] = { 0x11, 0x22, 0x33, 0x44 };

    return (0x11223344 == *((int *)data));
}

/**
 * Test system bit number.
 * @returns  Bits.
*/
int bitsize(void)
{
    return (sizeof(void *) * 8);
}

