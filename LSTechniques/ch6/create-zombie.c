#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/* A zombie process is a child that has exited before the parent, and the parent process
hasn't waited for the child's status. The name "zombie process" comes from the fact that
the process is undead. The process has exited, but there is still an entry for it in the system
process table */
int main(void)
{
    pid_t pid;
    printf("My PID is %d\n", getpid());

    /* fork, save the PID and check for errors */
    if ((pid = fork()) == -1)
    {
        perror("Can't fork");
        return 1;
    }

    if (pid == 0)
    {
        /* if pid is 0 we are in the child process */
        printf("Hello and goodbye from the child!\n");
        exit(0);
        /* the child exits using exit(0) before the parent exits */
    }
    else if (pid > 0)
    {
        /* if pid is greater than 0 we are in the parent process */
        printf("Hello from the parent process! "
               "My child had PID %d\n",
               pid);
        sleep(120);
        /* The parent sleeps for 2 minutes after the child has exited, without waiting for the child with waitpid(), thus creating
            a zombie process.  */
    }
    else
    {
        fprintf(stderr, "Something went wrong "
                        "forking\n");
        return 1;
    }
    return 0;
}
