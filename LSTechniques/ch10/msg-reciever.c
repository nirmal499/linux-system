/* we'll create a program that receives the messages from that queue */
#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int md; /* msg queue descriptor */
    char *buffer;
    struct mq_attr msgattr;

    md = mq_open("/my_queue", O_RDONLY);
    if (md == -1)
    {
        perror("Open message queue");
        return 1;
    }

    /* we also want to get the attributes of the message queue using mq_
        getattr(). Once we have the attributes of the queue, we can use its mq_
        msgsize member to allocate memory for a message of that size using calloc().
        We haven't seen calloc() before in this book. The first argument is the number
        of elements we want to allocate memory for, while the second argument is the size
        of each element. The calloc() function then returns a pointer to that memory */
    if ((mq_getattr(md, &msgattr)) == -1)
    {
        perror("Get message attribute");
        return 1;
    }
    /* We use the maximum message size from mq_msgsize to allocate memory for a message
        buffer using calloc(). The calloc() function returns "zeroed" memory, which its
        counterpart, malloc(), doesn't */
    buffer = calloc(msgattr.mq_msgsize, sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stderr, "Couldn't allocate memory");
        return 1;
    }
    /* we will use another member of the mq_attr struct called mq_curmsgs,
        which contains the number of messages currently in the queue. First, we will print
        the number of messages. Then, we will loop over all the messages using a for loop.
        Inside the loop, first, we receive a message using mq_receive. Then, we print the
        message using printf(). Finally, before iterating over the next message, we reset
        the entire memory to NULL characters using memset() */
    printf("%ld messages in queue\n", msgattr.mq_curmsgs);
    for (int i = 0; i < msgattr.mq_curmsgs; i++)
    {
        /* The first argument to mq_receive is the descriptor, the second argument is the
            buffer where the message goes, the third argument is the size of the message, and
            the fourth argument is the priority of the message, which in this case is NULL,
            meaning we receive all messages with the highest priority first: */
        if ((mq_receive(md, buffer, msgattr.mq_msgsize, NULL)) == -1)
        {
            perror("Message receive");
            return 1;
        }
        printf("%s\n", buffer);
        memset(buffer, '\0', msgattr.mq_msgsize);
    }
    /* Finally, we have some cleanup to do. First of all, we must free() the memory
        being pointed to by the buffer. Then, we must close the md queue descriptor, before
        removing the queue from the system using mq_unlink(): */
    free(buffer);
    mq_close(md);
    mq_unlink("/my_queue");
    return 0;
}
