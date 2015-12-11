/**
 * Maquinas de Buscas
 * linear.c
 * Implementacao da tabela hash com
 * tratamento de colisao por hashing linear
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */
 
#include "linear.h"

/**
 *  Insere termo em hash linear
 *
 *  @param hash hash a ter termo inserido
 *  @param t termo a ser inserido
 *  @param numDoc numero do documento
 *
 */
void inserirLinHash(Hash *hash, Termo *t, int numDoc) {
	int tam = hash->tam;
	int numDocs = hash->numDocs;
	Palavra **chaves = hash->chaves;
	
	Palavra *p;
	Doc *doc;
	
	int local = hashing(hash, t->str);
	int i;
	
	p = chaves[local];
	while(p != NULL) {
		if(!strcmp(t->str, p->str)) {
			doc = p->docs[numDoc];
			doc->ocor = t->ocor;
			doc->prim = t->prim;
			doc->ult = t->ult;
			return;
		}
		local = (local+1) % tam;
		p = chaves[local];
	}
	
	chaves[local] = (Palavra*) malloc(sizeof(Palavra));
	p = chaves[local];
	p->str = t->str;
	p->prox = NULL;
	
	p->docs = (Doc**) calloc(numDocs, sizeof(Doc*));
	
	for(i = 0; i < numDocs; i++)
		p->docs[i] = (Doc*) malloc(sizeof(Doc));
	
	for(i = 0; i < numDocs; i++) {
		doc = p->docs[i];
		if(i == numDoc) {
			doc->ocor = t->ocor;
			doc->prim = t->prim;
			doc->ult = t->ult;
		}
		else {
			doc->ocor = 0;
			doc->prim = NULL;
			doc->ult = NULL;
		}
	}
}

/**
 *  Busca string em hash
 *
 *  @param hash hash a ter string buscada
 *  @param str string a ser buscada
 *  @param nomes nomes dos documentos
 *
 */
void buscarLinHash(Hash *hash, char *str, char **nomes) {
	int tam = hash->tam;
	int numDocs = hash->numDocs;
	int i;
	
	Palavra **chaves = hash->chaves;
	Palavra *p;
	
	int local = hashing(hash, str);
	
	p = chaves[local];
	while(p != NULL) {
		if(!strcmp(str, p->str)) {
			for(i = 0; i < numDocs; i++) {
				if(p->docs[i]->ocor != 0)
					printf("%s\n", nomes[i]);
			}
			return;
		}
		local = (local+1) % tam;
		p = chaves[local];
	}
}


