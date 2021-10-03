/*
 * Laboratorio 1 Esercizio 3
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE

#define MAXL_COMANDO    15
#define MAXL            100
#define FILENAME        "corse.txt"
#define MAX_RECORD      1000

typedef enum {
    c_print, c_ord_data, c_ord_codice, c_ord_partenza, c_ord_arrivo, c_ricerca_lin, c_ricerca_bin, c_fine, c_errore
} comandi;

typedef enum {
   o_data, o_ora, o_codice, o_partenza, o_arrivo
} ordinamento;

typedef struct{
    int index, ritardo;
    char codice_tratta[MAXL + 1];
    char partenza[MAXL + 1];
    char destinazione[MAXL + 1];
    char data[MAXL + 1];
    char ora_partenza[MAXL + 1];
    char ora_arrivo[MAXL + 1];
} record;

comandi leggiComando();
void strToLower(char *);
int openFile(FILE **);
int getData(FILE **, record *);
int menuParola(record **, int);
void printRecord(record*);

int f_print(record **dati_punt, int num_record);
int sort(record **dati, int num_record, ordinamento ord);
int index_set(record **dati, int num_record);
int f_ricerca_lin(record **dati_punt,int num);
int f_ricerca_bin(record **dati_punt,int num);

int main() {
    FILE * fp;
    record dati[MAX_RECORD];
    record *dati_punt[MAX_RECORD];
    int numero_record;

    for(int i = 0; i < MAX_RECORD; i++)
	dati_punt[i] = &dati[i];

    // apertura file
    if(!openFile(&fp)) {
        fprintf(stderr,"Errore nell'apertura del file!\n");
        exit(EXIT_FAILURE);
    }

    // lettura dati da file
    numero_record = getData(&fp, dati);
    if(numero_record == -1) {
        fprintf(stderr,"Errore nella lettura del file!\n");
        exit(EXIT_FAILURE);
    }

    //chiusura file
    fclose(fp);

    printf("Ciao! Con questo programma puoi filtrare i dati del log.\n");
    printf("Caricati in memoria %d record.\n",numero_record);

    // stampa menu e parsing
    while(menuParola(dati_punt,numero_record));

    return 0;
}

comandi leggiComando () {
    comandi c;
    char cmd[MAXL_COMANDO];
    char tabella[c_errore][MAXL_COMANDO] = {"stampa","orddata","ordcodice","ordpartenza","ordarrivo","ricercalin","ricercabin","fine"};

    printf("Menù comandi:\n\t- stampa: stampa dei contenuti del log\n\t- orddata: ordinamento del vettore per data, e a parità di date per ora\n\t- ordcodice: ordinamento del vettore per codice di tratta\n\t- ordpartenza: ordinamento del vettore per stazione di partenza\n\t- ordarrivo: ordinamento del vettore per stazione di arrivo\n\t- ricercalin: ricerca -lineare- di una tratta per stazione di partenza\n\t- ricercabin: ricerca -binaria- di una tratta per stazione di partenza\n\t- fine: terminare il programma\nInserisci il comando: ");
    scanf("%s",cmd);

    strToLower(cmd);

    c=c_print;

    while(c<c_errore && strcmp(cmd,tabella[c])!=0)
        c++;

    return(c);
}

void strToLower(char *str) {
    for(int i = 0; str[i] != '\0'; i++)
        str[i] = tolower(str[i]);
}

int openFile(FILE **fp){
    *fp = fopen(FILENAME,"r");
    return(*fp != NULL);
}

int menuParola(record **dati_punt, int num) {
    comandi cmd = leggiComando();
    ordinamento ord;
    int ret;

    switch (cmd) {
        case c_print:
		ret = f_print(dati_punt,num);
            	break;
        case c_ord_data:
		ord = o_ora;
		sort(dati_punt,num,ord);		
		ord = o_data;
		sort(dati_punt,num,ord);
		ret = f_print(dati_punt,num);
            break;
        case c_ord_codice:
		ord = o_codice;
		sort(dati_punt,num,ord);
		ret = f_print(dati_punt,num);
	    break;
        case c_ord_partenza:
		ord = o_partenza;		
		sort(dati_punt,num,ord);
		ret = f_print(dati_punt,num);
            break;
        case c_ord_arrivo:
		ord = o_arrivo;
		sort(dati_punt,num,ord);
		ret = f_print(dati_punt,num);
            break;
	case c_ricerca_lin:
		ret = f_ricerca_lin(dati_punt,num);
	    break;
	case c_ricerca_bin:
		ret = f_ricerca_bin(dati_punt,num);
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
    return(ret);
}

int getData(FILE **fp, record *dati) {
    int numero_record;

    if(fscanf(*fp,"%d\n",&numero_record) != 1 || numero_record > MAX_RECORD)
     return(-1);


    for(int i = 0; i < numero_record; i++) {
	dati[i].index = i;
        if(fscanf(*fp,"%s %s %s %s %s %s %d",
                                dati[i].codice_tratta,
                                dati[i].partenza,
                                dati[i].destinazione,
                                dati[i].data,
                                dati[i].ora_partenza,
                                dati[i].ora_arrivo,
                                &dati[i].ritardo) != 7)    return(-1);

        // DEBUG printRecord(dati[i]);
    }

    return(numero_record);
}

void printRecord(record *rec) {
    printf("%s %s %s %s %s %s %d\n",
           rec->codice_tratta,
           rec->partenza,
           rec->destinazione,
           rec->data,
           rec->ora_partenza,
           rec->ora_arrivo,
           rec->ritardo);
}

int f_print(record **dati, int num_record) {
	for(int i = 0; i < num_record; i++)
		printRecord(dati[i]);
}

int compare(const void *a, const void *b, void *ord) {
	int ret;
	record *ptr_to_a = *(record**)a;
	record *ptr_to_b = *(record**)b;
	switch( *(ordinamento*)ord) {
		case o_data:
			ret = strcmp(ptr_to_a->data,(*((record**)b))->data);
			break;
		case o_ora:
			ret = strcmp(ptr_to_a->ora_partenza,(*((record**)b))->ora_partenza);
			break;
		case o_codice:
			ret = strcmp(ptr_to_a->codice_tratta,(*((record**)b))->codice_tratta);
			break;
		case o_partenza:
			ret = strcmp(ptr_to_a->partenza,(*((record**)b))->partenza);
			break;
		case o_arrivo:
			ret = strcmp(ptr_to_a->destinazione,(*((record**)b))->destinazione);
			break;
	}   
	
	// making quicksort stable using an index
	return ret != 0 ? ret : ptr_to_a->index - ptr_to_b->index;
}

int sort(record **dati, int num_record, ordinamento ord) {
	qsort_r(dati, num_record, sizeof(record*), compare, &ord);
	index_set(dati,num_record);
}

int index_set(record **dati, int num_record) {
	for(int i = 0; i < num_record; i++)
		dati[i]->index = i;
}

int f_ricerca_lin(record **dati_punt,int num) {
	char partenza[MAXL + 1],tmp[MAXL + 1];
	printf("Inserisci il nome della stazione di partenza che vuoi ricercare: ");
	if(scanf(" %s",partenza) != 1) return(-1);

	for(int i = 0; i < num; i++) {
		strncpy(tmp,dati_punt[i]->partenza,strlen(partenza));
		tmp[strlen(partenza)] = '\0';

		if(strcmp(partenza,tmp) == 0) printRecord(dati_punt[i]);
	}
}


int f_ricerca_bin(record **dati_punt,int num) {
	char partenza[MAXL + 1], tmp[MAXL+1];

	printf("Inserisci il nome della stazione di partenza che vuoi ricercare: ");
	if(scanf(" %s",partenza) != 1) return(-1);

	// data needs to be sorted
	ordinamento ord = o_partenza;		
	sort(dati_punt,num,ord);

	int found = 0;
	int i = num / 2, cmp,n_iterations=0;

	while(!found){
		strncpy(tmp,dati_punt[i]->partenza,strlen(partenza));
		tmp[strlen(partenza)] = '\0';
		
		cmp = strcmp(partenza,tmp);
		if(cmp > 0) {if(i == 3*i/2) i++; else i = 3*i/2;}
		else if(cmp < 0) {if(i == i/2) i--; else i = i/2;}
		else found = 1;
		
		if(i < 0 || i > num || n_iterations > num) {
			printf("Non trovato\n");
			return -1;
		}
		n_iterations++;
	}
	printRecord(dati_punt[i]);

	// controllo se ce ne sono altri subito prima o dopo di quello trovato
	//dopo
	for(int j = i+1; found && j < num; j++){
		strncpy(tmp,dati_punt[j]->partenza,strlen(partenza));
		if(strcmp(partenza,tmp) == 0)
			printRecord(dati_punt[j]);
		else
			found = 0;
			
	}

	//prima
	found = 1;
	for(int j = i-1; found && j >-1; j--){
		strncpy(tmp,dati_punt[j]->partenza,strlen(partenza));
		if(strcmp(partenza,tmp) == 0)
			printRecord(dati_punt[j]);
		else
			found = 0;	
	}
}
