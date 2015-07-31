#include "purge.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{ 
    if (argc < 2) {
      printf("Usage: %s directory\n", argv[0]);
      exit(EXIT_FAILURE);
    } 

    printf("**** PURGE ****\n");
    purge(argv[1]);
}
