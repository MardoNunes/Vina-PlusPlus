#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "libList.h"



void list(char *archiver){
    FILE *arq;
    arq = fopen(archiver, "r+");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    //vou pular todo o conteudo dos membros
    //e vou direto para os metadados
    short ok = 0;
    int jump = takeJump(arq, &ok);
    fseek(arq, jump, SEEK_SET);
    while(ok != 1){
        jump = jump + takeJump(arq, &ok);
        fseek(arq, jump, SEEK_SET);
    }
    //agora vou ler os metadados
    char *name = NULL;  //nome do arquivo
    char *mode = NULL;  //modo de abertura do arquivo
    char *id = NULL;    //id do arquivo
    char *size = NULL;  //tamanho do arquivo
    char *date = NULL;  //data de criação do arquivo
    char *ordem = NULL; //ordem de inserção do arquivo
    int pointer = jump;
    
    char lido;
    short stop = 0;
    while(stop != 1){
        pointer = fileName(pointer, &name, arq); 
        pointer = fileMode(pointer, &mode, arq);
        pointer = fileId(pointer, &id, arq);
        pointer = Size(pointer, &size, arq);
        pointer = Date(pointer, &date, arq);
        pointer = Order(pointer, &ordem, arq, &stop);
        printFile(name, mode, id, size, date, ordem);
    }
    fclose(arq);
    
}

//pega o nome do arquivo
int fileName(int jump, char **name, FILE *arq){
    fseek(arq, jump, SEEK_SET); //pulo o conteudo do arquivo
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
    *name = (char *) malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(*name, nome);
    
    return ftell(arq);
}

//pega o modo de abertura do arquivo
int fileMode(int jump, char **mode, FILE *arq){
    fseek(arq, jump, SEEK_SET); //pulo o conteudo do arquivo
    char buffer;
    char modo[50];
    int i = 0;
    fread(&buffer, 1, 1, arq);
    if(buffer >= '0' && buffer <= '9'){
        modo[i] = buffer;
        i++;
    }
    while(buffer != '-'){
        fread(&buffer, 1, 1, arq);
        if(buffer >= '0' && buffer <= '9'){
            modo[i] = buffer;
            i++;
        }
    }
    modo[i] = '\0';
    
    //mode recebe o que esta em modo
    *mode = (char *) malloc(sizeof(char) * (strlen(modo) + 1));
    strcpy(*mode, modo);
    
    return ftell(arq);
}

//pega o id do arquivo
int fileId(int jump, char **id, FILE *arq){
    fseek(arq, jump, SEEK_SET); //pulo o conteudo do arquivo
    char buffer;
    char idd[50];
    int i = 0;
    fread(&buffer, 1, 1, arq);
    if(buffer >= '0' && buffer <= '9'){
        idd[i] = buffer;
        i++;
    }
    while(buffer != '-'){
        fread(&buffer, 1, 1, arq);
        if(buffer >= '0' && buffer <= '9'){
            idd[i] = buffer;
            i++;
        }
    }
    idd[i] = '\0';
    
    //id recebe o que esta em idd
    *id = (char *) malloc(sizeof(char) * (strlen(idd) + 1));
    strcpy(*id, idd);

    return ftell(arq);
}

//pega o tamanho do arquivo
int Size(int jump, char **size, FILE *arq){
    fseek(arq, jump, SEEK_SET); //pulo o conteudo do arquivo
    char buffer;
    char tam[50];
    int i = 0;
    fread(&buffer, 1, 1, arq);
    if(buffer >= '0' && buffer <= '9'){
        tam[i] = buffer;
        i++;
    }
    while(buffer != '-'){
        fread(&buffer, 1, 1, arq);
        if(buffer >= '0' && buffer <= '9'){
            tam[i] = buffer;
            i++;
        }
    }
    tam[i] = '\0';
    
    //size recebe o que esta em tam
    *size = (char *) malloc(sizeof(char) * (strlen(tam) + 1));
    strcpy(*size, tam);
    return ftell(arq);
}

//pega a data de criação do arquivo
int Date(int jump, char **date, FILE *arq){
    fseek(arq, jump, SEEK_SET); //pulo o conteudo do arquivo
    char buffer;
    char aux;
    char data[50];
    int i = 0;
    fread(&aux, 1, 1, arq);
    while(aux != ':'){
        fread(&aux, 1, 1, arq);
    }
    fread(&buffer, 1, 1, arq);
    if(buffer >= '0' && buffer <= '9' || buffer >= 'a' && buffer <= 'z' || buffer >= 'A' && buffer <= 'Z' || buffer == '\b' || buffer == ':' || buffer == ' '){
        data[i] = buffer;
        i++;
    }
    while(buffer != '-'){
        fread(&buffer, 1, 1, arq);
        if(buffer >= '0' && buffer <= '9' || buffer >= 'a' && buffer <= 'z' || buffer >= 'A' && buffer <= 'Z' || buffer == '\b' || buffer == ':' || buffer == ' '){
            data[i] = buffer;
            i++;
        }
    }
    data[i] = '\0';
    
    //*date recebe o que esta em data
    *date = (char *) malloc(sizeof(char) * (strlen(data) + 1));
    strcpy(*date, data);
    return ftell(arq);
}

//pega aordem de inserção do arquivo
int Order(int jump, char **order, FILE *arq, short *stop){
    fseek(arq, jump, SEEK_SET); //pulo o conteudo do arquivo
    char buffer;
    char ordem[50];
    int i = 0;
    fread(&buffer, 1, 1, arq);
    if(buffer >= '0' && buffer <= '9'){
        ordem[i] = buffer;
        i++;
    }
    while(buffer != '@' && buffer != '#'){
        fread(&buffer, 1, 1, arq);
        if(buffer >= '0' && buffer <= '9'){
            ordem[i] = buffer;
            i++;
        }
    }
    if(buffer == '#')
        *stop = 1;
    ordem[i] = '\0';
    //order recebe o que esta em ordem
    *order = (char *) malloc(sizeof(char) * (strlen(ordem) + 1));
    strcpy(*order, ordem);
    return ftell(arq);
}

//tranforma o modo de int para string
void mode_to_string(int mode, char *str){
    asprintf(&str, "%c%c%c%c%c%c%c%c%c",
        (mode & S_IRUSR) ? 'r' : '-',
        (mode & S_IWUSR) ? 'w' : '-',
        (mode & S_IXUSR) ? 'x' : '-',
        (mode & S_IRGRP) ? 'r' : '-',
        (mode & S_IWGRP) ? 'w' : '-',
        (mode & S_IXGRP) ? 'x' : '-',
        (mode & S_IROTH) ? 'r' : '-',
        (mode & S_IWOTH) ? 'w' : '-',
        (mode & S_IXOTH) ? 'x' : '-'
    );
}

void printFile(char *name, char *mode, char *id, char *size, char *date, char *ordem){
   int modeInt = atoi(mode);
   mode_to_string(modeInt, mode);
   printf("%s %s %s %s %s %s\n", mode, id, size, date, ordem, name);
    
}

//FUNÇÃO JUMP RETIRADO DO INSERT!
int takeJump(FILE *arq, short *blz){
    char buffer;
    int jump = 0;
    int ok = 0;

    fread(&buffer, 1, 1, arq);

    if(buffer <= '0' || buffer >= '9'){
        *blz = 1;
        return 0;
    }
    
    jump = buffer - '0';
    while(ok != 1){
        fread(&buffer, 1, 1, arq);
        if(buffer == '@')
            ok = 1;
        else{
            jump = (jump * 10) + (buffer - '0');
        }
    }
    
    return jump;   
}
