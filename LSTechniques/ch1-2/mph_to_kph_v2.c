#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void printhelp(FILE *stream, const char *msg);

int main(int argc, char *argv[])
{
    char mph[10] = {0};
    int opt, cont = 0;

    /* Parse command line options : -c -h */
    while ((opt = getopt(argc, argv, "ch")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printhelp(stdout, argv[0]);
            return 0;
        case 'c':
            cont = 1;
            break;
        default:
            printhelp(stderr, argv[0]);
            return 1;
        }
    }

    while (fgets(mph, sizeof(mph), stdin) != NULL)
    {
        if (strspn(mph, "0123456789.-\n") == strlen(mph))
        {
            printf("%.1f\n", (atof(mph) * 1.60934));
        }
        else
        {
            /* If mph is NOT numeric, print error and return */
            fprintf(stderr, "Found non-numeric"
                            " value\n");
            if (cont == 1)
            {
                /* It means -c is set */
                continue;
                /* Skip and continue if -c is set */
            }
            else
            {
                /* It means -c is not set */
                return 1;
                /* Abort if -c is not set */
            }
        }
    }
    return 0;
}

void printhelp(FILE *stream, const char *msg)
{
    fprintf(stream, "%s [-c] [-h]\n", msg);
    fprintf(stream, " -c continues even though a non"
                    "-numeric value was detected in the input\n"
                    " -h print help\n");
}