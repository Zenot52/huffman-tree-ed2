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

    novoReg = (ListaReg*)malloc(sizeof(ListaReg));
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
            if (TL > 0) {
                palavra[TL] = '\0';
                existe = buscaPalavraEmTabela(*tabela, palavra);
                if (existe == NULL) {
                    inserePalavraNaTabela(tabela, palavra);
                } else {
                    aumentaFrequencia(existe);
                }
                TL = 0;
            }

            if (text[i] == ' ') {
                existe = buscaPalavraEmTabela(*tabela, " ");
                if (existe == NULL) {
                    inserePalavraNaTabela(tabela, " ");
                } else {
                    aumentaFrequencia(existe);
                }
            }
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
    int linha = 1;
    int colSimbolo = 2;
    int colPalavra = 12;
    int colFrequencia = 40;

    clrscr();

    gotoxy(colSimbolo, linha++);
    printf("Simbolo");
    gotoxy(colPalavra, linha - 1);
    printf("Palavra");
    gotoxy(colFrequencia, linha - 1);
    printf("Frequencia");

    linha++;

    while (tabela != NULL) {
        gotoxy(colSimbolo, linha);
        printf("%d", tabela->reg->simbolo);
        gotoxy(colPalavra, linha);
        printf("%s", tabela->reg->palavra);
        gotoxy(colFrequencia, linha);
        printf("%d", tabela->freq);
        tabela = tabela->prox;
        linha++;
    }
}

ListaArvh *criaFloresta(ListaReg *tabela){
    ListaArvh *L = NULL, *nb, *auxL, *ant;
    ListaReg *auxtabela = tabela;
    while (auxtabela != NULL) {
        nb = criaBlocoLista(auxtabela->reg->simbolo,auxtabela->freq);
        if(L == NULL)
            L = nb;
        else{
            if(L->Arvh->freq > auxtabela->freq) {
                nb->prox = L;
                L = nb;                
            } else {
                auxL = L->prox;
                ant = L;
                while(auxL != NULL && auxL->Arvh->freq < nb->Arvh->freq){
                    ant = auxL;
                    auxL = auxL->prox;
                }
                if(auxL == NULL){
                    ant->prox = nb;
                }else{
                    nb->prox = auxL;
                    ant->prox = nb;
                }
            }
        }
        auxtabela = auxtabela->prox;
    }
    return L;
}

ListaArvh *criaNovoNoAvrh(ListaArvh *atual, ListaArvh *atualprox) {
    ListaArvh *nb = (ListaArvh *)malloc(sizeof(ListaArvh));
    nb->prox = NULL;
    nb->Arvh = (Arvh *)malloc(sizeof(Arvh));
    nb->Arvh->esq = atual->Arvh;
    nb->Arvh->dir = atualprox->Arvh;
    nb->Arvh->simbolo = -1;
    nb->Arvh->freq = atual->Arvh->freq + atualprox->Arvh->freq;
    return nb;
}

void insereFloresta(ListaArvh **L, ListaArvh *nb) {
    ListaArvh *aux, *ant;
	
	if(*L == NULL) {
		*L = nb;
	}
	else {
		if(nb->Arvh->freq < (*L)->Arvh->freq) {
	        nb->prox = *L;
	        *L = nb;
    	}
	    else {
	        aux = (*L)->prox;
	        ant = *L;
	        while(aux != NULL && nb->Arvh->freq > aux->Arvh->freq) {
	            ant = aux;
	            aux = aux->prox;
	        }
	
	        if(aux != NULL) {
	            nb->prox = aux;
	            ant->prox = nb;
	        }   
	        else {
	            ant->prox = nb;
	        }
	    }
	}
}

void geraArvore(ListaArvh **L) {
    ListaArvh *nb, *atual, *atualprox;

    while((*L)->prox != NULL) {
        atual = *L;
        atualprox = (*L)->prox;
        
        *L = atualprox->prox;

        nb = criaNovoNoAvrh(atual, atualprox);
        
        free(atual);
        free(atualprox);
    
        insereFloresta(L, nb);
    }
}


int main() {
    char text[1000];
    FILE *file = fopen("text.txt", "r");
    ListaReg *tabela = NULL;
    ListaArvh *L= NULL;
    while (fgets(text, sizeof(text), file) != NULL) {
        separaNaTabela(&tabela, text);
    }
	
    insereSimboloNaTabela(tabela);
   
    L = criaFloresta(tabela);

    geraArvore(&L);
    
    printTabela(tabela);

    fclose(file);
    return 0;
}
