/**
 * Maquinas de Buscas
 * arvoreb.c
 * Implementacao da arvore b
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */
 

#include "arvoreb.h"

/**
 *  Cria arvore b
 *
 *  @param numDocs numero de documentos
 *  @return arv arvore b criada
 *
 */
ArvoreB *criarArvoreB(int numDocs) {
	ArvoreB *arv = (ArvoreB*) malloc(sizeof(ArvoreB));
	arv->numDocs = numDocs;
	arv->ocup = 0;
	
	arv->dados = (Palavra**) calloc(MAX_CHAVES, sizeof(Palavra*));
	arv->filhos = (ArvoreB**) calloc(MAX_FILHOS, sizeof(ArvoreB*));
	
	int i;
	for(i = 0; i < MAX_CHAVES; i++)
       arv->dados[i] = NULL;
	for(i = 0; i < MAX_FILHOS; i++)
		arv->filhos[i] = NULL;
	
	return arv;
}

/**
 *  Busca linear em arvore b
 *
 *  @param arv arvore b a ser feita a busca
 *  @param str string a ser procurada
 *  @return pos posicao da string
 *
 */
int busca_linear(ArvoreB *arv, char *str) 
{
	int pos;
	Palavra *p;
	
	for(pos = 0; pos < arv->ocup; pos++) {
		p = arv->dados[pos];
		if(strcmp(str, p->str) < 0)
			return pos;
	}
	
	return arv->ocup;
}

/**
 *  Insere termo caso ele ja esteja presente na árvore
 *
 *  @param arv arvore b
 *  @param t termo
 *  @param numDoc numero do documento
 *  @return TRUE se inserido com sucesso
 *  @return FALSE se nao foi inserido
 *
 */
bool inserirNovoTermo(ArvoreB *arv, Termo *t, int numDoc) 
{
	if(arv == NULL)
		return FALSE;
	
	Palavra *p;
	Doc *doc;
	
	int i, n = arv->ocup;
	for(i = 0; i < n; i++) {
		p = arv->dados[i];
		if(strcmp(t->str, p->str) < 0) {
			return (inserirNovoTermo(arv->filhos[i], t, numDoc));
		}
		
		else if(!strcmp(t->str, p->str)) {
			doc = p->docs[numDoc];
			doc->ocor = t->ocor;
			doc->prim = t->prim;
			doc->ult = t->ult;
			return TRUE;
		}
	}
	
	/* Caso a chave seja maior que todas as chaves de arv, 
	procura no filho mais à direita */
	return inserirNovoTermo(arv->filhos[n], t, numDoc);
}

/**
 *  Insere uma chave e o ponteiro para o filho da direita em um nó
 *
 *  @param raiz arvore b resultante
 *  @param p palavra
 *  @param filhodir arvore filha pela direita
 *
 */
void insere_chave(ArvoreB *raiz, Palavra *p, ArvoreB *filhodir)
{
  int k, pos;
  
  //busca para obter a posição ideal para inserir a nova chave
  pos = busca_linear(raiz, p->str);
  k = raiz->ocup;

  //realiza o remanejamento para manter as chaves ordenadas
  while (k > pos && strcmp(p->str, raiz->dados[k-1]->str) < 0)
  {
    raiz->dados[k] = raiz->dados[k-1];
    raiz->filhos[k+1] = raiz->filhos[k];
    k--;
  }
  
  raiz->dados[pos] = p;
  raiz->filhos[pos+1] = filhodir;
  raiz->ocup++;
}

/**
 *  Realiza a busca do nó para inserir a chave e faz as subdivisões quando necessárias
 *
 *  @param raiz arvore b
 *  @param p palavra
 *  @param h valor booleano
 *  @param p_retorno palavra retornada
 *  @return temp arvore b resultante
 *
 */
