/* A daemon is a background process that runs on the system and performs
some tasks. The SSH daemon is a great example of this. Another great example is the
NTP daemon, which takes care of synchronizing the computer clock and sometimes
even distributing the time to other computers.
Knowing how to create a daemon will enable you to create server software; for example,
web servers, chat servers, and more */
/* we'll write a small daemon that will run in the background in our system.
The only "work" the daemon will do is write the current date and time to a file */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

int main(void)
{
    pid_t pid;
    FILE *fp;
    time_t now; /* for the current time */
    const char pidfile[] = "/var/run/my-daemon.pid";
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
        /* open pid-file for writing and error
         * check it */
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

/*  One of these new and important concepts is how to
start a new session with setsid(). If we don't create a new session, the daemon will still
be a part of the user's login session and die when the user logs off. But since we've created
a new session for the daemon and it is inherited by systemd, it now lives on its own,
unaffected by the user and process that started it.
The reason for forking the second time is that a session leader—which is what our first
child after the setsid() call is—can acquire a new controlling terminal if it were to
open a terminal device. When we do the second fork, that new child is just a member
of the session that was created by the first child, not the leader, and hence it cannot
acquire a controlling terminal anymore. The reason for avoiding a controlling terminal
is that if that terminal would exit, so would the daemon. Forking twice when creating
a daemon is often called the double-fork technique. */