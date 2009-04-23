#ifndef RDTSC_H
#define TDTSC_H

#define RDTSC(var) asm volatile("rdtsc" : "=a"(var), "=d"( *((char*)&var+4) ))

#endif // RDTSC_H
