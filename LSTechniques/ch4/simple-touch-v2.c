#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
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
    /* Creating a file and given its permission rw-r--r-- */
    if (creat(filename, 00644) == -1)
    {
        /* This perror is "print error", it prints the error message directly to stderr */
        perror("Can't create file");
        return errno;
    }
    return 0;
}
