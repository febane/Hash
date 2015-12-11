#ifndef ARVORE_B
#define ARVORE_B

#define T 50
#define MAX_CHAVES 99 //Quantidade máxima de chaves
#define MAX_FILHOS 100 //Quantidade máxima de filhos
#define MIN_OCUP 49 //Ocupação mínima em cada nó
     
typedef enum {FALSE = 0, TRUE} bool;

#include "encadeamento.h"

typedef struct arvoreb {
	int numDocs;
	int ocup;
	Palavra **dados;
	struct arvoreb **filhos;
} ArvoreB;

ArvoreB *criarArvoreB(int numDocs);
int busca_linear(ArvoreB *arv, char *str);
bool inserirNovoTermo(ArvoreB *arv, Termo *t, int numDoc);
void insere_chave(ArvoreB *raiz, Palavra *p, ArvoreB *filhodir);
ArvoreB *insere(ArvoreB *raiz, Palavra *p, bool *h, Palavra **p_retorno);
ArvoreB *insere_ArvoreB(ArvoreB *raiz, Termo *t, int numDoc);
ArvoreB *inserirArvoreB(ArvoreB *arv, Palavra *p);
void indexarArvoreB(ArvoreB *arv, FILE *file, char **nomes);
void indexarAux(ArvoreB *arv, FILE *file);
ArvoreB *montarArvoreB(FILE *file, char ***nomes);
void buscarArvoreB(ArvoreB *arv, char *str, char **nomes);

#endif


