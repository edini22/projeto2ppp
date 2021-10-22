#include "estrutura_dados.h"

static char base[] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'c', 'c',
                      'e', 'e', 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 'i', 'i',
                      'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'u', 'u',
                      'u', 'u', 'u', 'u', '\0'};

static void colocar_minusculas(char *dest, const char *orig) {
    int i = 0, j = 0, c;
    c = (unsigned char) orig[0];
    while (i < 2 ) {
        if (c == 0){
            dest[j] = '\0';
            break;
        }
        else if ((c & 0x80) == 0) {
            // é um carater ASCII, basta converter as letras para minusculas
            if (c <= 'Z' && c >= 'A')
                c = c + 'a' - 'A';
            i++;
            dest[j] = (char) c;
            j++;
        }
        c = (unsigned char) orig[i];
    }
    dest[j] = 0;
}

static int comparar(char a[50], char b[50]) {//comparar no maximo 2 carateres das palavras
    char in[50],ot[50];
    colocar_minusculas(in,a);
    colocar_minusculas(ot,b);
    in[2] = ot[2] = '\0';
    int n;
    n = strcoll(in, ot);
    return n;

}

void inicializar(struct base *pf) {
    pf->ocupados = 0;
    // a fila está inicialmente vazia
    pf->entrada_lista = TAMFILA - 1;
    pf->saida_lista = TAMFILA - 1;
    pf->i = TAMFILA - 1;
    for (int i = 0; i < TAMFILA; i++)
        pf->no[i].ocupado = false;
}

bool colocar(struct base *pf, struct mensagem mens, int *maior) {
    int n = 0;//numero de palavras repetidas, neste caso um maximo de 1
    int local = 0;//i onde a palavra ja estava na lista(indice do array do nos)
    for (int l = TAMFILA -1; l >= 0; l--) {
        if (pf->no[l].ocupado) {
            if (strcmp(mens.palavra, pf->no[l].m.palavra) == 0) {
                n++;
                local = l;
            }
        }
    }

    if (n != 0) {//se houver essa palavra já na lista, retira da lista esse no e volta a colocar
        mens.ocorrencias = pf->no[local].m.ocorrencias += 1;
        if (*maior < mens.ocorrencias)
            *maior = mens.ocorrencias;
        pf->ocupados--;
        if (pf->ocupados == 0) {
            pf->entrada_lista = -1;
            pf->saida_lista = -1;
        } else {
            if (local == pf->entrada_lista) {
                pf->no[pf->no[local].mens_seguinte].mens_anterior = -1;
                pf->entrada_lista = pf->no[local].mens_seguinte;

            }
            if (local == pf->saida_lista) {
                pf->no[pf->no[local].mens_anterior].mens_seguinte = -1;
                pf->saida_lista = pf->no[local].mens_anterior;

            } else {
                pf->no[pf->no[local].mens_anterior].mens_seguinte = pf->no[local].mens_seguinte;
                pf->no[pf->no[local].mens_seguinte].mens_anterior = pf->no[local].mens_anterior;
            }
        }
        pf->no[local].ocupado = false;
        pf->no[local].mens_anterior = -1;
        pf->no[local].mens_seguinte = -1;

    }

    int i = pf->i, ant, prox;//procurar uma posição disponível de uma forma circular
    if (i < 0 || pf->no[i].ocupado)
        for (i = TAMFILA - 1; i > 0 && pf->no[i].ocupado; i--);
    //fila cheia - não é possível inserir mais nada
    if (pf->no[i].ocupado)
        return false;

    pf->i = i;

    //colocar mensagem na lista
    pf->no[i].m = mens;
    pf->ocupados++;

    //procurar a posição onde a mensagem deve ficar
    if (!(pf->no[pf->entrada_lista].ocupado)) {// lista estava vazia, colocar primeira mensagem
        pf->entrada_lista = i;
        pf->saida_lista = i;
        pf->no[i].mens_seguinte = -1;
        pf->no[i].mens_anterior = -1;
    } else {
        ant = -1;
        prox = pf->entrada_lista;

        //procurar na lista para ver as palavras com a ordem alfabeticas igual ou menor
        while(prox >= 0 && comparar(pf->no[prox].m.palavra , mens.palavra) <= 0){
            ant = prox;
            prox = pf->no[prox].mens_seguinte;
        }

        //procurar local para colocar a struct mens mas como ja esta definida a posiçao alfabetica, agora verifica as ocorrencias
        while(prox >= 0 && pf->no[prox].m.ocorrencias <= mens.ocorrencias &&  comparar(pf->no[prox].m.palavra , mens.palavra) == 0){
            ant = prox;
            prox = pf->no[prox].mens_seguinte;
        }

        if(ant < 0){ //colocar logo no inicio da lista pois o ant nao se alterou
            pf->entrada_lista = i;
            pf->no[i].mens_anterior = -1;
            pf->no[i].mens_seguinte = prox;
            pf->no[prox].mens_anterior = i;
        }
        if (prox < 0) {// colocar no final da lista pois o prox foi incrementando ate a se a lista.mens_saida
            pf->no[ant].mens_seguinte = i;
            pf->no[i].mens_seguinte = -1;
            pf->no[i].mens_anterior = ant;
            pf->saida_lista = i;
        } else {//colocar no meio da lista
            pf->no[ant].mens_seguinte = i;
            pf->no[i].mens_anterior = ant;
            pf->no[i].mens_seguinte = prox;
            pf->no[prox].mens_anterior = i;
        }
    }
    pf->no[i].ocupado = true;
    pf->i--;
    return true;
}

bool retirar(struct base *pf, struct mensagem *pmens) {
    if (!pf->no[pf->saida_lista].ocupado) {
        // lista vazia
        return false;
    }
    int s = pf->saida_lista; // indica a saida da lista
    *pmens = pf->no[s].m;
    pf->no[s].ocupado = false;
    if(pf->saida_lista == pf->entrada_lista){
        pf->saida_lista = -1;
        pf->entrada_lista = -1;
        pf->no[s].mens_seguinte = -1;
        pf->no[s].mens_anterior = -1;
        return true;
    }
    if (pf->no[s].mens_anterior != pf->entrada_lista) {
        // havia mais do que uma mensagem na lista
        pf->no[pf->no[s].mens_anterior].mens_seguinte = pf->entrada_lista;
        pf->saida_lista = pf->no[s].mens_anterior;
        pf->no[pf->entrada_lista].mens_anterior = pf->saida_lista;
    }
    return true;
}
