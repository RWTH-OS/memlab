#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "bw.h"
#include "main.h"
#include "info.h"
#include "tools.h"

//#define TEST_LONGLONG

#define MEMACCESS(type, var)    cnt = options.max_range/sizeof(type);           \
                                t_min = 1e32;                                   \
                                t_max = 0;                                      \
                                t_avg = 0;                                      \
                                for (j = 0; j < nbr_runs; j++) {                \
                                    rdtsc(&t1);                                 \
                                    for (i = 0; i < cnt; i++) {                 \
                                        var = ((type*)memory)[i];               \
                                        /*asm volatile("nop");*/                \
                                    }                                           \
                                    rdtsc(&t2);                                 \
                                    t = (double)t2.u64-t1.u64;                  \
                                    t /= information.tsc_per_usec;              \
                                    t = options.max_range/t;                    \
                                    t *= (1000.0*1000.0/1024.0/1024.0/1024.0);  \
                                    if (t>t_max) {j_max = j; t_max = t; }       \
                                    if (t<t_min) {j_min = j; t_min = t; }       \
                                    t_avg += t;                                 \
                                }                                               \
                                t_avg /= nbr_runs;

volatile char *memory;


int bw_bench()
{
    volatile char a_char;
    volatile short a_short;
    volatile int a_int;
    volatile long a_long;
    #ifdef TEST_LONGLONG
    volatile long long a_longlong;
    #endif
    tsc_t t1, t2;
    double t, t_min, t_max, t_avg;
    int i, j, nbr_runs, j_min=-1, j_max=-1;
    unsigned long cnt;
    
    OM_COMMENT; printf("==== bandwidth =====================================\n");

    memory = (char *)malloc(options.max_range);
    for (i = 0; i<options.max_range; i++) memory[i] = (char)(i%256);

    nbr_runs = 10;
    OM_COMMENT; printf("number of runs: %d\n", nbr_runs);
    
    if (options.output_mode == OM_GNUPLOT) {
        //printf("set logscale xy 2\n");
        //printf("set ticslevel 0\n");
        printf("set terminal X11 persist\n");
        printf("set xlabel 'stride'\n");
        printf("set ylabel 'range'\n");
        printf("set zlabel 'nsec'\n");
        printf("set key off\n");
        //printf("set xtics ('1' 1, '32' 32, '1k' 1024, '32k' 32*1024, '1M' 1024*1024, '32M' 32*1024*1024, '1G' 1024*1024*1024)\n");
        //printf("set ytics ('1' 1, '32' 32, '1k' 1024, '32k' 32*1024, '1M' 1024*1024, '32M' 32*1024*1024, '1G' 1024*1024*1024)\n");
        printf("plot '-' with linespoints\n");
    }
    OM_COMMENT; printf(" %12s  %12s     %12s %12s     \n", "size", "min GiB/s", "avg GiB/s", "max GiB/s");


    MEMACCESS(char, a_char);
    printf(" %12zd %12.3f (%d) %12.3f %12.3f  (%d)\n", sizeof(char), t_min, j_min, t_avg, t_max, j_max);
    
    MEMACCESS(short, a_short);
    printf(" %12zd %12.3f (%d) %12.3f %12.3f  (%d)\n", sizeof(short), t_min, j_min, t_avg, t_max, j_max);
    
    MEMACCESS(int, a_int);
    printf(" %12zd %12.3f (%d) %12.3f %12.3f  (%d)\n", sizeof(int), t_min, j_min, t_avg, t_max, j_max);
    
    MEMACCESS(long, a_long);
    printf(" %12zd %12.3f (%d) %12.3f %12.3f  (%d)\n", sizeof(long), t_min, j_min, t_avg, t_max, j_max);
    
    #ifdef TEST_LONGLONG
    MEMACCESS(long long, a_longlong);
    printf(" %12zd %12.3f (%d) %12.3f %12.3f  (%d)\n", sizeof(long long), t_min, j_min, t_avg, t_max, j_max);
    #endif

    

    

    free((char*)memory);
    return 0;
}

