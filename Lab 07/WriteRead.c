#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "resource_monitor.h"

typedef struct args_reader
{
    int id;
} args_reader;

typedef struct args_writer
{
    int id;
    int value;
} args_writer;

void printf_args_reader(args_reader* args,int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("id: %d\n", args->id);
    }
    printf("\n");
}
void printf_args_writer(args_writer* args, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("id: %d\n", args->id);
        printf("value: %d\n", args->value);
    }
}

void* producer(void* arg) {
    args_writer* i= (args_writer*)arg;
    put(i->value,i->id);
    printf("Produtor %d Produced: %d\n",i->id,i->value);
    pthread_exit(0);
}

void* consumer(void* arg) {
    args_reader* args= (args_reader*)arg;
    int i=take(args->id);
    printf("Consumer %d Consumed: %d\n",args->id,i);
    pthread_exit(0);
}

int main() {
    int qtd;
    int max_rand;

    printf("Entre com a quantidade de produtores e consumidores: ");
    scanf("%d", &qtd);
    printf("Entre com o valor maximo para o random: ");
    scanf("%d", &max_rand);

    
    initMonitor(qtd);

    pthread_t consumers[qtd];
    pthread_t producers[qtd];

    args_reader args_consumers[qtd];
    args_writer args_producers[qtd];

    for (size_t i = 0; i < qtd; i++)
    {
        args_consumers[i].id = i;
    }

    for (size_t i = 0; i < qtd; i++)
    {
        args_producers[i].id = i;
        args_producers[i].value =rand() % max_rand;
    }


    for (size_t i = 0; i < qtd; i++)
    {
        pthread_create(&producers[i], NULL, producer, (void*)&args_producers[i]);
    }    
    for (size_t i = 0; i < qtd; i++)
    {
        pthread_create(&consumers[i], NULL, consumer, (void*)&args_consumers[i]);
    }

    pthread_exit(&producers);
    pthread_exit(&consumers);
    destroyMonitor();
   
    return 0;
}