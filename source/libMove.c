#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include "libMove.h"
#include "libList.h"
#include "libVina.h"
#include "libInsert.h"


void movimenta(char *archiver, char *target, char *movi){

    //quero mover o conteudo de movi para de pois de target
    FILE *arq;
    arq = fopen(archiver, "rb+");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    //vou encontrar o arquivo target
    int fim = 0;
    char *nome;
    int comeco = 0;
    int found = foundFile(arq, target, &fim, &nome, &comeco);

    //vou encontrar o arquivo movi
    fseek(arq, 0, SEEK_SET);
    int fim2 = 0;
    char *nome2;
    int comeco2 = 0;
    int found2 = foundFile(arq, movi, &fim2, &nome2, &comeco2);

    
    if(comeco < comeco2){
        fprintf(stderr, "Erro: Arquivo a ser movido já está depois do target!\n");
        exit(1);
    }
    else if(comeco == fim2){
        //guardo metadados do arquivo
        struct Lista *lista = (struct Lista*)malloc(sizeof(struct Lista));
        lista->cabeca = NULL;
        lista->cauda = NULL;
        takeMetaData(arq, lista);

        //vou guardar o que vem depois de target na borda
        fseek(arq, 0, SEEK_END);
        int initEdge = ftell(arq);  //guardo o comeca da borda
        fseek(arq, fim, SEEK_SET);  //pulo o target

        //agora escreve os demais membros na borda
        int fimEdge;
        short ok = 0;
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

        //agora move o movi para alem da borda
        fseek(arq, comeco2, SEEK_SET);
        int beyondEdgeInit = fimEdge;
        jump = takeJump(arq, &ok);
        int blocoBeyondEdge = jump;
        fseek(arq, comeco2, SEEK_SET);
        WriteEdge(arq, jump);
        
        //agora vou mover o target para onde estava o movi
        fseek(arq, comeco, SEEK_SET);
        jump = takeJump(arq, &ok);
        fseek(arq, comeco, SEEK_SET);
        char buffer[jump];
        fread(buffer, sizeof(char), jump, arq);
        fseek(arq, comeco2, SEEK_SET);
        fwrite(buffer, sizeof(char), jump, arq);
        int fimBlocoTarget = ftell(arq);

        //agora vou mover o movi para onde estava o target
        fseek(arq, beyondEdgeInit, SEEK_SET);
        jump = takeJump(arq, &ok);
        fseek(arq, beyondEdgeInit, SEEK_SET);
        char buffer2[jump];
        fread(buffer2, sizeof(char), jump, arq);
        fseek(arq, fimBlocoTarget, SEEK_SET);
        fwrite(buffer2, sizeof(char), jump, arq);
        int fimBlocoMovi = ftell(arq);
        ftruncate(fileno(arq), fimEdge);    //trunquei no fim da borda eleminando o movi do fim 

        //agora vou escrever o que estava na borda no fim do arquivo
        fseek(arq, initEdge, SEEK_SET);
        char *buffer3 = (char*) malloc(sizeof(char) * (fimEdge - initEdge));
        fread(buffer3, sizeof(char), fimEdge - initEdge, arq);
        fseek(arq, fimBlocoMovi, SEEK_SET);
        fwrite(buffer3, sizeof(char), fimEdge - initEdge, arq);
        ftruncate(fileno(arq), ftell(arq)); //trunco o aarquivo pq tenho os metadados salvos
        free(buffer3);


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
    else{
        //guardo metadados do arquivo
        struct Lista *lista = (struct Lista*)malloc(sizeof(struct Lista));
        lista->cabeca = NULL;
        lista->cauda = NULL;
        takeMetaData(arq, lista);

        //vou guardar o quem depois de movi na borda
        fseek(arq, 0, SEEK_END);
        int initEdge = ftell(arq);  //guardo o comeca da borda
        fseek(arq, fim2, SEEK_SET);  //pulo o movi
        int initBlocoMiddle = ftell(arq);

        //agora escreve os demais membros na borda
        int fimEdge;
        int fimBlocoMiddle = comeco;
        short ok = 0;
        int soma = fim2;
        int jump = takeJump(arq, &ok);
        soma = soma + jump;
        fseek(arq, fim2, SEEK_SET);
        WriteEdge(arq, jump);
        while(ftell(arq) < comeco){
            fseek(arq, soma, SEEK_SET);
            jump = takeJump(arq, &ok);
            fseek(arq, soma, SEEK_SET);
            soma = soma + jump;
            WriteEdge(arq, jump);
        }
        int fimEdgeMiddle = ftell(arq);
        

        //agora escreve o que vem depois de target na borda
        fseek(arq, fim, SEEK_SET);

        //agora escreve os demais membros na borda
        soma = fim;
        jump = takeJump(arq, &ok);
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

        //agora move o movi para alem da borda
        fseek(arq, comeco2, SEEK_SET);
        int beyondEdgeInit = fimEdge;
        jump = takeJump(arq, &ok);
        int blocoBeyondEdge = jump;
        fseek(arq, comeco2, SEEK_SET);
        WriteEdge(arq, jump);

        //agora vou mover o bloco do meio para onde estava o movi
        fseek(arq, initBlocoMiddle, SEEK_SET);
       
        int blocoMiddle = fimBlocoMiddle - initBlocoMiddle;
        char buffer[blocoMiddle];
        fread(buffer, sizeof(char), blocoMiddle, arq);
        fseek(arq, comeco2, SEEK_SET);
        fwrite(buffer, sizeof(char), blocoMiddle, arq);
        fimBlocoMiddle = ftell(arq);

        //agora vou mover o target para depois do bloco do meio
        fseek(arq, comeco, SEEK_SET);
        jump = takeJump(arq, &ok);
        fseek(arq, comeco, SEEK_SET);
        char buffer2[jump];
        fread(buffer2, sizeof(char), jump, arq);
        fseek(arq, fimBlocoMiddle, SEEK_SET);
        fwrite(buffer2, sizeof(char), jump, arq);
        fim = ftell(arq);
        
        //agora vou mover o movi depois do target
        fseek(arq, beyondEdgeInit, SEEK_SET);
        jump = takeJump(arq, &ok);
        fseek(arq, beyondEdgeInit, SEEK_SET);
        char buffer3[jump];
        fread(buffer3, sizeof(char), jump, arq);
        fseek(arq, fim, SEEK_SET);
        fwrite(buffer3, sizeof(char), jump, arq);
        fim2 = ftell(arq);
        ftruncate(fileno(arq), fimEdge);    //trunquei no fim da borda eleminando o movi do fim

        //agora vou escrever o que estava na borda no fim do arquivo
        fseek(arq, fimEdgeMiddle, SEEK_SET);
        char *buffer4 = (char*) malloc(sizeof(char) * (fimEdge - fimEdgeMiddle));
        fread(buffer4, sizeof(char), fimEdge - fimEdgeMiddle, arq);
        fseek(arq, fim2, SEEK_SET);
        fwrite(buffer4, sizeof(char), fimEdge - fimEdgeMiddle, arq);
        ftruncate(fileno(arq), ftell(arq)); //trunco o aarquivo pq tenho os metadados salvos


        //escreve os metadados no arquivo
        fseek(arq, 0, SEEK_END);
        imprimi(lista, arq);
        fseek(arq, -1, SEEK_END);
        fwrite("#", 1, 1, arq);

        //libera a lista
        desenfilera(lista);
        free(lista);
        free(nome);
        free(buffer4);


    }
}

