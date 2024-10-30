/*Código responsável por criar uma shell para que o usuário consiga executar comandos. 
Para um comando que não é executado em segundo plano, foi utilizado o wait() para esperar
o retorno dele. Para um comando que é executado em segundo plano, o usuário pode executar
outros comandos sem nenhum problema.*/
//Autores: Ingrid Reupke Sbeguen Moran RA:2349388
//Data de criação:23.03.2023
//Data de atualização:07.09.2023

#include <stdio.h> // fgets stdin
#include <stdlib.h> // exit
#include <unistd.h> // fork execvp 
#include <sys/wait.h> // wait
#include <sys/types.h> // pid_t
#include <string.h> // strcmp strtok

int main(){

    // declarações para execvp
    char buff[200];
    char* argumentolist[200];

    // declarações extras
    pid_t pid; // pid do processo atual
    int status;
    int backgroud=0; // variável de controle 

    while(1){

        fgets(buff,200,stdin);// lê o comando de entrada e grava na string cmd

        buff[strcspn(buff, "\n")] = '\0'; // ao ler um \n na string cmd, coloca um 0 no seu lugar

        char *arg = strtok(buff, " "); // copia o comando para a lista de comandos
        int i=0;// iterador 
        argumentolist[i]=arg; // adiciona o primeiro argumento na pos 0
        i++; // incrementa o i em uma unidade 
        while (arg!=NULL)
        {
            arg=strtok(NULL," ");// pega o próximo token
            if(arg!=NULL && strcmp(arg,"&")!=0){ //verifica se arg é diferente & e verifca se arg diferente de NULL 
                argumentolist[i++]=arg;// adiciona argumento lista de argumentos
            }else if(arg!=NULL && strcmp(arg,"&")==0){
                backgroud=1;// habilita a flag back
            }else if (strcmp("exit",argumentolist[0])==0)
            {
                return 0;
            }
                          
        }
        argumentolist[i]=NULL;// coloca NULL na ultima possição, para indicar o fim dos argumentos
        pid = fork(); // cria  um processo filhos para executar o comando passado
        if (backgroud==0)
        {
            wait(&status);
        }if(pid == 0){ // caso o pid seja 0, ou seja, de um filho, o comando é executado 
            execvp(argumentolist[0], argumentolist); // execução do comando
        }
        backgroud=0;// quando termina o processo comando habilita o backgroud para o normal
    }
    exit(0);
    
}

