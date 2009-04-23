#include <stdlib.h>
#include <stdio.h>

#include "stride.h"
#include "main.h"
#include "rdtsc.h"

extern int output_mode;     /* aus main.c */
extern unsigned long max_range;
extern int verbose;

extern unsigned long info_tsc_per_usec; /* aus info.c */



int stride_bench()
{
    char *memory, a;
    unsigned int range, stride, j, i, nbr_runs;
    unsigned long t1, t2;
    
    memory = (char *)malloc(max_range);
    for (i = 0; i<max_range; i++) memory[i] = (char)(i%256);

    OM_COMMENT; printf(" %12s %12s %10s %10s\n", "range", "stride", "ticks", "nsec");
    if (output_mode == OM_GNUPLOT) {
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

    for (range = 1; range <= max_range; range *= 2) {
        for (stride = 1; stride < range; stride *= 2) {
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
            printf(" %12d %12d %10ld %10ld\n", range, stride, t2, t2*1000/info_tsc_per_usec);
        }
        if (output_mode == OM_GNUPLOT) {
            for (stride = range; stride < max_range; stride *= 2)
                printf(" %12d %12d %10ld %10ld\n", range, stride, 0L, 0L);
            printf("\n");
        }
    }

    if (output_mode == OM_GNUPLOT) {
        printf("e\n");
        //printf("pause -1 \n");
    }
    
    free(memory);
    return 0;
}