/**
 * Maquinas de Buscas
 * busca.c
 * Tratamento do modulo de busca
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */


#include "busca.h"

FILE *file;
Hash *hash;
ArvoreB *arv;
FILE *busca;

/**
 *  Funcao principal do modulo de busca
 *
 *  @param tipoModelo tipo de hash ou arvore b
 *  @param indice nome do arquivo que contem indice
 *  @param arquivoBuscas nome do arquivo que contem queries
 *
 */
void modulo_de_busca(char tipoModelo[], char indice[], char arquivoBuscas[]){
	
	setlocale(LC_ALL, "pt_BR_utf8");
	
	char buffer[PALAVRA_MAX];
	char **nomes = NULL;
	
	file = fopen(indice, "r");
	// hash = montarHash(file, &nomes);
	busca = fopen(arquivoBuscas, "r");
	
	if(!strcmp(tipoModelo,"E")) {
		hash = montarHash(file, &nomes);
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer) != EOF) {
			toLower(buffer);
			if(buffer[0]!='\"'){
				printf("%s\n", buffer);
				buscarEncHash(hash, buffer, nomes);
			}
			else if(strlen(buffer)>1 && buffer[strlen(buffer)-1]=='\"'){
				memmove(&buffer[0], &buffer[1], strlen(buffer));
				buffer[strlen(buffer)-1] = '\0';
				printf("%s\n", buffer);
				buscarEncHash(hash, buffer, nomes);
			}
			else
				trataBufferAspasEnc(buffer, nomes);
		}
	}
	else if(!strcmp(tipoModelo,"L")) {
		hash = montarHash(file, &nomes);
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer) != EOF) {
			toLower(buffer);
			if(buffer[0]!='\"'){
				printf("%s\n", buffer);
				buscarLinHash(hash, buffer, nomes);
			}
			else if(strlen(buffer)>1 && buffer[strlen(buffer)-1]=='\"'){
				memmove(&buffer[0], &buffer[1], strlen(buffer));
				buffer[strlen(buffer)-1] = '\0';
				printf("%s\n", buffer);
				buscarLinHash(hash, buffer, nomes);
			}
			else
				trataBufferAspasLin(buffer, nomes);
		}
	}
	else if(!strcmp(tipoModelo,"B")) {
		arv = montarArvoreB(file, &nomes);
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer) != EOF) {
			toLower(buffer);
			if(buffer[0]!='\"'){
				printf("%s\n", buffer);
				buscarArvoreB(arv, buffer, nomes);
			}
			else if(strlen(buffer)>1 && buffer[strlen(buffer)-1]=='\"'){
				memmove(&buffer[0], &buffer[1], strlen(buffer));
				buffer[strlen(buffer)-1] = '\0';
				printf("%s\n", buffer);
				buscarArvoreB(arv, buffer, nomes);
			}
			else
				trataBufferAspasArvoreB(buffer, nomes);
		}
	}
	else if(!strcmp(tipoModelo,"R")){
		hash = montarHash(file, &nomes);
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer) != EOF) {
			toLower(buffer);
			if(buffer[0]!='\"'){
				printf("%s\n", buffer);
				buscarReHash(hash, buffer, nomes);
			}
			else if(strlen(buffer)>1 && buffer[strlen(buffer)-1]=='\"'){
				printf("%s\n", buffer);
				memmove(&buffer[0], &buffer[1], strlen(buffer));
				buffer[strlen(buffer)-1] = '\0';
				buscarReHash(hash, buffer, nomes);
			}
			else
				trataBufferAspasRe(buffer, nomes);
		}
	}
	
	fclose(file);
	fclose(busca);
	
}

/**
 *  Busca na hash com rehashing com query entre aspas
 *
 *  @param str string a ser buscada
 *  @return p palavra encontrada
 *
 */
Palavra* buscarReHashAspas(char *str) {
	int tam = hash->tam;
	
	Palavra **chaves = hash->chaves;
	Palavra *p;
	
	int local = hashing(hash, str);
	int rehash = rehashing(hash, str);
	
	p = chaves[local];
	while(p != NULL) {
		
		if(!strcmp(str, p->str))
			return p;
		
		local = (local+rehash) % tam;
		p = chaves[local];
		
	}
	
	return NULL;
	
}

