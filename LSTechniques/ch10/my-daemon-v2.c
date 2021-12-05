
/* we'll write a small daemon that will run in the background in our system.
The only "work" the daemon will do is write the current date and time to a file */
/* In our my-daemon.c, the PID file isn't removed when the daemon is killed.
Likewise, the open file stream (/tmp/my-daemon-is-alive.txt) isn't closed when
the daemon is killed. A proper daemon should clean up after itself when it exits.
To be able to clean up on exit, we need to implement a signal handler. The signal handler
should then take care of all the cleanup before the daemon is terminated.  */
#define _POSIX_C_SOURCE 200809L /* for sigaction() */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

void sigHandler(int sig);
/* moved these variables to the global scope
    since they need to be access/deleted/closed
    from the signal handler */
FILE *fp;
const char pidfile[] = "/var/run/my-daemon.pid";

int main(void)
{
    pid_t pid;
    time_t now;              /* for the current time */
    struct sigaction action; /* for sigaction */
    const char daemonfile[] =
        "/tmp/my-daemon-is-alive.txt";
    if ((pid = fork()) == -1)
    {
        perror("Can't fork");
        return 1;
    }
    /*  say in 'if' fork() does not returned -1. Eventhough it did not returned -1, the fork was executed and return value
        stored in pid and since 'if' condition turns out to be false, so 'else if' condition was checked  */
    else if ((pid != 0))
    {
        exit(0);
    }
    /* the parent process has exited, so this is the
     * child. create a new session to lose the
     * 'controlling terminal' */
    /* In the child process, we will create a new session
        with setsid(). Creating a new session will free the process from the 'controlling
        terminal'[PID of terminal is setted as the PGID and SID of this child process] */
    setsid(); /* Replaced the old session id [SID] which was basically the PID of terminal */

    /* fork again, creating a grandchild, the actual daemon */
    if ((pid = fork()) == -1)
    {
        perror("Can't fork");
        return 1;
    }

    /* the child process[It is acting as a parent of grandchild process] which will exit */
    else if (pid > 0)
    {
        /* open pid-file for writing and check for error */
        if ((fp = fopen(pidfile, "w")) == NULL)
        {
            perror("Can't open file for writing");
            return 1;
        }
        /* write pid to file */
        fprintf(fp, "%d\n", pid);
        fclose(fp); /* close the file pointer */
        exit(0);    /* The child process[It is acting as a parent of grandchild process] exited */
    }
    /* Everything below this is run by the grandchild process */

    umask(022); /* set the umask to something ok */
    chdir("/"); /* change working directory to / */
    /* open the "daemonfile" for writing */
    if ((fp = fopen(daemonfile, "w")) == NULL)
    {
        perror("Can't open daemonfile");
        return 1;
    } /* from here, we don't need stdin, stdout or,
       * stderr anymore, so let's close them all,
       * then re-open them to /dev/null */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY); /* 0 = stdin */
    open("/dev/null", O_WRONLY); /* 1 = stdout */
    open("/dev/null", O_RDWR);   /* 2 = stderr */

    /* Now, it's finally time to prepare and register the signal handlers. */
    /* prepare for sigaction */
    action.sa_handler = sigHandler;
    sigfillset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    /* register the signals we want to handle */
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGABRT, &action, NULL);

    /* here we start the daemons "work" */
    for (;;)
    {
        /* get the current time and write it to the
        "daemonfile" that we opened above */
        time(&now);
        fprintf(fp, "Daemon alive at %s",
                ctime(&now));
        fflush(fp); /* flush the stream */
        sleep(30);
    }
    return 0;
}

/* Here, we clean up after the daemon by removing the PID file before exiting. We also close the open file
stream to the daemon file: */
void sigHandler(int sig)
{
    int status = 0;
    if (sig == SIGTERM || sig == SIGINT || sig == SIGQUIT || sig == SIGABRT)
    {
        /* remove the pid-file */
        if ((unlink(pidfile)) == -1)
            status = 1;
        if ((fclose(fp)) == EOF)
            status = 1;
        exit(status); /* exit with the status set*/
    }
    else /* some other signal */
    {
        exit(1);
    }
}
/* To cover the most common "exit"
signals, we registered the handler with four different signals: terminate, interrupt,
quit, and abort. When one of these signals is received by the daemon, it triggers the
sigHandler() function. This function then removes the PID file and closes the file
stream. Finally, the function exits the entire daemon by calling exit() */
/* However, since we can't pass the filename or the file stream as an argument to the signal
handler, we placed those variables in the global scope instead. This makes it possible for
both main() and sigHandler() to reach them */