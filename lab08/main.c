#include <stdio.h>
#include <stdlib.h>

#include "title_collection.h"
#include "title.h"

#define N_SCELTE 7
#define MAXL 256

void stampaMenu(char *scelte[], int *selezione){
	int i=0;
	printf("\nMENU'\n");
	for(i=0;i<N_SCELTE;i++)
		printf("%2d > %s\n",i,scelte[i]);
	scanf("%d",selezione);
}




int main() {
	FILE *fp;
	int n_titoli,i,selezione,fineProgramma = 0;
	char stringa[MAXL];
	char *scelte[] = {
			"Uscita",
			"Acquisisci file",
			"Ricerca titolo azionario",
			"Ricerca quotazione titolo",
			"Ricerca quotazione minima e massima in intervallo date",
			"Ricerca quotazione minima e massima in periodo intero",
			"Stampa tutti i dati in memoria"
		};

	title_t titolo;
	title_collection_t collezione_titoli = title_collection_init();
	datetime_t date;

	do {
		stampaMenu(scelte, &selezione);
		switch(selezione){

		case 0: {
			fineProgramma = 1;
		} break;

		case 1: {
			printf("Inserisci il nome del file: ");
			scanf("%s",stringa);
			fp = fopen(stringa,"r");
			if(fp == NULL) {
				fprintf(stderr,"Errore apertura file.\n");
				exit(0);
			}
			title_collection_read_file(collezione_titoli,fp);

			fclose(fp);
		} break;

		case 2: {
			printf("Inserisci nome titolo da cercare: ");
			scanf("%s",stringa);
			titolo = title_collection_search(collezione_titoli, stringa);
			title_print(titolo);
		} break;

		case 3: {
			printf("Inserisci nome titolo da cercare: ");
			scanf("%s",stringa);
			titolo = title_collection_search(collezione_titoli, stringa);
			title_search_quotation(titolo);
		} break;

		case 4: {
			printf("Inserisci nome titolo da cercare: ");
			scanf("%s",stringa);
			titolo = title_collection_search(collezione_titoli, stringa);

			title_search_quotation_interval(titolo);	
		} break;

		case 5: {
			printf("Inserisci nome titolo da cercare: ");
			scanf("%s",stringa);
			titolo = title_collection_search(collezione_titoli, stringa);

			title_search_quotation_interval_total(titolo);	

		} break;

		case 6: {
			title_collection_print_all(collezione_titoli);

		} break;

		default: {
			printf("Scelta non valida!\n");
		} break;
		} 
	} while(!fineProgramma);

	title_collection_free(collezione_titoli);
	return 0;
}