ArvoreB *insere(ArvoreB *raiz, Palavra *p, bool *h, Palavra **p_retorno)
{
  int i, pos;
  Palavra *info_mediano; //auxiliar para armazenar a chave que irá subir para o pai
  
  ArvoreB *temp, *filho_dir; //ponteiro para o filho à direita da chave

  if (raiz == NULL)
   {
     //O nó anterior é o ideal para inserir a nova chave (chegou em um nó folha)
     *h = TRUE;
     *p_retorno = p;
     return(NULL);
   }
  else {
         pos = busca_linear(raiz, p->str);
         
         // Acho que essa parte nunca vai ser executada
         if (raiz->ocup > pos && strcmp(raiz->dados[pos]->str, p->str) == 0)
           {
           	printf("\n\nAlgo de errado não está certo!\n\n");
           	 /* pal = raiz->dados[pos];
           	doc = pal->docs[numDoc];
			doc->ocor = p->ocor;
			doc->prim = p->prim;
			doc->ult = p->ult;
			return;
            *h = FALSE; */
           }
           
         else {
               //desce na árvore até encontrar o nó folha para inserir a chave.
               filho_dir = insere(raiz->filhos[pos], p, h, p_retorno);
               if (*h) //Se true deve inserir a info_retorno no nó.
                 {
                     if(raiz->ocup < MAX_CHAVES) //Tem espaço na página
                     {
                       insere_chave(raiz, *p_retorno, filho_dir);
                       *h = FALSE;
                     }
                   else { //Overflow. Precisa subdividir
                          temp = criarArvoreB(raiz->numDocs);

                         //elemento mediano que vai subir para o pai
                         info_mediano = raiz->dados[MIN_OCUP];

                         //insere metade do nó raiz no temp (efetua subdivisão)
                         temp->filhos[0] = raiz->filhos[MIN_OCUP+1];
                         for (i = MIN_OCUP + 1; i < MAX_CHAVES; i++)
                           insere_chave(temp, raiz->dados[i], raiz->filhos[i+1]);

                         //atualiza nó raiz. 
                         for (i = MIN_OCUP; i<MAX_CHAVES; i++)
                         {
                           raiz->dados[i] = 0;
                           raiz->filhos[i+1] = NULL;
                         }
                         raiz->ocup = MIN_OCUP;

                         //Verifica em qual nó será inserida a nova chave
                         if (pos <= MIN_OCUP)
                              insere_chave(raiz, *p_retorno, filho_dir);
                         else insere_chave(temp, *p_retorno, filho_dir);

                         //retorna o mediano para inserí-lo no nó pai e o temp como filho direito do mediano.
                         *p_retorno = info_mediano;
                         return(temp);
                       }
                 }
              }
     }
     
     // Se for executado, tem algo errado
     return NULL;
}

/**
 *  Insere termo em arvore b
 *
 *  @param raiz arvore b
 *  @param t termo a ser inserido
 *  @param numDoc numero do documento
 *  @return raiz arovre b resultante
 *
 */
