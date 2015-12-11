/**
 * Maquinas de Buscas
 * index.c
 * Tratamento do modulo de indexacao
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */


#include "index.h"

/**
 *  Funcao principal do modulo de indexacao
 *
 *  @param tipoModelo tipo de hash ou arvore b
 *  @param entradaDocs documentos de entrada
 *  @param indiceGerado indice a ser gerado
 *
 */
void modulo_de_indexacao(char tipoModelo[], char entradaDocs[], char indiceGerado[]){
	
	setlocale(LC_ALL, "pt_BR_utf8");
	
	FILE *f, *arquivo, *saida;
	char nome[NOME_MAX], buffer[NOME_MAX];
	int numDocs = 0;
	int docAtual = 0;
	char **nomeDocs;
	int numTermos, tamHash;
	ArvBin *global = criarArvBin(NULL);
	ArvBin **registro;
	ArvBin *arv;
	int i, j;
	
	f = fopen(entradaDocs, "r");
	saida = fopen(indiceGerado, "w");
	
	if(f == NULL) {
		
		printf("Erro ao acessar %s\n", entradaDocs);
		return;
		
	}
	
	if(saida == NULL) {
		
		printf("Erro ao acessar %s\n", indiceGerado);
		return;
		
	}
	
	while(fscanf(f, "%s", nome) != EOF) {
		numDocs++;
	}
	
	registro = (ArvBin**) calloc(numDocs, sizeof(ArvBin*));
	
	f = fopen(entradaDocs, "r");
	
	while(fscanf(f, "%s", nome)!=EOF){
		
		arquivo = fopen(nome, "r");
		arv = criarArvBin(nome);
		if(arquivo == NULL){
			
			printf("Error ao acessar %s\n", nome);
			
		}
		else{
			int pos = 0;
			while(fscanf(arquivo, "%[a-zA-ZÀ-ÿ0-9-]%*[^a-zA-ZÀ-ÿ0-9-]", buffer)!=EOF){
				
				for(i=0;i<strlen(buffer);i++){
					
					if(((buffer[i])&0xC0)==0x80 && buffer[i]<0xffffffa0)
						buffer[i] = buffer[i] + 0x20;
					else
						buffer[i] = tolower(buffer[i]);
					
				}
				
				inserirTermo(arv, buffer, pos);
				inserirTermo(global, buffer, pos);
				pos++;
				
			}
			// imprimirTermos(arv);
			registro[docAtual] = arv;
			docAtual++;
				
			fclose(arquivo);
		}
	}
	// imprimirTermos(global);
	numTermos = global->termos;
	
	int n, m;
	
	nomeDocs = (char**) calloc(numDocs, sizeof(char*));
	
	for(i = 0; i < numDocs; i++) {
		m = strlen(registro[i]->doc);
		nomeDocs[i] = (char*) calloc(m+1, sizeof(char));
		strcpy(nomeDocs[i], registro[i]->doc);
	}
	
	if(!strcmp(tipoModelo,"E")) {
		tamHash = proxPrimo(2*numTermos+1);
		Hash *hash = criarHash(tamHash, numDocs);
		for(i = 0; i < numDocs; i++) {
			Termo **vetor = vetorDeTermos(registro[i]);
			n = registro[i]->termos;
			for(j = 0; j < n; j++) {
				inserirEncHash(hash, vetor[j], i);
			}
			free(vetor);
		}
		
		indexarHash(hash, saida, nomeDocs);
		limparHash(hash);
	}
	else if(!strcmp(tipoModelo,"L")) {
		tamHash = proxPrimo(2*numTermos+1);
		Hash *hash = criarHash(tamHash, numDocs);
		for(i = 0; i < numDocs; i++) {
			Termo **vetor = vetorDeTermos(registro[i]);
			n = registro[i]->termos;
			for(j = 0; j < n; j++) {
				inserirLinHash(hash, vetor[j], i);
			}
			free(vetor);
		}
		
		indexarHash(hash, saida, nomeDocs);
		limparHash(hash);
	}
	else if(!strcmp(tipoModelo,"B")){
		ArvoreB *arv = criarArvoreB(numDocs);
		for(i = 0; i < numDocs; i++) {
			Termo **vetor = vetorDeTermos(registro[i]);
			n = registro[i]->termos;
			for(j = 0; j < n; j++) {
				arv = insere_ArvoreB(arv, vetor[j], i);
			}
		}
		indexarArvoreB(arv, saida, nomeDocs);
	}
	else if(!strcmp(tipoModelo,"R")) {
		tamHash = proxPrimo(2*numTermos+1);
		Hash *hash = criarHash(tamHash, numDocs);
		for(i = 0; i < numDocs; i++) {
			Termo **vetor = vetorDeTermos(registro[i]);
			n = registro[i]->termos;
			for(j = 0; j < n; j++) {
				inserirReHash(hash, vetor[j], i);
			}
			free(vetor);
		}
		
		indexarHash(hash, saida, nomeDocs);
		limparHash(hash);
	}
	
	freeArvBin(global);
	
	for(i = 0; i < numDocs; i++) {
		freeArvBin(registro[i]);
		free(nomeDocs[i]);
	}
	free(registro);
	free(nomeDocs);
	
	fclose(f); 
	fclose(saida);
}

/**
 *  Funcao que calcula o proximo numero primo dado um numero impar
 *
 *  @param num numero impar de entrada
 *  @return retorna o menor numero maior ou igual a num que eh primo
 *
 */

int proxPrimo(int num) {
	while(!ehPrimo(num))
		num += 2;
	return num;
}

/**
 *  Funcao que avalia se um dado numero impar eh primo
 *
 *  @param num numero impar a ser avaliado
 *  @return retorna se o numero eh primo
 *
 */

bool ehPrimo(int num) {
	int i = 2;
	while(i*i <= num) {
		if(num % i == 0)
			return FALSE;
		i++;
	}
	return TRUE;
}


