#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz.h"
#include <pthread.h>
#include <unistd.h>
#include <math.h>

struct chunck_media{
    int id_thread;
    double *media;     
};

double media_aritmetica(int* vetor, int size){
    double media = 0;
    for (int i = 0; i < size; i++){
        media += vetor[i];
    }
    return media/size;
}   

double media_geometrica(int* vetor, int size){
    double media = 1;
    for (int i = 0; i < size; i++){
        media *= vetor[i];
    }
    return pow(media,1.0/size);
}

double * set_invalid_array(int size){
    double *array = (double*)malloc(size*sizeof(double));
    for (int i = 0; i < size; i++){
        array[i] = -1;
    }
    return array;
}

int main() {
    int row, col,threads_row,threads_col,limitrandmax;
    int** matriz;
    printf("Digite o numero de linhas e colunas da matriz: \n");
    scanf("%d %d",&row,&col);

    printf("Digite o numero de threads para linhas: \n");
    scanf("%d",&threads_row);

    printf("Digite o numero de threads para linhas: \n");
    scanf("%d",&threads_col);

    printf("Digite o nome do arquivo da matrix: \n");
    char nome_arquivo[100];
    scanf("%s",nome_arquivo);

    if (strcmp(nome_arquivo,"matriz_6por8.in")!=0){
        printf("Digite o limite maximo para os elementos da matriz: \n");
        scanf("%d",&limitrandmax);

        matriz = create_matrix(row,col);
        generate_elements(matriz,row,col,limitrandmax);

        write_matriz_in_file(matriz,row,col,nome_arquivo);

    }else{
        strcpy(nome_arquivo,"matriz_6por8.in");
        matriz = read_matrix_from_file(nome_arquivo,&row,&col);
    }

    print_matrix(matriz,row,col);

    pthread_t thread[threads_col+threads_row];
    struct chunck_media chunck[threads_col+threads_row];

    for (size_t i = 0; i < threads_col+threads_row; i++)
    {
        chunck[i].id_thread = i;
        chunck[i].media = set_invalid_array(row);
    }

    int begin_thread_row = 0;
    int end_thread_row = threads_row-1;
    int begin_thread_col = threads_row;
    int end_thread_col = threads_row+threads_row-1;


    return 0;
}