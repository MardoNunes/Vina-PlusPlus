#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "libInsert.h"
#include "libList.h"
#include "libVina.h"



void insert(char *archiver, int membros, char **argv ){

    FILE *arq;
    arq = fopen(archiver, "rb+");
    if(testArq(arq) == 1){
        arq = fopen(archiver, "wb+");
        if(testArq(arq) == 1){
            printf("Erro ao abrir o arquivo\n");
            exit(1);
        }
    }
    if(vazio(arq) == 1){
       
       //escreve cada membro no arquivo
       //e tamanho dele tbm
        int i = membros;
        while(argv[i] != NULL){
        //escreve no arquivo
        WriteFileSize(arq, argv[i]); 
        escreve(arq, argv[i]);     
        i++;
        }

        //escreve meta dados  
        //reseta o i
        i = membros;
        int cont = 1;    //conta a ordem de inserção dos membros
        //agora adiciono os meta dados no fim do archiver
        //move ponteiro para o final do arquivo
        fseek(arq, 0, SEEK_END);
        fwrite("\n", 1, 1, arq);
        while(argv[i] != NULL){
            //pega os metadados do arquivo e joga no final do archiver
            metadados(arq, argv[i], cont);
            i++;
            cont++;
        }
        fseek(arq, -1, SEEK_END);
        fwrite("#", 1, 1, arq);
    }
    else{
        fseek(arq, 0, SEEK_SET);
        
        short ok;
        int fim;
        char *nome = NULL;
        int comeco;

        int choice = foundFile(arq, argv[membros], &fim, &nome, &comeco);
        
        if(choice == 1){    //subsitui primeiro arquivo.
            
            //guardo os metadados em uma estrutura
            struct Lista *lista = (struct Lista*) malloc(sizeof(struct Lista));
            lista->cabeca = NULL;
            takeMetaData(arq, lista);

            int tam = blocoSize(arq, nome);
            
            //guardo o comeco da borda
            fseek(arq, 0, SEEK_END);
            int initEdge = ftell(arq);

            //pulo o arquivo que vou substituir
            fseek(arq, fim, SEEK_SET);

            //agora escreve os demais membros na borda
            int fimEdge;
            int soma = fim;
            int jump = takeJump(arq, &ok);
            soma = soma + jump;
            fseek(arq, fim, SEEK_SET);
            WriteEdge(arq, jump);
            while(ok != 1){
                fseek(arq, soma, SEEK_SET);
                jump = takeJump(arq, &ok);
                fseek(arq, soma, SEEK_SET);
                soma = soma + jump;
                WriteEdge(arq, jump);
            }
            fimEdge = ftell(arq);

            //agora escreve o novo membro
            comeco = 0;
            fseek(arq, comeco, SEEK_SET);
            WriteFileSize(arq, argv[membros]);
            escreve(arq, argv[membros]);

            //movo o conteudo da borda para o final do novo membro
            fseek(arq, initEdge, SEEK_SET);
            char *buffer = (char*) malloc(sizeof(char) * (fimEdge - initEdge));
            fread(buffer, sizeof(char), fimEdge - initEdge, arq);
            fseek(arq, tam, SEEK_SET);
            fwrite(buffer, sizeof(char), fimEdge - initEdge, arq);
            ftruncate(fileno(arq), ftell(arq)); //trunco o aarquivo pq tenho os metadados salvos
            free(buffer);

                
            //atualizo o tamanho do novo membro na lista
            updateSize(lista, nome);
               
            //escreve os metadados no arquivo
            fseek(arq, 0, SEEK_END);
            imprimi(lista, arq);
            fseek(arq, -1, SEEK_END);
            fwrite("#", 1, 1, arq);
                
            //libera a lista
            desenfilera(lista);
            free(lista);
            free(nome);

        }
        else if(choice == 2){   //quando não é o primero membro
        
            
            //guardo os metadados em uma estrutura
            struct Lista *lista = (struct Lista*) malloc(sizeof(struct Lista));
            lista->cabeca = NULL;
            takeMetaData(arq, lista);

            int tam = blocoSize(arq, nome);

            //guardo o comeco da borda
            fseek(arq, 0, SEEK_END);
            int initEdge = ftell(arq);

            //pulo o arquivo que vou substituir
            fseek(arq, fim, SEEK_SET);

            //agora escreve os demais membros na borda
            int fimEdge;
            int soma = fim;
            int jump = takeJump(arq, &ok);
            soma = soma + jump;
            fseek(arq, fim, SEEK_SET);
            WriteEdge(arq, jump);
            while(ok != 1){
                fseek(arq, soma, SEEK_SET);
                jump = takeJump(arq, &ok);
                fseek(arq, soma, SEEK_SET);
                soma = soma + jump;
                WriteEdge(arq, jump);
            }

            fimEdge = ftell(arq);

            //agora escreve o novo membro
            fseek(arq, comeco, SEEK_SET);
            WriteFileSize(arq, argv[membros]);
            escreve(arq, argv[membros]);

            //movo o conteudo da borda para o final do novo membro
            fseek(arq, initEdge, SEEK_SET);
            char *buffer = (char*) malloc(sizeof(char) * (fimEdge - initEdge));
            fread(buffer, sizeof(char), fimEdge - initEdge, arq);
            fseek(arq, comeco+tam , SEEK_SET);
            fwrite(buffer, sizeof(char), fimEdge - initEdge, arq);
            ftruncate(fileno(arq), ftell(arq)); //trunco o aarquivo pq tenho os metadados salvos
            free(buffer);

            //atualizo o tamanho do novo membro na lista
            updateSize(lista, nome);

            //escreve os metadados no arquivo
            fseek(arq, 0, SEEK_END);
            imprimi(lista, arq);
            fseek(arq, -1, SEEK_END);
            fwrite("#", 1, 1, arq);

            //desenfilera
            desenfilera(lista);
            free(lista);
            free(nome);
        }
        else{
            

            //vou guardar os metadados em uma estrutura
            struct Lista *lista = (struct Lista*) malloc(sizeof(struct Lista));
            lista->cabeca = NULL;

            //pega os metadados do arquivo
            takeMetaData(arq, lista);
            
            //adiciona o novo membro no fim do arquivo
            //vou pular todo o conteudo para garantir
            fseek(arq, 0, SEEK_SET);
            short ok = 0;
            int jump = takeJump(arq, &ok);
            while(ok != 1){
                fseek(arq, jump, SEEK_SET);
                jump = jump + takeJump(arq, &ok);
            }
            
            int atual = ftell(arq);
            fseek(arq, atual - 1, SEEK_SET);
            //escreve os novos membros no arquivo
            int i = membros;
            while(argv[i] != NULL){
                //escreve no arquivo
                WriteFileSize(arq, argv[i]); 
                escreve(arq, argv[i]);     
                i++;
            }
            ftruncate(fileno(arq), ftell(arq)); //trunco o aarquivo pq tenho os metadados salvos
            
            //escreve os metadados no arquivo
            fseek(arq, 0, SEEK_END);
            imprimi(lista, arq);
            
            // //agora adiciono os novos metadados
            // //reseta o i
            i = membros;
            int cont = lastOrder(lista) + 1;    //conta a ordem de inserção dos membros
            //agora adiciono os meta dados no fim do archiver
            //move ponteiro para o final do arquivo
            fseek(arq, 0, SEEK_END);
            while(argv[i] != NULL){
                //pega os metadados do arquivo e joga no final do archiver
                metadados(arq, argv[i], cont);
                i++;
                cont++;
            }
            
            fseek(arq, -1, SEEK_END);
            fwrite("#", 1, 1, arq);
            desenfilera(lista);
        }
    }

    fclose(arq);
}

