/* A mutex is a locking mechanism that prevents access to a shared variable so that no
more than one thread can access it simultaneously. This prevents race conditions. With
a mutex, we only lock the critical part of the code, for example, the updating of a shared
variable. This will make sure that all other parts of the program run in parallel (if this is
possible with the locking mechanism) */

#include <stdio.h>
#include <pthread.h>
void *add(void *arg);
long long int i = 0;

/* we create a new variable called mutex of the pthread_mutex_t type. This is the variable that is
used for locking. We place this variable in the global area so that it can be reached
from both main() and add() */
pthread_mutex_t i_mutex;

int main(void)
{
    pthread_attr_t threadattr;
    pthread_attr_init(&threadattr);
    pthread_t tid_add1, tid_add2, tid_add3,
        tid_add4, tid_add5;

    /* the initialization of the
        mutex variable, using pthread_mutex_init(). NULL as the second argument
        means that we want the default attributes for the mutex: */
    if ((pthread_mutex_init(&i_mutex, NULL)) != 0)
    {
        fprintf(stderr, "Couldn't initialize mutex\n");
        return 1;
    }
    pthread_create(&tid_add1, &threadattr, add, NULL);
    pthread_create(&tid_add2, &threadattr, add, NULL);
    pthread_create(&tid_add3, &threadattr, add, NULL);
    pthread_create(&tid_add4, &threadattr, add, NULL);
    pthread_create(&tid_add5, &threadattr, add, NULL);
    pthread_join(tid_add1, NULL);
    pthread_join(tid_add2, NULL);
    pthread_join(tid_add3, NULL);
    pthread_join(tid_add4, NULL);
    pthread_join(tid_add5, NULL);

    printf("Sum is %lld\n", i);
    /* After we are done with the calculations, we destroy the mutex variable with
        pthread_mutex_destroy(): */
    if ((pthread_mutex_destroy(&i_mutex)) != 0)
    {
        fprintf(stderr, "Couldn't destroy mutex\n");
        return 1;
    }
    return 0;
}

void *add(void *arg)
{
    for (long long int j = 1; j <= 1000000000; j++)
    {
        /* we use the locking and unlocking mechanisms in the add() function.
            We lock the part where the i variable is updated and unlock it once the update is
            complete. That way, the variable is locked while the update is in progress so that no
            other threads can access it until the update is complete */
        pthread_mutex_lock(&i_mutex);
        i = i + 1;
        pthread_mutex_unlock(&i_mutex);
    }
    return NULL;
}