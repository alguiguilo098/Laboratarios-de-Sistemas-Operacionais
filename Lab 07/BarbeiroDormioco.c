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


int qtd_cut=0;
int qtd_cadeiras;
int qtd_clientes;

sem_t cadeiras_espera;
sem_t cadeiras_atendido;
sem_t cadeira_barbeiro;
pthread_mutex_t mutex;
pthread_mutex_t mutex_cut;
void init_sem(){
    sem_init(&cadeiras_espera,0,0);
    sem_init(&cadeira_barbeiro,0,0);
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex_cut,NULL);
}

void* barbeiro(void* arg){
    
    while (qtd_cut!=qtd_cadeiras){
        
        if (issleep){
            printf("O Barbeiro esta dormindo...\n");
        }

        sem_post(&cadeiras_espera);
        sem_wait(&cadeira_barbeiro);
        
        if (!issleep){
            printf("O Barbeiro chama o cliente para ser atendido ...\nO Barbeiro atende o cliente...\n");
        }

        sem_post(&cadeiras_atendido);
        sem_wait(&cadeira_barbeiro);

        pthread_mutex_lock(&mutex_cut);
        qtd_cut++;
        pthread_mutex_unlock(&mutex_cut);
    }
    printf("O Barbeiro volta a dormir ...\n");

}

void* client(void* arg){
    args_threads* args=(args_threads*)arg;
    sem_wait(&cadeiras_espera);
    printf("O cliente %d chega a barbearia...\n",args->id);
    if (issleep){
        printf("O cliente %d acorda o barbeiro...\n",args->id);
        pthread_mutex_lock(&mutex);
        issleep=false;
        pthread_mutex_unlock(&mutex);
        sem_post(&cadeira_barbeiro);
        sem_wait(&cadeiras_atendido);
    }else{
        printf("O cliente %d espera para ser atendido...\n",args->id);
        sem_post(&cadeira_barbeiro);
        sem_wait(&cadeiras_espera);
    }
    printf("O cabelo do cliente %d é cortado pelo barbeiro...\n",args->id);
    printf("O cliente %d sai da barbearia...\n",args->id);
    
    sem_post(&cadeiras_espera);
    sem_post(&cadeira_barbeiro);

    pthread_exit(NULL);

}

int main(int argc, char const *argv[]){
    

    init_sem();
    issleep=true;

    printf("Digite a quantidade de cadeiras: ");
    scanf("%d", &qtd_cadeiras);
    printf("Digite a quantidade de clientes: ");
    scanf("%d", &qtd_clientes);

    printf("\n");
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
