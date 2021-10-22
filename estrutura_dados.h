#ifndef LERTEXTOS_C_ESTRUTURA_DADOS_H
#define LERTEXTOS_C_ESTRUTURA_DADOS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TEXTO 10000

struct mensagem {
    char palavra[50];
    int ocorrencias;
    unsigned long len;
};

struct no_fila {
    struct mensagem m;
    bool ocupado;
    int mens_seguinte;
    int mens_anterior;
};

#define TAMFILA 500 /* maximo de mensagens em espera */
struct base {
    struct no_fila no[TAMFILA];
    int entrada_lista;
    int saida_lista;
    int ocupados;
    int i;
};

bool ler_texto(char *ptexto,char *modif);

void inicializar(struct base *pf);

bool colocar(struct base *pf, struct mensagem mens, int *maior);

bool retirar(struct base *pf, struct mensagem *pmens);

void print(struct mensagem mens, int maior);

#endif
