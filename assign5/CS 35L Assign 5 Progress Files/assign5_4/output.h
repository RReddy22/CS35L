#ifndef OUTPUT_H
#define OUTPUT_H

//#include <stdbool.h>

int output(long long nbytes, unsigned long long (*rand64) (void));

int output2(long long nbytes, unsigned long long (*rand64) (void), long long sizeofbuf);

#endif //OUTPUT_H

