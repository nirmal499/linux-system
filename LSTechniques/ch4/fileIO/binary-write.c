#include <stdio.h>

int main(void)
{
    FILE *fp;
    float x[2];

    /* . Notice that we open the file in write mode and binary mode, indicated by "wb" as the
        second argument to fopen(). In binary mode, we can write variables, arrays,
        and structures to a file */
    if ((fp = fopen("test-my-binary-file", "wb")) == 0)
    {
        fprintf(stderr, "Can't open file for "
                        "writing\n");
        return 1;
    }
    printf("Type two floating point numbers, "
           "separated by a space: ");

    scanf("%f %f", &x[0], &x[1]);
    /* The array in this program will be written to a file called
        test-my-binary-file in the current working directory. When we write binary data
        with fwrite(), we must specify the size of a single element (a float in this
        case) and how many of those elements we want to write. The second argument to
        fwrite() is the size of a single element and the third argument is the number of
        elements */
    fwrite(&x, sizeof(float), sizeof(x) / sizeof(float), fp);
    fclose(fp);
    return 0;
}