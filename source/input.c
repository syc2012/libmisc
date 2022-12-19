#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
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
 * Get one character from keyboard input.
 * @returns  Character value.
 */
int getch(void)
{
    struct termios org_opts, new_opts;
    int ch;

    /* store old settings */
    tcgetattr(STDIN_FILENO, &org_opts);

    /* set new terminal parms */
    memcpy(&new_opts, &org_opts, sizeof( new_opts ));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    ch = getchar();

    /* restore old settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);

    return ch;
}

/**
 * Wait for a key board event.
 */
void stop(void)
{
    fprintf(stderr, "Press any key to continue ...\n");
    getch();
}

