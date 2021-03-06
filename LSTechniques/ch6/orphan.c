/* An orphan is a child whose parent has died. However, as we have learned in this chapter,
every process needs a parent process. So, even orphans need a parent process. To solve
this dilemma, every orphan gets inherited by systemd, which is the first process on the system—PID 1.
we'll write a small program that forks, thus creating a child process. The
parent process will then exit, leaving the child as an orphan */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    printf("Parent PID is %d\n", getpid());
    /* fork, save the PID, and check for errors */
    if ((pid = fork()) == -1)
    {
        perror("Can't fork");
        return 1;
    }
    if (pid == 0)
    {
        /* if pid is 0 we are in the child process */
        printf("I am the child and will run for "
               "5 minutes\n");
        sleep(300);
        exit(0);
    }
    else if (pid > 0)
    {
        /* if pid is greater than 0 we are in
         * the parent */
        printf("My child has PID %d\n"
               "I, the parent, will exit when you "
               "press enter\n",
               pid);
        getchar();
        return 0;
    }
    else
    {
        fprintf(stderr, "Something went wrong "
                        "forking\n");
        return 1;
    }

    return 0;
}
