#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printhelp(const char *msg);

/* Here char const *argv[] is generating a warning */
int main(int argc, char *argv[])
{
    int opt, sum;
    /* Simple sanity check */
    if (argc == 1)
    {
        printhelp(argv[0]);
        return 1;
    }

    /* Parse command line options : -s -m -h */
    while ((opt = getopt(argc, argv, "smh")) != -1)
    {
        switch (opt)
        {
        case 's':
            sum = 0;
            for (size_t i = 2; i < argc; i++)
            {
                sum += atoi(argv[i]);
            }

            break;
        case 'm':
            sum = 1;
            for (size_t i = 2; i < argc; i++)
            {
                sum *= atoi(argv[i]);
            }
            break;
        case 'h':
            printhelp(argv[0]);
            return 0;
            break;

        default: /* in case of invalid options */
            printhelp(argv[0]);
            return 1;
            break;
        }
    }

    printf("Total: %d\n", sum);
    return 0;
}

void printhelp(const char *msg)
{
    printf("%s [-s] [-m] integer ...\n", msg);
    printf("-s sums all the integers\n"
           "-m multiplies all the integers\n"
           "This program takes any number of integer "
           "values and either add or multiply them.\n"
           "For example: %s -m 5 5 5\n",
           msg);
}