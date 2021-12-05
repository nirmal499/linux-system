/* we'll create a client for that socket and then communicate between the client and the server.
In this recipe, we'll see how we can use the socket to communicate between a server and
a client. Once they are connected, the client can send messages to the server, and the server will respond
with Message received */

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

int main(void)
{
    /* The first half of this program is similar to that of the unix-server.c, except we have two buffers instead of one and no signal handling */
    const char sockname[] = "/tmp/my_1st_socket";
    int fd;
    struct sockaddr_un addr;
    char sendbuffer[MAXLEN];
    char recvbuffer[MAXLEN];
    /* create socket file descriptor */
    fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd == -1)
    {
        perror("Create socket failed");
        return 1;
    }
    /* set address family and socket path */
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sockname);
    /* Now, instead of using bind(), listen(), and accept(), we will use
        connect() to initiate a connection to the server. The connect() function takes
        the same arguments as bind() */
    /* connect to the server */
    if ((connect(fd, (const struct sockaddr *)&addr,
                 sizeof(struct sockaddr_un))) == -1)
    {
        perror("Can't connect");
        fprintf(stderr, "The server is down?\n");
        return 1;
    }
    while (1) /* main loop */
    {
        /* send message to server */
        printf("Message to send: ");
        /* Now that we have connected to the server, we can use write() to send messages
            over the socket file descriptor. Here, we will use fgets() to read the messages
            from the user to a buffer, convert a newline character into a null character, and
            then write the buffer to a file descriptor: */
        fgets(sendbuffer, sizeof(sendbuffer), stdin);
        sendbuffer[strcspn(sendbuffer, "\n")] = '\0';
        if ((write(fd, sendbuffer, strlen(sendbuffer) + 1)) == -1)
        {
            perror("Couldn't write");
            break;
        }
        /* read response from server */
        if ((read(fd, recvbuffer, MAXLEN)) == -1)
        {
            perror("Can't read");
            return 1;
        }
        printf("Server said: %s\n", recvbuffer);
    }
    return 0;
}
