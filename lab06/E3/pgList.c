#include "pgList.h"
#include "debug.h"

typedef struct nodo *link;

struct nodo {
	pg_t pers;
	link next;
};

struct pgList_s {
	link head;
	int n;	
};

/* creatore e distruttore */
pgList_t pgList_init() {
	pgList_t lista = malloc(sizeof(struct pgList_s));
	lista->n = 0;
	lista->head = NULL;
	return lista;
}

void pgList_free(pgList_t pgList) {
	link p,q;
	for(p = pgList->head; p != NULL; p = q) {
		pg_clean(&(p->pers));
		q = p->next;

		free(p);
	}
	if(pgList != NULL)
		free(pgList);
}

/* lettura e scrittura su file */
void pgList_read(FILE *fp, pgList_t pgList) {
	pg_t pers;
	while(pg_read(fp, &(pers))) {
		pgList_insert(pgList,pers);
	}
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
	link p;
	fprintf(fp,"%-7s %-20s %-20s\t%s\n","CODICE","NOME","CLASSE","STATISTICHE");
	for(p = pgList->head; p != NULL; p = p->next) {
		pg_print(fp, &(p->pers), invArray);
	}
	fprintf(fp,"Numero personaggi: %d\n\n",pgList->n);
}

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg) {
	link toAdd = malloc(sizeof(struct nodo));
	toAdd->pers = pg;
	toAdd->next = pgList->head;
	pgList->head = toAdd;
	pgList->n++;
}

/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod) {
	link p,q;

	// caso limite, eliminazione primo elemento
	if(strcmp(pgList->head->pers.cod,cod) == 0) {
		pg_clean(&(pgList->head->pers));
		q = pgList->head;
		free(q);
		pgList->head = pgList->head->next;
		return;
	}

	for(p = pgList->head; p != NULL; q = p, p = p->next) {
		if(strcmp(p->pers.cod,cod) == 0) {
			pg_print(stdout,&(p->pers),NULL);
			q->next = p->next;
			pg_clean(&(p->pers));
			free(p);
			return;
		}
	}
	fprintf(stderr,"Personaggio non trovato.\n");
}

/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char* cod) {
	link p;
	for(p = pgList->head; p != NULL; p = p->next) {
		if(strcmp(p->pers.cod,cod) == 0) {
			return &(p->pers);
		}
	}
	fprintf(stderr,"Personaggio non trovato.\n");

	return NULL;
}
