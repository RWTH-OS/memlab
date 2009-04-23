#include "tools.h"
#include "main.h"

unsigned long inkrement(unsigned long currval)
{
    unsigned long i=1;
    currval = currval >> options.detail;
    if (currval == 0) return 1;
    do {
        if ((currval & ~i) == 0) return currval;
        currval = currval & ~i;
        i = i << 1;
    } while (1);

    return currval;
}
