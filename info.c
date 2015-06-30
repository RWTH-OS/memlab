#define INFO_C

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <sys/sysinfo.h>

#include "info.h"
#include "main.h"
#include "tools.h"

#define STR_SIZE 20



int info_get()
{
    unsigned int a, b, c, d, max;
    tsc_t tsc1, tsc2;
    struct timespec tp1, tp2;
    long msec;

    asm volatile ( "mov $0x80000000, %%eax\ncpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) );
    max = a;
    if (max >= 0x80000007) {
        asm volatile ( "mov $0x80000004, %%eax\ncpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) );
        information.invariant_tsc = (d >> 8) && 1;
    }
    
    if (options.verbose) { OM_COMMENT; fprintf(options.output_file_stream, "calibrating TSC... "); }
    clock_gettime(CLOCK_REALTIME, &tp1);
    rdtsc(&tsc1);
    do {
        clock_gettime(CLOCK_REALTIME, &tp2);
        rdtsc(&tsc2);
        msec = (tp2.tv_sec-tp1.tv_sec)*1000 + (tp2.tv_nsec-tp1.tv_nsec)/1000000;
    } while (msec < 200);
    information.tsc_per_usec = (tsc2.u64-tsc1.u64)/msec/1000;

    if (options.verbose) { OM_COMMENT; fprintf(options.output_file_stream, "determining TSC read overhead "); }
    rdtsc(&tsc1);
    asm volatile ("nop");
    rdtsc(&tsc2);
    information.tsc_diff = tsc2.u64-tsc1.u64;


    
    information.nbr_cpus = get_nprocs();        /* sys/sysinfo.h */
    
    if (options.verbose) fprintf(options.output_file_stream, "done.\n");
    return 0;
}

int info_print()
{
    char str[STR_SIZE];
    unsigned int a, b, c, d, max;
    char brand_string[4*4*3 + 1];

    /* Hostname */
    gethostname(str, STR_SIZE);
    OM_COMMENT; fprintf(options.output_file_stream, "local hostname:   %s\n", str);

    /* CPU info */
    asm volatile ( "mov $0x80000000, %%eax\ncpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) );
    max = a;
    if (max >= 0x80000002) {
        asm volatile ( "mov $0x80000002, %%eax\ncpuid" : "=a"(((unsigned int *)&brand_string)[0]), "=b"(((unsigned int *)&brand_string)[1]), "=c"(((unsigned int *)&brand_string)[2]), "=d"(((unsigned int *)&brand_string)[3]) );
    }
    if (max >= 0x80000003) {
        asm volatile ( "mov $0x80000003, %%eax\ncpuid" : "=a"(((unsigned int *)&brand_string)[4]), "=b"(((unsigned int *)&brand_string)[5]), "=c"(((unsigned int *)&brand_string)[6]), "=d"(((unsigned int *)&brand_string)[7]) );
        brand_string[4*4*2] = '\0';
    }
    if (max >= 0x80000004) {
        asm volatile ( "mov $0x80000004, %%eax\ncpuid" : "=a"(((unsigned int *)&brand_string)[8]), "=b"(((unsigned int *)&brand_string)[9]), "=c"(((unsigned int *)&brand_string)[10]), "=d"(((unsigned int *)&brand_string)[11]) );
        brand_string[4*4*3] = '\0';
    }
    OM_COMMENT; fprintf(options.output_file_stream, "CPU brand string: %s\n", brand_string);

    /* total memory */
    // ...

    OM_COMMENT; fprintf(options.output_file_stream, "Invariant TSC:     %d\n", information.invariant_tsc);
    OM_COMMENT; fprintf(options.output_file_stream, "TSC per usec:      %ld\n", information.tsc_per_usec);
    OM_COMMENT; fprintf(options.output_file_stream, "TSC read overhead: %ld\n", information.tsc_diff);

    if (information.nbr_cpus > -1) {
        OM_COMMENT; fprintf(options.output_file_stream, "Nbr. of CPUs:     %d\n", information.nbr_cpus);
        OM_COMMENT; fprintf(options.output_file_stream, "Nbr. CPUs conf.:  %d\n", get_nprocs_conf());        /* sys/sysinfo.h */

        //OM_COMMENT; fprintf(options.output_file_stream, "Nbr. phys. pages: %ld\n", get_phys_pages());
        //OM_COMMENT; fprintf(options.output_file_stream, "Nbr. avail. p. p.:%ld\n", get_avphys_pages());
        
        OM_COMMENT; fprintf(options.output_file_stream, "Mem in System:    %ld MiB\n", get_phys_pages() * 4 / 1024);     /* sys/sysinfo.h */
        OM_COMMENT; fprintf(options.output_file_stream, "Mem available:    %ld MiB\n", get_avphys_pages() * 4 / 1024);   /* sys/sysinfo.h */
        
    }
    
    /* Optimizing */
#if defined (CC) && defined(OPT)
    OM_COMMENT; fprintf(options.output_file_stream, "Compiler:         %s -O%s\n", CC, OPT);
#endif
    
    return 0;
}
