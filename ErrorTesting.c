#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

int main(int argc, char * * argv)
{
    int decision = (argc > 1) ? atoi(argv[1]) : 0; //argc will be 1 to 4.
    
    switch (decision)
    {
        case 1://Freeing at an address that is not malloced by the mymalloc
        {
            int x = 5;
            free(&x); //This should print out the error message of inappropriate free.
            break;
        }
            
        case 2: //Calling free(0 with an address not at the start of a chunk.
        {
            int * arr = malloc(2 * sizeof(int));
            free(arr + 1); //This is one byte after the address the arr, which is not the correct place.
            break; //Plus, since the arr itself did not get freed, then it will definitely report leaked bytes from objects.
        }
            
        case 3: //Double free a pointer.
        {
            int * p = malloc(30 * sizeof(int));
            int * q = p;
            free(p); free(q); //The free(q) will be the one causing the double free error message.
            
            break;
        }
            
        case 4: //Failure to malloce
        {
            int * largeArr = malloc(4089); //This will cause an error in mallocing.
            free(largeArr); //Inappropriate pointer.
            
            int * small1 = malloc(1016);
            int * small2 = malloc(1016);
            int * small3 = malloc(1016);
            int * small4 = malloc(1016);
            
            free(small1);
            int * newPtr = malloc(1017); //This should give ma a malloc error.
            free(newPtr);
            
            free(small2); free(small3); free(small4);
            
            break;
        }
            
        default: //When the input entered is not a valid code.
        {
            fprintf(stderr, "Error: Invalid decision code %d", decision);
            break;

        }
    }
    
    return EXIT_SUCCESS;
}
