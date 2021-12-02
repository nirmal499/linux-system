#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    /*  const char str1[] = "ABCDEFG019874";
        const char str2[] = "565A*^B#3C()D56";
        int len = strspn(str1, str2); len will have 4
    */
    char mph[10] = {0};

    while (fgets(mph, sizeof(mph), stdin) != NULL)
    {
        /* Check if mph is numeric (and do conversion) */
        if (strspn(mph, "012345689.-\n") == strlen(mph))
        {
            /* The strspn() function only reads the characters that we specified in the second
                argument to the function and then returns the number of read characters. We can then
                compare the number of characters read by strspn() with the entire length of the string,
                which we get with strlen(). If those match, we know that every character is either
                numeric, a dot, a minus, or a newline. If they don't match, this means an illegal character
                was found in the string. */
            printf("%.1f\n", (atof(mph) * 1.60934));
        }
        else
        {
            /* If mph is NOT numeric, print error and return */
            fprintf(stderr, "Found non-numeric"
                            " value\n");
            return 1;
        }
    }

    return 0;
}