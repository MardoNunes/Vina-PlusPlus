#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "libVina.h"
#include "libList.h"



//função que ira ler os metadados e guardar em um lista encadeada
void takeMetaData(FILE *arq, struct Lista *lista){
    

    //vou pular todo o conteudo dos membros
    short ok = 0;
    fseek(arq, 0, SEEK_SET);
    int jump = takeJump(arq, &ok);
    while(ok != 1){
        fseek(arq, jump, SEEK_SET);
        jump = jump + takeJump(arq, &ok);
    
    }
  

    int pointer = ftell(arq);
    short stop = 0;

    //vou começar a ler os metadados e guardar na lista
    while(stop != 1){
        struct nodo *novo = (struct nodo *) malloc(sizeof(struct nodo));
        novo->nome = NULL;
        novo->modo = NULL;
        novo->id = NULL;
        novo->size = NULL;
        novo->data = NULL;
        novo->ordem = NULL;
        pointer = fileName(pointer, &novo->nome, arq);
        pointer = fileId(pointer, &novo->id, arq);
        pointer = fileMode(pointer, &novo->modo, arq);
        pointer = Size(pointer, &novo->size, arq);
        pointer = Date(pointer, &novo->data, arq);
        pointer = Order(pointer, &novo->ordem, arq, &stop);
        
       

        //vou inserir na lista
        //se a lista estiver vazia
        if(lista->cabeca == NULL){
            lista->cabeca = novo;
            lista->cauda = novo;
            novo->prox = NULL;
        }
        else{
            lista->cauda->prox = novo;
            lista->cauda = novo;
            novo->prox = NULL;
        }
    }
}

//imprimi a lista no arquivo
void imprimi(struct Lista *lista, FILE *arq){
    struct nodo *aux = lista->cabeca;
    fwrite("\n", 1, 1, arq);
    while(aux != NULL){
        fwrite(aux->nome, 1, strlen(aux->nome), arq);
        fwrite("-", 1, 1, arq);
        fwrite("ID:", 1, strlen("ID:"), arq);
        fwrite(aux->id, 1, strlen(aux->id), arq);
        fwrite("-", 1, 1, arq);
        fwrite("MODE:", 1, strlen("MODE:"), arq);
        fwrite(aux->modo, 1, strlen(aux->modo), arq);
        fwrite("-", 1, 1, arq);
        fwrite("SIZE:", 1, strlen("SIZE:"), arq);
        fwrite(aux->size, 1, strlen(aux->size), arq);
        fwrite("-", 1, 1, arq);
        fwrite("TIME:", 1, strlen("TIME:"), arq);
        fwrite(aux->data, 1, strlen(aux->data), arq);
        fwrite("-", 1, 1, arq);
        fwrite("ORDEM:", 1, strlen("ORDEM:"), arq);
        fwrite(aux->ordem, 1, strlen(aux->ordem), arq);
        fwrite("@", 1, 1, arq);
        aux = aux->prox;
    }
}

int lastOrder(struct Lista *lista){
    struct nodo *aux = lista->cabeca;
    int maior = 0;
    while(aux != NULL){
        if(atoi(aux->ordem) > maior)
            maior = atoi(aux->ordem);
        aux = aux->prox;
    }
    return maior;
}


void updateSize(struct Lista *lista, char *nome){
    struct nodo *aux = lista->cabeca;
    struct stat st;
    
    while(aux != NULL){
        if(strcmp(aux->nome, nome) == 0){
            stat(nome, &st);
            char *size;
            asprintf(&size, "%ld", st.st_size);
            aux->size = size;
            return;
        }
        aux = aux->prox;
    }
}


//função que desenfilera toda a lista
void desenfilera(struct Lista *lista){
    struct nodo *aux = lista->cabeca;
    while(aux != NULL){
        lista->cabeca = aux->prox;
        free(aux);
        aux = lista->cabeca;
    }
}


void updateDate(struct Lista *lista, char *nome){
    struct nodo *aux = lista->cabeca;
    while(aux != NULL){
        if(strcmp(aux->nome, nome) == 0){
            struct stat st;
            stat(nome, &st);
            char data[50];
            strcpy(data, ctime(&st.st_mtime));
            data[strlen(data) - 1] = '\0';
            aux->data = (char *) malloc(sizeof(char) * (strlen(data) + 1));
            strcpy(aux->data, data);
            break;
        }
        aux = aux->prox;
    }
}

void removeMetaData(struct Lista *lista, char *nome){
    struct nodo *aux = lista->cabeca;
    struct nodo *ant = NULL;
    while(aux != NULL){
        if(strcmp(aux->nome, nome) == 0){
            if(ant == NULL){
                lista->cabeca = aux->prox;
                free(aux);
                return;
            }
            else{
                ant->prox = aux->prox;
                free(aux);
                return;
            }
        }
        ant = aux;
        aux = aux->prox;
    }
    printf("Arquivo nao encontrado\n");
    exit(1);
}

//atualiza a ordem dos arquivos
void updateOrder(struct Lista *lista){
    struct nodo *aux = lista->cabeca;
    while(aux != NULL){
        int ordem = atoi(aux->ordem);
        ordem--;
        char *ord = (char *) malloc(sizeof(char) * 10);
        sprintf(ord, "%d", ordem);
        aux->ordem = ord;
        aux = aux->prox;    
    }
}

void imprimiRemove(struct Lista *lista, FILE *arq, int jump){
    struct nodo *aux = lista->cabeca;
    fseek(arq, jump, SEEK_SET);
    fwrite("\n", 1, 1, arq);
    while(aux != NULL){
        fwrite(aux->nome, 1, strlen(aux->nome), arq);
        fwrite("-", 1, 1, arq);
        fwrite("ID:", 1, strlen("ID:"), arq);
        fwrite(aux->id, 1, strlen(aux->id), arq);
        fwrite("-", 1, 1, arq);
        fwrite("MODE:", 1, strlen("MODE:"), arq);
        fwrite(aux->modo, 1, strlen(aux->modo), arq);
        fwrite("-", 1, 1, arq);
        fwrite("SIZE:", 1, strlen("SIZE:"), arq);
        fwrite(aux->size, 1, strlen(aux->size), arq);
        fwrite("-", 1, 1, arq);
        fwrite("TIME:", 1, strlen("TIME:"), arq);
        fwrite(aux->data, 1, strlen(aux->data), arq);
        fwrite("-", 1, 1, arq);
        fwrite("ORDEM:", 1, strlen("ORDEM:"), arq);
        fwrite(aux->ordem, 1, strlen(aux->ordem), arq);
        if(aux->prox != NULL)
            fwrite("@", 1, 1, arq);
        else
            fwrite("#", 1, 1, arq);
        aux = aux->prox;
    }
}

int takeSizeLista(struct Lista *lista, char *nome){
    struct nodo *aux = lista->cabeca;
    while(aux != NULL){
        if(strcmp(aux->nome, nome) == 0){
            return atoi(aux->size);
        }
        aux = aux->prox;
    }

}

int foundModeLista(struct Lista *lista, char *nome){
    struct nodo *aux = lista->cabeca;
    while(aux != NULL){
        if(strcmp(aux->nome, nome) == 0){
            return atoi(aux->modo);
        }
        aux = aux->prox;
    }
}