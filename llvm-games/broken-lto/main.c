#include <stdio.h>
#include "mod.h"


__attribute__ ((section (".hash"))) const char hash[] = "helloimahash";




int main(int argc, char *argv[])
{
    printf("Hello main '%s'\n", hash);
    hello_mod();
    return 0;
}
