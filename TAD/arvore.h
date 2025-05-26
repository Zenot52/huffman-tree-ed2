struct ArvoreHuffman{
    int simbolo;
    int freq;
    struct ArvoreHuffman *esq, *dir;
}; typedef struct ArvoreHuffman Arvh;

struct ListaArvore{
    struct ArvoreHuffman *Arvh;
    struct ListaArvore *prox;
}; typedef struct ListaArvore ListaArvh;

struct Bits
{
 unsigned char b7:1;//-
 unsigned char b6:1;
 unsigned char b5:1;
 unsigned char b4:1;
 unsigned char b3:1;
 unsigned char b2:1;
 unsigned char b1:1;
 unsigned char b0:1;//+
}; typedef struct Bits bits;

struct Registro{
    char simbolo;
    char codHuffman[50];
    char palavra[50];
}; typedef struct Registro Reg;

struct ListaRegistro{
    int freq;
    struct Registro *reg;
    struct ListaRegistro *prox;
}; typedef struct ListaRegistro ListaReg;


Arvh *criaNoAvrh(char simbolo, int freq){
	Arvh *NovoNo = (Arvh*)malloc(sizeof(Arvh));
	NovoNo->simbolo = simbolo;
	NovoNo->freq = freq;
	NovoNo->esq = NovoNo->dir = NULL;
	return NovoNo;
}

ListaArvh *criaBlocoLista(char simbolo, int freq) {
	ListaArvh *NovoBloco = (ListaArvh*)malloc(sizeof(ListaArvh));
	NovoBloco->prox = NULL;
	NovoBloco->Arvh = criaNoAvrh(simbolo, freq);
	return NovoBloco;
}