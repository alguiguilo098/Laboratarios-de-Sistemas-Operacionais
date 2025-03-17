#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include <stdbool.h>

typedef struct args_threads{
    int id;
}args_threads;

bool issleep;
bool busy;
int qtd_cut=0;
int qtd_cadeiras;
int qtd_clientes;

sem_t cadeiras;
sem_t cadeira_barbeiro;
pthread_mutex_t mutex;
pthread_mutex_t mutex_cut;
void init_sem(){
    sem_init(&cadeiras,0,0);
    sem_init(&cadeira_barbeiro,0,0);
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex_cut,NULL);
}

void* barbeiro(void* arg){
    while (qtd_cut!=qtd_cadeiras){
        int qtd;
        sem_getvalue(&cadeiras,&qtd);

        if (qtd==0){
            printf("O Barbeiro esta dormindo...\n");
            pthread_mutex_lock(&mutex);
            issleep=true;
            pthread_mutex_unlock(&mutex);
            sem_wait(&cadeira_barbeiro);
        }
        sleep(rand()%10+1);

        printf("O Barbeiro corta o cabelo do cliente...\n");
        
        pthread_mutex_lock(&mutex);
        busy=true;
        pthread_mutex_unlock(&mutex);


        printf("O Barbeiro  chama o proximo cliente...\n");
        sem_post(&cadeiras);

        pthread_mutex_lock(&mutex);
        busy=false;
        pthread_mutex_unlock(&mutex);
    }
    
}

void* client(void* arg){
    args_threads* args=(args_threads*)arg;
    int qtd;
    sem_getvalue(&cadeiras,&qtd);

    if (qtd==qtd_cadeiras){
        printf("A Barbearia está cheia...\n");
        printf("O clinte %d foi embora...\n",args->id);
        pthread_mutex_lock(&mutex_cut);
        qtd_cut++;
        pthread_mutex_unlock(&mutex_cut);
        pthread_exit(NULL);
    }

    printf("O cliente %d chega na babearia...\n",args->id);

    if (issleep){
        printf("O cliente  %d acorda o barbeiro ...\n",args->id);
        sem_post(&cadeira_barbeiro);

        pthread_mutex_lock(&mutex);
        issleep=false;
        pthread_mutex_unlock(&mutex);
    }
    
    if (!busy){
        printf("O cliente %d senta na cadeira ... a espera do barbeiro\n",args->id);
        sem_wait(&cadeiras);
    }

    printf("O cliente %d sai da barbearia ...\n",args->id);


    pthread_mutex_lock(&mutex_cut);
    qtd_cut++;
    pthread_mutex_unlock(&mutex_cut);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
    

    init_sem();

    printf("Digite a quantidade de cadeiras: ");
    scanf("%d", &qtd_cadeiras);
    printf("Digite a quantidade de clientes: ");
    scanf("%d", &qtd_clientes);

    pthread_t barbeiros;
    pthread_t clientes_thread[qtd_clientes];
    args_threads args[qtd_clientes];

    for (size_t i = 0; i < qtd_clientes; i++){
        args[i].id=i;
    }
    
    pthread_create(&barbeiros,NULL,barbeiro,NULL);

    for (int i = 0; i < qtd_clientes; i++){
        pthread_create(&clientes_thread[i],NULL,client,(void*)&args[i]);
    }
    
    pthread_exit(&barbeiros);
    pthread_exit(&clientes_thread);

    return 0;
}
