#ifndef TOOLS_H
#define TOOLS_H

#include <stdint.h>

//#pragma GCC diagnostic ignored "-Wstrict-aliasing"

typedef union {
    uint64_t u64;
    struct {
        uint32_t low;
        uint32_t high;
    } u32;
} tsc_t;

/**
 * read time stamp counter 
 * (var) must be of type tsc_t !
*/
//#define RDTSC(var) asm volatile("rdtsc" : "=a"( var.u32.low ), "=d"( var.u32.high ))
static inline void rdtsc(tsc_t *tsc) {asm volatile("rdtsc" : "=a"( tsc->u32.low ), "=d"( tsc->u32.high ));}

unsigned long inkrement(unsigned long currval);

#endif // TOOLS_H
