struct nodo{
    char *nome;
    char *modo;
    char *id;
    char *size;
    char *data;
    char *ordem;
    struct nodo *prox;
};

struct Lista{
    struct nodo *cabeca;
    struct nodo *cauda;
};

void takeMetaData(FILE *arq, struct Lista *lista);
void imprimi(struct Lista *lista, FILE *arq);
int lastOrder(struct Lista *lista);
void updateSize(struct Lista *lista, char *nome);
void desenfilera(struct Lista *lista);
void updateDate(struct Lista *lista, char *nome);
int takeSizeLista(struct Lista *lista, char *nome);
int foundModeLista(struct Lista *lista, char *nome);
void removeMetaData(struct Lista *lista, char *nome);
void updateOrder(struct Lista *lista);
void imprimiRemove(struct Lista *lista, FILE *arq, int jump);