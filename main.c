#include "estrutura_dados.h"

int main() {
    char original[MAX_TEXTO + 1];//texto original
    char modif[MAX_TEXTO + 1];//texto modificado
    struct mensagem mens;

    if (ler_texto(original, modif)) {
        int maior = 0;//variavel para a palavra com maior numero de ocorrencias.

        struct base lista;
        inicializar(&lista);

        //separar palavras e introduzir na variavel token
        const char *sep = "0123456789 \t\n.(),:;!?\"\0";//carateres para separar
        char *token = strtok(modif, sep);
        while (token != NULL) {
            //printf("%s\n", token);
            if (isalpha(token[0])) {
                strcpy(mens.palavra, token);
                mens.ocorrencias = 1;
                mens.len = strlen(token);
                colocar(&lista, mens, &maior);
            }
            token = strtok(NULL, sep);
        }
        struct mensagem mens4;//struct vazia para receber as palavras da lista.saida

        while (retirar(&lista, &mens4)) {
            print(mens4, maior);
        }

        return 0;
    }
    return 1;
}
