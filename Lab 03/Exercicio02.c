#include"matriz.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<math.h>
//Autores: Ingrid Reupke Sbeguen Moran RA:2349388,Guilherme Almeida Lopes RA:2458802,Caio rangel ferreira rodrigues RA:2252716
// Data de criação: 11.09.2023
// Data de atualização: 11.09.2023
/*Desenvolva um programa que utilize N threads para buscar um valor específico em um vetor de inteiros. 
O vetor deve ser dividido entre as N threads para realizar a busca de forma paralela.*/ 
typedef struct chuck_linha_matriz{
    double *media;// vetor media de elementos
    int** matriz;// matriz 
    int linha;//quantidad de linhas de uma matriz
    int colunas;// quantidades de colunas

}chuck_linha_matriz;

chuck_linha_matriz* criar_chunck_media_aritmetica(int qtd_elementos,int**matriz,double*vetor_media){
    // Estrtura será utilzada de parâmtros que será utilizada pela thread
    chuck_linha_matriz*m=(chuck_linha_matriz*)malloc(sizeof(chuck_linha_matriz));
    m->media=vetor_media;// media do vetor
    m->matriz=matriz;// matriz 
    m->linha=0;// iterador 0>linha>quantidade_de_linhas de uma matriz
    m->colunas=qtd_elementos;// quantidade de colunas de uma matriz
    return m;
};
chuck_linha_matriz*criar_chunck_geometrica(int linha,int **matriz,double*vetor_media){
    chuck_linha_matriz*m=(chuck_linha_matriz*)malloc(sizeof(chuck_linha_matriz));
    m->media=vetor_media;// vetor com a media geometrica
    m->matriz=matriz;//matriz
    m->colunas=0;// iterador 0>colunas>quantidade_de_colunas
    m->linha=linha;//quantidade de elementos de uma matriz de elementos 
    return m;
}
void* media_aritimetica(void* ptr){
    chuck_linha_matriz*m=ptr;
    double sum=0.0;// acumulador 
    for(int i = 0; i <m->colunas; i++){
        // soma todos os elementos da linhas da matriz
        sum+=m->matriz[m->linha][i];
    }
    // realiza a media da linha que a thread está responsável
    m->media[m->linha]=sum/m->colunas;
} 
void*media_geometrica_funcao(void* ptr){
    double prod=1.0;// produto 
    chuck_linha_matriz*m=ptr;
    for (int i = 0; i <m->linha;i++){
        //multiplica todos os elementos daquela coluna
        prod=prod*m->matriz[i][m->colunas];
    }// calcula a meida geometrica da coluna
    m->media[m->colunas]=pow(prod,1.0/m->linha);    
}
int main(int argc, char const *argv[]){
    int quantidades_linhas=6;//quantidade de linhas da matriz
    int quantidades_colunas=6;//quantidade de colunas da matriz
    double*media_vetor_aritimetica=malloc(quantidades_linhas*sizeof(double));
    int max_rand=20;//maior numero aleatório possivel
    int**matriz=create_matrix(quantidades_linhas,quantidades_colunas);// cria uma matriz
    generate_elements(matriz,quantidades_colunas,quantidades_linhas,max_rand);// gerar uma matriz aleatória 
    print_matrix(matriz,quantidades_linhas,quantidades_colunas);// imprime a matriz
    chuck_linha_matriz* argumentos=criar_chunck_media_aritmetica(quantidades_colunas,matriz,media_vetor_aritimetica);//cria os argumetos da thread
    pthread_t thread[quantidades_linhas+quantidades_colunas];
    for (int i=0;i<quantidades_linhas;i++){
        // cria a thread 
        int status=pthread_create(&thread[i],NULL,media_aritimetica,(void*)argumentos);
        if (status<0){
            // retorna um erro se não for possivel criar a thread
            perror("impossível criar Thread !!!");
        }
        argumentos->linha=i;// atualiza o iterador
        pthread_join(thread[i],NULL);// espera a thread terminar
    }
    for (int i = 0; i <argumentos->linha; i++){
        getchar();
        printf("media da linha %d:%f\n",i+1,media_vetor_aritimetica[i]);
    }
    printf("\n");
    double*media_geometrica=malloc(quantidades_colunas*sizeof(double));// aloca o vetor de medias geometricas
    chuck_linha_matriz*argumentos_dois=criar_chunck_geometrica(quantidades_linhas,matriz,media_geometrica);// cria os argumentos da thread
    for (int i = 0; i <quantidades_colunas; i++){
        // cria a thread para calcular a operacao geometrica
        int status=pthread_create(&thread[quantidades_linhas+i+1],NULL,media_geometrica_funcao,(void*)argumentos_dois);
        if (status<0){
            // retorna uma mesagem de erro se não criou a thread
            perror("impossível criar Thread !!!");
        }
        argumentos_dois->colunas=i;// atualiza o iterador da thread
        pthread_join(thread[quantidades_linhas+i+1],NULL);//espera a thread terminar
    }
    for (int i = 0; i <argumentos->colunas; i++){
        getchar();
        printf("media da coluna %d:%f\n",i+1,argumentos_dois->media[i]);
    }
    write_matriz_in_file(matriz,quantidades_colunas,quantidades_linhas,"resposta.txt");// escrita da matriz 
    //
    pthread_exit(&thread);

    return 0;
}