/**
 *  Busca na hash encadeada com query entre aspas
 *
 *  @param str string a ser buscada
 *  @param nomes nomes dos documentos
 *  @return p palavra encontrada
 *
 */
Palavra* buscarEncHashAspas(char *str) {
	
	int local = hashing(hash, str);
	
	Palavra *termo = hash->chaves[local];
	
	while(termo != NULL) {
		if(!strcmp(termo->str, str)) {
			return termo;
		}
		termo = termo->prox;
	}
	return NULL;
}

/**
 *  Busca na hash linear com query entre aspas
 *
 *  @param str string a ser buscada
 *  @param nomes nomes dos documentos
 *  @return p palavra encontrada
 *
 */
Palavra* buscarLinHashAspas(char *str) {
	int tam = hash->tam;
	
	Palavra **chaves = hash->chaves;
	Palavra *p;
	
	int local = hashing(hash, str);
	
	p = chaves[local];
	while(p != NULL) {
		if(!strcmp(str, p->str)) {
			return p;
		}
		local = (local+1) % tam;
		p = chaves[local];
	}
	return NULL;
}

/**
 *  Busca na arvore b com query entre aspas
 *
 *  @param arv arvore b a ter palavra procurada
 *  @param str string a ser buscada
 *  @return p palavra encontrada
 *
 */
Palavra* BuscarArvoreBAspas(ArvoreB *arv, char *str) {
	if(arv == NULL)
		return NULL;
	
	int n = arv->ocup;
	int i;
	
	Palavra *termo;
	
	for(i = 0; i < n; i++) {
		termo = arv->dados[i];
		
		if(strcmp(str, termo->str) < 0) {
			return BuscarArvoreBAspas(arv->filhos[i], str);
		}
		
		if(!strcmp(str, termo->str)) {
			return termo;
		}
	}
	
	return BuscarArvoreBAspas(arv->filhos[n], str);
}

/**
 *  Tratamento de query entre aspas na hash com rehashing
 *
 *  @param buffer expressao de query entre aspas
 *  @param nomes nomes dos documentos
 *
 */
void trataBufferAspasRe(char buffer[], char **nomes) {
	Palavra *p;
	Pos *posdoc;
	int i, cont=0;
	char buscado[1000];
	
	strcpy(buscado, buffer); 
	
	if(strlen(buffer)>1){
	
		memmove(&buffer[0], &buffer[1], strlen(buffer));
		p = buscarReHashAspas(buffer);
		
	}
	else{
		
		fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer);
		toLower(buffer);
		p = buscarReHashAspas(buffer);
		strcat(buscado, buffer);
		
	}
	if(p==NULL){
		
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
			strcat(buscado, " ");
			strcat(buscado, buffer);
			if(buffer[strlen(buffer)-1]=='\"'){
				printf("%s\n", buscado);
				return;
			}
		}
		
	}
	
	for(i = 0; i < hash->numDocs; i++)
		cont += p->docs[i]->ocor;
		
	int ok[cont], j=0, k, encontra;
	int docok[cont];
	
	for(i = 0; i < hash->numDocs; i++) {
		posdoc = p->docs[i]->prim;
		while(posdoc!=NULL){
			docok[j] = i;
			ok[j] = posdoc->pos;
			j++;
			posdoc = posdoc->prox;	
		}
	}
	
	j=0;
	
	while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
		toLower(buffer);
		j++;
		strcat(buscado, " ");
		strcat(buscado, buffer);
		if(buffer[strlen(buffer)-1]=='\"')
			break;
		
		p = buscarReHashAspas(buffer);
		if(p==NULL){
			while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
				strcat(buscado, " ");
				strcat(buscado, buffer);
				if(buffer[strlen(buffer)-1]=='\"'){
					printf("%s\n", buscado);
					return;
				}
			}
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
		
	}
	
	if(strlen(buffer)>1){
		
		buffer[strlen(buffer)-1] = '\0';
		
		p = buscarReHashAspas(buffer);
		if(p==NULL){
			printf("%s\n", buscado);
			return;
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
	}
	
	printf("%s\n", buscado);
	for(i=0;i<cont;i++)
		if(ok[i]!=-1000&&docok[i]!=k){
			printf("%s\n", nomes[docok[i]]);
			k = docok[i];
		}
	
}

