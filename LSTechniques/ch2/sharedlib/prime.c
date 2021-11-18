#include "prime.h"

int isprime(long int number)
{
    int prime = 1;

    /* Test if the number is divisible, starting from 2 */
    for (int j = 2; j < number; j++)
    {
        if (number % j == 0)
        {
            prime = 0;
        }
    }

    if (prime == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}