/* memlab: memory latency and benchmark
 * cf. BS II, chap. 2.1.3.2 & Fig. 2.1/2.2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "info.h"
#include "stride.h"
#include "main.h"

#define KiB *1024
#define MiB KiB*1024
#define GiB MiB*1024


int output_mode = OM_SCREEN;
unsigned long max_range = 16 MiB;
int verbose = 0;

int print_help(char *argv[])
{
    printf("memlab - MEMory Latency And Benchmark\n");
    printf("usage: %s  [options] [test] [test] [...]\n", argv[0]);
    printf("options:\n");
    printf("     -h     help\n");
    printf("     -p     plot (format output for gnuplot)\n");
    printf("     -v     verbose (include system infos)\n");
    printf("     -rN    range N Bytes (eg. 8192, 8K, 2M, 1G)\n");
    printf("tests:\n");
    printf("     lat : latency range/stride\n");
    return 0;
}

int get_param(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hvpr:")) != -1) {
        switch (opt) {
            case 'h' :
                print_help(argv);
                exit(0);
                break;
            case 'v' :
                verbose = 1;
                break;
            case 'p' :
                output_mode = OM_GNUPLOT;
                break;
            case 'r' :
                max_range = atol(optarg);
                if (strchr(optarg, 'K')) max_range = max_range KiB;
                if (strchr(optarg, 'M')) max_range = max_range MiB;
                if (strchr(optarg, 'G')) max_range = max_range GiB;
                //printf("range: %s %ld\n", optarg, max_range);
                break;
            default :
                break;
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{

    get_param(argc, argv);
    
    OM_COMMENT; printf("memlat - Memory Latency Benchmark\n");
    
    info_tsc();
    if (verbose) info_print();
    stride_bench();
    return 0;
}