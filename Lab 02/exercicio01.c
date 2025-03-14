//Autores: Guilherme Almeida Lopes RA:2458802
// Data de criação: 01.09.2023
// Data de atualização: 01.09.2023

/*O programa que estamos criando irá gerar uma hierarquia de processos com N níveis, 
onde cada nível terá o dobro de processos do nível anterior.*/
   
#include <sys/types.h>//pid_t
#include<unistd.h>// fork
#include<stdio.h>// printf
#include<stdlib.h> //system

void _show_tree_process(int iterador, int hierarquia){
    // iterator-> variavel que indica quando a chamda recursiva vai parar
    //hierquia-> determina a altura da arvore de processos
    if (iterador==hierarquia-1){
        // caso base para parar a chamda recursiva
        return;
    }
    pid_t child_one=fork();// criar o processo filho 1
    if (child_one<0){
        // retorna uma mensagem de erro,se o processo não conseguir ser
        //criado
        perror("Impossível criar processo um");
    }if (child_one==0){
        _show_tree_process(iterador+1,hierarquia);// chamada recursiva do processo 2
        getchar();// pede um caracter no terminal
        exit(0);// encerra o processo filho
    }
    pid_t child_two=fork();// cria processo filhos 2
    if (child_two<0){
        // cria processo filho 1
        perror("Impossível criar processo dois");
     }else if (child_two==0){
        _show_tree_process(iterador+1,hierarquia);// chama recursivamete a função para o processo filho 2
        getchar();// espera a entrada de  caracter para continuar
        exit(0);// destroi o processo.
     }   
}
void tree_process(int hierarquia){
    // chamada da função  _questao, que implementa a função   
    _show_tree_process(0,hierarquia);
}
int main(int argc, char const *argv[])
{
    int hierarquia=atoi(argv[1]);
    char pid[200];
    sprintf(pid,"pstree -c -p %d",getpid());
    tree_process(hierarquia);

    system(pid);
    return 0;
}
