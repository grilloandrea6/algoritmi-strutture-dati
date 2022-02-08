#include "st.h"
#include <stdlib.h>
#include <string.h>

#include "debug.h"

// vettore non ordinato
// inserimento O(1)
// ricerca O(n)

struct st_s {
	char **name_vect;
	int max_n,n;
};


static int search(ST st, char *name);

ST STinit(int maxN) {
	ST st = malloc(sizeof(struct st_s));
	st->name_vect = malloc(maxN * sizeof(char *));
	st->n = 0;
	st->max_n = maxN;
	return st;
}


int STgetIndex(ST st, char *name) {
	int index = search(st,name);
	if(index != -1)
		return index;
	
	// non dovrebbe essere necessario, 
	// i nodi vengono caricati prima degli edges
	DEBUG("errore, arco utilizza un nodo che non è già in memoria");
	return STinsert(st,name);
}

int STinsert(ST st, char *name) {
	st->name_vect[st->n++] = strdup(name);
	return st->n-1;
}

char *STgetName(ST st, int index) {
	return st->name_vect[index];
}

void STfree(ST st) {
	for(int i = 0; i < st->n; i++)
		free(st->name_vect[i]);
	free(st->name_vect);
	free(st);
}

static int search(ST st, char *name) {
	for(int i = 0; i < st->n; i++) {
		if(strcmp(name,st->name_vect[i]) == 0)
			return i;
	}
	return -1;
}