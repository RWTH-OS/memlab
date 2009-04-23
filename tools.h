#ifndef TOOLS_H
#define TOOLS_H

/**
 * read time stamp counter (MAKRO)
 * (var) must be of type 'unsigned long' !
*/
#define RDTSC(var) asm volatile("rdtsc" : "=a"(var), "=d"( *((char*)&var+4) ))


unsigned long inkrement(unsigned long currval);

#endif // TOOLS_H