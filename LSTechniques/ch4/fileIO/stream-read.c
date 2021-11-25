#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *fp;                 /* pointer to a file stream */
    char linebuf[1024] = {0}; /* line buffer */

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [path]\n", argv[0]);
        return 1;
    }
    /* open file with read mode */
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        perror("Can't open file for writing");
        return 1;
    }
    /*loop over each line and write it to the file*/
    while (fgets(linebuf, sizeof(linebuf), fp) != NULL)
    {
        /* fgets:- Get a newline-terminated string of finite length from STREAM */
        /* To read each line, we loop over the file stream using fgets(). As you might have
            noticed in both this and the previous recipe, we don't use sizeof(linebuf)-1,
            only sizeof(linebuf). That is because fgets() only reads one less than the size
            we give it. 
        */
        printf("%s", linebuf);
    }

    fclose(fp); /* close the stream */
    return 0;
}
