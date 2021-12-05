/* Unix sockets are similar to TCP/IP sockets, but they are only local and are represented
by a socket file on the filesystem. But the overall functions that are used with Unix sockets
are more or less the same as for TCP/IP sockets. The complete name for Unix sockets is
Unix domain sockets.
Unix sockets are a common way for programs to communicate locally on a machine. */

/* we'll write a program that will act as a server. It will receive messages from
a client and respond with "Message received" every time a message is received. It will also
clean up after itself when either the server or the client exits */

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#define MAXLEN 128

void cleanUp(int signum);
/* Then, we'll declare some global variables (so that they can be reached from cleanUp()) */
const char sockname[] = "/tmp/my_1st_socket";
int connfd;
int datafd;

int main(void)
{
    int ret;
    /* The sockaddr_un struct is a special structure for Unix domain sockets. There's another
        one called sockaddr_in for TCP/IP sockets. The _un ending stands for Unix sockets,
        while _in stands for internet family sockets */
    struct sockaddr_un addr;
    char buffer[MAXLEN];
    struct sigaction action;
    /* We will also register the signal handler here for all the signals. The sockaddr_un struct will
        contain the socket type and file path: */
    /* prepare for sigaction */
    action.sa_handler = cleanUp;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    /* register the signals we want to handle */
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGABRT, &action, NULL);
    sigaction(SIGPIPE, &action, NULL);

    /* Now that we have all the signal handlers, variables, and structures in place, we can
        create a socket file descriptor using the socket() function */
    /* create socket file descriptor */
    connfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connfd == -1)
    {
        perror("Create socket failed");
        return 1;
    }
    /* Once that has been taken care of, we will set the type of connection (of the family type) and the path to
        the socket file. Then, we will call bind(), which will bind the socket for us so that
        we can use it: */
    /* set address family and socket path */
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sockname);
    /* bind the socket (we must cast our sockaddr_un to sockaddr) */
    if ((bind(connfd, (const struct sockaddr *)&addr,
              sizeof(struct sockaddr_un))) == -1)
    {
        perror("Binding socket failed");
        return 1;
    }
    /* we will prepare the socket file descriptor for connections by calling listen().
        The first argument is the socket file descriptor, while the second argument is
        the buffer size we want for the backlog. Once we've done that, we will accept a
        connection using accept(). This will give us a new socket file descriptor (which
        we will call datafd), which we will use when we send and receive data. Once a
        connection has been accepted, we can print Client connected to the local terminal */
    /* prepare for accepting connections */
    if ((listen(connfd, 20)) == -1)
    {
        perror("Listen error");
        return 1;
    }
    /* accept connection and create new socket file descriptor */
    datafd = accept(connfd, NULL, NULL);
    if (datafd == -1)
    {
        perror("Accept error");
        return 1;
    }
    printf("Client connected\n");
    /* In the outer loop, we'll just write a confirmation message when we received a message. In the inner loop, we'll read
        data from the new socket file descriptor, save it in buffer, and then print it on our
        terminal. If read() returns -1, then something has gone wrong, and we must break
        out of the inner loop to read the next line. If read() returns 0, then the client has
        disconnected, and we must run cleanUp() and quit: */
    while (1) /* main loop */
    {
        while (1) /* receive message, line by line */
        {
            ret = read(datafd, buffer, MAXLEN);
            if (ret == -1)
            {
                perror("Error reading line");
                cleanUp(1);
            }
            else if (ret == 0)
            {
                printf("Client disconnected\n");
                cleanUp(1);
            }
            else
            {
                printf("Message: %s\n", buffer);
                break;
            }
        }
        /* write a confirmation message */
        write(datafd, "Message received\n", 18);
    }
    return 0;
}

void cleanUp(int signum)
{
    printf("Quitting and cleaning up\n");
    close(connfd);
    close(datafd);
    unlink(sockname);
    exit(0);
}