/**
 *  Tratamento de query entre aspas na hash encadeada
 *
 *  @param buffer expressao de query entre aspas
 *  @param nomes nomes dos documentos
 *
 */
void trataBufferAspasEnc(char buffer[], char **nomes) {
	Palavra *p;
	Pos *posdoc;
	int i, cont=0;
	char buscado[1000];
	
	strcpy(buscado, buffer); 
	
	if(strlen(buffer)>1){
	
		memmove(&buffer[0], &buffer[1], strlen(buffer));
		p = buscarEncHashAspas(buffer);
		
	}
	else{
		
		fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer);
		toLower(buffer);
		p = buscarEncHashAspas(buffer);
		strcat(buscado, buffer);
		
	}
	if(p==NULL){
		
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
			strcat(buscado, " ");
			strcat(buscado, buffer);
			if(buffer[strlen(buffer)-1]=='\"'){
				printf("%s\n", buscado);
				return;
			}
		}
		
	}
	
	for(i = 0; i < hash->numDocs; i++)
		cont += p->docs[i]->ocor;
		
	int ok[cont], j=0, k, encontra;
	int docok[cont];
	
	for(i = 0; i < hash->numDocs; i++) {
		posdoc = p->docs[i]->prim;
		while(posdoc!=NULL){
			docok[j] = i;
			ok[j] = posdoc->pos;
			j++;
			posdoc = posdoc->prox;	
		}
	}
	
	j=0;
	
	while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
		toLower(buffer);
		j++;
		strcat(buscado, " ");
		strcat(buscado, buffer);
		if(buffer[strlen(buffer)-1]=='\"')
			break;
		
		p = buscarEncHashAspas(buffer);
		if(p==NULL){
			while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
				strcat(buscado, " ");
				strcat(buscado, buffer);
				if(buffer[strlen(buffer)-1]=='\"'){
					printf("%s\n", buscado);
					return;
				}
			}
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
		
	}
	
	if(strlen(buffer)>1){
		
		buffer[strlen(buffer)-1] = '\0';
		
		p = buscarEncHashAspas(buffer);
		if(p==NULL){
			printf("%s\n", buscado);
			return;
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
	}
	
	printf("%s\n", buscado);
	for(i=0;i<cont;i++)
		if(ok[i]!=-1000&&docok[i]!=k){
			printf("%s\n", nomes[docok[i]]);
			k = docok[i];
		}
	
}

/**
 *  Tratamento de query entre aspas na hash linear
 *
 *  @param buffer expressao de query entre aspas
 *  @param nomes nomes dos documentos
 *
 */
void trataBufferAspasLin(char buffer[], char **nomes) {
	Palavra *p;
	Pos *posdoc;
	int i, cont=0;
	char buscado[1000];
	
	strcpy(buscado, buffer); 
	
	if(strlen(buffer)>1){
	
		memmove(&buffer[0], &buffer[1], strlen(buffer));
		p = buscarLinHashAspas(buffer);
		
	}
	else{
		
		fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer);
		toLower(buffer);
		p = buscarLinHashAspas(buffer);
		
		strcat(buscado, buffer);
		
	}
	if(p==NULL){
		
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
			strcat(buscado, " ");
			strcat(buscado, buffer);
			if(buffer[strlen(buffer)-1]=='\"'){
				printf("%s\n", buscado);
				return;
			}
		}
		
	}
	
	for(i = 0; i < hash->numDocs; i++)
		cont += p->docs[i]->ocor;
		
	int ok[cont], j=0, k, encontra;
	int docok[cont];
	
	for(i = 0; i < hash->numDocs; i++) {
		posdoc = p->docs[i]->prim;
		while(posdoc!=NULL){
			docok[j] = i;
			ok[j] = posdoc->pos;
			j++;
			posdoc = posdoc->prox;	
		}
	}
	
	j=0;
	
	while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
		toLower(buffer);
		j++;
		strcat(buscado, " ");
		strcat(buscado, buffer);
		if(buffer[strlen(buffer)-1]=='\"')
			break;
		
		p = buscarLinHashAspas(buffer);
		if(p==NULL){
			while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
				strcat(buscado, " ");
				strcat(buscado, buffer);
				if(buffer[strlen(buffer)-1]=='\"'){
					printf("%s\n", buscado);
					return;
				}
			}
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
		
	}
	
	if(strlen(buffer)>1){
		
		buffer[strlen(buffer)-1] = '\0';
		
		p = buscarLinHashAspas(buffer);
		if(p==NULL){
			printf("%s\n", buscado);
			return;
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
	}
	
	printf("%s\n", buscado);
	for(i=0;i<cont;i++)
		if(ok[i]!=-1000&&docok[i]!=k){
			printf("%s\n", nomes[docok[i]]);
			k = docok[i];
		}
	
}

