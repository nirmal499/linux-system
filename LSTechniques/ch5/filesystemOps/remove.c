/* The name comes from the fact that when we remove a filename, we only remove a hard link to that inode; hence we
unlink a filename. If it happens to be the last filename to an inode, then the inode is
also removed. */
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [path]\n", argv[0]);
        return 1;
    }
    if (unlink(argv[1]) == -1)
    {
        perror("Can't remove file");
        return errno;
    }
    return 0;
}