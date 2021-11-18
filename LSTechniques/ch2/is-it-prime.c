#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sharedlib/prime.h"

int main(int argc, char const *argv[])
{
    /* Only one argument is accepted */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s number\n", argv[0]);
        return 1;
    }
    /* Only numbers 0-9 are accepted */
    if (strspn(argv[1], "0123456789") != strlen(argv[1]))
    {
        fprintf(stderr, "Only numeric values are accepted\n");
        return 1;
    }

    long int num = atol(argv[1]);
    if (isprime(num))
    {
        printf("%ld is a prime\n", num);
    }
    else
    {
        printf("%ld is not a prime\n", num);
    }
    return 0;
}
