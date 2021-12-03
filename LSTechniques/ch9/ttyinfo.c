#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    /* . The first one is isatty(), which checks if a file
        descriptor is a terminal. Here, we check if stdout is a terminal. The other function
        is ttyname(), which prints the terminal's name connected to stdout  */
    if ((isatty(STDOUT_FILENO) == 1))
    {
        printf("It's a TTY with the name %s\n", ttyname(STDOUT_FILENO));
    }
    else
    {
        perror("isatty");
    }
    printf("Hello world\n");
    return 0;
}