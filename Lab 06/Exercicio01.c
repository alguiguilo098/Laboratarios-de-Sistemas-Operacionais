#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
pthread_mutex_t mutex_ping;
pthread_mutex_t mutex_pong;
void *ping(void *arg)
{
    pthread_mutex_lock(&mutex_ping);
    printf("Ping... ");
    pthread_mutex_unlock(&mutex_pong);
    pthread_exit(NULL);
}
void *pong(void *arg)
{
    pthread_mutex_lock(&mutex_pong);
    printf("Pong\n");
    pthread_mutex_unlock(&mutex_ping);
    pthread_exit(NULL);
}

int main()
{
    pthread_mutex_init(&mutex_ping, NULL);
    pthread_mutex_init(&mutex_pong, NULL);
    int qtd_ping_pog;
    printf("Digite a quantidade de ping pong: ");
    scanf("%d", &qtd_ping_pog);
    pthread_t thread_ping[qtd_ping_pog];
    pthread_t thread_pong[qtd_ping_pog];
    for (int i = 0; i < qtd_ping_pog; i++)
    {
        pthread_create(&thread_ping[i], NULL, ping, NULL);
        pthread_create(&thread_pong[i], NULL, pong, NULL);
    }
    for (int i = 0; i < qtd_ping_pog; i++)
    {
        pthread_join(thread_ping[i], NULL);
        pthread_join(thread_pong[i], NULL);
    }
    return 0;
}