#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "libInsert-a.h"
#include "libList.h"
#include "libVina.h"
#include "libInsert.h"



void insert_a(char *archiver, int membros, char **argv ){

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
        

        //compara se o arquivo a ser substituido é mais recente que o arquivo a ser inserido
        if(compareRecently(arq, nome, argv[membros]) == 0){
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

                //atualizo a data do novo membro na lista
                updateDate(lista, nome);
                
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

                //atualizo a data do novo membro na lista
                updateDate(lista, nome);

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
        else
            fprintf(stderr, "Arquivo no archiver é mais recente!\n");
    }

    fclose(arq);
}

int compareRecently(FILE *arq, char *membro, char *sub){
    struct Lista *lista = (struct Lista *) malloc(sizeof(struct Lista));
    lista->cabeca = NULL;
    lista->cauda = NULL;

    //ler metadados
    takeMetaData(arq, lista);

    //pego a data do membro na lista
    struct nodo *aux = lista->cabeca;
    char data[50];
    while(aux != NULL){
        if(strcmp(aux->nome, membro) == 0){
            strcpy(data, aux->data);
            break;
        }
        aux = aux->prox;
    }

    //pego a data do sub com a struct stat
    struct stat st;
    stat(sub, &st);
    char dataSub[50];
    strcpy(dataSub, ctime(&st.st_mtime));

    //comparo as datas com strptime
    struct tm tm1;
    struct tm tm2;
    strptime(data, "%a %b %d %H:%M:%S %Y", &tm1);
    strptime(dataSub, "%a %b %d %H:%M:%S %Y", &tm2);
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if(t1 > t2){
        desenfilera(lista);
        return 1;
    }

    desenfilera(lista);
    return 0;
}