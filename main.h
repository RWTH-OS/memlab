#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define KiB *1024
#define MiB KiB*1024
#define GiB MiB*1024

#define FILE_NAME_LENGTH 50

#define OM_SCREEN  0
#define OM_GNUPLOT 1
#define OM_SQLPLOT 2

#define OM_COMMENT \
    if (options.output_mode==OM_GNUPLOT) {\
	    fprintf(options.output_file_stream, "#");\
    } else if (options.output_mode==OM_SQLPLOT) {\
	    fprintf(options.output_file_stream, "--");\
    }

typedef struct opt_t {
    int output_mode;
    FILE* output_file_stream;
    char output_file[FILE_NAME_LENGTH];
    int verbose;
    int detail;
    int threads;
    unsigned long max_range;
} opt_t;


#ifdef MAIN_C
opt_t options = {
	.output_mode = OM_SCREEN,
	.output_file_stream=NULL,
	.output_file = "",
	.verbose = 0, 
	.detail = 0,
	.threads = 1,
	.max_range = 16 MiB,
};
#else
extern opt_t options;     /* aus main.c */
#endif // MAIN_C

#endif // MAIN_H
