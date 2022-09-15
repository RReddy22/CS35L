#include "output.h"
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

//does the output without the -o option (just the Number)
bool writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0)
        return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}

int output(long long *nbytes, unsigned long long (*rand64) (void))
{
    int wordsize = sizeof rand64 ();
    int output_errno = 0; 

    do
    {
      unsigned long long x = rand64 ();
      int outbytes = *nbytes < wordsize ? *nbytes : wordsize;
        if (!writebytes (x, outbytes))
        {
	        output_errno = errno;
	        break;
	    }
      *nbytes -= outbytes;
    }
    while (0 < *nbytes);

  return output_errno; 
}

//for the -o option
int output(long long *nbytes, unsigned long long (*rand64) (void), int sizeofbuf)
{
    char *buffer = (char*)malloc(sizeofbuf); 
    int wordsize = sizeof rand64 ();
    //int output_errno = 0; 

    do
    {
      int offset = 0
      while (offset < sizeofbuf) {
        unsigned long long x = rand64 ();
        int outbytes = *nbytes < wordsize ? *nbytes : wordsize;

        if (outbytes > sizeofbuf - offset)
          outbytes = sizeofbuf - offset; //you only write as many as you can fit
        memcpy((void*)(buffer + offset), &x, outbytes)
        offset += outbytes; //offset should go forward by outbytes amount 

      }
      //*nbytes -= offset;
      *nbytes -= write(stdout, buffer, offset); 

    }
    while (0 < *nbytes);

  return output_errno; 
}
