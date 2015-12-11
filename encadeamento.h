#ifndef ENCADEAMENTO_H
#define ENCADEAMENTO_H

#include "arvbin.h"
#include <math.h>

typedef struct doc {
	int ocor;
	Pos *prim;
	Pos *ult;
} Doc;

typedef struct palavra {
	char *str;
	Doc **docs;
	struct palavra *prox;
} Palavra;

typedef struct hash {
	int tam;
	int numDocs;
	Palavra **chaves;
} Hash;

// Funções gerais de hash
Hash* criarHash(int tam, int numDocs);
int hashing(Hash *hash, char *str);
void indexarHash(Hash *hash, FILE *file, char **nomes);
Hash *montarHash(FILE *file, char ***nomes);

// Funções de hash específicas para o tipo
void inserirEncHash(Hash *hash, Termo *t, int numDoc);
void buscarEncHash(Hash *hash, char *str, char **nomes);

/* Funções de limpeza */
void removerHash(Hash *hash, int p);
void limparHash(Hash *hash);

#endif


