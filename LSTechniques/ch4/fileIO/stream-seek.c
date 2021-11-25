#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int ch;   /* for each character */
    FILE *fp; /* pointer to a file stream */
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Usage: %s [path] [from pos]"
                        " [to pos]\n",
                argv[0]);
        return 1;
    }
    /* open file with read mode */
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        perror("Can't open file for reading");
        return 1;
    }
    /* This program is similar to stream-read.c, but with the added ability to specify the start position
        and optionally the end position. Notice that we have added fseek() to set the
        start position. To abort the read, when we have reached the end position, we use
        ftell() to tell us the current position. If the end position is reached, we break out
        of the while loop. Also, we no longer read entire lines but individual characters.
        We do this with fgetc(). We also print individual characters instead of an entire
        string (line). We do this with putchar(). After the loop, we print a newline
        character so that the prompt won't end up on the same line as the output: */
    fseek(fp, atoi(argv[2]), SEEK_SET);
    /* loop over each line and write it to stdout */
    while ((ch = fgetc(fp)) != EOF)
    {
        if (argv[3] != NULL)
        {
            /* If end position given then keep checking whether we reached it or not*/
            if (ftell(fp) >= atoi(argv[3]))
            {
                break;
            }
        }
        /* If end position is not given then just putchar to console till we read the END OF FILE */
        putchar(ch);
    }
    printf("\n");
    fclose(fp); /* close the stream */
    return 0;
}
