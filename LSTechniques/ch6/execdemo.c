#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

int main(void)
{
    printf("My PID is %d\n", getpid());
    printf("Hit enter to continue ");
    getchar(); /* wait for enter key */
    printf("Executing /usr/bin/less...\n");
    /* execute less using execl and error check it */
    /* The execl() function takes four mandatory arguments. The first one is the path to
        the program we want to execute. The second argument is the program's name, as it
        would be printed from argv[0]. Finally, the third and any following argument is the
        argument we want to pass to the program we are about to execute. To terminate this list
        of arguments that we want to pass to the program, we must end it with a pointer to NULL,
        cast as a char. */
    if (execl("/usr/bin/less", "less", "/etc/passwd", (char *)NULL) == -1)
    {
        perror("Can't execute program");
        return 1;
    }
    return 0;
}