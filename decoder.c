#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio2.h>

#include "./TAD/arvore.h"

Arvh *criaGalho() {
    Arvh *novoNo = (Arvh*)malloc(sizeof(Arvh));
    novoNo->simbolo = -1;
    novoNo->freq = -1;
    novoNo->esq = novoNo->dir = NULL;
    return novoNo;
}

Arvh *criaFolha(char simbolo) {
    Arvh *novoNo = (Arvh*)malloc(sizeof(Arvh)); 
    novoNo->simbolo = simbolo;
    novoNo->freq = -1;
    novoNo->esq = novoNo->dir = NULL;
    return novoNo;
}

void recriaArvore(Arvh **tree, Reg registro) {
    int i = 0;
    Arvh atual;

    if(*tree == NULL) {
        *tree = criaGalho();
    }
    else {
        atual = *tree;
        while(i < strlen(registro.codHuffman)) {
            if(registro.codHuffman[i] == '1') {
                if(i == strlen(registro.codHuffman) - 1) {
                    atual->dir = criaFolha(registro.simbolo);
                }else{
                    if(atual->dir == NULL) {
                        atual->dir = criaGalho();
                        atual = atual->dir;
                    }
                }
            }
            else { // código será 0
                if(i == strlen(registro.codHuffman) - 1) {
                    atual->esq = criaFolha(registro.simbolo);
                }else{
                    if(atual->esq == NULL) {
                        atual->esq = criaGalho();
                        atual = atual->esq;
                    }
                }
            }
            i++;
        }
    }
}

int main() {
    FILE *encodedFile = fopen("outCode.dat", "rb");
    FILE *recordFile = fopen("record.dat", "rb");
    Reg registro;
    Arvh *tree = NULL;

    fread(&registro, sizeof(Reg), 1, recordFile);
    while(!feof(recordFile)) {
        recriaArvore(&tree, registro);

        fread(&registro, sizeof(Reg), 1, recordFile);
    }

    fclose(encodedFile);
    fclose(recordFile);
    return 0;
}