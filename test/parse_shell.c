#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmisc.h"

int parseLine(char *pStr, int len, int count)
{
    printf("[shell] %s\n", pStr);
    return PARSE_CONTINUE;
}

int main(int argc, char *argv[])
{
    char *pCmd = "ls -al";

    if (argc > 1) pCmd = argv[1];
    shell_command(pCmd, parseLine);

    return 0;
}

