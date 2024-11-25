#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMSIZE 4096

/**This function has the heap array, in char, as well as "unused" to ensure 8-byte alignment.**/
static union
{
    char bytes[MEMSIZE]; //This is the heap array, with 4096 bytes.
    double unused; //This ensures that the heap arrays start at a location of 8 multiple.
} heap;

/**This structure models the metadata.**/
struct metadata
{
    int length; //This is the size of the object being allocated.
    int used; //This refers to whether the place is used: 0 = Free ; 1 = Used
}; //The header is always 8 bytes.

static int initialized = 0; //This checks whether the heap memory has been initialized.

struct metadata * head = (struct metadata *) heap.bytes; //Linked list of bytes.

/**This function checks the leaks**/
static void leak_checker()
{
    int leaked_bytes = 0; //This refers to the number of bytes leaked when the program terminates.
    int leaked_objects = 0; //This refers to the number of unfreed objects.
    
    //Use iteration to check for leaked objects and bytes.
    struct metadata * cleaner = head;
    
    while ((char *) cleaner + sizeof(struct metadata) < (char *) head + MEMSIZE)
    {
        if (cleaner -> used == 1)
        {
            leaked_bytes += cleaner -> length; //Count the number of bytes leaked.
            leaked_objects++;
        }
        
        cleaner = (struct metadata *)((char *) cleaner + sizeof(struct metadata) + cleaner -> length);
    }
    
    //Print the leak error.
    if ((leaked_bytes > 0) && (leaked_objects > 0))
    {
        fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", leaked_bytes, leaked_objects);
    }
}

/**This function initializes the heap.**/
static void initialize_heap(void)
{
    head -> length = MEMSIZE - sizeof(struct metadata);
    head -> used = 0;
    
    initialized++;
    
    atexit(leak_checker);
}

/**An implementation of the logistics of the malloc() function**/
void * mymalloc(size_t size, char * file, int line)
{
    //If the input has zero bytes, then do nothing and return null.
    if (size == 0)
    {
        return heap.bytes + MEMSIZE + sizeof(struct metadata);
    }
    
    //If the heap block is not initialized, then initialize the block.
    if (initialized == 0)
    {
        initialize_heap();
    }
    
    //If the size is not a multiple of 8, then make it a multiple of 8, for 8-byte alignment.
    if (size % 8 != 0)
    {
        size = (size + 7) & ~7;
    }
    
    struct metadata * ptr = head; //This is the pointer to the head
    
    //Use iteration to find a vacant place.
    while ((char *) ptr < heap.bytes + MEMSIZE) //Have a pointer, pointing to the first header
    {
        //When we find a size of block equal to the size of malloced block.
        if ((ptr -> used == 0) && (ptr -> length == (int) size))
        {
            ptr -> used = 1; //Mark that as used.
            
            //Return the pointer to the desired data.
            return (void *)((char *) ptr + sizeof(struct metadata));
        }
        
        //When we find a size of block larger than the size of malloced block.
        if ((ptr -> used == 0) && (ptr -> length > sizeof(struct metadata) + (int) size))
        {
            if (ptr -> length - (int) size - sizeof(struct metadata) >= sizeof(struct metadata))
            {
                //This represents a freed block, unused.
                struct metadata * newBlock = (struct metadata *)((char *) ptr + sizeof(struct metadata) + size);
                
                newBlock -> length = ptr -> length - sizeof(struct metadata) - (int) size;
                newBlock -> used = 0;
                
                ptr -> length = (int) size;
            }
            
            ptr -> used = 1;
            
            //Return the occuppied block.
            return (void *)((char *) ptr + sizeof(struct metadata));
        }
        
        //Increment the pointer.
        ptr = (struct metadata *)((char *) ptr + sizeof(struct metadata) + ptr -> length);
    }
    
    //No appropriate space for the pointer.
    fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
    
    return NULL; //Returns an invalid address, which is equal to NULL.
}

/**An implementation of the logistics of the free() function**/
void myfree(void * ptr, char * file, int line)
{
    //If the heap array is not initialized
    if (initialized == 0)
    {
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line); exit(2);
    }
    
    //If the pointer passed into is null, then there is nothing done, and not even an error message.
    if (ptr == NULL)
    {
        return;
    }
    
    //The pointer should not be pointing at anywhere else other than the heap.
    if (((char *) ptr - sizeof(struct metadata) < heap.bytes) || ((char *) ptr - sizeof(struct metadata) >= heap.bytes + MEMSIZE))
    {
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line); exit(2);
    }
    
    //Create a pointer to evaluate against the ptr to free.
    struct metadata * pointer = (struct metadata *) head;
    struct metadata * prev = head; //This pointer is used to find the position of backward coalescing.
    
    //Use iteration to check whether free is necessary.
    while ((char *) pointer < heap.bytes + MEMSIZE)
    {
        if ((char *) pointer == (char *) ptr - sizeof(struct metadata))
        {
            if (pointer -> used == 1) //Free, and coalesce if necessary.
            {
                pointer -> used = 0; //Mark it as free.
                
                //Coalesce adjacent blocks, forward
                struct metadata * ref = (struct metadata *)((char *) pointer + sizeof(struct metadata) + pointer -> length);
                
                while ((char *) ref < heap.bytes + MEMSIZE)
                {
                    if (ref -> used == 0)
                    {
                        pointer -> length = pointer -> length + sizeof(struct metadata) + ref -> length; //Adding the blocks for the original freed place.
                    }
                    
                    else //Terminate the process because we encountered a malloced block.
                    {
                        break;
                    }
                    
                    ref = (struct metadata *)((char *) ref + sizeof(struct metadata) + ref -> length);
                }
                
                //Backward block coalescing.
                if ((char *) prev != (char *) pointer)
                {
                    if (prev -> used == 0)
                    {
                        prev -> length = prev -> length + sizeof(struct metadata) + pointer -> length; //Coalesce into a big large block.
                    }
                }
                
                break;
            }
            
            else //Either double free, or An invalid address.
            {
                fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line); exit(2);
            }
        }
        
        else if ((char *) pointer > (char *) ptr - sizeof(struct metadata)) //The pointer does not point to the start of a memory chunk, terminate the process.
        {
            fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line); exit(2);
        }
        
        prev = pointer;
        pointer = (struct metadata *)((char *) pointer + sizeof(struct metadata) + pointer -> length);
    }
}
