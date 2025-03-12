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
    int qtd_ping, qtd_pong;
    printf("Digite a quantidade de pong: ");
    scanf("%d", &qtd_pong);
    printf("Digite a quantidade de ping ping: ");
    if (qtd_pong != qtd_ping)
    {
        printf("A quantidade de ping e pong devem ser iguais\n");
        return 0;
    }
    else if ('qtd_ping' < 0 || 'qtd_pong' < 0)
    {
        printf("A quantidade de ping e pong devem ser maior que 0\n");
        return 0;
    }

    scanf("%d", &qtd_ping);
    pthread_t thread_ping[qtd_ping];
    pthread_t thread_pong[qtd_pong];
    for (int i = 0; i < qtd_ping; i++)
    {
        pthread_create(&thread_ping[i], NULL, ping, NULL);
    }
    for (size_t i = 0; i < qtd_pong; i++)
    {
        pthread_create(&thread_pong[i], NULL, pong, NULL);
    }
    pthread_exit(&thread_ping);
    pthread_exit(&thread_pong);

    return 0;
}