#include "output.h"
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int output(long long nbytes, unsigned long long (*rand64) (void))
{
    int wordsize = sizeof rand64 ();
    int output_errno = 0; 

    do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
        if (!writebytes (x, outbytes))
        {
	        output_errno = errno;
	        break;
	    }
      nbytes -= outbytes;
    }
    while (0 < nbytes);

  return output_errno; 
}

//for the -o option
int output2(long long nbytes, unsigned long long (*rand64) (void), long long sizeofbuf)
{
    char *buffer = (char*)malloc(sizeofbuf); 
    int wordsize = sizeof rand64 ();
    //int output_errno = 0; 

    do
    {
      int offset = 0;
      while (offset < sizeofbuf && offset < nbytes) {
        unsigned long long x = rand64 ();
        int outbytes = nbytes < wordsize ? nbytes : wordsize;

        if (outbytes > sizeofbuf - offset)
          outbytes = sizeofbuf - offset; //you only write as many as you can fit
        memcpy((void*)(buffer + offset), &x, outbytes);
        offset += outbytes; //offset should go forward by outbytes amount 

      }
      //*nbytes -= offset;
      if ((offset = write(1, buffer, offset)) == -1)
	  return 1; 
      nbytes -= offset; 

    }
    while (0 < nbytes);

  return 0; 
}
