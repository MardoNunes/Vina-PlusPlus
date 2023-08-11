



#ifndef __LIBINSERT__
#define __LIBINSERT__

//estrutura para armazenar os dados do arquivo


void insert(char *archiver, int membros, char **argv );
void escreve(FILE *arq, char *membro);
void WriteFileSize(FILE *arq, char *membro);
int testArq(FILE *arq);
int vazio(FILE *arq);
void metadados(FILE *arq, char *membro, int cont);
int fileSize(char *membro);
int foundFile(FILE *arq, char *membro, int *size, char **nome, int *ant);
void takeName(FILE *arq, char **name);
void WriteEdge(FILE *arq, int bloco);
int blocoSize(FILE *arq, char *membro);


#endif