//essa função escreve no arquivo
void escreve(FILE *arq, char *membro){
    FILE *aux;
    char buffer[1024];  //buffer para copiar o conteudo do membro
    char *nome;     //nome do membro
    int bytesLidos; //quantidade de bytes lidos
    
    
    //abre o membro a ser escrito
    aux = fopen(membro, "rb");
    testArq(aux);
    

    //copia o conteudo do membro para o arquivo
    while((bytesLidos = fread(buffer, sizeof(char), sizeof(buffer), aux)) > 0) {
        //escreve no arquivo bytesLidos é a quantidade de bytes lidos em aux
        fwrite(buffer, sizeof(char), bytesLidos, arq);
    }
   
    fclose(aux); 
}

int fileSize(char *membro){
    //pega o tamanho do arquivo
    struct stat st;
    stat(membro, &st);
    
    int size = st.st_size;
    return size;
}


//escreve o tamanho do bloco
void WriteFileSize(FILE *arq, char *membro){

    //pega o tamanho do nome do membro em bytes
    int size = strlen(membro) + 1; //+1 para o separador

    //vou pegar o tamanho do arquivo
    struct stat st;
    stat(membro, &st);
    int sizeArq = st.st_size;

    //agora somo os dois tamanhos
    size = size + sizeArq + 1; //+1 para o separador

    //agora, calculo o tamanho do size em bytes
    char *tam;
    asprintf(&tam, "%d", size);
    size = size + strlen(tam);

    //agora escrevo no arquivo
    char *tamanho;
    asprintf(&tamanho, "%d", size);
    fwrite(tamanho, sizeof(char), strlen(tamanho), arq);
    fwrite("@", sizeof(char), 1, arq);
    fwrite(membro, sizeof(char), strlen(membro), arq);
    fwrite("@", sizeof(char), 1, arq);
    free(tamanho);

}


