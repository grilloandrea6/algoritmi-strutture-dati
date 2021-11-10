#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "personaggio.h"


// funzioni interne al modulo
void aggiungiNodo(tabPg_t *tabella, pg_t pers);
int leggiPersonaggio(pg_t *pers, FILE *fp);
void printPers(pg_t pers);
nodoPg_t* ricercaNodoCodice(char *codice, tabPg_t *tabella);

tabPg_t *allocaTabellaPersonaggi() {
	tabPg_t *tabella = malloc(sizeof(tabPg_t));
	tabella->head = NULL;
	tabella->tail = NULL;
	tabella->nPg = 0;
}

void acquisisciPersonaggiFile(char *filename, tabPg_t *tabella) {
	FILE *fp = NULL;
	pg_t pers;
	if(strcmp("stdout",filename) == 0) fp = stdout;
	else fp = fopen(filename,"r");

	if(fp == NULL) {
		fprintf(stderr, "Errore apertura file personaggi.\n");
		exit(-1);
	}

	while(leggiPersonaggio(&pers,fp)) {
		aggiungiNodo(tabella,pers);
	}

	if(strcmp("stdout",filename) != 0) fclose(fp);

	DEBUG("Numero personaggi in memoria: %d\n",tabella->nPg);

	return;
}


void stampaPersonaggi(tabPg_t *tabella) {
	nodoPg_t *nodo;
	for(nodo = tabella->head; nodo != NULL; nodo = nodo->next)
		printPers(nodo->val);
}

void aggiungiPersonaggio(tabPg_t *tabella) {
	pg_t pers;
	printf("Inserisci i dati del nuovo personaggio:\n");
	if(leggiPersonaggio(&pers,stdin)) {
		aggiungiNodo(tabella,pers);
		printf("Nodo aggiunto correttamente.\n");
		return;
	}

	fprintf(stderr,"Errore nella lettura del nuovo personaggio.\n");
}

/*void eliminaPersonaggio(tabPg_t *tabella) {
	char codice[MAXL];
	nodoPg_t *nodo, *prec = NULL;

	printf("Inserisci il codice del personaggio da eliminare: ");
	scanf("%s",codice);

	// ricerca lineare ed eliminazione
	for(nodo = tabella->head; nodo != NULL; prec = nodo, nodo = nodo->next) {
		if(strcmp(codice,nodo->val.codice) == 0) {
			printf("Eliminazione di: ");
			printPers(nodo->val);
			DEBUG("codice precedente = %s\ncodice successivo = %s\n",prec->val.codice, nodo->next->val.codice);

			if(prec == NULL) {
				DEBUG("è il primo nodo\n");
				tabella->head = nodo->next;
			} else
				prec->next = nodo->next;

			if(tabella->tail == nodo) {
				DEBUG("è l'ultimo/unico nodo\n");
				tabella->tail = prec;
			}

			tabella->nPg--;

			free(nodo);

			printf("Eliminazione effettuata correttamente!\n");
			return;
		}
	}

	fprintf(stderr, "Personaggio non trovato in memoria!\n");
	return;
}*/


void eliminaPersonaggio(tabPg_t *tabella) {
	char codice[MAXL];
	nodoPg_t *nodo, *prec = NULL;

	printf("Inserisci il codice del personaggio da eliminare: ");
	scanf("%s",codice);

	nodo = ricercaNodoCodice(codice,tabella);
	if(nodo == NULL) {
		fprintf(stderr,"Codice non trovato!\n");
		return;
	}
	
	// ricerca del precedente
	for(prec = tabella->head; prec != NULL && prec->next != nodo; prec = prec->next);

	printf("Eliminazione di: ");
	printPers(nodo->val);
	DEBUG("codice precedente = %s\ncodice successivo = %s\n",prec->val.codice, nodo->next->val.codice);

	if(prec == NULL) {
		DEBUG("è il primo nodo\n");
		tabella->head = nodo->next;
	} else
		prec->next = nodo->next;

	if(tabella->tail == nodo) {
		DEBUG("è l'ultimo nodo\n");
		tabella->tail = prec;
	}

	tabella->nPg--;

	free(nodo);

	printf("Eliminazione effettuata correttamente!\n");
	return;
}

void stampaEquipaggiamento(tabPg_t *tabella) {
	char str[MAXL];
	nodoPg_t *nodo;
	int i;

	printf("Inserisci il codice del personaggio di cui stampare gli oggetti: ");
	scanf("%s",str);

	nodo = ricercaNodoCodice(str,tabella);
	if(nodo == NULL) {
		fprintf(stderr,"Codice non trovato!\n");
		return;
	}

	if(nodo->val.equip.inUso == 0) {
		printf("Non sono presenti oggetti per questo personaggio.\n");
		return;
	}

	printf("Oggetti del personaggio '%s':\n", nodo->val.nome);
	tabInv_t tabella_oggetti = {nodo->val.equip.vettEq,nodo->val.equip.inUso,MAX_OGGETTI};
	stampaOggetti(tabella_oggetti);

	return;
}

