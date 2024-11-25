#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

int main(int argc, char * * argv)
{
    int * array = malloc(10 * sizeof(int)); //Create an array.
    
    //Test whether allocation is successful.
    if (array == NULL)
    {
        fprintf(stderr, "Error: Allocation is unsuccessful!\n"); //Unsuccessful.
        
        exit(EXIT_FAILURE);
    }
    
    else
    {
        printf("Allocation successful!\n");
        
        int * arr2 = malloc(4040);
        
        if (arr2 == NULL)
        {
            fprintf(stderr, "Error: malloc() failed to reserve unallocated spaces.\n");
            
            exit(EXIT_FAILURE);
        }
        
        else
        {
            printf("Success! malloc() reserved unallocated spaces!\n");
        }
        
        free(arr2);
    }
    
    free(array);
    
    return EXIT_SUCCESS;
}
