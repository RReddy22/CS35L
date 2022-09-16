#include "output.h"
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> //for mrand48_r

bool writebytes_mrand48(unsigned long int x, int nbytes)
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

//option for mrand48_r 
int output3(long long nbytes)
{
  int wordsize = sizeof (long);
  int output_errno = 0;

  //parameters for mdrand48_r
  struct drand48_data *buffer = malloc(sizeof(struct drand48_data));
  memset(buffer, '\0', sizeof(struct drand48_data));

  //long int is the same thing as long
  long int result; 

    do
    {
      unsigned int x = mrand48_r(buffer, &result);
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
        if (!writebytes_mrand48(x, outbytes))
        {
	        output_errno = errno;
	        break;
	    }
      nbytes -= outbytes;
    }
    while (0 < nbytes);

  return output_errno; 
}

//for the case that mrand48_r and -o (output) are chosen
int output4(long long nbytes, long long sizeofbuf)
{
   char *buffer = (char*)malloc(sizeofbuf);
   int wordsize = sizeof (long);
    //int output_errno = 0; 

   //initializes the struct and fills the buffer with null bytes 
   struct drand48_data *cBuffer = malloc(sizeof(struct drand48_data));
   memset(cBuffer, '\0', sizeof(struct drand48_data));

   long int result;
   
    do
    {
      int offset = 0;
      while (offset < sizeofbuf && offset < nbytes) {
        mrand48_r(cBuffer, &result);
        int outbytes = nbytes < wordsize ? nbytes : wordsize;

        if (outbytes > sizeofbuf - offset)
          outbytes = sizeofbuf - offset; //you only write as many as you can fit
        memcpy((void*)(buffer + offset), &result, outbytes);
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
