/* we'll write a program that creates a FIFO and sends a message to it */
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

void cleanUp(int signum);
int fd; /* the FIFO file descriptor */
const char fifoname[] = "/tmp/my-2nd-fifo";

int main(int argc, char *argv[])
{
    struct sigaction action; /* for sigaction */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s 'the message'\n", argv[0]);
        return 1;
    }
    /*  we must register the signal handler for all the signals we want to catch. We are
        doing this so that we can remove the FIFO when the program exits */
    /* prepare for sigaction and register signals (for cleanup when we exit) */
    action.sa_handler = cleanUp;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGABRT, &action, NULL);
    sigaction(SIGPIPE, &action, NULL);

    /* let's create the FIFO with mode 644. Since mode 644 is octal, we need to write
        it as 0644 in the C code; otherwise, it will be interpreted as 644 decimal (any number
        that starts with a 0 in C is an octal number). After that, we must open the FIFO using
        the open() system call—the same system call we use to open regular files */
    if ((mkfifo(fifoname, 0644)) != 0)
    {
        perror("Can't create FIFO");
        return 1;
    }
    if ((fd = open(fifoname, O_WRONLY)) == -1)
    {
        perror("Can't open FIFO");
        return 1;
    }

    /* we must create an endless loop. Inside this loop, we will print the user-provided
        message once every second. After the loop, we will close the file descriptor and
        remove the FIFO file. We shouldn't reach this under normal circumstances, thoug */
    while (1)
    {
        dprintf(fd, "%s\n", argv[1]);
        sleep(1);
    }
    /* just in case, but we shouldn't reach this */
    close(fd);
    unlink(fifoname);

    return 0;
}

/* we must create the cleanUp() function, which we registered as the signal
handler. We use this function to clean up before the program exits. We must then
close the file descriptor and remove the FIFO file: */
void cleanUp(int signum)
{
    if (signum == SIGPIPE)
        printf("The receiver stopped receiving\n");
    else
        printf("Aborting...\n");
    if ((close(fd)) == -1)
        perror("Can't close file descriptor");
    if ((unlink(fifoname)) == -1)
    {
        perror("Can't remove FIFO");
        exit(1);
    }
    exit(0);
}

/* When the other end terminates—in our case, the cat program—our program will
receive a SIGPIPE signal. If we hadn't caught that signal, our program would have exited
with signal 141, and no clean-up would have occurred. From this exit code, we can figure
out that it was due to a SIGPIPE signal since 141-128 = 13; and signal 13 is SIGPIPE */
