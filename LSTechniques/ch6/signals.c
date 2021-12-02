/* C program that catches two signals, USR1 and USR2,
    and blocks the SIGINT signal (Ctrl+C). Depending on the signal we send to the program,
    different texts are printed. We did this by implementing a signal handler. A signal handler
    is a function that we write ourselves, just like any other function. Then, we registered that
    function as a signal handler with the sigaction() functio */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
/* To register a signal handler in a program, we can use the sigaction() system call. We need
to define _POSIX_C_SOURCE since sigaction() and its friends aren't included
in strict C99 */

void sigHandler(int sig);

int main(void)
{
    pid_t pid;               /* to store our pid in */
    pid = getpid();          /* get the pid */
    struct sigaction action; /* for sigaction */
    sigset_t set;            /* signals we want to ignore */
    printf("Program running with PID %d\n", pid);
    /* prepare sigaction() */
    /* First, we must set sa_handler to our function, which will execute when the signal is received. Second, we set sa_mask
        to all signals using sigfillset(). This will ignore all the signals while our
        signal handler is being executed, preventing it from being interrupted. Third, we
        set sa_flags to SA_RESTART, meaning any interrupted systems calls will be
        restarted: */
    action.sa_handler = sigHandler;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    /* register two signal handlers, one for USR1 and one for USR2 */
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    /* Remember that we wanted the program to ignore Ctrl+C (the interrupt signal)?
        This can be achieved by calling sigprocmask() before the code that should
        ignore the signal.But first, we must create a signal set with all the signals it should
        ignore/block. First, we will empty the set with sigemptyset(), and then add
        the required signals with sigaddset() */

    /* create a "signal set" for sigprocmask() */
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    /* block SIGINT and run an infinite loop */
    sigprocmask(SIG_BLOCK, &set, NULL);
    /* infinite loop to keep the program running */
    for (;;)
    {
        sleep(10);
    }
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    return 0;
}

void sigHandler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("Received USR1 signal\n");
    }
    else if (sig == SIGUSR2)
    {
        printf("Received USR2 signal\n");
    }
}