//Autores: Ingrid Reupke Sbeguen Moran RA:2349388,Guilherme Almeida Lopes RA:2458802,Caio rangel ferreira rodrigues RA:2252716
// Data de criação: 05.09.2023
// Data de atualização: 05.09.2023

/*programa que aceite um comando Linux como entrada e o execute como um processo filho do programa. 
O processo pai deve aguardar a conclusão da execução do comando antes de terminar.*/
#include<stdlib.h> // exit
#include<sys/wait.h> //wait
#include<sys/types.h> // pid_t  tipo 
#include<unistd.h> // fork, execvp
#include <stdio.h> // printf
#include<string.h> // strtork
void questao2(const char *comando, char*const* argv,int*status){
    /*função que cria um processo filhos, faz o pai esperar
    o processo filho terminar para executar
    o  suas instruções*/
    // comando-> parâmetro que vai receber a ser executado
    // argv->recebe o comando novamente, e seu parâmetros a serem executado e  NULL, como ultimo elemento
    // status->um ponteiro que armazena se a operação foi bem sucedida, vai retorna um valor positivo, se retorna negativo
    //a operação não foi bem sucedida
    printf("Criar Processo Filho\n");
    pid_t child=fork(); // cria o processo filho, espera a sua morte para continuar o processo pai
    if (child<0){// retorna uma execessão quando o processo não cosegue ser criado
      perror("Impossível criar processo");
    }else if (child==0){
        // código executado em processo filho
        *(status)=execvp(comando,argv);// substitui o código do processo por executável
    }else{
        //comando que o processo pai vai executar
        printf("Aguardando...processo filho\n");
        wait(status);// Aguarda o fim do processo filho
        printf("Executando processo pai\n");
    }
    exit(0);// finaliza o processo
}

int main(int argc, char const *argv[])
{
  char comando[200];
  char* argumentos[200];
  fgets(comando,200,stdin);
  comando[strcspn(comando, "\n")] = '\0';
  char* args=strtok(comando," ");// recebe o comando ls, o parâmetros -l, NULL
  int j=0;
  argumentos[j]=args;
  j++;
  while (args!=NULL)
  {
    args=strtok(NULL," ");
    argumentos[j]=args;
    j++;
  }
  int status;
  questao2(argumentos[0],argumentos,&status);// chamada da função que implementa a questão 2
  return 0;// retorno para para oprograma
}