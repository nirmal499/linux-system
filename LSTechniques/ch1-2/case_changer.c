#include <stdio.h>
#include <string.h>

/* When running the program Try it out by typing some words in it. 
    Quit the program by pressing Ctrl + D: 
*/
int main(void)
{
    char buffer[20] = {0};
    char newcase[20] = {0};

    /* You can input EOF to stdin, in which case fgets() will return 
        NULL when there is nothing left to return. */
    /* The fgets() function returns the character read on success and NULL on error or when
        an EOF occurs while no characters have been read (that is, no more input) */
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        for (int i = 0; i < sizeof(buffer); i++)
        {
            if ((buffer[i] >= 65) && (buffer[i] <= 90))
            {
                /* Upper case to lower case */
                newcase[i] = buffer[i] + 32;
            }
            else if ((buffer[i] >= 97 && buffer[i] <= 122))
            {
                /* Lower case to upper case */
                newcase[i] = buffer[i] - 32;
            }
            else
            {
                newcase[i] = buffer[i];
            }
        }
        /* fgets add '\n' at the end of the buffer */
        /* The function counts the number of characters until it hits a '\n' (in other words, it finds the first '\n').
            If it doesn't hit anything, it stops at the '\0' (returning the length of the string)
            Note that this works fine even if there is no newline, because strcspn stops at a '\0'.
            In that case, the entire line is simply replacing '\0' with '\0' 
        */
        newcase[strcspn(newcase, "\n")] = 0;

        printf("%s\n", newcase);
        /* zero out the arrays so there are no
            left-overs in the next run */
        memset(buffer, 0, sizeof(buffer));
        memset(newcase, 0, sizeof(newcase));
    }

    return 0;
}
