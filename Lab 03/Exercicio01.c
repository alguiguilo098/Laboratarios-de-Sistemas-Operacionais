#include"matriz.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<math.h>

//Autores: Guilherme Almeida Lopes RA:2458802
// Data de criação: 11.09.2023
// Data de atualização: 11.09.2023
/*Desenvolva um programa que utilize N threads para buscar um valor específico em um vetor de inteiros. 
O vetor deve ser dividido entre as N threads para realizar a busca de forma paralela.*/

typedef struct  chunck_vetor{
    int* vetorinicial;//Endereco de inicial do vetor
    int* vetorfinal;//Endereco final do vetor 
    int elemento;// elemento que queremos procurar
    int id_thread;// Numero da thread
}chunck_vetor;

void convert_to_int_array(char* vetor, int* vetortcast, int tamanhodovetor) {
    char* temp = strtok(vetor, ",");
    int tempint = atoi(temp);
    int i = 0;
    while (temp != NULL) {
        vetortcast[i] = tempint;
        temp = strtok(NULL, ",");
        if (temp != NULL) tempint = atoi(temp);
        i++;
    }
}

int _search(int* vetorinicial,int *vetorfinal, int elemento)
{
    int count = 0; // contador
    // elemento-> elemento que buscaremos no vetor
    // vetor-> endereço do vetor
    // inicio-> posição inicial
    // fim-> posicao final
    for (int*p=vetorinicial; p!=vetorfinal; p++)
    {
        if (*p== elemento)
        {
            // retorna 1, quando encotra o elemento no intervalo
            count++;
        }
    }
    // retorna 0, quando o elemento não é encontrado
    return count;
}

void print_chunck_vetor(chunck_vetor* chunck){
    printf("Thread: %d\n",chunck->id_thread);
    printf("Vetor Inicial: %d\n",*chunck->vetorinicial);
    printf("Vetor Final: %d\n",*chunck->vetorfinal);
    printf("\n");

}

void* search(void* chunck){
    chunck_vetor* chunck_thread = (chunck_vetor*)chunck;
    int resultado = _search(chunck_thread->vetorinicial,chunck_thread->vetorfinal, chunck_thread->elemento); // chama a função busca para achar o elemento
    if (resultado>0){
        printf("O elemento foi encontardo pela thread: %d um total de %d \n", chunck_thread->id_thread,resultado);
    }else if (resultado==0){
        /* code */
        printf("O elemento foi encontrado pela thread: %d \n", chunck_thread->id_thread);
    }
    

}
int main(int argc, char const *argv[])
{
    int tamanhodovetor;                // quantidade de elementos do vetor
    printf("Digite o tamanho do vetor: ");
    scanf("%d",&tamanhodovetor); // ex:20
    
    int quantidades_threads;                // quantidade de processos filho
    printf("Digite Quantas partes deseja dividir o vetor: ");
    scanf("%d",&quantidades_threads); // ex:4

    char vetor[tamanhodovetor]; // vetor de elementos 
    printf("Digite o vetor: ");
    scanf("%s",vetor); // ex: 100,200,56,300,899
    vetor[tamanhodovetor-1]=',';// adiciona um virgula no final do vetor

    int elemento;    // elemento
    printf("Digite o elemento da busca: ");
    scanf("%d",&elemento);
    int vetorcast[tamanhodovetor]; // vetor de elementos convertidos para int

    convert_to_int_array(vetor, vetorcast, tamanhodovetor); // chamada da função
    
    chunck_vetor vetor_thread[quantidades_threads];
    pthread_t thread[quantidades_threads];
    int tamanhochunck = (tamanhodovetor/quantidades_threads);
    for (size_t i = 0; i < quantidades_threads; i++){
        vetor_thread[i].id_thread=i;    
        vetor_thread[i].vetorinicial=&vetorcast[i*tamanhochunck];
        vetor_thread[i].vetorfinal=&vetorcast[(i+1)*tamanhochunck-1];
        vetor_thread[i].elemento=elemento;
    }
    for (size_t i = 0; i < quantidades_threads; i++){
        pthread_create(&thread[i],NULL,search,(void*)&vetor_thread[i]);
    }

    pthread_exit(&thread);

    return 0;
}
