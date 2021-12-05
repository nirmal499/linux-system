/* In this recipe, we'll learn how to use a file descriptor to mapped memory to share that memory
between two unrelated processes. Using shared memory in this way automatically creates
an underlying file for the memory in the /dev/shm directory, where shm stands for
shared memory. */

/* we'll write a program that opens and creates a file descriptor for shared memory
and also maps the memory. Then, we'll write another program that reads the memory
area */

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define DATASIZE 128

int main(void)
{
    int fd;
    float *addr;
    const char memid[] = "/my_memory";
    const float numbers[3] = {3.14, 2.718, 1.202};
    /* create shared memory file descriptor */
    /*  Note that even if I call it a file descriptor here, it's a descriptor to a memory area. memid contains
        the name of the memory-mapped descriptor. Then, we must use shm_open() to
        open and create the "file descriptor": */
    if ((fd = shm_open(memid, O_RDWR | O_CREAT, 0600)) == -1)
    {
        /* O_RDWR for reading and writing and O_CREATE for creating the file if it doesn't exist. */
        perror("Can't open memory fd");
        return 1;
    }
    /* truncate memory to DATASIZE */
    /* The file-backed memory is 0 bytes in size initially. To extend it to our 128 bytes,
        we must truncate it with ftruncate() */
    if ((ftruncate(fd, DATASIZE)) == -1)
    {
        perror("Can't truncate memory");
        return 1;
    }
    /* map memory using our file descriptor */
    /* we must map the memory, just as we did in the previous recipe. But this
        time, we will give it the fd file descriptor instead of -1. We have also left out the
        MAP_ANONYMOUS part, thus making this memory backed by a file.  */
    addr = mmap(NULL, DATASIZE, PROT_WRITE,
                MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Memory mapping failed");
        return 1;
    }
    /* Then, we must copy our array of floats to memory using memcpy(). */
    /* copy data to memory */
    memcpy(addr, numbers, sizeof(numbers));
    /* To let the reading program have a chance to read the memory, we must pause the program and wait for an
        Enter key with getchar(). Then, it's just a matter of cleaning up by unmapping
        the memory and deleting the file descriptor and the underlying file with
        shm_unlink()  */
    /* wait for enter */
    printf("Hit enter when finished ");
    getchar();
    /* clean up */
    munmap(addr, DATASIZE);
    shm_unlink(memid);
    return 0;
}