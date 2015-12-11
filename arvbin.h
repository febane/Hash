#ifndef ARVBIN_H
#define ARVBIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pos {
	int pos;
	struct pos *prox;
} Pos;

typedef struct termo {
	char *str;
	int ocor;
	Pos *prim;
	Pos *ult;
	struct termo *esq;
	struct termo *dir;
} Termo;

typedef struct arvBin {
	int termos;
	char *doc;
	Termo *prim;
} ArvBin;

// Principais
ArvBin *criarArvBin(char *doc);
void inserirTermo(ArvBin *arv, char *str, int pos);
void freeArvBin(ArvBin *arv);
void imprimirTermos(ArvBin *arv);
Termo **vetorDeTermos(ArvBin *arv);

// Auxiliares
Termo *criarTermo(char *str, int pos);
int inserirAux(Termo **t, char *str, int pos);
void imprimirAux(Termo *t);
void freeTermo(Termo *t);
void auxVetor(Termo **vetor, Termo *t, int *pos);

#endif


