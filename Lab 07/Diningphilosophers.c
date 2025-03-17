#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include <semaphore.h>
#include "Diningphilosophers.h"

#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct args_threads{
    int id;
}args_threads;


int state[5];
sem_t sem[5];
pthread_mutex_t mutex;

void test(int i){
    int right=(i+1)%5;
    int left=(i-4)%5;
    if (state[i]==HUNGRY && state[left]!=EATING && state[right]!=EATING){
        state[i]=EATING;
        sem_post(&sem[i]);
    }
    
}

void put_forks(int i){
    printf("O Filosofo %d está cheio...\n",i);
    pthread_mutex_lock(&mutex);
    state[i]=HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex);
    sem_wait(&sem[i]);
}

void take_forks(int i){
    int right=(i+1)%5;
    int left=(i-4)%5;
    pthread_mutex_lock(&mutex);
    state[i]=THINKING;
    printf("O Filosofo %d pega o garfo direito...\n",i);
    test(right);
    printf("O Filosofo %d pega o garfo esquerdo...\n",i);
    test(left);
    pthread_mutex_unlock(&mutex);
}
void thinking(args_threads *args)
{
    printf("O Filosofo %d está pensando ...\n", args->id);
    sleep(rand() % 5 + 1);
}

void *philosophers(void *arg)
{
    args_threads* args=(args_threads*)arg;
    while (1){
        thinking(args);
        printf("O Filosofo %d esta com fome ...\n",args->id);
        take_forks(args->id);
        printf("O Filosofo %d está comendo...\n",args->id);
        put_forks(args->id);
    }
    


    
    pthread_exit(0);
}



void init_sem_mutex(){
    sem_init(&sem[0], 0, 0);
    sem_init(&sem[1], 0, 0);
    sem_init(&sem[2], 0, 0);
    sem_init(&sem[3], 0, 0);
    sem_init(&sem[4], 0, 0);
    pthread_mutex_init(&mutex, NULL);
}

void destroy_sem_mutex(){
    sem_destroy(&sem[0]);
    sem_destroy(&sem[1]);
    sem_destroy(&sem[2]);
    sem_destroy(&sem[3]);
    sem_destroy(&sem[4]);
}
void init_state(){
    for (size_t i = 0; i < 5; i++) state[i]=THINKING;
}
int main(int argc, char const *argv[]){
    pthread_t phi[5];
    args_threads args[5];

    init_sem_mutex();
    init_state();

    for (int i = 0; i < 5; i++) args[i].id=i;
    
    for (int i = 0; i < 5; i++){
        pthread_create(&phi[i],NULL,philosophers,(void*)&args[i]);
    }

    destroy_sem_mutex();
    pthread_exit(&phi);

    return 0;
}


