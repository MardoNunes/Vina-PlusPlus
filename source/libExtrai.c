#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "libExtrai.h"
#include "libVina.h"
#include "libList.h"
#include "libInsert.h"


void extrai(char *archiver, int membros, char **argv){
    FILE *arq = fopen(archiver, "rb+");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    //verefica se ele quer extrair todos os membros
    if(argv[membros] == NULL){  //extrair tudo
        extraiAll(arq);
    }
    else{   //extrai especifico
        extraiEspec(arq, argv[membros]);
    }

}

void extraiAll(FILE *arq){
    short ok = 0;
    int tam;
    int jump = 0;
    char *name = NULL;
    struct Lista *lista = malloc(sizeof(struct Lista));
    lista->cabeca = NULL;
    lista->cauda = NULL;
    
    takeMetaData(arq, lista);
    fseek(arq, 0, SEEK_SET);
    
    while(ok != 1){
        
        jump = jump + takeJump(arq, &ok);
        
        if(ok == 1){
            break;
        }
        takeName(arq, &name);
        

        // //pega tamanho do arquivo na lista
        tam = takeSizeLista(lista, name);
        
        //cria o arquivo
        char buffer[tam];
        fread(buffer, 1, sizeof(buffer), arq);
        FILE *aux = fopen(name, "wb");
        fwrite(buffer, 1, sizeof(buffer), aux);
        
        //mudnando as permissoes
        int mode = foundModeLista(lista, name);
        
        chmod(name, mode);
        fclose(aux);

        //proximo membro
        fseek(arq, jump, SEEK_SET);

    }
    desenfilera(lista);
    free(lista);
    fclose(arq);

}

void extraiEspec(FILE *arq, char *membro){
    short ok;
    int tam;
    int fim;
    char *name = NULL;
    int comeco;
    int choice = foundFile(arq, membro, &fim, &name, &comeco);
    struct Lista *lista = malloc(sizeof(struct Lista));
    lista->cabeca = NULL;
    lista->cauda = NULL;
    takeMetaData(arq, lista);

    if(choice == 1){
        comeco = 0;
      
        fseek(arq, 0, SEEK_SET);    //sei que é o primeior, entao vou no comeco do arquivo;
        int jump = takeJump(arq, &ok);  //jump caminha os bytes ate o nome
        takeName(arq, &name);
        
       //pega tamanho do arquivo na lista
        tam = takeSizeLista(lista, name);
        
       
        //cria o arquivo
        char buffer[tam];
        fread(buffer, 1, sizeof(buffer), arq);
        FILE *aux = fopen(name, "wb");
        fwrite(buffer, 1, sizeof(buffer), aux);
        
        //mudnando as permissoes
        int mode = foundModeLista(lista, name);
        
        chmod(name, mode);
        fclose(aux);

    }
    else if(choice == 2){   //quando o membro esta no meio do arquivo
        fseek(arq, comeco, SEEK_SET);
        int jump = takeJump(arq, &ok);
        takeName(arq, &name);
        
        //pega tamanho do arquivo na lista
        tam = takeSizeLista(lista, name);
        
        //cria o arquivo
        char buffer[tam];
        fread(buffer, 1, sizeof(buffer), arq);
        FILE *aux = fopen(name, "wb");
        fwrite(buffer, 1, sizeof(buffer), aux);
        
        //mudnando as permissoes
        int mode = foundModeLista(lista, name);
        
        chmod(name, mode);
        fclose(aux);
    }
    else{
        printf("Arquivo nao encontrado\n");
    }
    desenfilera(lista);
    free(lista);
    fclose(arq);
}


void nextName(FILE *arq){
    char buffer;
    while(buffer != '@'){
        fread(&buffer, 1, 1, arq);
    }
}


void foundName(FILE *arq, char **name){
    short ok = 0;
    char buffer;
    char nome[50];
    int i = 0;

    fread(&buffer, 1, 1, arq);
    if(buffer >= 'a' && buffer <= 'z' || buffer >= 'A' && buffer <= 'Z' || buffer >= '0' && buffer <= '9' || buffer == '.'){
        nome[i] = buffer;
        i++;
    }
    while(buffer != '-'){
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

