/* memlab: memory latency and benchmark
 * cf. BS II, chap. 2.1.3.2 & Fig. 2.1/2.2
 */

#define MAIN_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "info.h"
#include "stride.h"
#include "main.h"
#include "bw.h"

int print_help(char *argv[])
{
    printf("memlab - MEMory Latency And Benchmark\n");
    printf("usage: %s  [options] [test] [test] [...]\n", argv[0]);
    printf("options:\n");
    printf("     -h     help\n");
    printf("     -p     plot (format output for gnuplot)\n");
    printf("     -q     sqlplot (format output for sqlplot)\n");
    printf("     -v     verbose (include system infos)\n");
    printf("     -rN    range N Bytes (eg. 8192, 8K, 2M, 1G)\n");
    printf("     -dN    detail for iteration: 2^N steps between Powers-of-two\n");
    printf("     -tN    create N threads\n");
    printf("tests:\n");
    printf("     lat : latency range/stride\n");
    printf("     bw  : bandwidth test\n");
    printf("     bwt : threaded bandwidth test\n");
    return 0;
}

int get_param(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hvpqr:d:t:")) != -1) {
        switch (opt) {
            case 'h' :
                print_help(argv);
                exit(0);
                break;
            case 'v' :
                options.verbose = 1;
                break;
            case 'p' :
                options.output_mode = OM_GNUPLOT;
                break;
            case 'q' :
                options.output_mode = OM_SQLPLOT;
                break;
            case 'r' :
                options.max_range = atol(optarg);
                if (strchr(optarg, 'K')) options.max_range = options.max_range KiB;
                if (strchr(optarg, 'M')) options.max_range = options.max_range MiB;
                if (strchr(optarg, 'G')) options.max_range = options.max_range GiB;
                //printf("range: %s %ld\n", optarg, max_range);
                break;
            case 'd' :
                options.detail = atoi(optarg);
                break;
            case 't' :
                options.threads = atoi(optarg);
                break;
            default :
                break;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i;

    get_param(argc, argv);
    
    OM_COMMENT; printf("memlab - MEMory Latency And Benchmark\n");
    
    info_get();
    if (options.verbose) info_print();
    for (i = optind; i<argc; i++) {
        if ((strcmp(argv[i], "lat")) == 0) {
            stride_bench();
        } else if ((strcmp(argv[i], "bw")) == 0) {
            bw_bench();
        } else if ((strcmp(argv[i], "bwt")) == 0) {
            bw_pthread();
        } else {
            printf("WARNING: unknown test: '%s'\n", argv[i]);
        }
    }
    return 0;
}