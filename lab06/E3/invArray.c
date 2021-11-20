#include "invArray.h"
#include "debug.h"

struct invArray_s {
	int n;
	inv_t *inv;
};

/* creatore e disruttore */
invArray_t invArray_init() {
	invArray_t arr = malloc(sizeof(struct invArray_s));

	arr->n = 0;

	if(arr == NULL) {
		fprintf(stderr,"Errore allocazione.\n");
		exit(EXIT_FAILURE);
	}
	
	return arr;
}	

void invArray_free(invArray_t invArray) {
	if(invArray != NULL && invArray->inv != NULL)
		free(invArray->inv);

	if(invArray != NULL)
		free(invArray);
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp, invArray_t invArray) {
	int i = 0;

	fscanf(fp,"%d",&(invArray->n));

	if(invArray->inv != NULL)
		free(invArray->inv);

	invArray->inv = malloc(invArray->n * sizeof(inv_t));

	for(i = 0; i < invArray->n; i++) {
		inv_read(fp,&(invArray->inv[i]));
	}
}

void invArray_print(FILE *fp, invArray_t invArray) {
	int i;
	fprintf(fp,"#\t%-20s %-20s\t%s\n","NOME","TIPO","STATISTICHE");
	for(i = 0; i < invArray->n; i++) {
		fprintf(fp,"%d\t",i);
		inv_print(fp,&(invArray->inv[i]));
	}
	fprintf(fp,"Numero oggetti in inventario: %d\n\n",invArray->n);

}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
	inv_print(fp,&(invArray->inv[index]));
}

/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t *invArray_getByIndex(invArray_t invArray, int index) {
	return &(invArray->inv[index]);
}

/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name) {
	int i;

	for(i = 0; i < invArray->n; i++) {
		if(strcmp(name,invArray->inv[i].nome) == 0)
			return i;
	}

	return -1;
}

