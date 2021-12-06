/* With condition variables, we can signal a thread when another thread has finished its
work or when some other event occurs. For example, with condition variables, we can
rewrite the prime number program from the Reading return values from threads recipe to
join with the thread that finishes first. That way, the program isn't compelled to join with
thread 1 first and then thread 2. Instead, the thread that finishes first signals to main()
using a condition variable that it has finished and then joins with that thread */

/*  we'll rewrite the prime number program from the Reading return values
from threads recipe to use condition variables */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

void *isprime(void *arg);
void *progress(void *arg);

/* a mutex that we name lock, a condition variable that we name ready, and a thread ID for the
prime thread, which we name primeid.  */
pthread_mutex_t lock;
pthread_cond_t ready;
pthread_t primeid = 0;
/* The primeid variable will be used to
send the thread ID from the thread that has finished */

int main(int argc, char *argv[])
{
    long long number1;
    long long number2;
    pthread_t tid_prime1;
    pthread_t tid_prime2;
    pthread_t tid_progress;
    pthread_attr_t threadattr;
    void *prime1Return;
    void *prime2Return;

    /* we must initialize both the mutex and the condition variable: */
    if ((pthread_mutex_init(&lock, NULL)) != 0)
    {
        fprintf(stderr, "Couldn't initialize mutex\n");
        return 1;
    }
    if ((pthread_cond_init(&ready, NULL)) != 0)
    {
        fprintf(stderr, "Couldn't initialize condition variable\n");
        return 1;
    }

    if (argc != 3)
    {
        fprintf(stderr, "Please supply two numbers.\n"
                        "Example: %s 9 7\n",
                argv[0]);
        return 1;
    }
    number1 = atoll(argv[1]);
    number2 = atoll(argv[2]);
    pthread_attr_init(&threadattr);
    pthread_create(&tid_progress, &threadattr,
                   progress, NULL);
    pthread_detach(tid_progress);
    pthread_create(&tid_prime1, &threadattr,
                   isprime, &number1);
    pthread_create(&tid_prime2, &threadattr,
                   isprime, &number2);

    /* We'll start by locking the mutex so that the
        primeid variable is protected. Then, we wait for the signal from the condition
        variable using pthread_cond_wait(). This will release the mutex so that the
        threads can write to primeid. Note that we also loop the pthread_cond_
        wait() call in a while loop. We do this because we only want to wait for the
        signal if primeid is still 0. Since pthread_cond_wait() will block, it won't
        use any CPU cycles. When we get the signal, we move down to the if statement.
        This checks which thread it was that finished and joins it. Then we go back and
        start again using the for loop. Each time an if or else statement has
        completed—when a thread has joined—the primeid variable is reset to 0. This
        will make the next iteration wait again with pthread_cond_wait() */
    pthread_mutex_lock(&lock);
    /* There are two threads we are waiting for, so the for loop in main() will run the loop two times.
     */
    for (int i = 0; i < 2; i++)
    {
        /* we wait in the while loop with pthread_cond_wait(), we called it with
            both the condition variable (ready) and the mutex (lock). That way, it knows which
            mutex to release and which signal to wait for. It's when we wait that the mutex is released */
        /* During the waiting, the other threads can write to the primeid variable. The other
            threads will first lock the variable with the mutex before writing to it. Once they have
            written to the variable, they signal the condition variable and release the mutex. This
            wakes up the main() function, which is currently waiting with pthread_cond_
            wait(). The main() function then checks which thread it was that finished and joins it
            with pthread_join(). Then, the main() function will reset the primeid variable to
            0 and go back to waiting with pthread_cond_wait() until the next thread signals that
            it's finished */
        while (primeid == 0)
            pthread_cond_wait(&ready, &lock);
        if (primeid == tid_prime1)
        {
            pthread_join(tid_prime1, &prime1Return);
            if ((uintptr_t)prime1Return == 1)
                printf("\n%lld is a prime number\n", number1);
            else
                printf("\n%lld is not a prime number\n", number1);
            primeid = 0;
        }
        else
        {
            pthread_join(tid_prime2, &prime2Return);
            if ((uintptr_t)prime2Return == 1)
                printf("\n%lld is a prime number\n", number2);
            else
                printf("\n%lld is not a prime number\n", number2);
            primeid = 0;
        }
    }
    pthread_mutex_unlock(&lock);

    pthread_attr_destroy(&threadattr);
    if (pthread_cancel(tid_progress) != 0)
        fprintf(stderr,
                "Couldn't cancel progress thread\n");
    return 0;
}
/* Now if we test it with both the smaller number as the first argument and then as the second argument.
    Either way, the fastest number to compute will be displayed instantly, without having to wait for the other thread
    to join: */

void *isprime(void *arg)
{
    long long int number = *((long long *)arg);
    long long int j;
    int prime = 1;
    for (j = 2; j < number; j++)
    {
        if (number % j == 0)
            prime = 0;
    }
    /* Once the function is done calculating the number, we lock the mutex to protect the
        primeid variable. Then we set the primeid variable to the thread's ID. Then,
        we signal the condition variable (ready) and release the mutex lock. This will wake
        up the main() function since it's now waiting with pthread_cond_wait() */
    pthread_mutex_lock(&lock);
    /* Each thread gets its own thread ID using pthread_self()  */
    primeid = pthread_self();
    pthread_cond_signal(&ready);
    pthread_mutex_unlock(&lock);
    if (prime == 1)
        return (void *)1;
    else
        return (void *)0;
}

void *progress(void *arg)
{
    while (1)
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    return NULL;
}