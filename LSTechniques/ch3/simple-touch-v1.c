#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>

int main(int argc, char const *argv[])
{
    int errornum;
    char filename[PATH_MAX] = {0};
    if (argc != 2)
    {
        fprintf(stderr, "You must supply a filename as an argument\n");
        return 1;
    }
    strncpy(filename, argv[1], PATH_MAX);
    /* Creating a file and given its permission rw-r--r-- */
    if (creat(filename, 00644) == -1)
    {
        /* We have also saved the value from errno in a new variable called errornum. We did
            this because on the next error that occurs, the value in errno will be overwritten by the
            new error code. To safeguard against showing the wrong error message in case errno
            gets overwritten, it's safer to save it to a new variable */
        errornum = errno;
        fprintf(stderr, "Can't create file %s\n", filename);
        fprintf(stderr, "%s\n", strerror(errornum));
        return 1;
    }
    return 0;
}
