//arquivo .c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "huffman.h" // inclui os prototipos do aqruivo .h

//tipo do meu dado char que guarda byte 
typedef unsigned char byte;

//minha arvore
typedef struct tree{
	byte ch;
    int freq;
	struct tree *esq;
	struct tree *dir;
} ARVORE;

//aponta para o proximo na lista
typedef struct noDaLista{
    ARVORE *n;
    struct noDaLista *prox;
} NO;

//definição de lista (vou add sempre na cab)
typedef struct list{
    NO *cab;
    int valor;
} lista;

//aloca memoria para uma nova variavel no tipo NO e seta os valores dela
NO* novoNoNalista(ARVORE* noarv){
	NO* novo;
	if((novo = malloc(sizeof(*novo))) == NULL) return NULL;
	novo->n = noarv;
	novo->prox = NULL;
	return novo;
}

//aloca memoria para uma nova variavel no tipo ARVORE e seta os valores dela
ARVORE* novoNoNaArv(byte c, int f, ARVORE* e, ARVORE* d){
	ARVORE* novo;
	if((novo = malloc(sizeof(*novo))) == NULL) return NULL;
	novo->ch = c;
	novo->freq = f;
	novo->esq = e;
	novo->dir = d;
	return novo;
}

//funcao de insercao na cabeca se a lista de frequencia estiver vazia ou nao
void insereL(NO* n, lista* el){	
	if (!el->cab){
		el->cab=n;
	}
	else if (n->n->freq < el->cab->n->freq){
		n->prox = el->cab;
		el->cab =n;
	} else{
		NO *temp = el->cab->prox;
		NO *aux = el->cab;
		while (temp && temp->n->freq <= n->n->freq){
			aux = temp;
			temp = aux->prox;
		}
		aux->prox = n;
		n->prox= temp;
	}
	el->valor++;
}

//funcao que tira o menor no da lista de frequencia 
ARVORE* desenfMenor(lista* el){
	NO* aux = el->cab;
	ARVORE* aux2 = aux->n;
	el->cab = aux->prox;
	free(aux);
	aux = NULL;
	el->valor--;
	return aux2;
}

//funcao que conta frequencia de um simbolo
void contaFreq(FILE *ent, unsigned int* listaTam){
	byte aux;
	while (fread(&aux, 1, 1, ent) >= 1){
		listaTam[(byte)aux]++;
	}
	rewind(ent);
}

//funcao que busca um determinado byte
bool buscaB(ARVORE* n, byte c, char* b, int tam){
	if (!(n->esq || n->dir) && n->ch==c){
		b[tam] = '\0';
		return true;
	} else {
		bool alvo = false;
		if (n->esq){
			b[tam] = '\0';
			alvo = buscaB(n->esq, c, b, tam+1);
		}
		if (!alvo && n->dir){
			b[tam] = '1';
			alvo = buscaB(n->dir, c, b, tam+1);
		}
		if (!alvo){
			b[tam] = '\0';
		}
		return alvo;
	}
}

//funcao que cria a arvore de huffman
ARVORE* criaHuffmanTree(unsigned* listaB){
	lista nlista = {NULL, 0};
	for (int i = 0; i < 256; i++){
		if (listaB [i]){
			insereL(novoNoNalista(novoNoNaArv(i, listaB[i], NULL, NULL)), &nlista);
		}
	}
	while (nlista.valor > 1){
		ARVORE *noesq = desenfMenor(&nlista);
		ARVORE *nodir = desenfMenor(&nlista);
		ARVORE *soma = novoNoNaArv('/', noesq->freq + nodir->freq, noesq, nodir);
		insereL(novoNoNalista(soma), &nlista);
	}
	return desenfMenor(&nlista);
}

//funcao que vai liberar a arvore de huffman
void freeHuffmanTree(ARVORE* n){
	if (!n) return;
	else{
		ARVORE *esq = n->esq;
		ARVORE *dir = n->dir;
		free(n);
		freeHuffmanTree(esq);
		freeHuffmanTree(dir);
	}
}

//funcao que verifica um no e retorna um bit
int retornaBit(FILE* ent, int p, byte* n){
	(p % 8 == 0) ? fread(n, 1, 1, ent) : NULL == NULL;
	return !!((*n) & (1 << (p % 8)));
}

