#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mymalloc.h"

int main(int argc, char * * argv)
{
    //Introduce randomness, for generating random numbers in an array.
    srand(time(NULL));
    
    //Allocate a first array.
    int * firstArr = malloc(20 * sizeof(int));
    
    for (int index = 0; index < 20; index++)
    {
        firstArr[index] = (rand() % 50) + 10; //Generate a random integer
    }
    
    //Allocate a second array.
    int * secondArr = malloc(20 * sizeof(int));
    
    for (int index2 = 0; index2 < 20; index2++)
    {
        secondArr[index2] = (rand() % 100) + 70; //Again, generate a series of randoms.
    }
    
    //Print out the index elements. There should be zero overlapping, or the two sets are completely different.
    for (int print = 0; print < 20; print++)
    {
        if (print < 10)
        {
            printf("firstArr[0%d] = %d | secondArr[0%d] = %d\n", print, firstArr[print], print, secondArr[print]);
        }
        
        else
        {
            printf("firstArr[%d] = %d | secondArr[%d] = %d\n", print, firstArr[print], print, secondArr[print]);
        }
    }
    
    //Free when finished.
    free(firstArr); free(secondArr);
    
    return EXIT_SUCCESS;
}
