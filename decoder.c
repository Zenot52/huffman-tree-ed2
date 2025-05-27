#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio2.h>

#include "./TAD/arvore.h"

void recriaArvore(Arvh **raiz, char *codigo, int simbolo)
{
    Arvh *atual = *raiz;
    int i;
    for (i = 0; codigo[i] != '\0'; i++)
    {
        if (codigo[i] == '0')
        {
            if (atual->esq == NULL)
            {
                atual->esq = criaNoAvrh(-1, 0);
            }
            atual = atual->esq;
        }
        else if (codigo[i] == '1')
        {
            if (atual->dir == NULL)
            {
                atual->dir = criaNoAvrh(-1, 0);
            }
            atual = atual->dir;
        }
    }
    atual->simbolo = simbolo;
}
/*
void printArvore(Arvh *nodo, int n) {
    int i;
    if (nodo != NULL) {
    	
        printArvore(nodo->dir, n+1);
        for (i = 0; i < 8*n; i++)
            printf(" ");
        textcolor(n+2);
        printf("(%d, %d)\n", nodo->simbolo, nodo->freq);
        printArvore(nodo->esq, n+1);
    }
}
*/

void decode(Arvh *raiz, FILE *recordFile){
	FILE *encodedFile = fopen("outCode.dat", "rb");
    Arvh *aux = raiz;
    unsigned char byte;
    int i,bit;
    Reg registro;
    printf("Frase decodificada: ");
    while(fread(&byte, sizeof(byte), 1, encodedFile)){
        for (i = 7; i >= 0; i--)
        {
            bit = (byte >> i) & 1;
			if (bit == 0)
                aux = aux->esq;
            else
                aux = aux->dir;
            if (aux->simbolo != -1) //achou
            {
                
                rewind(recordFile);

                fread(&registro, sizeof(Reg), 1, recordFile);
                while(!feof(recordFile) && registro.simbolo != aux->simbolo)
                    fread(&registro, sizeof(Reg), 1, recordFile);

                if(registro.simbolo == aux->simbolo){//achou a palavra
                    printf("%s",registro.palavra);
                }
                aux = raiz;
            }
        }
    }
    fclose(encodedFile);
}

int main() {
    FILE *recordFile = fopen("record.dat", "rb");
    Reg registro;
    Arvh *tree = criaNoAvrh(-1, 0);

    fread(&registro, sizeof(Reg), 1, recordFile);
    while(!feof(recordFile)) {
        recriaArvore(&tree, registro.codHuffman,registro.simbolo);

        fread(&registro, sizeof(Reg), 1, recordFile);
    }

    rewind(recordFile);
    //printArvore(tree,0);
    decode(tree, recordFile);
	

	
    fclose(recordFile);
    return 0;
}