//funcao para comprimir um arquivo usando huffman
void comprimeArq(const char* arquivoIN, const char* arquivoOUT){
	clock_t inicio, fim;
	double time;
	inicio = clock();
	unsigned listaB[256] = {0};
	
	FILE* in = fopen(arquivoIN, "rb");
	(!in) ? printf ("erro! arquivo n existe\n") : NULL == NULL;

	FILE* out = fopen(arquivoOUT, "wb");
	(!out) ? printf ("erro! arquivo n existe\n") : NULL == NULL;

	contaFreq (in, listaB);

	ARVORE* r = criaHuffmanTree(listaB);

	fwrite(listaB, 256, sizeof(listaB[0]), out);

	fseek(out, sizeof(unsigned int), SEEK_CUR);

	byte c, aux = 0;	
	unsigned tam = 0;

	while (fread(&c, 1, 1, in) >= 1){
		char buffer [1024] = {0};
		buscaB(r, c, buffer, 0);
		for (char *i = buffer; *i ; i++){
			if (*i == '1'){
				aux = aux | (1 << (tam % 8));
			}
			tam++;
			if (tam % 8 == 0){
				fwrite(&aux, 1, 1, out);
				aux = 0;
			}
		}
	}
	fwrite (&aux, 1, 1, out);
	fseek (out, 256 *sizeof(unsigned int), SEEK_SET);
	fwrite (&tam, 1, sizeof(unsigned), out);

	fim = clock();
	time = (double)(fim - inicio) / CLOCKS_PER_SEC;

	fseek(in, 0L, SEEK_END);
	double tamInicio = ftell(in);

	fseek(out, 0L, SEEK_END);
	double tamFim = ftell(out);

	freeHuffmanTree(r);

	printf("Arquivo inicial: %s (%g bytes)\n Arquivo final: %s (%g bytes)\nTempo gasto na execução foi: %gs\n", arquivoIN, tamInicio / 1000, arquivoOUT, tamFim / 1000, time);
    printf("Taxa de compressao: %d%%\n", (int)((100 * tamFim) / tamInicio));

	fclose(in);
	fclose(out);
}

//funcao para descomprimir um arquivo usando huffman
void descomprimeArq(const char* arquivoIN, const char* arquivoOUT){
	clock_t inicio, fim;
	double time;
	inicio = clock();
	unsigned listaB[256] = {0};

	FILE* in = fopen(arquivoIN, "rb");
	(!in) ? printf ("erro! arquivo n existe\n") : NULL == NULL;

	FILE* out = fopen(arquivoOUT, "wb");
	(!out) ? printf ("erro! arquivo n existe\n") : NULL == NULL;

	fread(listaB, 256, sizeof(listaB[0]), in);
	
	ARVORE* r = criaHuffmanTree(listaB);

	byte aux = 0;	
	unsigned tam = 0, position = 0;
	fread(&tam, 1, sizeof(tam), in);

	while (position < tam){
		ARVORE* noAtual = r;
		while (noAtual->esq || noAtual->dir){
			noAtual = retornaBit(in, position++, &aux) ? noAtual->dir : noAtual->esq; 
		}
		fwrite(&(noAtual->ch), 1, 1, fim);
	}
	freeHuffmanTree(r);
	fim = clock();
	time = (double)(fim - inicio)/ CLOCKS_PER_SEC;

	fseek(in, 0L, SEEK_END);
	double tamEntrada = ftell(in);

	fseek(out, 0L, SEEK_END);
	double tamSaida = ftell(out);

	printf("Arquivo inicial: %s (%g bytes)\n Arquivo final: %s (%g bytes)\nTempo gasto na execução foi: %gs\n", arquivoIN, tamEntrada / 1000, arquivoOUT, tamSaida / 1000, time);
    printf("Taxa de compressao: %d%%\n", (int)((100 * tamSaida) / tamEntrada));

	fclose(in);
	fclose(out);
}	

/*void impressao(ARVORE *raiz, int nivel){
	if (raiz!=NULL){
		printf("%d	%d\n", raiz->valor, nivel);			
		impressao(raiz->esq, nivel + 1);			
		impressao(raiz->dir, nivel + 1);
	}
}*/

//função que mostra como inserir o arq
void printAjuda() {
    printf("Use '-c' para Comprimir e '-d' para Descomprimir um arquivo.");
	printf("\nExemplo: ./mainHuffman -c comprima.isso\n");
}