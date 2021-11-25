/* It converts the initial part of the string in str to a long int value according to the given base, 
    which must be between 2 and 36 inclusive, or be the special value 0. */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[30] = "2030300 This is test";
    char *ptr;
    long ret, ret2;
    char str2[10] = "4755";

    ret = strtol(str, &ptr, 10);
    ret2 = strtol(str2, NULL, 8);

    printf("The number(unsigned long integer) is %ld\n", ret);
    printf("String part is |%s|\n", ptr);
    printf("The number(unsigned long integer) is %ld\n", ret2);

    return (0);
}