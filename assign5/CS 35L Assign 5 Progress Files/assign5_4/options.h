#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

//command-line options processing
bool process(int argc, char **argv, long long *nbytes, long long *buffSize, bool mrand48_r, char *fileName); 

#endif //OPTIONS_H
