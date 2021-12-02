#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    /* This "FULLNAME" variable won't be available in
        the shell that started the program. The program we run is a child process of the shell,
        and hence it can't change the shell's variable; that is, its parent process. But any other
        programs started from inside our own program will be able to see those variables.  */
    setenv("FULLNAME", "Jack-Benny", 1);
    /* The 1 in the third argument to
        setenv() means that we want to overwrite the variable if it already exists.
        If we change it to a 0, it prevents overwriting */
    printf("Your full name is %s\n", getenv("FULLNAME"));
    return 0;
}