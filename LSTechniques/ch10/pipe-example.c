/* we'll create a program that forks and then communicates between two
processes using a pipe. Sometimes, when we fork a process, the parent and the child
need a way to communicate. A pipe is often a simple way to do just that */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MAX 128

int main(void)
{
    int pipefd[2] = {0};
    pid_t pid;
    char line[MAX];
    /* we will create the pipe using the pipe() system call. We'll give it the integer
        array as an argument. After that, we'll fork using the fork() system call */
    if ((pipe(pipefd)) == -1)
    {
        perror("Can't create pipe");
        return 1;
    }

    /* The pipe() system call returns two file descriptors to the integer array. The first one,
        pipefd[0], is the read end of the pipe, while the other, pipefd[1], is the write end of
        the pipe. In the parent, we write a message to the write end of the pipe. Then, in the child
        process, we read that data from the read end of the pipe. But before we do any reading or
        writing, we close the end of the pipe that we're not using in the respective process */
    if (pid > 0)
    {
        /* If we are inside the parent process, we close the read end (since we only want
            to write from the parent) */
        /* inside the parent */
        close(pipefd[0]); /* close the read end */
        dprintf(pipefd[1], "Hello from parent");
        /* we write a message to the pipe's file descriptor (the write end) using dprintf() */
    }
    else
    {
        /* Inside the child, we do the opposite; that is, we close the write end of the pipe.
            Then, we read the data in the pipe using the read() system call. Finally, we print
            the message using printf() */
        /* inside the child */
        close(pipefd[1]); /* close the write end */
        read(pipefd[0], line, MAX - 1);
        printf("%s\n", line); /* print message from the parent */
    }
    return 0;
}
