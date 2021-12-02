#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int fd; /* for the file descriptor */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s [path] [string]\n", argv[0]);
        return 1;
    }
    /* Open the file (argv[1]) and create it if it
        doesn't exist and set it in read-write mode.
        Set the access mode to 644 
        The open() system call takes three arguments: the path to the file, which mode
        the file shall open in (in this case, create the file if it doesn't exist, and open it in
        read-write mode), and the access mode (here 0644)
    */
    /* The first one, O_CREAT, means that if the file doesn't exist, it is created. The second one, O_RDWR,
    means that the file should be open for both reading and writing */
    if ((fd = open(argv[1], O_CREAT | O_RDWR, 00644)) == -1)
    {
        perror("Can't open file for writing");
        return 1;
    }
    /* write content to file */
    if ((write(fd, argv[2], strlen(argv[2]))) == -1)
    {
        perror("Can't write to file");
        return 1;
    }
    return 0;
}
