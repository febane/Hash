#include "encadeamento.h"

// As funções gerais de hash são herdadas de encadeamento.h

// Funções de hash específicas para o tipo
void inserirLinHash(Hash *hash, Termo *t, int numDoc);
void buscarLinHash(Hash *hash, char *str, char **nomes);


