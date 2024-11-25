#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> //This allows the use for srand() and rand() to generate random integers in the arrays.
#include <sys/time.h> //This allows the use for gettimeofday() function.
#include "mymalloc.h"

/**This is the first component of the performance evaluation, involving allocating and immediately deallocating a same object 120 times.**/
void firstComponent(void)
{
    for (int times = 0; times < 120; times++)
    {
        char * obj = malloc(1); free(obj); //malloc and free a 1-byte object 120 times
    }
}

/**This is the second component of the performance evaluation, involving allocating 120 1 byte objects, then free them all.**/
void secondComponent(void)
{
    void * ptrArr[120];
    
    for (int index = 0; index < 120; index++)
    {
        char * object = malloc(1);
        
        ptrArr[index] = object;
    }
    
    for (int num = 0; num < 120; num++)
    {
        free(ptrArr[num]);
    }
}

/**This is the third component of the performance evaluation, involving randomnized malloc and free**/
/**This part generates an integer that is either 0 or 1. When it is 0, it frees a latest malloced object; When it is a 1, it allocates an object.**/
void thirdComponent(void)
{
    void * pointArray[120]; int last = 0; int count = 0;
    
    while (count < 120)
    {
        int r = rand() % 2;
        
        if ((r == 1) && (last >= 0) && (last < 120))
        {
            char * thing = malloc(1); pointArray[last] = thing; last++; count++;
        }
        
        else if ((r == 0) && (last > 0) && (last <= 120))
        {
            last--; free(pointArray[last]);
        }
    }
    
    for (int clear = 0; clear < last; clear++)
    {
        free(pointArray[clear]);
    }
}

/**This is the fourth component of the performance evaluation, involving allocating all objects, then freeing every alternative object.**/
/**This function tests fragmentation. It sees whether the malloc is able to deal with small objects, in small freed spaces. It tests whether the malloc effectively use wasted spaces.**/
/**This function would first allocate 150 1-byte objects, then free every first object allocated. It then allocates another 75 1- byte objects, and sees if it use spaces effectively**/
void fourthComponent(void)
{
    void * storage[150]; //Create an array of 200 pointers available.
    
    //First, we store 150 pointers for every index in the array.
    for (int index = 0; index < 150; index++)
    {
        char * pointer = malloc(1); storage[index] = pointer;
    }
    
    //We free every even-numbered index in the array, to create a fragmentation-like environment.
    for (int index = 0; index < 150; index += 2)
    {
        free(storage[index]);
    }
    
    //We allocate another another 75 ones, and measures if the malloc() effectively and efficiently uses the freed spaces.
    //If it does, then there should be no error messages.
    for (int again = 0; again < 150; again += 2)
    {
        char * ptr = malloc(1); storage[again] = ptr;
    }
    
    //We free them all at the last.
    for (int vacate = 0; vacate < 150; vacate++)
    {
        free(storage[vacate]);
    }
}

/**This is the fifth component of the performance evaluation, where the system chooses a random integer, which refers to the number of bytes to allocate.**/
/**In essence, it involves letting the system to decide how many bytes to allocate (anywhere between 1 to 56), for a recurrence of 64 times, and free them all.**/
/**The goal of this measurement is to see whether the memory can be allocated effectiveness when there are a random, unknown number of bytes allocated.**/
void fifthComponent(void)
{
    //Seed the random number generator.
    srand(time(NULL));
    
    void * point[56];
    
    //Use iteration to count the number of times a random allocation occurs.
    for (int time = 0; time < 56; time++)
    {
        point[time] = malloc((rand() % 56) + 1); //A random number of bytes between 1 and 56.
    }
    
    //Free the arrays
    for (int index = 0; index < 56; index++)
    {
        free(point[index]);
    }
}

/**This is the main driver**/
int main(int argc, char * * argv)
{
    //This tests the first component.
    struct timeval start1, end1;
    
    printf("This is the first component test:\n");
    
    gettimeofday(&start1, NULL);
    
    for (int round = 0; round < 50; round++)
    {
        firstComponent();
    }
    
    gettimeofday(&end1, NULL);
    
    double elapse = (end1.tv_usec - start1.tv_usec) / 50; printf("Average: %f microseconds.\n\n", elapse);
    
    
    
    
    //This tests the second component.
    struct timeval start2, end2;
    
    printf("This is the second component test:\n");
    
    gettimeofday(&start2, NULL);
    
    for (int round = 0; round < 50; round++)
    {
        secondComponent();
    }
    
    gettimeofday(&end2, NULL);
    
    double elapse2 = (end2.tv_usec - start2.tv_usec) / 50; printf("Average: %f microseconds.\n\n", elapse2);
    
    
    
    
    //This tests the third component.
    struct timeval start3, end3;
    
    printf("This is the third component test:\n");
    
    gettimeofday(&start3, NULL);
    
    for (int round = 0; round < 50; round++)
    {
        thirdComponent();
    }
    
    gettimeofday(&end3, NULL);
    
    double elapse3 = (end3.tv_usec - start3.tv_usec) / 50; printf("Average: %f microseconds.\n\n", elapse3);
    
    
    
    
    //This tests the fourth component.
    struct timeval start4, end4;
    
    printf("This is the fourth component test:\n");
    
    gettimeofday(&start4, NULL);
    
    for (int round = 0; round < 50; round++)
    {
        firstComponent();
    }
    
    gettimeofday(&end4, NULL);
    
    double elapse4 = (end4.tv_usec - start4.tv_usec) / 50; printf("Average: %f microseconds.\n\n", elapse4);
    
    
    
    
    //This tests the fifth component.
    struct timeval start5, end5;
    
    printf("This is the fifth component test:\n");
    
    gettimeofday(&start5, NULL);
    
    for (int round = 0; round < 50; round++)
    {
        fifthComponent();
    }
    
    gettimeofday(&end5, NULL);
    
    double elapse5 = (end5.tv_usec - start5.tv_usec) / 50; printf("Average: %f seconds.\n\n", elapse5);
    
    return EXIT_SUCCESS;
}
