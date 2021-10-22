#include "estrutura_dados.h"

static char acentuadas[][8] =
        {"á", "Á", "à", "À", "ã", "Ã", "â", "Â", "ä", "Ä", "ç", "Ç",
         "é", "É", "è", "È", "ê", "Ê", "í", "Í", "ì", "Ì", "î", "Î",
         "ó", "Ó", "ò", "Ò", "õ", "Õ", "ô", "Ô", "ö", "Ö", "ú", "Ú",
         "ù", "Ù", "û", "Û", "\0"};
static char base[] =
        {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'c', 'c',
         'e', 'e', 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 'i', 'i',
         'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'u', 'u',
         'u', 'u', 'u', 'u', '\0'};

static int comp_mchar(const char *um, const char *outro) {
    int i = 0;
    while (um[i] != 0) {
        if (um[i] != outro[i])
            return 0;
        i++;
    }
    return i;
}

static void strtobase_u8_(char *dest, const char *orig) {
    int i = 0, j = 0, c;
    // percorrer a string origem
    c = (unsigned char) orig[0];
    while (c != 0) {
        // é um carater UTF-8 com mais de um byte
        int k = 0, tc = 0;
        while (acentuadas[k][0] != 0) {
            // se for uma das letras acentuadas, retira-se o acento
            if ((tc = comp_mchar(&acentuadas[k][0], &orig[i])) != 0) {
                // encontramos uma minúscula acentuada
                dest[j] = base[k];
                j++;
                i = i + tc;
                break;
            }
            k++;
        }
        if (tc == 0) {
            // não é uma letra acentuada, logo é de copiar para
            //   o destino, sem alterações, todos os carateres UTF-8
            do {
                dest[j] = orig[i];
                i++;
                j++;
            } while ((orig[i] & 0xC0) == 0x80);
        }

        c = (unsigned char) orig[i];
    }
    dest[j] = 0;
}

bool ler_texto(char *ptexto,char *modif) {
    FILE *fich1;
    int i, c;
    fich1 = fopen("C:\\Users\\Eduardo22\\Desktop\\FC\\1ºano\\2ºsemestre\\PPP\\projeto_2\\texto.txt", "r");
    if (fich1 == NULL) {
        // Ficheiro não existe
        return false;
    }
    i = 0;
    while (i < (MAX_TEXTO) && (c = fgetc(fich1)) != EOF) {
        ptexto[i] = (char) c;
        // detetar e eliminar o UTF BOM
        if (i == 2 && ptexto[0] == (char) 0xEF && ptexto[1] == (char) 0xBB && ptexto[2] == (char) 0xBF)
            i = 0;
        else
            i++;
    }
    ptexto[i] = 0;
    i++;
    fclose(fich1);
    if (i <= MAX_TEXTO) {
        strtobase_u8_(modif, ptexto); //colocar o texto sem assentos e sem cedilhas
        return true;
    }
    else
        // limite de bytes ultrapassado
        return false;
}

void print(struct mensagem mens, int maior) {
    int space = 80 - (mens.ocorrencias * 80 / maior + (int) mens.len);

    for (int i = 0; i <= space + 1; i++)
        printf(" ");
    printf("%s %d\n", mens.palavra,mens.ocorrencias);
}
