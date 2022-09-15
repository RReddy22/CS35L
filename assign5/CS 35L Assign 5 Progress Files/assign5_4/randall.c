/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
#include "rand64-sw.h"
#include "rand64-hw.h"
#include "options.h"
#include "output.h"
#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  
  //output variables
  long long nbytes = -1; 
  long long sizeOfbuffer = 0;

  //input variables
  bool mrand48_r = false; 
  char fileName[] = "/dev/random"; 
  //malloc(sizeof(char)*strlen("/dev/random")); 
  //strcpy(fileName, "/dev/random"); 
  
  bool valid = process(argc, argv, &nbytes, &sizeOfbuffer, &mrand48_r, fileName);

  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }
  
  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

 //if we have a file that is not "/dev/random"
  if (strcmp(fileName, "/dev/random") != 0) { 
    setFileName(fileName);
    initialize = software_rand64_init;
    rand64 = software_rand64;
    finalize = software_rand64_fini; 
  }

//hardware case where rdrand is supported
  else if (rdrand_supported ()) {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
  }

// /dev/random case 
  else
    {
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
    }
  
  initialize ();

  int output_errno;
  
  if (sizeOfbuffer != 0) //we are using the -o option, so we need the 3rd param (buffer size)
    output_errno = output2(nbytes, rand64, sizeOfbuffer);
  else //size of buffer is 0
    output_errno = output(nbytes, rand64); 
  
  
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      fprintf(stderr, "error");
      return 1;
    }

  finalize ();
  return !!output_errno;
}
