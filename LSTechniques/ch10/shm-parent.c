/* Shared memory in Linux can be used between related processes, as we are about to
explore in this recipe, but also between unrelated processes using file descriptors to
shared memory. When we use shared memory in this way, the memory is backed by
a file in the /dev/shm directory. We'll look at this in the next recipe.
In this recipe, we'll be using anonymous shared memory—memory not backed by a file.
Shared memory is just what it sounds like—a piece of memory that is shared between
processes. */

/* we'll write a program that uses shared memory. First, before forking, the
process will write a message to the shared memory. Then, after forking, the child will
replace the message in the shared memory. And then, finally, the parent process will
replace the content of the shared memory once again. */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define DATASIZE 128

int main(void)
{
    char *addr;
    int status;
    pid_t pid;
    const char startmsg[] = "Hello, we are running";
    const char childmsg[] = "Hello from child";
    const char parentmsg[] = "New msg from parent";

    /* mapping the shared memory space. There's a total of six arguments we need to provide to the memory mapping function; that is, mmap().
        The first argument is the memory address, which we'll set to NULL - meaning the kernel will take care of it for us.
        The second argument is the size of the memory area.
        The third argument is the protection the memory should have. Here, we will set it to write and read.
        The fourth argument is our flags, which we set to shared and anonymous—meaning it can be shared among processes and
            won't be backed by a file.
        The fifth argument is a file descriptor. But in our case, we're using anonymous, which means that this memory won't be backed by a file.
            Due to this, we will set it to -1 for compatibility reasons.
        The last argument is the offset, which we will set to 0: */
    addr = mmap(NULL, DATASIZE, PROT_WRITE | PROT_READ,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    /* The memory area is mapped using mmap(). This function returns the address to the
        mapped memory. If an error occurs, it returns the MAP_FAILED macro. Once we mapped
        the memory, we checked the pointer variable for MAP_FAILED and aborted it in case
        there was an error */
    /* Once we've mapped the memory and got a pointer to it, we used memcpy() to copy data to it */
    if (addr == MAP_FAILED)
    {
        perror("Memory mapping failed");
        return 1;
    }

    /* Now that the memory is ready for us, we will copy our first message to it using
        memcpy(). The first argument to memcpy() is a pointer to the memory, which
        in our case is the addr character pointer. The second argument is the data or
        message we want to copy from, which in our case is startmsg. The last argument
        is the size of the data we want to copy, which in this case is the length of the string
        in startmsg + 1. The strlen() function doesn't include the terminating null
        character; that's why we need to add 1 */
    memcpy(addr, startmsg, strlen(startmsg) + 1);
    /* we print the PID of the process and the message in the shared memory. After
        that, we fork: */
    printf("Parent PID is %d\n", getpid());
    printf("Original message: %s\n", addr);
    if ((pid = fork()) == -1)
    {
        perror("Can't fork");
        return 1;
    }

    /* If we are in the child process, we copy the child's message to the shared memory. If
        we are in the parent process, we'll wait for the child. Then, we can copy the parent
        message to the memory and also print both messages. Finally, we will clean up by
        unmapping the shared memory.  */
    if (pid == 0)
    {
        /* child */
        memcpy(addr, childmsg, strlen(childmsg) + 1);
        /* after forking, the child will replace the message in the shared memory */
    }
    else if (pid > 0)
    {
        /* parent */
        /* Fork returns the pid of the child, so we pass the pid of the child process in waitpid() */
        waitpid(pid, &status, 0);
        printf("Child executed with PID %d\n", pid);
        printf("Message from child: %s\n", addr);
        /* finally, the parent process will replace the content of the shared memory once again. */
        memcpy(addr, parentmsg, strlen(parentmsg) + 1);
        printf("Parent message: %s\n", addr);
    }
    munmap(addr, DATASIZE);
    return 0;
}

/* Shared memory is a common IPC technique between unrelated processes, related
processes, and threads. In this recipe, we saw how we could use shared memory between
a parent and a child */
