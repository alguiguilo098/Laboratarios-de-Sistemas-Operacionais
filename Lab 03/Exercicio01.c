#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<stdlib.h>
#define NUM_T INT_MAX/100000 
//Autores: Ingrid Reupke Sbeguen Moran RA:2349388,Guilherme Almeida Lopes RA:2458802,Caio rangel ferreira rodrigues RA:2252716
// Data de criação: 11.09.2023
// Data de atualização: 11.09.2023
/*Desenvolva um programa que utilize N threads para buscar um valor específico em um vetor de inteiros. 
O vetor deve ser dividido entre as N threads para realizar a busca de forma paralela.*/ 
typedef struct  chunck_vetor{
    int* vetor;//Endereco de inicial do vetor
    int inicio;//Posição inicial de onde a thread vai realizar a busca
    int fim;// Posicao final de onde a thread vai realizar a busca 
    int elemento;// elemento que queremos procurar
    int id_thread;// Numero da thread
}chunck_vetor;
int* criar_vetor(int tam,int rand_max){
    int*v=malloc(tam*sizeof(int));
    for (int i=0; i<tam;i++){
        v[i]=rand()%rand_max;
    }
    return v;
}
chunck_vetor*criar_vetor_chunck(int*vetor,int inicio,int fim,int elemento){
    // CRIAR ESTRUTURA 
    // POSSUI PARAMETROS QUE PASSAREMOS REALIZAR A BUSCA NA THREAD
    chunck_vetor* chunck=malloc(sizeof(chunck_vetor));// Alocar ponteiro da estrutura
    chunck->elemento=elemento;// Atribui o elemento que vamos buscar    
    chunck->fim=fim;// Atribui a posição final do vetor
    chunck->inicio=inicio;// Atribui a posição inicial do vetor
    chunck->vetor=vetor;//Atribui o endereço inicial do vetor que iremos realizar a busca 
    return chunck; // RETORNO DO PONTEIRO DA ESTRUTURA
}
void* busca(void* ptr){
    chunck_vetor*argumentos=(chunck_vetor*)ptr;// REALIZAR UMA CAST PARA PONTEIRO CHUNCK_VETOR
    for (size_t i =argumentos->inicio; i<argumentos->fim; i++){
        // Realizar um loop para buscar o vetor na região que a thread e responsável 
        if (argumentos->vetor[i]==argumentos->elemento){
            // compara se o valor que estamos atualmente é o elemento
            printf("O elemento foi encontrado pela thread %d\n",argumentos->id_thread);
            // imprime o numero da thread (NÃO É O ID THREAD), 0>numero>NUM_T
            printf("Na posicao: %d\n",i);
        }
    }
    printf("\n");
}
int main(int argc, char const *argv[]){
    pthread_t thread[NUM_T];
    int tam=1000000;
    int rand_max=200;
    int*vetor=criar_vetor(tam,rand_max);
    int elemento_busca=30;
    int qtd_elementos_vetor=(tam)/NUM_T;
    chunck_vetor* chunck=criar_vetor_chunck(vetor,0,(int)tam/NUM_T,elemento_busca);
    for (int i = 0; i < NUM_T; i++){
        chunck->id_thread=i;
        int status=pthread_create(&thread[i],NULL,busca,(void*)chunck);
        if (status<0){
            perror("Impossível criar THREAD");
        }
        pthread_join(thread[i],NULL);
        chunck->inicio=chunck->fim;
        chunck->fim+=qtd_elementos_vetor;
    }
    pthread_exit(&thread);    
    return 0;
}
