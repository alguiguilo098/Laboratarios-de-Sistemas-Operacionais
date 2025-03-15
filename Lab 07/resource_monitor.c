#include "resource_monitor.h"
#include <stdlib.h>
#include <stdio.h>
/* --- concurrency controls --- */
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;

/* --- resource --- */

int* buffer=NULL;
int count_write = 0;
int is_writing = 0;
int empty;   // 1 true, 0 false

void printfBuffer()
{
    for (size_t i = 0; i < count_write; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

void initMonitor(int size)
{
    // Initialize the mutex and condition variables
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condc, NULL); /* Initialize consumer condition variable */
    pthread_cond_init(&condp, NULL); /* Initialize producer condition variable */

    buffer = (int*)malloc(size * sizeof(int));
    empty = 1;
}


void put(int value)
{
    pthread_mutex_lock(&the_mutex); /* protect buffer */
    while (!empty)             /* If there is something  in the buffer then wait */
        pthread_cond_wait(&condp, &the_mutex);

    is_writing = 1;
    buffer[count_write++] = value;
    empty = 0;
    is_writing = 0;

    pthread_cond_signal(&condc);      /* wake up consumer */
    pthread_mutex_unlock(&the_mutex); /* release the buffer */
}

int take(int pos)
{
    while (empty|| is_writing)                   /* If there is nothing in  the buffer then wait */
        pthread_cond_wait(&condc, &the_mutex);
    int value = buffer[pos];
    empty = 1;
    pthread_cond_signal(&condp);      /* wake up producer */
    return value;
}

void destroyMonitor()
{
    // Cleanup -- would happen automatically at end of program
    free(buffer);
    pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
    pthread_cond_destroy(&condc);      /* Free up consumer condition variable */
    pthread_cond_destroy(&condp);      /* Free up producer condition variable */
}
