/**
 * Maquinas de Buscas
 * arvbin.c
 * Implementacao da arvore binaria
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */
 

#include "arvbin.h"

/**
 *  Cria árvore binária
 *
 *  @param doc documento referente a arvore binaria
 *  @return arv arvore criada
 *
 */
ArvBin *criarArvBin(char *doc) {
	ArvBin *arv = (ArvBin*) malloc(sizeof(ArvBin));
	int tam;
	
	// Caso seja a árvore global
	if(doc == NULL) {
		arv->doc = NULL;
	}
	
	// Caso seja a árvore de um documento
	else {
		tam = strlen(doc);
		arv->doc = (char*) calloc(tam+1, sizeof(char));
		strcpy(arv->doc, doc);
	}
	
	arv->termos = 0;
	arv->prim = NULL;
	return arv;
}

/**
 *  Cria termo
 *
 *  @param str string correspondente ao termo
 *  @param pos posicao da referente a string
 *  @return termo termo criado
 *
 */
Termo *criarTermo(char *str, int pos) {
	Termo *termo = (Termo*) malloc(sizeof(Termo));
	int tam = strlen(str);
	termo->str = (char*) calloc(tam+1, sizeof(char));
	strcpy(termo->str, str);
	termo->ocor = 1;
	termo->prim = (Pos*) malloc(sizeof(Pos));
	termo->prim->pos = pos;
	termo->prim->prox = NULL;
	termo->ult = termo->prim;
	termo->esq = NULL;
	termo->dir = NULL;
	return termo;
}

/**
 *  Insere termo auxiliar
 *
 *  @param t termo a ser verificado
 *  @param str string procurada
 *  @param pos posicao da string
 *  @return 1 foi criado termo para string
 *  @return 0 ja havia string na arvore binaria
 *
 */
int inserirAux(Termo **t, char *str, int pos) {
	Pos *aux;
	if(*t == NULL) {
		*t = criarTermo(str, pos);
		return 1;
	}
	else {
		if(strcmp((*t)->str, str) == 0) {
			aux = (Pos*) malloc(sizeof(Pos));
			(*t)->ocor += 1;
			(*t)->ult->prox = aux;
			(*t)->ult = aux;
			aux->pos = pos;
			aux->prox = NULL;
			return 0;
		}
		else if(strcmp((*t)->str, str) > 0) {
			return inserirAux(&(*t)->esq, str, pos);
		}
		else if(strcmp((*t)->str, str) < 0) {
			return inserirAux(&(*t)->dir, str, pos);
		}
	}
	return 0;
}

/**
 *  Insere termo em arvore binaria
 *
 *  @param arv arvore a receber termo
 *  @param str string do termo
 *  @param pos posicao da string
 *
 */
void inserirTermo(ArvBin *arv, char *str, int pos) {
	arv->termos += inserirAux(&(arv->prim), str, pos);
}

/**
 *  Libera memoria de termo
 *
 *  @param t termo a ser liberado
 *
 */
void freeTermo(Termo *t) {
	if(t == NULL)
		return;
		
	Pos *aux;
	
	freeTermo(t->esq);
	freeTermo(t->dir);
	free(t->str);
	
	aux = t->prim;
	while(aux != NULL) {
		t->prim = aux->prox;
		free(aux);
		aux = t->prim;
	}
	
	free(t);
}

/**
 *  Libera memoria de arvore binaria
 *
 *  @param arv arvore binaria a ser liberada
 *
 */
void freeArvBin(ArvBin *arv) {
	freeTermo(arv->prim);
	free(arv->doc);
	free(arv);
}

/**
 *  Imprime termos
 *
 *  @param arv arvore que contem termos a serem mostrados
 *
 */
void imprimirTermos(ArvBin *arv) {
	// Caso seja a árvore global
	if(arv->doc == NULL) {
		printf("No total, %d termos foram lidos.\n", arv->termos);
	}
	
	// Caso seja a árvore de um documento
	else {
		printf("Os %d seguintes termos foram lidos em %s\n", arv->termos, arv->doc);
		imprimirAux(arv->prim);
		printf("\n");
	}
}

/**
 *  Imprime termo auxiliar
 *
 *  @param t termo a ser mostrado
 *
 */
void imprimirAux(Termo *t) {
	Pos *aux;
	if(t == NULL)
		return;
	
	// Imprimindo os termos à esquerda
	imprimirAux(t->esq);
	
	// Imprimindo os termos da árvore em questão
	printf("%s, nas posições: ", t->str);
	aux = t->prim;
	while(aux != NULL) {
		printf("%d ", aux->pos);
		aux = aux->prox;
	}
	printf("\n");
	
	// Imprimindo os termos à direita
	imprimirAux(t->dir);
}

/**
 *  Cria vetor de Termos
 *
 *  @param arv arvore binaria que recebera vetor
 *  @return vetor vetor de termos
 *
 */
Termo **vetorDeTermos(ArvBin *arv) {
	Termo **vetor = (Termo**) calloc(arv->termos, sizeof(Termo*));
	int *pos = (int*) malloc(sizeof(int));
	*pos = 0;
	auxVetor(vetor, arv->prim, pos);
	free(pos);
	return vetor;
}

/**
 *  Cria vetor de Termos auxiliar
 *
 *  @param vetor vetor de termos
 *  @param t termo a ser verificado
 *  @param pos posicao do termo
 *
 */
void auxVetor(Termo **vetor, Termo *t, int *pos) {
	if(t == NULL)
		return;
		
	auxVetor(vetor, t->esq, pos);
	vetor[*pos] = t;
	(*pos) += 1;
	auxVetor(vetor, t->dir, pos);
}
