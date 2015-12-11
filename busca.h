#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "encadeamento.h"
#include "linear.h"
#include "rehashing.h"
#include "arvoreb.h"
#define PALAVRA_MAX 101

void modulo_de_busca(char tipoModelo[], char indice[], char arquivoBuscas[]);
Palavra* buscarReHashAspas(char *str);
Palavra* buscarEncHashAspas(char *str);
Palavra* buscarLinHashAspas(char *str);
Palavra* BuscarArvoreBAspas(ArvoreB *arv, char *str);
void trataBufferAspasRe(char buffer[], char **nomes);
void trataBufferAspasEnc(char buffer[], char **nomes);
void trataBufferAspasLin(char buffer[], char **nomes);
void trataBufferAspasArvoreB(char buffer[], char **nomes);
void toLower(char buffer[]);

