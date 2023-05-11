#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmisc.h"


#define MAX_BUF_SIZE 4096

unsigned char g_buf[MAX_BUF_SIZE];

int main(int argc, char *argv[])
{
    char *pFile = "Hex.txt";
    unsigned int len;

    if (argc > 1) pFile = argv[1];

    len = parse_hex_string_file(pFile, g_buf, MAX_BUF_SIZE);
    if (len > 0)
    {
        mem_dump(pFile, g_buf, len, 1);
    }

    return 0;
}

