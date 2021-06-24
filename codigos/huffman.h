//arquivo .h

//tipo do meu dado char que guarda byte 
typedef unsigned char byte;

//minha arvore
typedef struct tree ARVORE;

//aponta para o proximo na lista
typedef struct noDaLista NO;

//definição de lista (vou add sempre na cab)
typedef struct list lista;

//aloca memoria para uma nova variavel no tipo NO e seta os valores dela
NO* novoNoNalista(ARVORE* noarv);

//aloca memoria para uma nova variavel no tipo ARVORE e seta os valores dela
ARVORE* novoNoNaArv(byte c, int f, ARVORE* e, ARVORE* d);

//funcao de insercao na cabeca se a lista de frequencia estiver vazia ou nao
void insereL(NO* n, lista* el);

//funcao que tira o menor no da lista de frequencia 
ARVORE* desenfMenor(lista* el);

//funcao que conta frequencia de um simbolo
void contaFreq(FILE *ent, unsigned int* listaTam);

//funcao que busca um determinado byte
bool buscaB(ARVORE* n, byte c, char* b, int tam);

//funcao que cria a arvore de huffman
ARVORE* criaHuffmanTree(unsigned* listaB);

//funcao que vai liberar a arvore de huffman
void freeHuffmanTree(ARVORE* n);

//funcao que verifica um no e retorna um bit
int retornaBit(FILE* ent, int p, byte* n);

//funcao para comprimir um arquivo usando huffman
void comprimeArq(const char* arquivoIN, const char* arquivoOUT);

//funcao para descomprimir um arquivo usando huffman
void descomprimeArq(const char* arquivoIN, const char* arquivoOUT);	

//função que mostra como inserir o arq
void printAjuda();