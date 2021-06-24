//arquivo com a função main usando o .h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "huffman.h" // inclui os prototipos do aqruivo .h

int main(int argc, char* argv[]){
	if (argc != 4){
		printAjuda();
		return 1;
	}
	if (strcmp(argv[1], "-c")==0){
		comprimeArq(argv[2], argv[3]);
	} else if (strcmp(argv[1], "-d")==0){
		descomprimeArq(argv[2], argv[3]);
	} else{
		printAjuda();
	}
	return 0;
}