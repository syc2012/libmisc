#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmisc.h"

void baz(void)
{
    stack_dump("Dump stack:");
}

void bar(void)
{
    baz();
}

void foo(void)
{
    bar();
}

int main(int argc, char *argv[])
{
    foo();
    return 0;
}

