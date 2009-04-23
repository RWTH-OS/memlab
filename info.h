#ifndef INFO_H
#define INFO_H

int info_get();
int info_print();


typedef struct info_t {
    int invariant_tsc;
    unsigned long tsc_per_usec;
} info_t;


#ifdef INFO_C
info_t information = {-1, 0};
#else
extern info_t information;     /* aus info.c */
#endif // INFO_C


#endif // INFO_H