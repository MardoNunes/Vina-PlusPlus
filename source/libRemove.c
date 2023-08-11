#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "libRemove.h"
#include "libList.h"
#include "libVina.h"
#include "libInsert.h"




void removeMember(char *archiver, int membros, char **argv){
    FILE *arq;
    arq = fopen(archiver, "rb+");
    if(arq == NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    //vou pegar o tamanho do arquivo e nome, ate encontrar o arquivo que quero remover
    char *membro = argv[membros];
    int size;
    int ant;
    char *nome = NULL;
    int choice = foundFile(arq, membro, &size, &nome, &ant);
    

    if(choice == 1){    //se for o primeiro arquivo
        short ok = 0;

        ant = 0;
        fseek(arq, 0, SEEK_SET);


        //agora vou ler os metadados e guardar na lista
        struct Lista *lista;
        lista = (struct Lista *) malloc(sizeof(struct Lista));
        lista->cabeca = NULL;
        takeMetaData(arq, lista);

        //vou pegar tudo que vem depois do arquivo que quero remover
        //sobreescrevendo o que quero remover
        fseek(arq, 0, SEEK_SET);
        int jump = takeJump(arq, &ok);
        fseek(arq, jump, SEEK_SET);

        //vou ler tudo que vem depois do arquivo que quero remover
        jump = jump + takeJump(arq, &ok);
        
        fseek(arq, jump, SEEK_SET);
        while(ok != 1){
           
            jump = jump + takeJump(arq, &ok);
            
            if(ok == 1){
                break;
            }
            fseek(arq, jump, SEEK_SET);
        }

        fseek(arq, size, SEEK_SET);
        char buffer[jump];
        fread(buffer, 1, jump, arq);


        //vou escrever o que vem depois em cima do que quero remover
        fseek(arq, ant, SEEK_SET);
        fwrite(buffer, 1, strlen(buffer), arq);
        
        fseek(arq, 0, SEEK_SET);

        //vou caminha ate os metadados
        jump = takeJump(arq, &ok);
        fseek(arq, jump, SEEK_SET);
        while(ok != 1){
            jump = jump + takeJump(arq, &ok);
            if(ok == 1){
                break;
            }
            fseek(arq, jump, SEEK_SET);
        }

        //agora caminho ate o final dos meta dados e trunco o arquivo
        char buffer2;
        fread(&buffer2, 1, 1, arq);
        while(buffer2 != '#'){
            fread(&buffer2, 1, 1, arq);
        }
        //trunco o arquivo
        ftruncate(fileno(arq), ftell(arq));

        
        //removo os metadados do arquivo que removi
        removeMetaData(lista, nome);
        updateOrder(lista);

        fseek(arq, 0, SEEK_SET);
        //vou caminha ate os metadados atualizados e escrever no arquivo
        ok = 0;
        jump = takeJump(arq, &ok);
       
        fseek(arq, jump, SEEK_SET);
        while(ok != 1){
            jump = jump + takeJump(arq, &ok);
            
            if(ok == 1){
                break;
            }
            fseek(arq, jump, SEEK_SET);
        }

        imprimiRemove(lista, arq, jump);
        //trunco o arquivo
        ftruncate(fileno(arq), ftell(arq));

    }
    else if(choice == 2){   //se nao for o primeiro arquivo
        //size = tamanho do arquivo que quero remover
        //ant = tamanho do arquivo anterior ao que quero remover
        int bloco = size;
        size = size + ant;
        

        char buffer[1024];
        short ok = 0;
        //agora vou ler os metadados e guardar na lista
        struct Lista *lista;
        lista = (struct Lista *) malloc(sizeof(struct Lista));
        lista->cabeca = NULL;
        takeMetaData(arq, lista);

        //vou pegar tudo que vem depois do arquivo que quero remover
        //sobreescrevendo o que quero remover
        fseek(arq, size, SEEK_SET);

        //vou ler tudo que vem depois do arquivo que quero remover
        ok = 0;
        int jump = takeJump(arq, &ok);
        
        
        while(ok != 1){ 
            fseek(arq, jump+size, SEEK_SET);
            jump = jump + takeJump(arq, &ok); 
            
            if(ok == 1){
                break;
            }
            
        }
        
        fseek(arq, size, SEEK_SET);
        char buffer2[jump];
        fread(buffer2, 1, jump, arq);

        //vou escrever o que vem depois em cima do que quero remover
        fseek(arq, ant, SEEK_SET);
        fwrite(buffer2, 1, strlen(buffer2), arq);
        ftruncate(fileno(arq), ftell(arq));


        //removo os metadados do arquivo que removi
        removeMetaData(lista, nome);
        updateOrder(lista);

        fseek(arq, 0, SEEK_SET);
        //vou caminha ate os metadados atualizados e escrever no arquivo
        ok = 0;
        jump = takeJump(arq, &ok);
       
        fseek(arq, jump, SEEK_SET);
        while(ok != 1){
            jump = jump + takeJump(arq, &ok);
            
            if(ok == 1){
                break;
            }
            fseek(arq, jump, SEEK_SET);
        }

        imprimiRemove(lista, arq, jump);
        //trunco o arquivo
        ftruncate(fileno(arq), ftell(arq));

        //libero memoria
        free(lista);

    }
    else{   //se nao encontrar o arquivo
        printf("Arquivo nao encontrado\n");
        exit(1);
    }
    
}
