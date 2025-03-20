#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "matriz.h"

pthread_mutex_t mutex_soma;
int soma_matriz = 0;

typedef struct thread_args {
    int **matrix;
    int* begin_row;
    int* end_row;
    int col;
    int thread_id;
}thread_args;

void * sum(void *arg) {
    thread_args *args = (thread_args *) arg;;
    int sum=0;
   for (int* i = args->begin_row; i < args->end_row; i++)
   {
         for (size_t j = 0; j < args->col; j++)sum+=args->matrix[*i][j];
   }
   
   
    pthread_mutex_lock(&mutex_soma);
    soma_matriz += sum;
    pthread_mutex_unlock(&mutex_soma);
    pthread_exit(NULL);
}
void pritn_shuffle_array(int *vector_begin, int *vector_end){
    for (size_t i = 0; i < vector_end-vector_begin; i++)
    {
        printf("%d ", vector_begin[i]);
    }
    printf("\n");
}

void shuffle(int* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}
int * create_vector(int size) {
    int *vector = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        vector[i] = i;
    }
    shuffle(vector, size);
    return vector;
}
int* vector_row(int qtd_threads, int linhas) {
    if (qtd_threads > linhas)
    {
        perror("Quantidade de threads maior que a quantidade de linhas");
        exit(EXIT_FAILURE);
    }
    
    int *vector = (int *)malloc( qtd_threads* sizeof(int));
    for (size_t i = 0; i < qtd_threads; i++) vector[i] = 1;
    linhas -= qtd_threads;
    for (size_t i = 0; i < qtd_threads; i++){
        if (linhas == 0)
        {
            break;
        }
        
        int rands = rand() % linhas+1;
        vector[i] += rands;
        linhas -= rands;
    }

    return vector;
}
void print_args(thread_args *args, int size) {
    for (int i = 0; i < size; i++) {
        printf("Thread %d:", args[i].thread_id);
        pritn_shuffle_array(args[i].begin_row, args[i].end_row);
    }
}
int main() {
    int linhas, colunas, qtd_threads,max_value_element;
    printf("Quantidade de linhas: ");
    scanf("%d", &linhas);
    printf("Quantidade de colunas: ");
    scanf("%d", &colunas);
    printf("Quantidade de threads: ");
    scanf("%d", &qtd_threads);
    printf("Valor máximo do elemento: ");   
    scanf("%d", &max_value_element);
    pthread_t threads[qtd_threads];
    pthread_mutex_init(&mutex_soma, NULL);
    thread_args args[qtd_threads];


    int **matriz = create_matrix(linhas, colunas);
    generate_elements(matriz, linhas, colunas, max_value_element);

    int * vetor_row_shuffle = create_vector(linhas);
    int * div = vector_row(qtd_threads, linhas);
    
    print_vector(vetor_row_shuffle, linhas);
    print_matrix(matriz, linhas, colunas);
    int start= 0;
    for (size_t i = 0; i < qtd_threads; i++)
    {
        args[i].matrix = matriz;
        args[i].begin_row =vetor_row_shuffle;
        args[i].end_row = vetor_row_shuffle+div[i];
        vetor_row_shuffle+=div[i];
        args[i].col = colunas;
        args[i].thread_id = i;

    }
    for (size_t i = 0; i < qtd_threads; i++)
    {
        pthread_create(&threads[i], NULL, sum, &args[i]); 
    }
    for (size_t i = 0; i < qtd_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }   

    printf("Soma: %d\n", soma_matriz);
  
    return 0;
}