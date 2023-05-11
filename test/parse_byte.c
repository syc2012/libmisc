#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmisc.h"


int parseByte(unsigned char *pBuf, int size, unsigned int offset)
{
    char label[16];

    sprintf(label, "offset %u", offset);
    mem_dump(label, pBuf, size, 0);

    return PARSE_CONTINUE;
}

int main(int argc, char *argv[])
{
    char *pFile = "Hex.txt";

    if (argc > 1) pFile = argv[1];

    parse_file_into_byte(pFile, parseByte, 256);

    return 0;
}

