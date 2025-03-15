#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "resource_monitor.h"

typedef struct args_reader
{
    int id;
    int pos;
} args_reader;

typedef struct args_writer
{
    int id;
    int value;
} args_writer;


void* producer(void* arg) {
    args_writer* i= (args_writer*)arg;
    put(i->value);
    printf("Produtor %d Produced: %d\n",i->id,i->value);
    pthread_exit(0);
}

void* consumer(void* arg) {
    args_reader* args= (args_reader*)arg;
    int i=take(args->pos);
    printf("Consumer %d Consumed: %d in position %d\n",args->id,i,args->pos);
    pthread_exit(0);
}

int main() {
    int qtd_consumers;
    int qtd_producers;
    int max_rand;

    printf("Entre com a quantidade de produtores: ");
    scanf("%d", &qtd_producers);
    printf("Entre com a quantidade de consumidores: ");
    scanf("%d", &qtd_consumers);
    printf("Entre com o valor maximo para o random: ");
    scanf("%d", &max_rand);

    initMonitor(qtd_producers);

    pthread_t consumers[qtd_consumers];
    pthread_t producers[qtd_producers];
    args_reader args_consumers[qtd_producers];
    args_writer args_producers[qtd_consumers];

    for (size_t i = 0; i < qtd_consumers; i++)
    {
        args_consumers[i].id = i;
        args_consumers[i].pos = i%qtd_producers;
    }

    for (size_t i = 0; i < qtd_producers; i++)
    {
        args_producers[i].id = i;
        args_producers[i].value =rand() % max_rand;
    }

    for (size_t i = 0; i < qtd_producers; i++)
    {
        pthread_create(&producers[i], NULL, producer, (void*)&args_producers[i]);
    }    
    for (size_t i = 0; i < qtd_consumers; i++)
    {
        pthread_create(&consumers[i], NULL, consumer, (void*)&args_consumers[i]);
    }

    pthread_exit(&producers);
    pthread_exit(&consumers);
    destroyMonitor();
   
    return 0;
}