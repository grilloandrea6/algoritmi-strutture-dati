/*
 * Laboratorio 1 Esercizio 4
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

/*
 * Utilizza per l'ordinamento la funzione qsort della libreria C.
 * L'esercizio richiede che l'ordinamento sia stabile, ma qsort si basa
 * sull'algoritmo quicksort che non è stabile.
 * Per renderlo stabile, nella struttura dati è stato aggiunto un campo index.
 * Il campo index viene settato dalla funzione index_set, in ordine crescente.
 * Nella funzione "compare", oltre alle comparazioni in base al tipo di ordinamento,
 * a parità di chiave, si indica come "minore" quello con indice inferiore.
 * Dopo ogni ordinamento i valori dell'index vengono aggiornati.
 */

/*
 * Per l'ordinamento per data, si è utilizzato un approccio simile a quello del 
 * RadixSort. I dati vengono ordinati prima per orario, e poi per data con 
 * ordinamento stabile.
 */

// necessario per evitare warning
// funzione qsort_r
#define _GNU_SOURCE

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXL_COMANDO    15
#define MAXL            100
#define FILENAME        "corse.txt"
#define MAX_RECORD      1000

typedef enum {
    c_print, c_ricerca_bin, c_fine, c_errore
} comandi;

typedef enum {
   o_ora, o_data, o_codice, o_partenza, o_arrivo, o_errore
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

// prototipi
void printRecord(record *rec);
comandi leggiComando ();
void strToLower(char *str);
int openFile(FILE **fp);
int menuParola(record *dati_punt[][MAX_RECORD], int num);
int getData(FILE **fp, record *dati);
int f_print(record *dati_punt[][MAX_RECORD], int num_record);
int compare(const void *a, const void *b, void *ord);
int sort(record **dati, int num_record, ordinamento ord);
int index_set(record **dati, int num_record);
void getElement(char *tmp, record *record, int len, ordinamento ord);
int f_ricerca_bin(record *dati_punt[][MAX_RECORD],int num);


int main() {
	FILE * fp;
	record dati[MAX_RECORD];
	record *dati_punt[4][MAX_RECORD];
	int numero_record;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < MAX_RECORD; j++)
			dati_punt[i][j] = &dati[j];

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

	// sorting dei dati
	ordinamento ord = o_ora;

	// per la data ordino prima per ora
	sort(dati_punt[0],numero_record,ord++);	

	for(int i = 0; ord < o_errore; i++, ord++)
		sort(dati_punt[i],numero_record,ord);

	printf("Ciao! Con questo programma puoi filtrare i dati del log.\n");
	printf("Caricati in memoria %d record.\n",numero_record);

	// stampa menu e parsing
	while(menuParola(dati_punt,numero_record));

	return 0;
}

void printRecord(record *rec) {
    printf("%s\t%-10s\t%s\t%s\t%s\t%s\t%d\n",
           rec->codice_tratta,
           rec->partenza,
           rec->destinazione,
           rec->data,
           rec->ora_partenza,
           rec->ora_arrivo,
           rec->ritardo);
}


