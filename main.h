#ifndef MAIN_H
#define MAIN_H

#define KiB *1024
#define MiB KiB*1024
#define GiB MiB*1024


#define OM_SCREEN  0
#define OM_GNUPLOT 1
#define OM_SQLPLOT 2

#define OM_COMMENT if (options.output_mode==OM_GNUPLOT) {printf("#");} else if (options.output_mode==OM_SQLPLOT) {printf("--");}

typedef struct opt_t {
    int output_mode;
    int verbose;
    int detail;
    int threads;
    unsigned long max_range;
} opt_t;


#ifdef MAIN_C
opt_t options = {OM_SCREEN, 0, 0, 1, 16 MiB};
#else
extern opt_t options;     /* aus main.c */
#endif // MAIN_C

#endif // MAIN_H
