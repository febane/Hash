/**
 * Maquinas de Buscas
 * rehashing.c
 * Implementacao da tabela hash com
 * tratamento de colisao por rehashing
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */
 
#include "rehashing.h"

/**
 *  Funcao de rehashing
 *
 *  @param hash hash a ser trabalhada
 *  @param str string a ser rehasheada
 *  @return ((tam-2) - h % (tam-2)) valor de rehashing
 *
 */
int rehashing(Hash *hash, char *str) {
	int tam = hash->tam;
	int i = 0;
	
	int h = 1;
	int value;
	while(str[i] != '\0') {
		value = abs((int) str[i]);
		h = (h*251 + value) % (tam-2);
		i++;
	}
	return ((tam-2) - h % (tam-2));
}

/**
 *  Insere termo em hash com tratamento de rehashing
 *
 *  @param hash hash a ter termo inserido
 *  @param t termo a ser inserido
 *  @param numDoc numero do documento
 *
 */
void inserirReHash(Hash *hash, Termo *t, int numDoc) {
	int tam = hash->tam;
	int numDocs = hash->numDocs;
	Palavra **chaves = hash->chaves;
	
	Palavra *p;
	Doc *doc;
	
	int local = hashing(hash, t->str);
	int rehash = rehashing(hash, t->str);
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
		local = (local+rehash) % tam;
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
 *  Busca em hash com tratamento de rehashing
 *
 *  @param hash hash a ter string buscada
 *  @param str string a ser buscada
 *  @param nomes nomes dos documentos
 *
 */
void buscarReHash(Hash *hash, char *str, char **nomes) {
	int tam = hash->tam;
	int numDocs = hash->numDocs;
	int i;
	
	Palavra **chaves = hash->chaves;
	Palavra *p;
	
	int local = hashing(hash, str);
	int rehash = rehashing(hash, str);
	
	p = chaves[local];
	while(p != NULL) {
		if(!strcmp(str, p->str)) {
			for(i = 0; i < numDocs; i++) {
				if(p->docs[i]->ocor != 0)
					printf("%s\n", nomes[i]);
			}
			return;
		}
		local = (local+rehash) % tam;
		p = chaves[local];
	}
}





