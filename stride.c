#include <stdlib.h>
#include <stdio.h>

#include "stride.h"
#include "main.h"
#include "info.h"
#include "tools.h"

static
int print_caption_row()
{
    OM_COMMENT; fprintf(options.output_file_stream, " %12s %12s %10s %10s\n", "range", "stride", "ticks", "nsec");

    return 0;
}

static
int print_header()
{
    if (options.output_mode == OM_GNUPLOT) {
        fprintf(options.output_file_stream, "set logscale xy 2\n");
        fprintf(options.output_file_stream, "set ticslevel 0\n");
        fprintf(options.output_file_stream, "set terminal X11 persist\n");
        fprintf(options.output_file_stream, "set xlabel 'stride'\n");
        fprintf(options.output_file_stream, "set ylabel 'range'\n");
        fprintf(options.output_file_stream, "set zlabel 'ticks'\n");
        fprintf(options.output_file_stream, "set key off\n");
        fprintf(options.output_file_stream, "set xtics ('1' 1, '32' 32, '1k' 1024, '32k' 32*1024, '1M' 1024*1024, '32M' 32*1024*1024, '1G' 1024*1024*1024)\n");
        fprintf(options.output_file_stream, "set ytics ('1' 1, '32' 32, '1k' 1024, '32k' 32*1024, '1M' 1024*1024, '32M' 32*1024*1024, '1G' 1024*1024*1024)\n");
        fprintf(options.output_file_stream, "splot '-' using 2:1:3 with line\n");
    } else if (options.output_mode == OM_SQLPLOT) {
        fprintf(options.output_file_stream, "create table result (range numeric, stride numeric, ticks numeric, nsec numeric);\n");
        fprintf(options.output_file_stream, "\n");
        fprintf(options.output_file_stream, "\n");
        fprintf(options.output_file_stream, "\n");
    }
    print_caption_row();

    return 0;
}



int stride_bench()
{
    volatile char *memory, a;
    unsigned int range, stride, j, i, nbr_runs;
    tsc_t t1, t2;
    uint64_t t;
    char cur_filename[FILE_NAME_LENGTH] = {[0 ... FILE_NAME_LENGTH-1] = 0};
    
    memory = (char *)malloc(options.max_range);
    for (i = 0; i<options.max_range; i++) memory[i] = (char)(i%256);

    OM_COMMENT; fprintf(options.output_file_stream, "==== latency (range/stride) =====================\n");
    
    print_header();

    for (range = 1; range <= options.max_range; range += inkrement(range)) {
	if ((options.output_file_stream != stdout) && (options.individual_files)) {
	    fclose(options.output_file_stream);
	    sprintf(cur_filename, "%s_%u.dat", options.output_file, range);
	    options.output_file_stream = fopen(cur_filename, "w");
    	    print_caption_row();
	}
        for (stride = 1; stride < range; stride += inkrement(stride)) {
            nbr_runs = 100000L * stride / range;
            if (nbr_runs < 10) nbr_runs = 10;
            rdtsc(&t1);
            for (j = 0; j < nbr_runs; j++) {
                for (i = 0; i < range; i += stride) {
                    a = memory[i];
                }
            }
            rdtsc(&t2);
            t = t2.u64 - t1.u64;
            t -= information.tsc_diff;
            t *= stride;
            t /= range;
            t /= nbr_runs;
            if (options.output_mode == OM_SQLPLOT) {
                fprintf(options.output_file_stream, "insert into result (range, stride, ticks, nsec) values (%12d, %12d, %10lu, %10lu);\n", range, stride, (unsigned long)t, (unsigned long)t*1000/information.tsc_per_usec);
            } else {
                fprintf(options.output_file_stream, " %12d %12d %10lu %10lu\n", range, stride, (unsigned long)t, (unsigned long)t*1000/information.tsc_per_usec);
            }
        }
        if (options.output_mode != OM_SCREEN) {
            for ( ; stride < options.max_range; stride += inkrement(stride)) {
                if (options.output_mode == OM_SQLPLOT) {
                    fprintf(options.output_file_stream, "insert into result (range, stride, ticks, nsec) values (%12d, %12d, null, null);\n", range, stride);
                } else {
                    fprintf(options.output_file_stream, " %12d %12d %10ld %10ld\n", range, stride, 0L, 0L);
                }
            }
            fprintf(options.output_file_stream, "\n");
        }
    }

    if (options.output_mode == OM_GNUPLOT) {
        fprintf(options.output_file_stream, "e\n");
        //printf("pause -1 \n");
    }
    
    free((char*)memory);
    return 0;
}
