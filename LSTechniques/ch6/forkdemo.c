#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    /* When a process forks, it creates a duplicate of itself. This duplicate becomes a child process
        of the process that called fork()—the parent process. The child process is identical to
        the parent process, except it has a new PID. Inside the parent process, fork() returns
        the PID of the child process. Inside the child process, 0 is returned. This is why the parent
        could print the PID of the child process. */
    pid_t pid;
    printf("My PID is %d\n", getpid());
    /* fork, save the PID, and check for errors */
    if ((pid = fork()) == -1)
    {
        perror("Can't fork");
        return 1;
    }

    /* Both processes contain the same program code, and both processes are running, but only
        the specific parts in the if statements get executed, depending on whether the process is
        the parent or the child */
    if (pid == 0)
    {
        /* if pid is 0 we are in the child process */
        printf("Hello from the child process! %d %d \n", pid, getpid());
        sleep(120);
    }
    else if (pid > 0)
    {
        /* if pid is greater than 0 we are in the parent */
        printf("Hello from the parent process! %d : "
               "My child has PID %d\n",
               getpid(), pid);
        sleep(120);
    }
    else
    {
        fprintf(stderr, "Something went wrong "
                        "forking\n");
        return 1;
    }

    return 0;
}