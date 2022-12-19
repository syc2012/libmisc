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
    unsigned int i;

    if (argc > 1) pFile = argv[1];

    num = parse_complex_string_file(pFile, g_buf, MAX_BUF_SIZE);

    for (i=0; i<num; i++)
    {
        printf("%d  %+f %+f\n", (i + 1), g_buf[i].real, g_buf[i].imag);
    }

    return 0;
}

