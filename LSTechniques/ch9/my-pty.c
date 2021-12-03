/* In this recipe, we'll create a PTY using a C program. A PTY consists of two parts: a master
(referred to as a pseudo-terminal master, or PTM) and a slave, or PTS. The program will
create a PTY and print the path to the slave on the current terminal. We can then connect
to that PTS with an application called screen and type away, and the characters will
be printed to both the master and the slave. The slave is where the screen program is
connected to, which is our terminal in this case. The master is usually quiet and runs in
the background */
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
/* A PTY, on the other hand, is a pseudo-terminal, meaning it's emulated in software.
Examples of PTYs are programs such as xterm, rxvt, Konsole, Gnome Terminal,
or a terminal multiplexer such as tmux. It could also be a remote terminal, such as SSH. */

int main(void)
{
    char rxbuf[1];
    char txbuf[3];
    int master; /* for the pts master fd */
    int c;      /* to catch read's return value */
    /* it's time to create the PTY device with posix_openpt(). This will return
        a file descriptor, which we'll save in master. Then, we run grantpt(), which sets
        the owner of the device to the current user, the group to tty, and changes the mode
        of the device to 620. We must also unlock it with unlockpt() before using it.
        To know where we should connect, we also print the path to the slave device */
    master = posix_openpt(O_RDWR);
    grantpt(master);
    unlockpt(master);
    printf("Slave: %s\n", ptsname(master));

    /* we create the main loop of the program. In the loop, we read a character from
        the PTS and then write it back again to the PTS. Here, we also print the character
        to the master so that we'll see that it's a master/slave pair. Since a terminal device
        is rather primitive, we must manually check for a carriage return character
        (the Enter key) and instead print a newline and a carriage return to make
        a new line */
    while (1) /* main loop */
    {
        /* read from the master file descriptor */
        /* Return the number read in bytes, -1 for errors or 0 for EOF */
        c = read(master, rxbuf, 1);
        /* We are reading only 1 byte */
        if (c == 1)
        {
            /* convert carriage[It represents ENTER] return to '\n\r' */
            if (rxbuf[0] == '\r')
            {
                printf("\n\r");         /* on master */
                sprintf(txbuf, "\n\r"); /* on slave */
                /* txbuf has "\n\r" */
            }
            else
            {
                printf("%c", rxbuf[0]);
                sprintf(txbuf, "%c", rxbuf[0]);
            }
            fflush(stdout);
            write(master, txbuf, strlen(txbuf));
        }
        /* If no characters are received, the device connected to the slave has disconnected.
            If that is the case, we return, and hence exit from the program: */
        else /* if c is not 1, it has disconnected */
        {
            printf("Disconnected\n\r");
            return 0;
        }
    }
    return 0;
}
