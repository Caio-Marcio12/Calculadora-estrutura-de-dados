#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

#define MAX_PILHA 512
#define PI 3.14159265358979323846


typedef struct {
    char itens[MAX_PILHA][512];
    int topo;
} PilhaString;

typedef struct {
    float itens[MAX_PILHA];
    int topo;
} PilhaFloat;


void inicializarPilhaString(PilhaString *p) {
    p->topo = -1;
}

int vaziaString(PilhaString *p) {
    return p->topo == -1;
}

void empilharString(PilhaString *p, char *str) {
    if (p->topo < MAX_PILHA - 1) {
        strcpy(p->itens[++(p->topo)], str);
    }
}

char* desempilharString(PilhaString *p) {
    if (p->topo >= 0) {
        return p->itens[(p->topo)--];
    }
    return NULL;
}


void inicializarPilhaFloat(PilhaFloat *p) {
    p->topo = -1;
}

int vaziaFloat(PilhaFloat *p) {
    return p->topo == -1;
}

void empilharFloat(PilhaFloat *p, float valor) {
    if (p->topo < MAX_PILHA - 1) {
        p->itens[++(p->topo)] = valor;
    }
}

float desempilharFloat(PilhaFloat *p) {
    if (p->topo >= 0) {
        return p->itens[(p->topo)--];
    }
    return 0.0;
}


int ehOperadorBinario(char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
            strcmp(token, "%") == 0 || strcmp(token, "^") == 0);
}

int ehFuncaoUnaria(char *token) {
    return (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
            strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
            strcmp(token, "log") == 0);
}

int ehOperando(char *token) {
    if (token == NULL || *token == '\0') return 0;
    
    char *endptr;
    strtod(token, &endptr);
    
    return (*endptr == '\0');
}

int getPrecedencia(char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    return 0;
}

int contemOperador(char *str) {
    return (strpbrk(str, "+-*/%^") != NULL);
}


char* getFormaInFixa(char *Str) {
    if (Str == NULL) return NULL;
    
    PilhaString pilha;
    inicializarPilhaString(&pilha);
    
    char *resultado = (char*)malloc(512 * sizeof(char));
    if (resultado == NULL) return NULL;
    
    char copia[512];
    strncpy(copia, Str, 511);
    copia[511] = '\0';
    
    char *token = strtok(copia, " ");
    
    while (token != NULL) {
        if (ehOperando(token)) {
            empilharString(&pilha, token);
        }
        else if (ehFuncaoUnaria(token)) {
            if (pilha.topo < 0) {
                free(resultado);
                return NULL;
            }
            
            char *op = desempilharString(&pilha);
            char temp[512];
            snprintf(temp, 512, "%s(%s)", token, op);
            empilharString(&pilha, temp);
        }
        else if (ehOperadorBinario(token)) {
            if (pilha.topo < 1) {
                free(resultado);
                return NULL;
            }
            
            char *op2 = desempilharString(&pilha);
            char *op1 = desempilharString(&pilha);
            char temp[512];
            
            int precAtual = getPrecedencia(token);
            int precisaParentesesEsq = 0;
            int precisaParentesesDir = 0;
            
            if (contemOperador(op1) && strchr(op1, '(') == NULL) {
                char *opPos = strpbrk(op1, "+-");
                if (opPos != NULL && precAtual >= 2) {
                    precisaParentesesEsq = 1;
                }
                
                opPos = strpbrk(op1, "*/");
                if (opPos != NULL && precAtual == 3) {
                    precisaParentesesEsq = 1;
                }
            }
            
            if (contemOperador(op2) && strchr(op2, '(') == NULL) {
                if (strcmp(token, "-") == 0 || strcmp(token, "/") == 0 || strcmp(token, "%") == 0) {
                    if (strpbrk(op2, "+-") != NULL) {
                        precisaParentesesDir = 1;
                    }
                }
                
                if (strcmp(token, "^") == 0) {
                    if (strpbrk(op2, "+-*/") != NULL) {
                        precisaParentesesDir = 1;
                    }
                }
                
                if (precAtual == 2 && strcmp(token, "*") == 0) {
                    if (strpbrk(op2, "+-") != NULL) {
                        precisaParentesesDir = 1;
                    }
                }
            }
            
            if (precisaParentesesEsq && precisaParentesesDir) {
                snprintf(temp, 512, "(%s)%s(%s)", op1, token, op2);
            }
            else if (precisaParentesesEsq) {
                snprintf(temp, 512, "(%s)%s%s", op1, token, op2);
            }
            else if (precisaParentesesDir) {
                snprintf(temp, 512, "%s%s(%s)", op1, token, op2);
            }
            else {
                snprintf(temp, 512, "%s%s%s", op1, token, op2);
            }
            
            empilharString(&pilha, temp);
        }
        else {
            free(resultado);
            return NULL;
        }
        
        token = strtok(NULL, " ");
    }
    
    if (pilha.topo != 0) {
        free(resultado);
        return NULL;
    }
    
    strncpy(resultado, desempilharString(&pilha), 511);
    resultado[511] = '\0';
    
    return resultado;
}

float getValorPosFixa(char *StrPosFixa) {
    if (StrPosFixa == NULL) return 0.0;
    
    PilhaFloat pilha;
    inicializarPilhaFloat(&pilha);
    
    char copia[512];
    strncpy(copia, StrPosFixa, 511);
    copia[511] = '\0';
    
    char *token = strtok(copia, " ");
    
    while (token != NULL) {
        if (ehOperando(token)) {
            float valor = (float)atof(token);
            empilharFloat(&pilha, valor);
        }
        else if (ehFuncaoUnaria(token)) {
            if (pilha.topo < 0) return 0.0;
            
            float op = desempilharFloat(&pilha);
            float res = 0.0;
            
            if (strcmp(token, "raiz") == 0) {
                res = sqrt(op);
            }
            else if (strcmp(token, "sen") == 0) {
                res = sin(op * PI / 180.0);
            }
            else if (strcmp(token, "cos") == 0) {
                res = cos(op * PI / 180.0);
            }
            else if (strcmp(token, "tg") == 0) {
                res = tan(op * PI / 180.0);
            }
            else if (strcmp(token, "log") == 0) {
                res = log10(op);
            }
            
            empilharFloat(&pilha, res);
        }
        else if (ehOperadorBinario(token)) {
            if (pilha.topo < 1) return 0.0;
            
            float op2 = desempilharFloat(&pilha);
            float op1 = desempilharFloat(&pilha);
            float res = 0.0;
            
            if (strcmp(token, "+") == 0) {
                res = op1 + op2;
            }
            else if (strcmp(token, "-") == 0) {
                res = op1 - op2;
            }
            else if (strcmp(token, "*") == 0) {
                res = op1 * op2;
            }
            else if (strcmp(token, "/") == 0) {
                if (fabs(op2) > 0.0000001) {
                    res = op1 / op2;
                } else {
                    return 0.0;
                }
            }
            else if (strcmp(token, "%") == 0) {
                res = fmod(op1, op2);
            }
            else if (strcmp(token, "^") == 0) {
                res = pow(op1, op2);
            }
            
            empilharFloat(&pilha, res);
        }
        
        token = strtok(NULL, " ");
    }
    
    if (pilha.topo != 0) return 0.0;
    
    return desempilharFloat(&pilha);
}