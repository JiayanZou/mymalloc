Jiayan Zou
jz937
215003270

To compile the program, just type in "make", and all the files will be compiled, and ./<fileName> to run any .c files in this package.

TESTS OF THE PROGRAM

1. The first part measures the basic functions of malloc(), on the logistics on memory allocation.

The malloc() should be able to allocate enough memory for any input size less than or equal to 4088 (The total size of the heap is 4096 bytes, subtract the 8-byte header). When allocation is successful, then malloc() should return an address at the start of the chunk (not the header itself). On the other hand, what the input size is larger than 4088, or when there is simply not enough space, then malloc() should simply return NULL. I have written two programs to test this.

In the C program "BasicAllocation.c", I have created a hypothetic array with length 10. In this case, since we are allocating at the very beginning of the heap, allocation should be successful. Therefore, the output should be "Allocation successful!" If malloc() failed due to the lack of space, then malloc() should return NULL. To test if malloc() reserves unallocated spaces, I malloced another 4040-byte pointer on line 22 (Which is the size of the space left over), and if there are no error messages, then malloc is successful here, which indicates the malloc() reserves unallocated spaces.

To test whether malloc() should fail in lack of space, the program "OverAllocatingTesting.c" was written. To be specific, in line 10, since 4100 bytes are allocated, it should fail because there are not enough space for 4100 bytes. It should return a NULL for the megaArr, where it prints "The malloc correctly returned NULL. This part passed." as a proof that it returns NULL. From line 23 to line 30, I have simulated a situation where malloc() failed when four chunks of 1016 bytes are allocated, then we free one of them, and then allocate a 1020-byte memory inside. In this case, the malloc() should also return NULL, and print "This block failed to malloc. This part is correct."

_____________________________________________________________________________________________________


2. The second part measures whether writing to different objects in a malloc would change the bytes of the previous ones. It should NOT change any bytes.

A typical implementation of malloc() and free() should not corrupt any chunks allocated. To be specific, when we allocate multiple objects or chunks into the heap array, and fill each object with a distinct byte pattern in a loop, the bytes should still remain the same at the end. Otherwise, it could be considered a form of corruption, which should not happen in a typical malloc() implementation. The files "memtest.c" and "NoOverlapping.c" tests this requirement. 

The program "memtest.c" tests the invariable property of bytes of allocated objects. To be specific, it allocates several objects, and fill each object with a distinct byte pattern (The bit number is the index number, which guarantees distinction.), and then it tests whether the bytes still have the same pattern. If it does, then it prints "%d incorrect bytes", where %d should be 0 in a correct "memtest.c" implementation.

The program "NoOverlapping.c" checks whether there are unwanted overlapping when we allocate multiple chunks in the heap array. One of the manifestation of overlapping can be seen in the case of arrays: Byte overlapping often leads to a situation where two or elements in the arrays allocated in the heap are identical: It means that some parts of the dynamically-allocated arrays is not working properly. For this program, I have declared two arrays with size multiple of 8, and then I allocated elements into the array, and ensure that the numbers inside each array are distinct, so it reduces the chance that some of the elements inside the arrays are "coincidentally" similar, which makes it hard to determine whether it is due to overlapping or else in this case. For this, one should be able to generate two arrays with very distinct elements.

______________________________________________________________________________________________________


3. The third part measures whether coalescing is correctly executed when we are freeing blocks.

When we free a block, we need to mark that block as unused. If the adjacent blocks are also free, then there are mechanisms to coalesce, or merge the free blocks, so the total size of that free block should be equal to the sum of the sizes of the three blocks. This property is tested in the program "CoalescingTest.c". For this program, I have simulated a bidirectional coalescing: To be specific, after I have allocated four chunks of size 1016 (Occupying all the heap array), on line 15, I have freed the the second and fourth chunk first. Then I freed arr3 to simulate the bidirectional coalescing: The total size of the free space is 3064 bytes (The size of three freed pointers plus the two overheads). To verify, I have declared another pointer of size  3064, and the malloc is successful, as it returns no error message.

______________________________________________________________________________________________________


4. The fourth part measures whether the malloc() and free() properly detect and report errors.

The most common reasons for malloc() and free() to return error messages are: 
  
   1) Freeing at an address not malloced by malloc().
   2) Calling free() at an address that is not at the start of a memory chunk. (This would also report the malloced object as leaked at the end).
   3) Double freeing a pointer (Double free). 
   4) Attempting to allocate a chunk that is larger than any of the existing blocks.

To test this part, I have written a program called "ErrorTesting.c". The program allows the user to enter a random number between 1 and 4 (inclusive) on the terminal command line, corresponding to the four points made above for the most common reasons. 

For number 1, I simulated an unmalloced address by creating an integer, and try to free the pointer to that integer (Where it is not created from malloc()). This should return an error message saying "free: Inappropriate pointer".

For number 2, I simulated the process of freeing the pointer at an address that is not at the start of a chunk. To do this, I attempted to allocate 8 bytes into the array using the name int * arr, and free at the address arr + 1(Of course, this is the not at the start of the memory chunk!). In this case, there should be two error messages printed: The first one should tell us that we freed an inappropriate pointer, and the second one should tell us about the number of bytes leaked and the number of objects leaked (Because we did not successfully free that pointer). In here, the number of objects leak report is properly tested.

For number 3, I simulated the double free error: I first attempted to allocate a 120-byte pointer p, and then declare another pointer q = p. Now, p and q points to the same memory address. When I free p, it successfully freed the block; However, when I free q, it fails because the address that q points to have already been freed. In this case, it is a double free.

For number 4, I tested the scenario when I attempted to allocate a large block that cannot be fitted within the free spaces. 
First, I attempt to allocate a memory chunk that is larger than 4088 bytes (The maximum available free space, happens when all bytes are free). This would return a free error message, as seen in the program.
Second, I attempt to allocate four memory chunks of equal size (All 1016 bytes), and free one of them, and allocate a 1017 size into the array, in which it should not successfully malloc, and this program correctly returns a NULL pointer and an error message.

____________________________________________________________________________________________________


5. The program "memgrind.c" measures the performance of some arbitrary tasks, instead of testing whether each part functions properly. The performance is measured in microseconds.

There are a total of five tasks inside this part. The first three tasks are exactly described in the write up file. 
The fourth task involves simulating a fragmentation. In this case, I allocated 150 single-byte objects into the heap array. Then, I free every consecutive chunk. (This creates a situation of fragmentation) Next, I allocated 75 more chunks into the array, and measures how well the system handles the fragmentation. In other words, it sees whether fragmentation is dealt with effectively.
The fifth task involves simulating the generation of different-sized chunks. To be specific, for each, time, the system generates a random number of bytes that is between 1 and 56 bytes (Inclusive). The goal of the randomness is to measure the effectiveness of measurements when the number of bytes for each round is not known in advance. 