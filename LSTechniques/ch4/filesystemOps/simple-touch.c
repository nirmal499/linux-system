#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <utime.h>
#include <linux/limits.h>

int main(int argc, char const *argv[])
{
    char filename[PATH_MAX] = {0};
    if (argc != 2)
    {
        fprintf(stderr, "You must supply a filename as an argument\n");
        return 1;
    }
    strncpy(filename, argv[1], PATH_MAX);

    /* Update the access and modification time to current time*/
    if (utime(filename, NULL) == -1)
    {
        /* If the file doesn't exist, create it */
        if (errno == ENOENT)
        {

            /* Creating a file and given its permission rw-r--r-- */
            if (creat(filename, 00644) == -1)
            {
                /* This perror is "print error", it prints the error message directly to stderr */
                perror("Can't create file");
                return errno;
            }
        } /* If we can't update the timestamp,something is wrong */
        else
        {
            perror("Can't update timestamp");
            return errno;
        }
    }
    return 0;
}
