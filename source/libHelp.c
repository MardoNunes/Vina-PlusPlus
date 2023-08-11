#include <stdio.h>
#include <stdlib.h>
#include "libHelp.h"



void help(){
    printf("Opções de linha de comando para o vina++:\n");
    printf("-i : insere/acrescenta um ou mais membros ao archive. Caso o membro já exista no archive, ele deve ser substituído. Novos membros são inseridos respeitando a ordem da linha de comando, ao final do archive;\n");
    printf("-a : mesmo comportamento da opção -i, mas a substituição de um membro existente ocorre APENAS caso o parâmetro seja mais recente que o arquivado;\n");
    printf("-m target : move o membro indicado na linha de comando para imediatamente depois do membro target existente em archive. A movimentação deve ocorrer na seção de dados do archive;\n");
    printf("-x : extrai os membros indicados de archive. Se os membros não forem indicados, todos devem ser extraídos. A extração consiste em ler o membro de archive e criar um arquivo correspondente, com conteúdo idêntico, em disco;\n");
    printf("-r : remove os membros indicados de archive;\n");
    printf("-c : lista o conteúdo de archive em ordem, incluindo as propriedades de cada membro (nome, UID, permissões, tamanho e data de modificação) e sua ordem no arquivo.\n");
    printf("-h : imprime uma pequena mensagem de ajuda com as opções disponíveis e encerra.\n\n");
    printf("Execução do programa:\n");
    printf("./vina++ <parametros> <arquivos>\n\n");
    printf("Exemplos:   \n");
    printf("insere aquivos 'arq.txt', 'foto.jpg' 'despeos.ods' em backup.vpp\n");
    printf("./vina++ -i backup.vpp arq.txt foto.jpg despesas.ods\n\n");
    printf("atualiza 'despesas.ods' em backup.vpp, se o arquivo externo for mais recente\n");
    printf("./vina++ -a backup.vpp despesas.ods\n\n");
    printf("extrai o arquivo arq.txt de backup.vpp\n");
    printf("./vina++ -x backup.vpp arq.txt\n\n");
    printf("extrai todos os arquivos de backup.vpp\n");
    printf("./vina++ -x backup.vpp\n\n");
    printf("move o arquivo 'arq.txt' para depois de 'despesas.ods' em backup.vpp\n");
    printf("./vina++ -m despesas.ods backup.vpp arq.txt\n\n");
    printf("remove o arquivo 'arq.txt' de backup.vpp\n");
    printf("./vina++ -r backup.vpp arq.txt\n\n");
    printf("lista o conteúdo de backup.vpp\n");
    printf("./vina++ -c backup.vpp\n\n");
}