comandi leggiComando () {
    comandi c;
    char cmd[MAXL_COMANDO];
    char tabella[c_errore][MAXL_COMANDO] = {"stampa","ricerca","fine"};

    printf("Menù comandi:\n\t- stampa: stampa dei contenuti del log\n\t- ricerca: ricerca dicotomica di una tratta\n\t- fine: terminare il programma\nInserisci il comando: ");
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

int menuParola(record *dati_punt[][MAX_RECORD], int num) {
    comandi cmd = leggiComando();
    ordinamento ord;
    int ret;

    switch (cmd) {
        case c_print:
		ret = f_print(dati_punt,num);
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


int f_print(record *dati_punt[][MAX_RECORD], int num_record) {
	char cmd[MAXL + 1];

	char tabella[o_errore][MAXL_COMANDO] = {"data", "codice", "partenza", "arrivo"};

	printf("La lista può essere stampata in ordine secondo i seguenti parametri: data, codice, partenza, arrivo.\nInserisci il parametro: ");
	if(scanf(" %s",cmd) != 1) return(-1);

	ordinamento ord=o_data;

    	while(ord<o_errore && strcmp(cmd,tabella[ord-1])!=0)
        	ord++;

	if(ord == o_errore) {
		printf("Errore nella lettura del parametro.\n");
		return(-1);
	}

	for(int i = 0; i < num_record; i++)
		printRecord(dati_punt[ord-1][i]);
}

int compare(const void *a, const void *b, void *ord) {
	int ret;
	record *ptr_to_a = *(record**)a;
	record *ptr_to_b = *(record**)b;

	switch( *(ordinamento*)ord) {
		case o_data:
			ret = strcmp(ptr_to_a->data, ptr_to_b->data);
			break;
		case o_ora:
			ret = strcmp(ptr_to_a->ora_partenza, ptr_to_b->ora_partenza);
			break;
		case o_codice:
			ret = strcmp(ptr_to_a->codice_tratta, ptr_to_b->codice_tratta);
			break;
		case o_partenza:
			ret = strcmp(ptr_to_a->partenza, ptr_to_b->partenza);
			break;
		case o_arrivo:
			ret = strcmp(ptr_to_a->destinazione, ptr_to_b->destinazione);
			break;
	}   
	
	// rendo il quicksort stabile utilizzando un indice
	// se i dati sono uguali prendo quello che stava prima
	// alla fine di ogni ordinamento viene riaggiornato l'indice
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

void getElement(char *tmp, record *record, int len, ordinamento ord) {
	switch(ord) {
		case o_data:
			strncpy(tmp, record->data, len);
			break;
		case o_ora:
			strncpy(tmp, record->ora_partenza, len);
			break;
		case o_codice:
			strncpy(tmp, record->codice_tratta, len);
			break;
		case o_partenza:
			strncpy(tmp, record->partenza, len);
			break;
		case o_arrivo:
			strncpy(tmp, record->destinazione, len);
			break;
	}   

	tmp[len] = '\0';
}

int f_ricerca_bin(record *dati_punt[][MAX_RECORD],int num) {
	char tosearch[MAXL + 1], tmp[MAXL+1], cmd[MAXL+1];

	char tabella[o_errore][MAXL_COMANDO] = {"data", "codice", "partenza", "arrivo"};

	printf("Puoi ricercare secondo i seguenti parametri: data, codice, partenza, arrivo.\n Inserisci il parametro: ");
	if(scanf(" %s",cmd) != 1) return(-1);

	ordinamento ord=o_data;

    	while(ord<o_errore && strcmp(cmd,tabella[ord-1])!=0)
        	ord++;
	
	if(ord == o_errore){
		printf("Errore nella lettura del parametro.\n");
		return(-1);
	}

	record **lista = dati_punt[ord-1];

	printf("Inserisci il nome che vuoi ricercare: ");
	if(scanf(" %s",tosearch) != 1) return(-1);

	int found = 0;
	int i = num / 2, cmp,n_iterations=0;

	while(!found){
		getElement(tmp, lista[i], strlen(tosearch),ord);

		cmp = strcmp(tosearch,tmp);

		if(cmp > 0) {if(i == 3*i/2 || 3*i/2 == num) i++; else i = 3*i/2;}
		else if(cmp < 0) {if(i == i/2) i--; else i = i/2;}
		else found = 1;

		if(i < 0 || i > num || n_iterations > num) {
			printf("Non trovato\n");
			return -1;
		}
		n_iterations++;
	}
	printRecord(lista[i]);

	// controllo se ce ne sono altri subito prima o dopo di quello trovato
	//dopo
	for(int j = i+1; found && j < num; j++){
		getElement(tmp, lista[j], strlen(tosearch),ord);

		if(strcmp(tosearch,tmp) == 0)
			printRecord(lista[j]);
		else
			found = 0;
			
	}

	//prima
	found = 1;
	for(int j = i-1; found && j >-1; j--){
		getElement(tmp, lista[j], strlen(tosearch),ord);

		if(strcmp(tosearch,tmp) == 0)
			printRecord(lista[j]);
		else
			found = 0;	
	}
}
