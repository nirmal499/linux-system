#include <stdio.h>

int main(void)
{
    FILE *fp;
    float x[2];

    /* Notice that we use "rb" here, for read and binary. */
    if ((fp = fopen("test-my-binary-file", "rb")) == 0)
    {
        fprintf(stderr, "Can't open file for "
                        "writing\n");
        return 1;
    }

    /* The arguments to fread() are the same as fwrite()  */
    fread(&x, sizeof(float), sizeof(x) / sizeof(float), fp);
    printf("The first number was: %f\n", x[0]);
    printf("The second number was: %f\n", x[1]);
    fclose(fp);
    return 0;
}