int testArq(FILE *arq){
    if(arq == NULL)
        return 1;
    return 0;
}


int vazio(FILE *arq){
    //move o ponteiro para o final do arquivo
    fseek(arq, 0, SEEK_END);
    //pega a posição atual do ponteiro
    long int pos = ftell(arq);

    //se a posição for 0 o arquivo é vazio
    if(pos == 0)
        return 1;
    return 0;
}

void metadados(FILE *arq, char *membro, int cont){
    struct stat st;

    char *nome;
    const char *time; //vai recever a ultima modificação do arquivo
    char *id;   //vai receber o id do arquivo
    char *mode; //vai receber o mode do arquivo
    char *size; //vai receber o tamanho do arquivo
    char *ordem; //vai receber a ordem de inserção do arquivo



    //pega o meta dados do arquivo
    stat(membro, &st);
    
    //pega o nome do arquivo
    nome = membro;

    //pega os metadados do arquivo e transforma em string
    time = ctime(&st.st_mtim.tv_sec);   //pega a ultima modificação do arquivo

    //tirando o \n do final da string
    char *aux = strdup(time);
    aux[strlen(aux) - 1] = '\0';
    time = aux;

    asprintf(&id, "%d", st.st_uid);
    asprintf(&mode, "%d", st.st_mode);
    asprintf(&size, "%ld", st.st_size);
    asprintf(&ordem, "%d", cont);
    
    //escreve os meta dados no arquivo, todos sao strings
    fwrite(nome, sizeof(char), strlen(nome), arq);
    fwrite("-", 1, 1, arq);
    fwrite("ID:", strlen("ID:"), 1, arq);
    fwrite(id, strlen(id), 1, arq);
    fwrite("-", 1, 1, arq);
    fwrite("MODE:", strlen("MODE:"), 1, arq);
    fwrite(mode,strlen(mode), 1, arq);
    fwrite("-", 1, 1, arq);
    fwrite("SIZE:", strlen("SIZE:"), 1, arq);
    fwrite(size, strlen(size), 1, arq);
    fwrite("-", 1, 1, arq);
    fwrite("TIME:", strlen("TIME:"), 1, arq);
    fwrite(time, sizeof(char), strlen(time), arq);
    fwrite("-", 1, 1, arq);
    fwrite("ORDEM:", strlen("ORDEM:"), 1, arq);
    fwrite(ordem, strlen(ordem), 1, arq);
    fwrite("@", 1, 1, arq);
    


}




//retorna o tamanho do bloco
int blocoSize(FILE *arq, char *membro){

    //pega o tamanho do nome do membro em bytes
    int size = strlen(membro) + 1; //+1 para o separador

    //vou pegar o tamanho do arquivo
    struct stat st;
    stat(membro, &st);
    int sizeArq = st.st_size;

    //agora somo os dois tamanhos
    size = size + sizeArq + 1; //+1 para o separador

    //agora, calculo o tamanho do size em bytes
    char *tam;
    asprintf(&tam, "%d", size);
    size = size + strlen(tam);

    return size;
}


//pega um boco de conteudo e escreve no fim do arquivo
void WriteEdge(FILE *arq, int bloco){
    char buffer[bloco];
    
    fread(buffer, sizeof(char), bloco, arq);

    fseek(arq, 0, SEEK_END);

    fwrite(buffer, sizeof(char), bloco, arq);
}



int foundFile(FILE *arq, char *membro, int *size, char **nome, int *ant){
    short ok = 0;
    char stop = 1;

   

    *size = takeJump(arq, &ok);
    takeName(arq, nome);
    if(strcmp(*nome, membro) == 0){
        return 1;
    }
    else{
        while(stop == 1){
            fseek(arq, *size, SEEK_SET);
            *ant = *size;
            *size = *size + takeJump(arq, &ok);
            if(ok == 1){
                return 0;
            }
            takeName(arq, nome);
            if(strcmp(*nome, membro) == 0){
                return 2;
            }
            
        }
    }
    return 0;
 }

void takeName(FILE *arq, char **name){
    short ok = 0;
    char buffer;
    char nome[50];
    int i = 0;

    fread(&buffer, 1, 1, arq);
    if(buffer >= 'a' && buffer <= 'z' || buffer >= 'A' && buffer <= 'Z' || buffer >= '0' && buffer <= '9' || buffer == '.'){
        nome[i] = buffer;
        i++;
    }
    while(buffer != '@'){
        fread(&buffer, 1, 1, arq);
        if(buffer >= 'a' && buffer <= 'z' || buffer >= 'A' && buffer <= 'Z' || buffer >= '0' && buffer <= '9' || buffer == '.'){
            nome[i] = buffer;
            i++;
        }
    }
    nome[i] = '\0';
    
    //name recebe o que esta em nome
    *name = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(*name, nome);
    
}

