#include <stdio.h>
#include <string.h>

int main(void)
{
    char text[20];
    /* , we'll write a simple program that fills a character array with the character x.
        Then we'll copy a new, shorter string on top of that and finally print the string. It's only the
        newly copied string that is printed, even if all the x characters are still there.  */
    memset(text, 'x', 20);
    strcpy(text, "Hello");
    printf("%s\n", text);
    return 0;
}