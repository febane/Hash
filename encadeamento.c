/**
 * Maquinas de Buscas
 * encadeamento.c
 * Implementacao da tabela hash com
 * tratamento de colisao por encadeamento
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */
 
#include "encadeamento.h"
 
/**
 *  Cria hash
 *
 *  @param tam tamanho da hash
 *  @param numDocs numeros de documentos
 *  @return hash hash criada
 *
 */
Hash* criarHash(int tam, int numDocs) {
	Hash *hash = (Hash*) malloc(sizeof(Hash));
	hash->tam = tam;
	hash->numDocs = numDocs;
	hash->chaves = (Palavra**) calloc(tam, sizeof(Palavra*));
	
	int i;
	for(i = 0; i < tam; i++)
		hash->chaves[i] = NULL;
		
	return hash;
}

/**
 *  Funcao de hashing
 *
 *  @param hash hash base para o hashing
 *  @param str a ser hasheada
 *  @return (h % tam) valor de hashing
 *
 */
int hashing(Hash *hash, char *str) {
	int tam = hash->tam;
	int i = 0;
	
	int h = 1;
	int value;
	while(str[i] != '\0') {
		value = abs((int) str[i]);
		h = (h*251 + value) % tam;
		i++;
	}
	return (h % tam);
}

/**
 *  Inserir em hash encadeada
 *
 *  @param hash hash a ter termo inserido
 *  @param t termo a ser inserido
 *  @param numDoc numero do documento
 *
 */
