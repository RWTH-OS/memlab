#include <stdlib.h>
#include <stdio.h>

#include "stride.h"
#include "main.h"
#include "rdtsc.h"
#include "info.h"




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

int stride_bench()
{
    char *memory, a;
    unsigned int range, stride, j, i, nbr_runs;
    unsigned long t1, t2;
    
    memory = (char *)malloc(options.max_range);
    for (i = 0; i<options.max_range; i++) memory[i] = (char)(i%256);

    OM_COMMENT; printf("latency (range/stride)\n");
    if (options.output_mode == OM_GNUPLOT) {
        printf("set logscale xy 2\n");
        printf("set ticslevel 0\n");
        printf("set terminal X11 persist\n");
        printf("set xlabel 'stride'\n");
        printf("set ylabel 'range'\n");
        printf("set zlabel 'nsec'\n");
        printf("set key off\n");
        printf("set xtics ('1' 1, '32' 32, '1k' 1024, '32k' 32*1024, '1M' 1024*1024, '32M' 32*1024*1024, '1G' 1024*1024*1024)\n");
        printf("set ytics ('1' 1, '32' 32, '1k' 1024, '32k' 32*1024, '1M' 1024*1024, '32M' 32*1024*1024, '1G' 1024*1024*1024)\n");
        printf("splot '-' using 2:1:3 with line\n");
    }
    OM_COMMENT; printf(" %12s %12s %10s %10s\n", "range", "stride", "ticks", "nsec");

    for (range = 1; range <= options.max_range; range += inkrement(range)) {
        for (stride = 1; stride < range; stride += inkrement(stride)) {
            nbr_runs = 100000L * stride / range;
            if (nbr_runs < 10) nbr_runs = 10;
            RDTSC(t1);
            for (j = 0; j < nbr_runs; j++) {
                for (i = 0; i < range; i += stride) {
                    a = memory[i];
                }
            }
            RDTSC(t2);
            t2 -= t1;
            t2 *= stride;
            t2 /= range;
            t2 /= nbr_runs;
            printf(" %12d %12d %10ld %10ld\n", range, stride, t2, t2*1000/information.tsc_per_usec);
        }
        if (options.output_mode == OM_GNUPLOT) {
            for (stride = range; stride < options.max_range; stride += inkrement(stride))
                printf(" %12d %12d %10ld %10ld\n", range, stride, 0L, 0L);
            printf("\n");
        }
    }

    if (options.output_mode == OM_GNUPLOT) {
        printf("e\n");
        //printf("pause -1 \n");
    }
    
    free(memory);
    return 0;
}