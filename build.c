#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio2.h>

#include "./TAD/arvore.h"

ListaReg *buscaPalavraEmTabela(ListaReg *tabela, char *palavra)
{
    ListaReg *atual = tabela;
    while (atual != NULL && strcmp(atual->reg->palavra, palavra) != 0)
        atual = atual->prox;

    if (atual != NULL && strcmp(atual->reg->palavra, palavra) == 0)
        return atual;
    return NULL;
}

void aumentaFrequencia(ListaReg *registro)
{
    if (registro != NULL)
        registro->freq++;
}

void inserePalavraNaTabela(ListaReg **tabela, char *palavra) {
    ListaReg *novoReg, *aux;

    novoReg = (ListaReg*)malloc(sizeof(novoReg));
    novoReg->reg = (Reg*)malloc(sizeof(Reg));
    strcpy(novoReg->reg->palavra, palavra);
    novoReg->freq = 1;
    novoReg->prox = NULL;
    
    if (*tabela == NULL)
        *tabela = novoReg;
    else {
        aux = *tabela;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novoReg;
    }
    
}

void separaNaTabela(ListaReg **tabela, char *text) {
    char palavra[300] = "";
    int TL = 0, i;
    ListaReg *existe = NULL;

    for (i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ' && text[i] != '\n') {
            palavra[TL++] = text[i];
        } else {
            palavra[TL] = '\0';
            if (strlen(palavra) > 0) {
                existe = buscaPalavraEmTabela(*tabela, palavra);
                if (existe == NULL) {
                    inserePalavraNaTabela(tabela, palavra);
                } else {
                    aumentaFrequencia(existe);
                }
            }
            TL = 0;

            existe = buscaPalavraEmTabela(*tabela, " ");
            if (existe == NULL) {
                inserePalavraNaTabela(tabela, " ");
            } else {
                aumentaFrequencia(existe);
            }
            printf("%s", palavra);
        }
    }

    if (TL > 0) {
        palavra[TL] = '\0';
        existe = buscaPalavraEmTabela(*tabela, palavra);
        if (existe == NULL) {
            inserePalavraNaTabela(tabela, palavra);
        } else {
            aumentaFrequencia(existe);
        }
    }
}



void insereSimboloNaTabela(ListaReg *tabela) {
    int i = 1;
    while (tabela != NULL) {
        tabela->reg->simbolo = i;
        i++;
        tabela = tabela->prox;
    }
}

void printTabela(ListaReg *tabela) {
    printf("Simbolo\t\tPalavra\t\tFrequencia\n");
    while(tabela != NULL) {
        printf("%c\t\t%s\t\t%d\n", tabela->reg->simbolo, tabela->reg->palavra, tabela->freq);
        tabela = tabela->prox;
    }
}

int main() {
    char text[1000];
    FILE *file = fopen("text.txt", "r");
    ListaReg *tabela = NULL;

    while (fgets(text, sizeof(text), file) != NULL) {
        separaNaTabela(&tabela, text);
    }
	
    insereSimboloNaTabela(tabela);
    
    //teste
    printTabela(tabela);

    fclose(file);
    return 0;
}
