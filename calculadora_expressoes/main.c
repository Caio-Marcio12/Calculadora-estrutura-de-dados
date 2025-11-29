#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

void testarExpressao(int numTeste, char *posFixa, char *infixaEsperada, float valorEsperado) {
    printf("\n========== TESTE %d ==========\n", numTeste);
    printf("Posfixa: %s\n", posFixa);
    
    char *infixa = getFormaInFixa(posFixa);
    if (infixa != NULL) {
        printf("Infixa obtida: %s\n", infixa);
        printf("Infixa esperada: %s\n", infixaEsperada);
        
        if (strcmp(infixa, infixaEsperada) == 0) {
            printf("CONVERSAO CORRETA\n");
        } else {
            printf("CONVERSAO INCORRETA\n");
        }
        free(infixa);
    } else {
        printf("ERRO na conversao para infixa\n");
    }
    
    float valor = getValorPosFixa(posFixa);
    printf("Valor obtido: %.2f\n", valor);
    printf("Valor esperado: %.2f\n", valorEsperado);
    
    if (fabs(valor - valorEsperado) < 0.01) {
        printf("CALCULO CORRETO\n");
    } else {
        printf("CALCULO INCORRETO\n");
    }
}

int main() {
    printf("===========================================\n");
    printf("  TESTADOR DE EXPRESSOES MATEMATICAS\n");
    printf("===========================================\n");
    
    testarExpressao(1, "3 4 + 5 *", "(3+4)*5", 35.0);
    testarExpressao(2, "7 2 * 4 +", "7*2+4", 18.0);
    testarExpressao(3, "8 5 2 4 + * +", "8+5*(2+4)", 38.0);
    testarExpressao(4, "6 2 / 3 + 4 *", "(6/2+3)*4", 24.0);
    testarExpressao(5, "9 5 2 8 * 4 + * +", "9+5*(4+8*2)", 109.0);
    testarExpressao(6, "2 3 + log 5 /", "log(2+3)/5", 0.14);
    testarExpressao(7, "10 log 3 ^ 2 +", "log(10)^3+2", 3.0);
    testarExpressao(8, "45 60 + 30 cos *", "(45+60)*cos(30)", 90.93);
    testarExpressao(9, "0.5 45 sen 2 ^ +", "0.5+sen(45)^2", 1.0);
    
    printf("\n===========================================\n");
    printf("  FIM DOS TESTES\n");
    printf("===========================================\n");
    
    return 0;
}
