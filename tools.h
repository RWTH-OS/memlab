#ifndef TOOLS_H
#define TOOLS_H

#include <stdint.h>

/**
 * read time stamp counter (MAKRO)
 * (var) must be 64 bit (use 'uint64_t' from stdint.h) !
*/
#define RDTSC(var) asm volatile("rdtsc" : "=a"(*((uint32_t*)&var)), "=d"( *((uint32_t*)&var+1) ))

unsigned long inkrement(unsigned long currval);

#endif // TOOLS_H