#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmisc.h"


int parseToken(char *pStr, int len, int count)
{
    if (count > 1) printf(", ");
    printf("\"%s\"", pStr);
    return PARSE_CONTINUE;
}

int parseLine(char *pStr, int len, int count)
{
    parse_string_into_token(pStr, parseToken);
    printf("\n\n");
    return PARSE_CONTINUE;
}

int main(int argc, char *argv[])
{
    char *pFile = "Text.txt";

    if (argc > 1) pFile = argv[1];

    parse_file_into_line(pFile, parseLine);

    return 0;
}

