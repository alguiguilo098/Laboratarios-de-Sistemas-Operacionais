#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz.h"
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

typedef struct  chunck_media{
    int id_thread;
    int** matrix_begin;
    int** matrix_end;
    float *media;
    int col;
    int row;
    int qtd_medias;     
}chunck_media;

float media_aritmetica(int* vetor, int size){
    float media = 0;
    for (int i = 0; i < size; i++){
        media += vetor[i];
    }
    return media/size;
}   

double media_geometrica(int* vetor, int size) {
    if (size <= 0) return -1;

    double soma_log = 0.0;
    for (int i = 0; i < size; i++) {
        if (vetor[i] <= 0) return -1;
        soma_log += log(vetor[i]);
    }

    return exp(soma_log / size);
}

float* set_invalid_array(int size){
    float *array = (float*)malloc(size*sizeof(float));
    for (int i = 0; i < size; i++){
        array[i] = -1;
    }
    return array;
}

void* threads_media(void* arg){
    chunck_media *chunck = (chunck_media*)arg;
    int j=0;
    for (int** i =chunck->matrix_begin; i!=chunck->matrix_end; i++){
        chunck->media[j] = media_aritmetica(*i,chunck->row);
        chunck->qtd_medias++;
        j++;
    }
    pthread_exit(NULL);
}
void transpor_matriz(int **matriz, int **transposta, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            transposta[j][i] = matriz[i][j];
        }
    }
}

void* threads_geometrica_media(void* arg){
    chunck_media *chunck = (chunck_media*)arg;
    int j=0;
    for (int** i =chunck->matrix_begin; i!=chunck->matrix_end; i++){
        chunck->media[j] = media_geometrica(*i,chunck->col);
        chunck->qtd_medias++;
        j++;
    }
    pthread_exit(NULL);
}

void printf_chunck(chunck_media chunck){
    printf("Thread: %d\n",chunck.id_thread);
    printf("Qtd medias: %d\n",chunck.qtd_medias);
    for (int i = 0; i < chunck.qtd_medias; i++){
        printf("Media %d: %.2f\n",i,chunck.media[i]);
    }
    
}
void write_media_in_file(chunck_media* chuncks, int num_threads, const char* filename,bool iscol,const char* tipo) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < chuncks[i].qtd_medias; j++) {
            if (iscol)
            {
                fprintf(file, "%s %d: Media da coluna %d é %.2f\n", tipo, i, j, chuncks[i].media[j]);
            }else{
                fprintf(file, "%s %d: Media da linha %d é %.2f\n", tipo, i, j, chuncks[i].media[j]);
            }
            
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
int main() {
    int row, col,threads_row,threads_col,limitrandmax;
    int** matriz;
    printf("Digite o numero de linhas e colunas da matriz: \n");
    scanf("%d %d",&row,&col);

    printf("Digite o numero de threads para linhas: \n");
    scanf("%d",&threads_row);

    printf("Digite o numero de threads para colunas: \n");
    scanf("%d",&threads_col);

    printf("Digite o nome do arquivo da matrix: \n");
    char nome_arquivo[100];
    scanf("%s",nome_arquivo);
    strcat(nome_arquivo,".in");

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

    pthread_t thread_media[threads_row];
    chunck_media chunck[threads_row];

    int row_thread = row/threads_row;

    for (size_t i = 0; i < threads_row; i++)
    {
        chunck[i].id_thread = i;
        chunck[i].matrix_begin = &matriz[i*row_thread];
        chunck[i].matrix_end = &matriz[(i+1)*row_thread];
        chunck[i].media = set_invalid_array(row_thread);
        chunck[i].qtd_medias = 0;
        chunck[i].col = col;
        chunck[i].row = row;
    }

    for (size_t i = 0; i < threads_row; i++)
    {
        pthread_create(&thread_media[i],NULL,threads_media,(void*)&chunck[i]);
    } 
    
    
    for (size_t i = 0; i < threads_row; i++)
    {
        pthread_join(thread_media[i],NULL);
    }
    
    int** transposta = create_matrix(col,row);
    transpor_matriz(matriz,transposta,row,col);

    pthread_t thread_geo_media[threads_col];
    chunck_media chunck_col[threads_col];

    int col_thread = col/threads_col;

    for (size_t i = 0; i < threads_col; i++)
    {
        chunck_col[i].id_thread = i;
        chunck_col[i].matrix_begin = &transposta[i*col_thread];
        chunck_col[i].matrix_end = &transposta[(i+1)*col_thread];
        chunck_col[i].media = set_invalid_array(col_thread);
        chunck_col[i].qtd_medias = 0;
        chunck_col[i].col = col;
        chunck_col[i].row = row;
    }

    for (size_t i = 0; i < threads_col; i++)
    {
        pthread_create(&thread_geo_media[i],NULL,threads_geometrica_media,(void*)&chunck_col[i]);
    }

    for (size_t i = 0; i < threads_col; i++)
    {
        pthread_join(thread_geo_media[i],NULL);
    }
    

    printf("A matriz de gerada esta no arquivo %s \n",nome_arquivo);
    
    nome_arquivo[strlen(nome_arquivo)-3] = '\0';
    strcat(nome_arquivo,".out");
    write_media_in_file(chunck,threads_row,nome_arquivo,true,"Media Aritmetica");
    write_media_in_file(chunck_col,threads_col,nome_arquivo,false,"Media Geometrica");
    printf("Os Resultados está no arquivo %s \n",nome_arquivo);
    
    
    return 0;
}