pthread_barrier_t barrier;

typedef struct {
    int id;
    double t_min;
    double t_avg;
    double t_max;
} args_t;

void thread_func(void *arg)
{
    int id = ((args_t *)arg)->id;
    volatile long a_long;
    tsc_t t1, t2;
    double t, t_min = -1, t_max = -1, t_avg = -1;
    int thr, i, j, nbr_runs, j_min=-1, j_max=-1;
    unsigned long cnt;
    volatile char *memory;
    double *p_t_min = &(((args_t *)arg)->t_min);
    double *p_t_avg = &(((args_t *)arg)->t_avg);
    double *p_t_max = &(((args_t *)arg)->t_max);

    nbr_runs = 10;

    memory = (char *)malloc(options.max_range);
    if (memory == NULL) {
        printf("ERROR: not enough memory (%d MiB times %d threads = %ld GiB needed)\n", options.max_range/1024/1024, options.threads, options.max_range/1024/1024*options.threads/1024);
        exit(1);
    }
    for (i = 0; i<options.max_range; i++) memory[i] = (char)(i%256);
    
    if (options.verbose) { OM_COMMENT; printf("Thread %d ok\n", id); }

    for (thr = 1; thr<= options.threads; thr += inkrement(thr)) {
        pthread_barrier_wait(&barrier);
        if (id < thr) {
            cnt = options.max_range/sizeof(long);
            t_min = 1e32;
            t_max = 0;
            t_avg = 0;
            for (j = 0; j < nbr_runs; j++) {
                rdtsc(&t1);
                for (i = 0; i < cnt; i++) {
                    a_long = ((long*)memory)[i];
                }
                rdtsc(&t2);
                t = (double)t2.u64-t1.u64;
                t /= information.tsc_per_usec;
                t = options.max_range/t;
                t *= (1000.0*1000.0/1024.0/1024.0/1024.0);
                if (t>t_max) {j_max = j; t_max = t; }
                if (t<t_min) {j_min = j; t_min = t; }
                t_avg += t;
            }
            t_avg /= nbr_runs;
            *p_t_min = t_min;
            *p_t_avg = t_avg;
            *p_t_max = t_max;
        }
        for (j=0; j<options.threads; j++) {
            pthread_barrier_wait(&barrier);
            if (j==id && j<thr) {
                printf(" %2d %2d : ", thr, id);
                printf(" %12.3f (%d) %12.3f %12.3f  (%d)\n", t_min, j_min, t_avg, t_max, j_max);
            }
            if (id==0 && j>0 && j<thr) {
                *p_t_min += (((args_t *)arg)[j]).t_min;
                *p_t_avg += (((args_t *)arg)[j]).t_avg;
                *p_t_max += (((args_t *)arg)[j]).t_max;
            }
        }
        pthread_barrier_wait(&barrier);
        if (id==0) {
            printf(" %2d -- : ", thr);
            printf(" %12.3f     %12.3f %12.3f\n", *p_t_min, *p_t_avg, *p_t_max);
            printf("\n");
        }
    }
    free((char*)memory);
}

#define MAX_THREADS 16
int bw_pthread()
{
    pthread_t t[MAX_THREADS];
    int i;
    volatile args_t *args;
    
    OM_COMMENT; printf("==== bandwidth (threaded) ===========================\n");

    if (options.threads > MAX_THREADS) options.threads = MAX_THREADS;

    //memory = (char *)malloc(options.max_range);
    //for (i = 0; i<options.max_range; i++) memory[i] = (char)(i%256);

    pthread_barrier_init(&barrier, NULL, options.threads);
    args = (args_t *)calloc(options.threads, sizeof(args_t));
    for (i = 0; i<options.threads; i++) {
        args[i].id = i;
        pthread_create(&t[i], NULL, (void*)thread_func, (void*)&args[i]);
    }
    
    for (i = 0; i<options.threads; i++) {
        pthread_join(t[i], NULL);
    }
    free((void *)args);
    pthread_barrier_destroy(&barrier);

    //free((char*)memory);
    return 0;
}
