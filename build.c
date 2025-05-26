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
    int colFrequencia = 30;
    int colCod = 45;

    clrscr();

    gotoxy(colSimbolo, linha++);
    printf("Simbolo");
    gotoxy(colPalavra, linha - 1);
    printf("Palavra");
    gotoxy(colFrequencia, linha - 1);
    printf("Frequencia");
	gotoxy(colCod, linha - 1);
    printf("Codigo Huffman");

    linha++;

    while (tabela != NULL) {
        gotoxy(colSimbolo, linha);
        printf("%d", tabela->reg->simbolo);
        gotoxy(colPalavra, linha);
        printf("%s", tabela->reg->palavra);
        gotoxy(colFrequencia, linha);
        printf("%d", tabela->freq);
        gotoxy(colCod, linha);
        printf("%s", tabela->reg->codHuffman);
        tabela = tabela->prox;
        linha++;
    }
}

void printArvore(ListaArvh *L, int n) {
    int i;
    if (L != NULL) {
        printArvore(L->Arvh->dir, n+1);
        for (i = 0; i < 5*n; i++)
            printf(" ");
        printf("(%d, %d)\n", L->Arvh->simbolo, L->Arvh->freq);
        printArvore(L->Arvh->esq, n-1);
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

    while((*L) != NULL && (*L)->prox != NULL) {
        atual = *L;
        atualprox = (*L)->prox;
        
        *L = atualprox->prox;

        nb = criaNovoNoAvrh(atual, atualprox);
        
        free(atual);
        free(atualprox);
    
        insereFloresta(L, nb);
    }
}

inserirCod(ListaReg *tabela, int simbol, char *cod){
    while(tabela != NULL && tabela->reg->simbolo != simbol){
        tabela = tabela->prox;
    }
    if(tabela != NULL){
        strcpy(tabela->reg->codHuffman,cod);
    }
}

void setCodHuffman(Arvh *raiz, ListaReg *tabela, char *cod, int TL)
{
    if (raiz != NULL)
    {
        if (raiz->simbolo != -1)
        {
            cod[TL] = '\0';
            inserirCod(tabela, raiz->simbolo, cod);
        }
        else
        {
            cod[TL] = '0';
            setCodHuffman(raiz->esq, tabela, cod, TL + 1);
            cod[TL] = '1';
            setCodHuffman(raiz->dir, tabela, cod, TL + 1);
        }
    }
}

void codificarFrase(ListaReg *tabela, char *frase, FILE *arquivo) {
	ListaReg *existe = NULL;
	char palavra[300] = "";
	int vetBin[1000], i, j, TL = 0, TLvet = 0;
	bits Byte;
	for(i = 0; frase[i] != '\0'; i++) {
		if(frase[i] == ' ' && frase[i] != '\n')
			palavra[TL++] = frase[i];
		else {
			if(TL > 0) {
				palavra[TL] = '\0';
				existe = buscaPalavraEmTabela(tabela, palavra);
				if(existe != NULL) {
					j = 0;
					while(j < strlen(existe->reg->codHuffman)) {
						vetBin[TLvet] = existe->reg->codHuffman[j] - '0';
						TLvet++;
						j++;
					}
				}
			}
			TL = 0;
			vetBin[TLvet] = 0;//adicionando espaÃ§o manualmente
            TLvet++;

		}
	}
    if(TL > 0) {
		palavra[TL] = '\0';
		existe = buscaPalavraEmTabela(tabela, palavra);
		if(existe != NULL) {
			j = 0;
			while(j < strlen(existe->reg->codHuffman)) {
				vetBin[TLvet] = existe->reg->codHuffman[j] - '0';
				TLvet++;
				j++;
			}
		}
	}

    i = 0;
    while (i < TLvet) {
        if (i < TLvet)
            Byte.b0 = vetBin[i];
        else
            Byte.b0 = 0;

        if (i + 1 < TLvet)
            Byte.b1 = vetBin[i + 1];
        else
            Byte.b1 = 0;

        if (i + 2 < TLvet)
            Byte.b2 = vetBin[i + 2];
        else
            Byte.b2 = 0;

        if (i + 3 < TLvet)
            Byte.b3 = vetBin[i + 3];
        else
            Byte.b3 = 0;

        if (i + 4 < TLvet)
            Byte.b4 = vetBin[i + 4];
        else
            Byte.b4 = 0;

        if (i + 5 < TLvet)
            Byte.b5 = vetBin[i + 5];
        else
            Byte.b5 = 0;

        if (i + 6 < TLvet)
            Byte.b6 = vetBin[i + 6];
        else
            Byte.b6 = 0;

        if (i + 7 < TLvet)
            Byte.b7 = vetBin[i + 7];
        else
            Byte.b7 = 0;

        fwrite(&Byte, sizeof(Byte), 1, arquivo);

        i += 8;
    }
}

void gravaRegistro(ListaReg *tabela, FILE *arquivo) {
    Reg *aux;

    while (tabela != NULL) {
        aux = tabela->reg;
        fwrite(aux, sizeof(Reg), 1, arquivo);
        tabela = tabela->prox;
    }

}

int main() {
	ListaReg *tabela = NULL;
    ListaArvh *L= NULL;
    FILE *dataFile = fopen("text.txt", "r");
    FILE *baseFile = fopen("phrase.txt", "r");
    FILE *encodedFile = fopen("outCode.dat", "wb");
    FILE *recordFile = fopen("record.dat", "wb");
    char text[1000], frase[1000], straux[30];
    
    while (fgets(text, sizeof(text), dataFile) != NULL) {
        separaNaTabela(&tabela, text);
    }
	
    insereSimboloNaTabela(tabela);
   
    L = criaFloresta(tabela);

    geraArvore(&L);

    straux[0] = '\0';
    setCodHuffman(L->Arvh, tabela, straux, 0);
    
    while (fgets(frase, sizeof(frase), baseFile) != NULL) {
        codificarFrase(tabela, frase, encodedFile);
    }
    
    //gravaRegistro(tabela, recordFile);
    
    printTabela(tabela);
    printf("\n\n\n\n\n");
    printArvore(L, 0);

    fclose(recordFile);
	fclose(encodedFile);
	fclose(baseFile);
    fclose(dataFile);
    return 0;
}