/**
 *  Tratamento de query entre aspas para arvore b
 *
 *  @param buffer expressao de query entre aspas
 *  @param nomes nomes dos documentos
 *
 */
void trataBufferAspasArvoreB(char buffer[], char **nomes) {
	Palavra *p;
	Pos *posdoc;
	int i, cont=0;
	char buscado[1000];
	
	strcpy(buscado, buffer); 
	
	if(strlen(buffer)>1){
	
		memmove(&buffer[0], &buffer[1], strlen(buffer));
		p = BuscarArvoreBAspas(arv, buffer);
		
	}
	else{
		
		fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer);
		toLower(buffer);
		p = BuscarArvoreBAspas(arv, buffer);
		strcat(buscado, buffer);
		
	}
	if(p==NULL){
		
		while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
			strcat(buscado, " ");
			strcat(buscado, buffer);
			if(buffer[strlen(buffer)-1]=='\"'){
				printf("%s\n", buscado);
				return;
			}
		}
		
	}
	
	for(i = 0; i < arv->numDocs; i++)
		cont += p->docs[i]->ocor;
		
	int ok[cont], j=0, k, encontra;
	int docok[cont];
	
	for(i = 0; i < arv->numDocs; i++) {
		posdoc = p->docs[i]->prim;
		while(posdoc!=NULL){
			docok[j] = i;
			ok[j] = posdoc->pos;
			j++;
			posdoc = posdoc->prox;	
		}
	}
	
	j=0;
	
	while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
		toLower(buffer);
		j++;
		strcat(buscado, " ");
		strcat(buscado, buffer);
		if(buffer[strlen(buffer)-1]=='\"')
			break;
		
		p = BuscarArvoreBAspas(arv, buffer);
		if(p==NULL){
			while(fscanf(busca, "%[\"a-zA-ZÀ-ÿ0-9-]%*[^\"a-zA-ZÀ-ÿ0-9-]", buffer)){
				strcat(buscado, " ");
				strcat(buscado, buffer);
				if(buffer[strlen(buffer)-1]=='\"'){
					printf("%s\n", buscado);
					return;
				}
			}
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
		
	}
	
	if(strlen(buffer)>1){
		
		buffer[strlen(buffer)-1] = '\0';
		
		p = BuscarArvoreBAspas(arv, buffer);
		if(p==NULL){
			printf("%s\n", buscado);
			return;
		}
		
		for(k=0;k<cont;k++){
			encontra = 0;
			posdoc = p->docs[docok[k]]->prim;
			while(posdoc!=NULL){
				if(posdoc->pos == ok[k]+j)
					encontra=1;	
				posdoc = posdoc->prox;
			}
			if(encontra==0)
				ok[k]=-1000;
		}
	}
	
	printf("%s\n", buscado);
	for(i=0;i<cont;i++)
		if(ok[i]!=-1000&&docok[i]!=k){
			printf("%s\n", nomes[docok[i]]);
			k = docok[i];
		}
	
}

/**
 *  Transforma caracteres UTF-8 em minúsculas
 *
 *  @param buffer string a ter suas letras transformadas em minusculas
 *
 */
void toLower(char buffer[]){
	int i;
	for(i=0;i<strlen(buffer);i++){
		if(((buffer[i])&0xC0)==0x80 && buffer[i]<0xffffffa0)
			buffer[i] = buffer[i] + 0x20;
		else
			buffer[i] = tolower(buffer[i]);				
	}	
}
