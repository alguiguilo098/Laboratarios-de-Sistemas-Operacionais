// Autores: Ingrid Reupke Sbeguen Moran RA:2349388, Guilherme Almeida Lopes RA:2458802,Caio rangel ferreira rodrigues RA:2252716
//  Data de criação: 01.09.2023
//  Data de atualização: 01.09.2023
/*programa que receba um vetor e distribua o processamento em partes iguais para N filhos,
cada um encarregado de procurar um item específico.
O programa deve exibir o PID dos filhos que encontrarem o valor procurado.*/

#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <sys/wait.h>  //wait
#include <sys/types.h> // pid_t  tipo
#include <unistd.h>    // fork()
#include<string.h>     // strtork,atoi,strcspn

int busca(int* vetorinicial,int *vetorfinal, int elemento)
{
    // elemento-> elemento que buscaremos no vetor
    // vetor-> endereço do vetor
    // inicio-> posição inicial
    // fim-> posicao final
    for (int*p=vetorinicial; p!=vetorfinal; p++)
    {
        if (*p== elemento)
        {
            // retorna 1, quando encotra o elemento no intervalo
            return 1;
        }
    }
    // retorna 0, quando o elemento não é encontrado
    return 0;
}

void _questao03(int inicio,int fim, int qtd_filhos, int elemento, int *vetor)
{
    // inicio-> posição inicial vetor
    // fim-> posição final do vetor
    // qtd_filhos-> quantidade de processos filhos vamos criar
    // vetor->endereço do vetor
    int qtd_elementos_por_processo = (fim + 1) / qtd_filhos; // quantidade de elemento que cada processo irá buscar
    for (int i = 0; i < qtd_filhos; i++)
    {
        // loop executado para quantidade de processos filhos
        pid_t pid = fork(); // criar o processo filho
        if (pid < 0)
        {
            // imprime mensagem de erro quando não consegue criar processo
            perror("Imposssível criar processo");
        }
        if (pid == 0)
        {
            // executa a busca do elemento
            int resultado = busca(&vetor[inicio],&vetor[inicio+qtd_elementos_por_processo], elemento); // chama a função busca para achar o elemento
            if (resultado==1)
            {
                // imprime o numero do PID que encontrou o elemento
                printf("O elemento foi encontardo pelo processo de PID:%d\n", getpid());
            }
            exit(0);
        }else{
            inicio+=qtd_elementos_por_processo;// modifica a posição inicial do vetor
        }
        
    }
    char pid[200];// armazena o comando pstree -c -p 
    sprintf(&pid[0], "pstree -c -p %d", getpid()); // copia o comando 
    system(pid); // excuta no terminal
    wait(NULL); // espera o fim dos prcessos filhos para executar o exit
    exit(0);    // sai do processo
}

void questao3(int *vetor, int qtd_filhos, int elemento, int tamanho)
{
    // função que serve uma uma casaca para processo real
    _questao03(0, tamanho - 1, qtd_filhos, elemento, vetor);
}

int main(int argc, char const *argv[])
{
    int tamanhodovetor;                // quantidade de elementos do vetor
    printf("Digite o tamanho do vetor: ");
    scanf("%d",&tamanhodovetor); // ex:20
    
    int quantidades_filhos;                // quantidade de processos filho
    printf("Digite Quantas partes deseja dividir o vetor: ");
    scanf("%d",&quantidades_filhos); // ex:4

    char vetor[tamanhodovetor]; // vetor de elementos 
    printf("Digite o vetor: ");
    scanf("%s",vetor); // ex: 100,200,56,300,899
    vetor[tamanhodovetor-1]=',';// adiciona um virgula no final do vetor

    int vetortcast[tamanhodovetor]; // vetor de inteiros
    char* temp=strtok(vetor,",");
    int tempint=atoi(temp);
    int i=0;
    while (temp!=NULL){
         vetortcast[i]=tempint;
         temp=strtok(NULL,",");
         if (temp!=NULL) tempint=atoi(temp);
         i++;
    }
    int elemento;    // elemento
    printf("Digite o elemento da busca: ");
    scanf("%d",&elemento);
    
    questao3(vetortcast, quantidades_filhos, elemento, tamanhodovetor); // chamada da função

    return 0;
}
