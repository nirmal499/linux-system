/* We saw that a threaded program isn't necessarily any faster than
a non-threaded program. Much of this slowness is due to switching back and forth and locking
and unlocking billions of times.
The solution to all of this locking and unlocking and switching back and forth is to lock
and unlock as few times as possible. And also, to update the i variable as few times as
possible and do as much work as possible in each thread */

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

/* we increment a local j variable in a while
loop until it reaches 1,000,000,000. Then, we add the local j variable to the global
i variable. This reduces the number of times we have to lock and unlock the mutex
(from 5,000,000,000 times to only 5 times) */
void *add(void *arg)
{
    long long int j = 1;
    while (j < 1000000000)
    {
        j = j + 1;
    }
    /* First, this program only
        locks and unlocks the mutex 5 times (compared to 5,000,000,000 times in the previous
        recipe). Secondly, each thread can now complete its work (the while loop) fully before
        writing anything to the global variable */
    pthread_mutex_lock(&i_mutex);
    i = i + j;
    pthread_mutex_unlock(&i_mutex);
    return NULL;
}

// void *add(void *arg)
// {
//     for (long long int j = 1; j <= 1000000000; j++)
//     {
//         /* we use the locking and unlocking mechanisms in the add() function.
//             We lock the part where the i variable is updated and unlock it once the update is
//             complete. That way, the variable is locked while the update is in progress so that no
//             other threads can access it until the update is complete */
//         pthread_mutex_lock(&i_mutex);
//         i = i + 1;
//         pthread_mutex_unlock(&i_mutex);
//     }
//     return NULL;
// }
