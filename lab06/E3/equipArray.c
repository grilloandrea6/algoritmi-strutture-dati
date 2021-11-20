#include "equipArray.h"


/* ADT di prima classe collezione di oggetti di equipaggiamento */
struct equipArray_s {
	int *array;
	int n,max_n;
};

/* creatore e distruttore */
equipArray_t equipArray_init() {
	equipArray_t equipArray = malloc(sizeof(struct equipArray_s));
	equipArray->n = 0;
	equipArray->max_n = EQUIP_SLOT;

	// possibile fare riallocazione se necessario aggiugere altri elementi
	equipArray->array = malloc(EQUIP_SLOT * sizeof(int));

	if(equipArray->array == NULL) {
		fprintf(stderr, "Errore allocazione dinamica.\n");
		exit(EXIT_FAILURE);
	}

	return equipArray;
}

void equipArray_free(equipArray_t equipArray) {
	if(equipArray != NULL && equipArray->array != NULL)
		free(equipArray->array);

	if(equipArray != NULL)
		free(equipArray);
}

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray) {
	return equipArray->n;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
	int i;
		if(equipArray->n == 0) return;

	fprintf(fp,"\tEquipaggiamenti:\n\t\t%-20s %-20s\t%s\n","NOME","TIPO","STATISTICHE");
	for(i = 0; i < equipArray->n; i++) {
		fprintf(fp,"\t\t");
		invArray_printByIndex(fp,invArray,equipArray->array[i]);
	}
}


/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
	int i;
	if(equipArray->n >= equipArray->max_n) {
		fprintf(stderr, "Errore, slot per l'aggiunta di equipaggiamento pieni.\nRiallocazione non implementata.\n");
		return;
	}

	printf("Equipaggiamenti disponibili:\n");
	invArray_print(stdout,invArray);

	printf("Scegli quale equipaggiamento aggiungere: ");
	scanf("%d",&i);
	equipArray->array[equipArray->n++] = i;
}

/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
	if(index >= equipArray->n) {
		fprintf(stderr, "Errore, equipaggio non esistente!\n");
	}
	return(equipArray->array[index]);
}