ArvoreB *insere_ArvoreB(ArvoreB *raiz, Termo *t, int numDoc)
{
  // Caso o termo já esteja presente na árvore
  if(inserirNovoTermo(raiz, t, numDoc)) {
  	return raiz;
  }
  
  // Caso o termo seja novo, crie uma palavra para ele e a insira
  Palavra *p;
  Doc *doc;
  int i, n = raiz->numDocs;
  
  p = (Palavra*) malloc(sizeof(Palavra));
  p->str = t->str;
  
  p->docs = (Doc**) calloc(n, sizeof(Doc*));
  for(i = 0; i < n; i++)
  	p->docs[i] = (Doc*) malloc(sizeof(Doc));
	
	for(i = 0; i < n; i++) {
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
	p->prox = NULL;
  
  // Inserindo a palavra na posição correta
  bool h;
  Palavra *p_retorno;
  ArvoreB *filho_dir, *nova_raiz;

  filho_dir = insere(raiz, p, &h, &p_retorno);
  if (h)
   { //Aumetará a altura da árvore
     nova_raiz = criarArvoreB(raiz->numDocs);
     nova_raiz->ocup = 1;
     nova_raiz->dados[0] = p_retorno;
     nova_raiz->filhos[0] = raiz;
     nova_raiz->filhos[1] = filho_dir;
     
     for (i = 2; i <= MAX_CHAVES; i++)
       nova_raiz->filhos[i] = NULL;
     return(nova_raiz);
   }
  else return(raiz);
  
  // Se for executado, tem algo errado
  printf("Algo de errado não está certo.\n");
  return NULL;
}

/**
 *  Insere palavra em arvore b
 *
 *  @param raiz arvore b
 *  @param p palavra a ser inserida
 *  @return nova_raiz arvore b resultante
 *
 */
ArvoreB *inserirArvoreB(ArvoreB *raiz, Palavra *p) {
	bool h;
	int i;
  	Palavra *p_retorno;
   	ArvoreB *filho_dir, *nova_raiz;

	filho_dir = insere(raiz, p, &h, &p_retorno);
  	if (h)
   	{ //Aumentará a altura da árvore
    	nova_raiz = criarArvoreB(raiz->numDocs);
    	nova_raiz->ocup = 1;
    	nova_raiz->dados[0] = p_retorno;
	    nova_raiz->filhos[0] = raiz;
    	nova_raiz->filhos[1] = filho_dir;
     
    	for(i = 2; i <= MAX_CHAVES; i++)
    		nova_raiz->filhos[i] = NULL;
    	return(nova_raiz);
   	}
  	else return(raiz);
  
  	// Se for executado, tem algo errado
  	printf("Algo de errado não está certo.\n");
  	return NULL;
}

/**
 *  Indexa arvore b
 *
 *  @param arv arvore b
 *  @param file arquivo a ser criado indice
 *  @param nomes nomes dos arquivos
 *
 */
void indexarArvoreB(ArvoreB *arv, FILE *file, char **nomes) {
	
	int numDocs = arv->numDocs;
	int i;
		
	// Cabeçalho com número de documentos
	fprintf(file, "%d\n\n", numDocs);
	
	for(i = 0; i < numDocs; i++) {
		fprintf(file, "%s\n", nomes[i]);
	}
	
	fprintf(file, "\n");
	
	indexarAux(arv, file);
	
	// Marca de encerramento da indexação
	fprintf(file, "*");
}

/**
 *  Indexa arvore b auxiliar
 *
 *  @param arv arvore b
 *  @param file arquivo a ser criado indice
 *
 */
void indexarAux(ArvoreB *arv, FILE *file) {
	if(arv == NULL)
		return;
	
	Palavra *atual;
	Doc *doc;
	Pos *p;
	int numDocs = arv->numDocs;
	int i, j, n;
	
	n = arv->ocup;
	for(i = 0; i < n; i++) {
		indexarAux(arv->filhos[i], file);
		atual = arv->dados[i];
		
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
	}
	
	indexarAux(arv->filhos[n], file);
}

/**
 *  Monta arvore b
 *
 *  @param file arquivo a ser lido
 *  @param nomes nomes dos documentos
 *  @return arv arvore montada
 *
 */
ArvoreB *montarArvoreB(FILE *file, char ***nomes) {
	int numDocs, m, comp, doc, ocor, pos, i, j;
	char buffer[101];
	
	fscanf(file, "%d", &numDocs);
	
	ArvoreB *arv = criarArvoreB(numDocs);
	Palavra *termo;
	Doc *d;
	Pos *p, *paux;
	
	*nomes = (char**) calloc(numDocs, sizeof(char*));
	for(i = 0; i < numDocs; i++) {
		fscanf(file, "%s", buffer);
		m = strlen(buffer);
		(*nomes)[i] = (char*) calloc(m+1, sizeof(char));
		strcpy((*nomes)[i], buffer);
	}
	
	fscanf(file, "%s", buffer);
	while(buffer[0] != '*') {
		termo = (Palavra*) malloc(sizeof(Palavra));
		
		comp = strlen(buffer);
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
		termo->prox = NULL;
		
		// Inserindo a palavra na posição correta
  		arv = inserirArvoreB(arv, termo);
		
		fscanf(file, "%s", buffer);
	}
	
	return arv;
}

/**
 *  Realiza busca em arvore b
 *
 *  @param arv arvore b
 *  @param str string a ser buscada
 *  @param nomes nomes dos documentos
 *
 */
void buscarArvoreB(ArvoreB *arv, char *str, char **nomes) {
	if(arv == NULL)
		return;
	
	int numDocs = arv->numDocs;
	int n = arv->ocup;
	int i, j;
	
	Palavra *termo;
	
	for(i = 0; i < n; i++) {
		termo = arv->dados[i];
		
		if(strcmp(str, termo->str) < 0) {
			buscarArvoreB(arv->filhos[i], str, nomes);
		}
		
		if(!strcmp(str, termo->str)) {
			for(j = 0; j < numDocs; j++) {
				if(termo->docs[j]->ocor != 0)
					printf("%s\n", nomes[j]);
			}
			return;
		}
	}
	
	buscarArvoreB(arv->filhos[n], str, nomes);
}


