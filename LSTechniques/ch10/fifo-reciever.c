/* we'll write a C program that reads from the FIFO.
Reading from a FIFO isn't any different than reading from a regular file, or let's say, stdin. */
#include <stdio.h>

int main(void)
{
    FILE *fp;
    signed char c;
    const char fifoname[] = "/tmp/my-2nd-fifo";
    if ((fp = fopen(fifoname, "r")) == NULL)
    {
        perror("Can't open FIFO");
        return 1;
    }
    /* Since the FIFO is a file on the filesystem, we can receive data from it using the usual
        functions in C, such as file streams, getc(), putchar() */
    while ((c = getc(fp)) != EOF)
        putchar(c);
    fclose(fp);
    return 0;
}