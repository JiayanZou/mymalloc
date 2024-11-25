#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

/**This is the main function**/
int main(int argc, char * * argv)
{
    //Allocating a single array, where the size exceeds the available memory.
    int * megaArr = malloc(4100); //This should not be malloced.
    
    if (megaArr == NULL)
    {
        printf("The malloc correctly returned NULL. This part passed.\n");
    }
    
    else
    {
        printf("The malloc failed to return NULL. This part failed.\n");
    }
    
    //Right now, allocate four arrays of 1016 bytes, and free one of them.
    int * arr_1 = malloc(1016);
    int * arr_2 = malloc(1016);
    int * arr_3 = malloc(1016);
    int * arr_4 = malloc(1016);
    
    free(arr_1);
    
    int * unfittableBlock = malloc(1020); //This should fail.
    
    //Decide whether the block malloced successfully or unsuccessfully. It should not.
    if (unfittableBlock == NULL)
    {
        printf("This block failed to malloc. This part is correct.\n");
    }
    
    else
    {
        printf("This block unreasonably malloced. This part is incorrect.\n");
    }
    
    //Last but not least, clear up all unfreed ones.
    free(arr_2);
    free(arr_3);
    free(arr_4);
    
    return EXIT_SUCCESS;
}
