#include "encadeamento.h"

// As funções gerais de hash são herdadas de encadeamento.h

// Funções de hash específicas para o tipo
int rehashing(Hash *hash, char *str);
void inserirReHash(Hash *hash, Termo *t, int numDoc);
void buscarReHash(Hash *hash, char *str, char **nomes);