void aggiungiOggetto(tabPg_t *tabella_personaggi, tabInv_t tabella_inventario) {
	char str[MAXL];
	nodoPg_t *nodo;
	inv_t oggetto;

	printf("Inserisci il codice del personaggio a cui aggiungere un oggetto: ");
	scanf("%s",str);

	nodo = ricercaNodoCodice(str,tabella_personaggi);
	if(nodo == NULL) {
		fprintf(stderr,"Codice non trovato!\n");
		return;
	}
	
	if(nodo->val.equip.inUso >= MAX_OGGETTI) {
		fprintf(stderr,"Mi dispiace, limite massimo di oggetti raggiunto.\n");
		return;
	}

	printf("Inserisci il nome dell'oggetto da aggiungere: ");
	scanf("%s",str);

	if(ricercaOggettoNome(str,tabella_inventario,&oggetto))
		nodo->val.equip.vettEq[nodo->val.equip.inUso++] = oggetto;
	else
		fprintf(stderr,"Oggetto non trovato!\n");

	return;
}

void rimuoviOggetto(tabPg_t *tabella) {
	char str[MAXL];
	nodoPg_t *nodo;
	int i;

	printf("Inserisci il codice del personaggio a cui rimuovere un oggetto: ");
	scanf("%s",str);

	nodo = ricercaNodoCodice(str, tabella);
	if(nodo == NULL) {
		fprintf(stderr,"Codice non trovato!\n");
		return;
	}

	printf("Inserisci il nome dell'oggetto da rimuovere: ");
	scanf("%s",str);

	for(i = 0; i < nodo->val.equip.inUso && strcmp(str,nodo->val.equip.vettEq[i].nome) != 0; i++);
	
	if(i == nodo->val.equip.inUso && strcmp(str,nodo->val.equip.vettEq[i].nome) != 0) {
		printf("Oggetto non trovato nell'equipaggiamento del personaggio.\n");
		return;
	}

	// shifting degli altri elementi
	for(i; i < nodo->val.equip.inUso; i++)
		nodo->val.equip.vettEq[i] = nodo->val.equip.vettEq[i+1];

	nodo->val.equip.inUso--;
}

void stampaStatistiche(tabPg_t *tabella) {
	char str[MAXL];
	nodoPg_t *nodo;
	int i;

	printf("Inserisci il codice del personaggio di cui calcolare le statistiche: ");
	scanf("%s",str);
	
	nodo = ricercaNodoCodice(str, tabella);
	if(nodo == NULL) {
		fprintf(stderr,"Codice non trovato!\n");
		return;
	}

	printStat(calcolaStatistiche(nodo->val.stat, nodo->val.equip.vettEq,nodo->val.equip.inUso));
	return;
}

void aggiungiNodo(tabPg_t *tabella, pg_t pers) {
	nodoPg_t *nodo = malloc(sizeof(nodoPg_t));
	nodo->val = pers;
	nodo->next = NULL;

	if(tabella->head == NULL) {
		tabella->head = tabella->tail = nodo;
	} else {
		tabella->tail->next = nodo;
		tabella->tail = nodo;
	}
	tabella->nPg++;
}

int leggiPersonaggio(pg_t *pers, FILE *fp) {
	int ret;
	pers->equip.inUso = 0;
	ret = fscanf(fp,"%s %s %s %d %d %d %d %d %d",
		(*pers).codice,
		(*pers).nome,
		(*pers).classe,
		&((*pers).stat.hp),
		&((*pers).stat.mp),
		&((*pers).stat.atk),
		&((*pers).stat.def),
		&((*pers).stat.mag),
		&((*pers).stat.spr));	
	return (ret == 9);
}

void printPers(pg_t pers) {
	printf("%s %s %s %d %d %d %d %d %d\n",
		pers.codice,
		pers.nome,
		pers.classe,
		pers.stat.hp,
		pers.stat.mp,
		pers.stat.atk,
		pers.stat.def,
		pers.stat.mag,
		pers.stat.spr);

#ifdef DEBUGGING
	// in caso di segfault può saltare l'output su stdout
	// se non si fa il fflush del buffer
	fflush(stdout);
#endif

	return;
}

nodoPg_t* ricercaNodoCodice(char *codice, tabPg_t *tabella) {
	nodoPg_t *nodo;

	// ricerca lineare ed eliminazione
	for(nodo = tabella->head; nodo != NULL; nodo = nodo->next) {
		if(strcmp(codice,nodo->val.codice) == 0) {
			return nodo;
		}
	}
	return NULL;
}