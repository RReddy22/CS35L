#include "options.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //getopt
#include <string.h> //for strcmp

bool isNumber(char *comparison)
{ 
      for (int i = 0; i < strlen(comparison); ++i) {
            if (!isdigit(comparison[i])) 
                  return false; 
      }
      return true; 
}

bool process(int argc, char **argv, long long *nbytes, int *buffSize)
{ 
      const char *options = "io"; 
      int c; 

      //if there are no more options, it will return -1
      while ((c = getopt(argc, argv, options)) != -1) { 
            switch (c) { 
            case 'i':
                  perror("not yet implemented"); 
                  continue; 
            case 'o': 
                  //checks if the input is "stdio"
                  if (strcmp(optarg, "stdio") == 0) 
                        continue; 

                  //else its a number
                  char *endptr;
                  errno = 0;
                  buffSize = strtoll (optarg, &endptr, 10);
                  if (errno)
                        return false;

                  //no valid number, therefore throw an error
                  //buff size should be greater than 0 
                  else if (*endptr && 0 >= buffSize)
                        return false; 
                  continue; 
            default: 
                  break; 
            }
      } 

      //else its a number

      if (optind < argc) {
            char *endptr;
            errno = 0;
            *nbytes = strtoll (argv[optind], &endptr, 10);
            if (errno)
                  return false;

            //no valid number, therefore throw an error
            else if (*endptr && 0 > *nbytes)
                  return false;

            return true; 
      }     

      //not valid input
      else 
            return false; 
} 
