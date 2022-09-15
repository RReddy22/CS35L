#include "rand64-sw.h"

#include <stdio.h>
//#include <stdbool.h>
#include <stdlib.h>
//#include <limits.h>

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;
static char fileName[] = "/dev/random"; 

/* Initialize the software rand64 implementation.  */
void
software_rand64_init (void)
{
  urandstream = fopen (fileName, "r");
  if (! urandstream)
    abort ();
}

//software rand64 implementation that uses a file: "/F"
void
setFileName(char *file)
{
  free(fileName); 
  fileName = strdup(file); 
}

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
  free(fileName); 
}


