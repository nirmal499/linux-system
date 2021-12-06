#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *isprime(void *arg);
void *progress(void *arg);

int main(int argc, char const *argv[])
{
    long long number1;
    long long number2;
    pthread_t tid_prime1;
    pthread_t tid_prime2;
    pthread_t tid_progress;
    pthread_attr_t threadattr;

    if (argc != 3)
    {
        fprintf(stderr, "Please supply two numbers.\n"
                        "Example: %s 9 7\n",
                argv[0]);
        return 1;
    }

    /* convert the first and second arguments to long long integers with atoll() */
    number1 = atoll(argv[1]);
    number2 = atoll(argv[2]);

    /* we'll initialize the threads attribute structure, threadattr, with some default settings using pthread_attr_init() */
    pthread_attr_init(&threadattr);

    /* Then, we will create the three threads using pthread_create(). The
        pthread_create() function takes four arguments. The first argument is
        the thread ID variable; the second argument is the attributes for the thread;
        the third argument is the function that will execute in the thread; the fourth
        argument is the argument for that function. We will also mark the thread for
        the "progress bar" as detached using pthread_detach(). This makes the
        thread's resources release automatically when it terminates */
    pthread_create(&tid_progress, &threadattr, progress, NULL);
    pthread_detach(tid_progress);
    pthread_create(&tid_prime1, &threadattr, isprime, &number1);
    pthread_create(&tid_prime2, &threadattr, isprime, &number2);

    /* we called pthread_join() on both of the thread
        IDs (that are executing isprime()). This is necessary to make the process wait until
        the threads are finished; otherwise, it would end right away.  */
    pthread_join(tid_prime1, NULL);
    pthread_join(tid_prime2, NULL);
    pthread_attr_destroy(&threadattr);
    /* To make the program wait for all the threads to finish, we must use pthread_
        join() for each thread. Notice that we don't wait for the progress thread, but we
        did mark it as detached. Here, we will cancel the progress thread before we exit the
        program using pthread_cancel(): */
    if (pthread_cancel(tid_progress) != 0)
        fprintf(stderr, "Couldn't cancel progress thread\n");
    printf("Done!\n");
    return 0;
}

/* Notice that the return type of the function is a void
pointer. The argument is also a void pointer. This is the requirement in order for
pthread_create() to work. Since the argument is a void pointer, and we want
it as a long long int, we must first convert it. We do this by casting the void
pointer to a long long int and save what it's pointing to in a new variable */
void *isprime(void *arg)
{
    long long int number = *((long long *)arg);
    long long int j;
    int prime = 1;
    /* Test if the number is divisible, starting
     * from 2 */
    for (j = 2; j < number; j++)
    {
        /* Use the modulo operator to test if the
         * number is evenly divisible, i.e., a
         * prime number */
        if (number % j == 0)
        {
            prime = 0;
        }
    }
    /* Notice that we return NULL
        in this function. This is because we have to return something, so NULL will do just
        fine here: */
    if (prime == 1)
    {
        printf("\n%lld is a prime number\n", number);
        return NULL;
    }
    else
    {
        printf("\n%lld is not a prime number\n", number);
        return NULL;
    }
}

void *progress(void *arg)
{
    /*
        it just prints a dot every second to show the user that the program is still running.
        We must use fflush() after the call to printf() since we aren't printing any
        newline characters (remember that stdout is line-buffered) */
    while (1)
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    return NULL;
}