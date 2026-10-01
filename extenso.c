#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

static const char *unidades[20] = {
    "", "Um", "Dois", "Três", "Quatro", "Cinco", "Seis", "Sete", "Oito", "Nove",
    "Dez", "Onze", "Doze", "Treze", "Catorze", "Quinze", "Dezesseis",
    "Dezessete", "Dezoito", "Dezenove"
};

static const char *dezenas[10] = {
    "", "", "Vinte", "Trinta", "Quarenta", "Cinquenta",
    "Sessenta", "Setenta", "Oitenta", "Noventa"
};

static const char *centenas[10] = {
    "", "Cento", "Duzentos", "Trezentos", "Quatrocentos", "Quinhentos",
    "Seiscentos", "Setecentos", "Oitocentos", "Novecentos"
};

/* Escreve em 'out' um número de 1 a 999 por extenso */
static void escreve_ate_999(int n, char *out) {
    if (n == 100) {
        strcat(out, "Cem");
        return;
    }

    int c = n / 100;
    int resto = n % 100;
    int tem_centena = 0;

    if (c > 0) {
        strcat(out, centenas[c]);
        tem_centena = 1;
    }

    if (resto > 0) {
        if (tem_centena) strcat(out, " e ");

        if (resto < 20) {
            strcat(out, unidades[resto]);
        } else {
            int d = resto / 10;
            int u = resto % 10;
            strcat(out, dezenas[d]);
            if (u > 0) {
                strcat(out, " e ");
                strcat(out, unidades[u]);
            }
        }
    }
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001); /* para exibir acentos no terminal do Windows */
#endif

    char entrada[64];
    char digitos[16];
    int qtd = 0;

    printf("Digite um valor em R$ (até 6 dígitos): ");
    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        printf("Erro ao ler a entrada.\n");
        return 1;
    }

    /* Mantém só os dígitos (ignora pontos, espaços e quebra de linha) */
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (isdigit((unsigned char)entrada[i])) {
            if (qtd >= 6) {
                printf("Erro: o número deve ter no máximo 6 dígitos.\n");
                return 1;
            }
            digitos[qtd++] = entrada[i];
        } else if (entrada[i] != '.' && entrada[i] != '\n' &&
                   entrada[i] != '\r' && entrada[i] != ' ') {
            printf("Erro: digite apenas números inteiros.\n");
            return 1;
        }
    }
    digitos[qtd] = '\0';

    if (qtd == 0) {
        printf("Erro: nenhum número informado.\n");
        return 1;
    }

    int n = 0;
    for (int i = 0; i < qtd; i++) {
        n = n * 10 + (digitos[i] - '0');
    }

    char saida[256] = "";

    if (n == 0) {
        strcpy(saida, "Zero");
    } else {
        int mil = n / 1000;
        int resto = n % 1000;

        if (mil > 0) {
            if (mil == 1) {
                strcat(saida, "Mil");
            } else {
                escreve_ate_999(mil, saida);
                strcat(saida, " Mil");
            }
        }

        if (resto > 0) {
            if (mil > 0) {
                /* "Mil e Cinquenta", "Mil e Duzentos" -> usa "e";
                   "Mil, Duzentos e Setenta" -> usa vírgula */
                if (resto < 100 || resto % 100 == 0) {
                    strcat(saida, " e ");
                } else {
                    strcat(saida, ", ");
                }
            }
            escreve_ate_999(resto, saida);
        }
    }

    printf("%s %s\n", saida, (n == 1) ? "Real" : "Reais");

    return 0;
}
