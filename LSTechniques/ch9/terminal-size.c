/* we will write a program that first clears the screen using a special escape sequence,
then fetches the terminal's size and prints to the screen */

/* The program uses an endless loop, so to quit the program, we must use Ctrl + C. On each
iteration of the loop, we first clear the screen by printing a special escape sequence.
Then, we get the terminal size with ioctl() and print the size on the screen: */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int main(void)
{
    struct winsize termsize;
    while (1)
    {
        /* Then, to clear the screen, we use printf() to print a special escape sequence,
            \033[1;1H\033[2J. The \033 sequence is the escape code. After the escape code,
            we have a [ character, then we have the actual code telling the terminal what to do. The
            first one, 1;1H, moves the cursor to position 1,1 (the first row and first column). Then,
            we use the \033 escape code again so that we can use another code. First, we have the
            [ character, just as before. Then, we have the [2J code, which means to erase the entire
            display */
        printf("\033[1;1H\033[2J");
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &termsize);
        /* Once we have cleared the screen and moved the cursor, we use ioctl() to get the
            terminal size. The first argument is the file descriptor; here, we use stdout. The second
            argument is the command to send; here, it's TIOCGWINSZ to get the terminal size */
        printf("Height: %d rows\n",
               termsize.ws_row);
        printf("Width: %d columns\n",
               termsize.ws_col);
        sleep(0.1);
    }
    return 0;
}