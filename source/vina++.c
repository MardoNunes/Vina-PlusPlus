#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include "libInsert.h"
#include "libInsert-a.h"
#include "libList.h"
#include "libMove.h"
#include "libRemove.h"
#include "libHelp.h"
#include "libExtrai.h"





int main(int argc, char **argv){
    int opcao;
    int flag_i = 0;
    int flag_a = 0; 
    int flag_m = 0;
    int flag_x = 0;
    int flag_r = 0;
    int flag_c = 0;
    int flag_h = 0;

    char *membros_i = NULL;
    char *membros_a = NULL;
    char *membros_m = NULL;
    char *membros_x = NULL;
    char *membros_r = NULL;
    char *membros_c = NULL;


    while((opcao = getopt(argc, argv, "i:a:m:x:r:c:h")) != -1){
        switch(opcao){
        case 'i':   
            
            flag_i = 1;
            membros_i = optarg;
            break; 

        case 'a':
            
            flag_a = 1;
            membros_a = optarg;
            break;

        case 'm':
            
            flag_m = 1;
            membros_m = optarg; //aqui está o arquivo que quero mexer
                //o archiver vem por segundo nesse caso!
            break;

        case 'x':
            
            flag_x = 1;
            membros_x = optarg;
            break;

        case 'r':
           
            flag_r = 1;
            membros_r = optarg;
            break;

        case 'c':
            
            flag_c = 1;
            membros_c = optarg;
            break;

        case 'h':
            flag_h = 1;
            break;
        }
    }
    
   
    
    // se for para inserir arquivos -i
    if(flag_i == 1){
        if(!membros_i){
            printf("Erro: -i precisa de um argumento\n");
            exit(1);
        }
        else{
            //chama a função de inserir arquivos
            insert(membros_i, optind, argv);
        }

    }
    else if(flag_a == 1){   //mesmo comportamento da opção -i, mas a substituição de um membro existente ocorre APENAS caso o parâmetro seja mais recente que o arquivado;
        if(!membros_a){
            printf("Erro: -a precisa de um argumento\n");
            exit(1);
        }
        else{
            printf("Inserindo arquivos\n");
            printf("Arquivos: %s\n", membros_a);
            //chama a função de inserir arquivos
            insert_a(membros_a, optind, argv);
        }
    }
    else if(flag_h == 1){
        help();
    }
    else if(flag_x == 1){
        if(!membros_x){
            printf("Erro: -x precisa de um argumento\n");
            exit(1);
        }
        else{
            printf("Extraindo arquivos\n");
            printf("Arquivos: %s\n", membros_x);
            //chama a função de extrair arquivos
            extrai(membros_x, optind, argv);
        }
    }
    else if(flag_m == 1){
        if(!membros_m){
            printf("Erro: -m precisa de um argumento\n");
            exit(1);
        }
        else{
            printf("Movendo arquivos\n");
            printf("Arquivos: %s\n", membros_m);
            //chama a função de mover arquivos
            movimenta(argv[optind], membros_m, argv[optind+1]);
        }
    }
    else if(flag_r == 1){
        if(!membros_r){
            printf("Erro: -r precisa de um argumento\n");
            exit(1);
        }
        else{
            printf("Removendo arquivos\n");
            printf("Arquivos: %s\n", membros_r);
            //chama a função de remover arquivos
            removeMember(membros_r, optind, argv);
        }
    }
    else if(flag_c == 1){
        if(!membros_c){
            printf("Erro: -c precisa de um argumento\n");
            exit(1);
        }
        else{
            printf("Listando arquivos\n");
            printf("Arquivos: %s\n", membros_c);
            //chama a função de listar arquivos
            list(membros_c);
        }
    }
    else{
        printf("Erro: Nenhuma opção foi selecionada\n");
        exit(1);
    }    
    return 0;
}

