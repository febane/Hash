/**
 * Maquinas de Buscas
 * main.c
 * Arquivo Principal para as Maquinas
 * de Buscas e tratamento de suas implementacoes
 * Ver documentacao para mais informacoes
 * 
 * @author Fernando Barbosa Neto
 * @author Jeferson de Oliveira Batista
 * 
 * @version 1.0
 * 
 */


#include "index.h"
#include "busca.h"


int main(int argc, char *argv[]) {
	
	if(argc != 5){
	
		printf("Usage: ./trab2 modulo tipoModelo arquivo1 arquivo2\n");
		return 1;
		
	}
	
	if(!strcmp(argv[1],"-i")){
		
		modulo_de_indexacao(argv[2], argv[3], argv[4]);
		
	}
	else if(!strcmp(argv[1],"-b")){
		
		modulo_de_busca(argv[2], argv[3], argv[4]);
		
	}
	else{
		
		printf("Modulo invalido.\nUsage: ./trab2 modulo tipoModelo arquivo1 arquivo2\n");
		return 1;
		
	}
	
	return 0;
}