void inserirEncHash(Hash *hash, Termo *t, int numDoc) {
	int local = hashing(hash, t->str);
	Palavra *atual = hash->chaves[local];

	Palavra *ult = NULL;
	Doc *doc;
	int i, n = hash->numDocs;
	
	while(atual != NULL) {
		if(!strcmp(atual->str, t->str)) {
			doc = atual->docs[numDoc];
			doc->ocor = t->ocor;
			doc->prim = t->prim;
			doc->ult = t->ult;
			return;
		}
		ult = atual;
		atual = atual->prox;
	}
	
	atual = (Palavra*) malloc(sizeof(Palavra));
	atual->str = t->str;
	
	atual->docs = (Doc**) calloc(n, sizeof(Doc*));
	for(i = 0; i < n; i++)
		atual->docs[i] = (Doc*) malloc(sizeof(Doc));
	
	for(i = 0; i < n; i++) {
		doc = atual->docs[i];
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
	atual->prox = NULL;
	
	if(ult != NULL)
		ult->prox = atual;
	else
		hash->chaves[local] = atual;
}

/**
 *  Indexar hash
 *
 *  @param hash hash a ser indexada
 *  @param file arquivo de indice
 *  @param nomes nomes dos documentos
 *
 */
void indexarHash(Hash *hash, FILE *file, char **nomes) {
	Palavra *atual;
	Doc *doc;
	Pos *p;
	int numDocs = hash->numDocs;
	int i, j, n = hash->tam;
	
	// Cabeçalho com tamanho da hash e número de documentos
	fprintf(file, "%d %d\n\n", n, numDocs);
	
	for(i = 0; i < numDocs; i++) {
		fprintf(file, "%s\n", nomes[i]);
	}
	
	fprintf(file, "\n");
	
	for(i = 0; i < n; i++) {
		atual = hash->chaves[i];
		if(atual == NULL)
			continue;
		
		fprintf(file, "%d\n", i);
		while(atual != NULL) {
			fprintf(file, "%s ", atual->str);
			
			for(j = 0; j < numDocs; j++) {
				doc = atual->docs[j];
				fprintf(file, "%d ", j);
				fprintf(file, "%d ", doc->ocor);
				
				p = doc->prim;
				while(p != NULL) {
					fprintf(file, "%d ", p->pos);
					p = p->prox;
				}
			}
			fprintf(file, "\n");
			atual = atual->prox;
		}
		fprintf(file, "*end*\n\n");
	}
	
	// Marca de encerramento da indexação
	fprintf(file, "%d\n", n);
	// Não há o local n na hash de tamanho n
}

/**
 *  Monta hash
 *
 *  @param file arquivo que criara hash
 *  @param nomes nomes dos documentos
 *  @return hash hash montada
 *
 */
Hash* montarHash(FILE *file, char ***nomes) {
	Hash *hash = (Hash*) malloc(sizeof(Hash));
	int tam, numDocs, i, j, ocor, m;
	char buffer[101];
	
	fscanf(file, "%d %d", &tam, &numDocs);
	hash->tam = tam;
	hash->numDocs = numDocs;
	
	*nomes = (char**) calloc(numDocs, sizeof(char*));
	for(i = 0; i < numDocs; i++) {
		fscanf(file, "%s", buffer);
		m = strlen(buffer);
		(*nomes)[i] = (char*) calloc(m+1, sizeof(char));
		strcpy((*nomes)[i], buffer);
	}
	
	hash->chaves = (Palavra**) calloc(tam, sizeof(Palavra*));
	
	for(i = 0; i < tam; i++) {
		hash->chaves[i] = NULL;
	}
	
	int local, comp, doc, pos;
	Palavra *termo, *termoaux;
	Doc *d;
	Pos *p, *paux;
	
	while(1) {
		fscanf(file, "%d", &local);
		
		if(local == tam)
			break;
			
		while(1) {
			fscanf(file, "%s", buffer);
			
			if(!strcmp(buffer, "*end*"))
				break;
			
			comp = strlen(buffer);
			termo = (Palavra*) malloc(sizeof(Palavra));
			termo->str = (char*) calloc(comp+1, sizeof(char));
			strcpy(termo->str, buffer);
			
			termo->docs = (Doc**) calloc(numDocs, sizeof(Doc*));
			
			for(i = 0; i < numDocs; i++) {
				termo->docs[i] = (Doc*) malloc(sizeof(Doc));
			}
			
			for(i = 0; i < numDocs; i++) {
				fscanf(file, "%d", &doc);
				d = termo->docs[doc];
				fscanf(file, "%d", &ocor);
				d->ocor = ocor;
				
				if(ocor == 0) {
					d->prim = d->ult = NULL;
				}
				else {
					fscanf(file, "%d", &pos);
					p = (Pos*) malloc(sizeof(Pos));
					p->pos = pos;
					d->prim = p;
					paux = p;
					
					for(j = 1; j < ocor; j++) {
						fscanf(file, "%d", &pos);
						p = (Pos*) malloc(sizeof(Pos));
						p->pos = pos;
						paux->prox = p;
						paux = p;
					}
					p->prox = NULL;
					d->ult = p;
				}
			}
			
			if(hash->chaves[local] == NULL)
				hash->chaves[local] = termo;
			else
				termoaux->prox = termo;
			
			termoaux = termo;
		}
		termo->prox = NULL;
	}
	return hash;
}	

/**
 *  Busca em hash encadeada
 *
 *  @param hash hash a ser procurado termo
 *  @param str string a ser buscada
 *  @param nomes nomes dos documentos
 *
 */
void buscarEncHash(Hash *hash, char *str, char **nomes) {
	int numDocs = hash->numDocs;
	int i;
	
	int local = hashing(hash, str);
	
	Palavra *termo = hash->chaves[local];
	
	while(termo != NULL) {
		if(!strcmp(termo->str, str)) {
			for(i = 0; i < numDocs; i++) {
				if(termo->docs[i]->ocor != 0)
					printf("%s\n", nomes[i]);
			}
			break;
		}
		termo = termo->prox;
	}
}

void removerHash(Hash *hash, int p) {
	Palavra **chaves = hash->chaves;
	int i, numDocs = hash->numDocs;

	if(chaves[p] != NULL) {
		for(i = 0; i < numDocs; i++) {
			free(chaves[p]->docs[i]);
		}
		free(chaves[p]->docs);
		free(chaves[p]);
		return;
	}
}

void limparHash(Hash *hash) {
	int i, tam = hash->tam;
	
	for(i = 0; i < tam; i++)
		removerHash(hash, i);
}


