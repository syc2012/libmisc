#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmisc.h"


#define MAX_BUF_SIZE 16

tComplex g_buf[MAX_BUF_SIZE];

int main(int argc, char *argv[])
{
    char *pFile = "Complex.txt";
    unsigned int num;

    if (argc > 1) pFile = argv[1];

    num = parse_complex_string_file(pFile, g_buf, MAX_BUF_SIZE);

    cplx_dump(pFile, g_buf, num);

    return 0;
}

