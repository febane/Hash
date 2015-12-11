#include "arvbin.h"
#include <locale.h>
#include <ctype.h>
#include "encadeamento.h"
#include "linear.h"
#include "rehashing.h"
#include "arvoreb.h"
#define NOME_MAX 101

void modulo_de_indexacao(char tipoModelo[], char entradaDocs[], char indiceGerado[]);
int proxPrimo(int num);
bool ehPrimo(int num);

