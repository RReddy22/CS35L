#include "options.h"
#include "rand64-hw.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //getopt
#include <string.h> //for strcmp

bool process(int argc, char **argv, long long *nbytes, long long *buffSize, bool mrand48_r, char *fileName)
{ 
      const char *options = "i:o:"; 
      int c; 

      //if there are no more options, it will return -1
      while ((c = getopt(argc, argv, options)) != -1) { 
            switch (c) { 
            case 'i':
                  if (strcmp(optarg, "rdrand"))
                        if (!rdrand_supported ())
                              return false; 
                        else 
                              continue; 
                  if (strcmp(optarg, "mrand48_r")) 
                        perror("not yet implemented"); 
                  if (optarg[0] == '/')

            case 'o': 
	          //checks if the input is "stdio"
	      
                  if (strcmp(optarg, "stdio") == 0) 
		      continue; 
		  
                  //else its a number
                  //char *endptr = 0; 
                  errno = 0;

                  *buffSize = strtoll (optarg, NULL, 10);
		  
		  if (errno)
                        return false;

                  //no valid number, therefore throw an error
                  //buff size should be greater than 0 
                  else if (0 >= *buffSize)
                        return false; 
                  continue; 
            default: 
                  break; 
            }
      } 
      
      //else its a number

      if (optind < argc) {
	//char *endptr = 0; 
            errno = 0;
            *nbytes = strtoll (argv[optind], NULL, 10);
            if (errno)
                  return false;

            //no valid number, therefore throw an error
            else if (0 > *nbytes)
                  return false;

            return true; 
      }     

      //not valid input
      else 
            return false; 
} 
