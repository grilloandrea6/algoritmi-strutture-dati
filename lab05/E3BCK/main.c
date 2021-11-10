#include <stdio.h>
#include <string.h>

#include "personaggio.h"
#include "inventario.h"
#include "debug.h"

#define MAXL_FILE		256
#define MAXL_COMANDO	28


typedef enum {
	c_stampa_pg, c_stampa_inv, c_aggiungi_pg, c_elimina_pg, c_stampa_oggetti_pg, c_aggiungi_oggetto, c_rimuovi_oggetto, c_calcola_statistiche, c_fine, c_errore
} comandi;


// prototipi
int menuParola(tabPg_t *tabella_personaggi, tabInv_t tabella_inventario);
comandi leggiComando();


int main() {
	tabPg_t *tabella_personaggi = allocaTabellaPersonaggi();
	tabInv_t tabella_inventario;
	char filename[MAXL_FILE];

#ifndef DEBUGGING
	printf("Inserisci il nome del file da cui leggere i personaggi: ");
	scanf("%s",filename);
#else
	strcpy(filename,"pg.txt");
#endif

	acquisisciPersonaggiFile(filename,tabella_personaggi);
#ifdef DEBUGGING
	printf("Stampo i personaggi per debug.\n");
	stampaPersonaggi(tabella_personaggi);
	printf("\n\n");
#endif
#ifndef DEBUGGING
	printf("Inserisci il nome del file da cui leggere l'inventario: ");
	scanf("%s",filename);
#else
	strcpy(filename,"inventario.txt");
#endif
	tabella_inventario = acquisisciInventarioFile(filename);
#ifdef DEBUGGING
	printf("Stampo gli oggetti in inventario per debug.\n");
	stampaOggetti(tabella_inventario);
	printf("\n\n");
#endif
	while (menuParola(tabella_personaggi,tabella_inventario));

	// TODO free

	return 0;
}


int menuParola(tabPg_t *tabella_personaggi, tabInv_t tabella_inventario) {
	comandi cmd = leggiComando();

	switch (cmd) {
		case c_stampa_pg:
				stampaPersonaggi(tabella_personaggi);
			break;
		case c_stampa_inv:
				stampaOggetti(tabella_inventario);
			break;
		case c_aggiungi_pg:
				// aggiungi personaggio in memoria
				aggiungiPersonaggio(tabella_personaggi);
			break;
		case c_elimina_pg:
				// elimina personaggio per codice
				eliminaPersonaggio(tabella_personaggi);	
			break;
		case c_stampa_oggetti_pg:
				// stampa gli oggetti di un personaggio
				stampaEquipaggiamento(tabella_personaggi);
			break;
		case c_aggiungi_oggetto:
				// aggiungi un oggetto ad un personaggio
				aggiungiOggetto(tabella_personaggi,tabella_inventario);
			break;
		case c_rimuovi_oggetto:
				// rimuovi un oggetto da un personaggio
				rimuoviOggetto(tabella_personaggi);
			break;
		case c_calcola_statistiche:
				printf("calcola statistiche!\n");	
				// calcola le statistiche di un personaggio
				stampaStatistiche(tabella_personaggi);
				// TODO print 0 if negative - make function floor
			break;
		case c_fine:
			printf("Arrivederci!\n");
			return(0);
			break;

		case c_errore:
		default:
			printf("Errore nella lettura del comando.\n");
			return(-1);
			break;
	}
	return 1;
}


comandi leggiComando () {
	comandi c;
	char cmd[MAXL_COMANDO];
	char tabella[c_errore][MAXL_COMANDO] = {"stampa_personaggi","stampa_inventario","aggiungi_personaggio","elimina_personaggio","stampa_oggetti_personaggio","aggiungi_oggetto","rimuovi_oggetto","calcola_statistiche","fine"};

	printf("\nMenù comandi:\n\t- stampa_personaggi\t\t: stampa la lista dei personaggi in memoria\n\t- stampa_inventario\t\t: stampa la lista degli oggetti in memoria\n\t- aggiungi_personaggio\t\t: aggiungi un nuovo personaggio alla lista\n\t- elimina_personaggio\t\t: elimina un personaggio da quelli in memoria\n\t- stampa_oggetti_personaggio\t: stampa gli oggetti appartenenti ad un personaggio\n\t- aggiungi_oggetto\t\t: aggiungi un oggetto nell'equipaggiamento di un personaggio\n\t- rimuovi_oggetto\t\t: elimina un oggetto dall'equipaggiamento di un personaggio\n\t- calcola_statistiche\t\t: calcola le statistiche di un personagio\n\t- fine\t\t\t\t: terminare il programma\n\nInserisci il comando: ");
	scanf("%s",cmd);

	c=c_stampa_pg;

	while(c<c_errore && strcmp(cmd,tabella[c])!=0)
		c++;

	return(c);
}
