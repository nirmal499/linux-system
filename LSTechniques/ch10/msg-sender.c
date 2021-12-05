/* Another popular IPC technique is message queues. It's pretty much what the name
suggests. A process leaves messages in a queue, and another process reads them */
/* We will create a new message queue and some messages to it. */

#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#define MAX_MSG_SIZE 2048

int main(int argc, char const *argv[])
{
    int md; /* msg queue descriptor */

    /* attributes for the message queue */
    struct mq_attr msgattr;
    msgattr.mq_maxmsg = 10;
    msgattr.mq_msgsize = MAX_MSG_SIZE;
    /* This queue can contain a maximum of 10 messages using the mq_maxmsg member of msgattr.
        Also set the maximum length of each message to 2,048 characters using the
        mq_msgsize member*/

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s 'my message'\n",
                argv[0]);
        return 1;
    }
    /* it's time to open and create the message queue with mq_open() */
    /* The first argument is the name of the queue; here, it's /my_queue. The second argument is
        the flags, which in our case are O_CREATE and O_RDWR. These are the same flags
        that we have seen previously, for example, with open(). The third argument is
        the permission mode; once again, this is the same as for files. The fourth and last
        argument is the struct we created earlier. The mq_open() function then returns
        a message queue descriptor to the md variable */
    md = mq_open("/my_queue", O_CREAT | O_RDWR, 0644, &msgattr);
    /* We named the queue /my_queue when we called mq_open(). A message queue must
        start with a forward slash */
    if (md == -1)
    {
        perror("Creating message queue");
        return 1;
    }
    /* Then, finally, we send the message to the queue using mq_send(). Here, first, we
        give it the md descriptor. Then, we have the message we want to send, which in this
        case is the first argument to the program. Then, as the third argument, we must
        specify the size of the message. Finally, we must set a priority for the message; in this
        case, we will just go with 1. It can be any positive number (an unsigned int). */
    if ((mq_send(md, argv[1], strlen(argv[1]), 1)) == -1)
    {
        perror("Message queue send");
        return 1;
    }
    mq_close(md);
    return 0;
}
