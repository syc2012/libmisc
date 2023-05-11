#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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

static int _logOutput = L_ALL;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Log output option.
 * @param [in]  mask  A @ref tLogMask enumeration.
 */
void log_option(int mask)
{
    _logOutput = (mask & L_ALL);
}

/**
 * Log print function.
 * @param [in]  mask     A @ref tLogMask enumeration.
 * @param [in]  pFormat  Print format.
 */
void log_print(int mask, char *pFormat, ...)
{
    va_list args;

    if (_logOutput & mask)
    {
        switch ( mask )
        {
            case L_INFO:
                printf("[LOG-I] ");
                break;
            case L_WARN:
                printf("[LOG-[1;33mW[0m] ");
                break;
            case L_ERROR:
                printf("[LOG-[1;31mE[0m] ");
                break;
            case L_DEBUG:
                printf("[LOG-[1;36mD[0m] ");
                break;
            case L_TRACE:
                printf("[LOG-[1;32mT[0m] ");
                break;
            default:
                printf("[LOG-?] ");
        }

        va_start(args, pFormat);
        vprintf(pFormat, args);
        va_end( args );
    }
}

