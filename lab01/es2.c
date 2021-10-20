/*
 * Laboratorio 1 Esercizio 2
 * Da laboratorio 6 esercizio 1 TdP
 * Autore: Andrea Grillo
 * Data: Maggio 2021
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXL_COMANDO    15
#define MAXL            100
#define FILENAME        "corse.txt"
#define MAX_RECORD      1000

typedef enum {
    c_date, c_partenza, c_capolinea, c_ritardo, c_ritardo_tot, c_fine, c_errore
} comandi;

typedef struct{
    char codice_tratta[MAXL + 1];
    char partenza[MAXL + 1];
    char destinazione[MAXL + 1];
    char data[MAXL + 1];
    char ora_partenza[MAXL + 1];
    char ora_arrivo[MAXL + 1];
    int ritardo;
} record;

comandi leggiComando();
void strToLower(char *);
int openFile(FILE **);
int getData(FILE **, record *);
int menuParola(record *, int);
void printRecord(record);

int f_date(record *dati, int num_record);
int f_partenza(record *dati, int num_record);
int f_capolinea(record *dati, int num_record);
int f_ritardo(record *dati, int num_record);
int f_ritardo_tot(record *dati, int num_record);

int main() {
    FILE * fp;
    record dati[MAX_RECORD];
    int numero_record;

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
    while(menuParola(dati,numero_record));

    return 0;
}

comandi leggiComando () {
    comandi c;
    char cmd[MAXL_COMANDO];
    char tabella[c_errore][MAXL_COMANDO] = {"date","partenza","capolinea","ritardo","ritardo_tot","fine"};

    printf("Menù comandi:\n\t- date: elencare tutte le corse partite in un certo intervallo di date\n\t- partenza: elencare tutti le corse partite da una certa fermata\n\t- capolinea: elencare tutti le corse aventi una specifica destinazione \n\t- ritardo: elencare tutte le corse che hanno raggiunto la destinazione in ritardo, in un certo intervallo di date\n\t- ritardo_tot: elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta\n\t- fine: terminare il programma\nInserisci il comando: ");
    scanf("%s",cmd);

    strToLower(cmd);

    c=c_date;

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

int menuParola(record *dati, int num) {
    comandi cmd = leggiComando();
    int ret;

    switch (cmd) {
        case c_date:
            ret = f_date(dati,num);
            break;
        case c_partenza:
            ret = f_partenza(dati,num);
            break;
        case c_capolinea:
            ret = f_capolinea(dati,num);
            break;
        case c_ritardo:
            ret = f_ritardo(dati,num);
            break;
        case c_ritardo_tot:
            ret = f_ritardo_tot(dati,num);
            break;
        case c_fine:
            printf("Arrivederci!\n");
            ret = 0;
            break;

        case c_errore:
        default:
            printf("Errore nella lettura del comando.\n");

            ret = -1;
            break;
    }
    return(ret);
}

int getData(FILE **fp, record *dati) {
    int numero_record;

    if(fscanf(*fp,"%d\n",&numero_record) != 1 || numero_record > MAX_RECORD)
     return(-1);


    for(int i = 0; i < numero_record; i++) {
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

void printRecord(record rec) {
    printf("%s %s %s %s %s %s %d\n",
           rec.codice_tratta,
           rec.partenza,
           rec.destinazione,
           rec.data,
           rec.ora_partenza,
           rec.ora_arrivo,
           rec.ritardo);
}

int f_date(record *dati, int num_record){
    char data_inizio[MAXL + 1],
        data_fine[MAXL + 1];

    printf("Inserisci la data di inizio (formato yyy/mm/dd): ");
    if(scanf(" %s",data_inizio) != 1) return(-1);


    printf("Inserisci la data di fine (formato yyy/mm/dd): ");
    if(scanf(" %s",data_fine) != 1) return(-1);

    for(int i = 0; i < num_record; i++) {
        if(strcmp(dati[i].data,data_inizio) >= 0 && strcmp(dati[i].data,data_fine) <= 0) {
            printRecord(dati[i]);
        }
    }
    return(1);
}

int f_partenza(record *dati, int num_record){
    char partenza[MAXL + 1];
    printf("Inserisci il nome della fermata di partenza: ");
    if(scanf(" %s",partenza) != 1) return(-1);

    for(int i = 0; i < num_record; i++) {
        if(strcmp(dati[i].partenza,partenza) == 0) {
            printRecord(dati[i]);
        }
    }
    return(1);
}

int f_capolinea(record *dati, int num_record){
    char capolinea[MAXL + 1];
    printf("Inserisci il nome del capolinea: ");
    if(scanf(" %s",capolinea) != 1) return(-1);

    for(int i = 0; i < num_record; i++) {
        if(strcmp(dati[i].destinazione,capolinea) == 0) {
            printRecord(dati[i]);
        }
    }
    return(1);
}

int f_ritardo(record *dati, int num_record){
    char data_inizio[MAXL + 1],
            data_fine[MAXL + 1];

    printf("Inserisci la data di inizio (formato yyy/mm/dd): ");
    if(scanf(" %s",data_inizio) != 1) return(-1);


    printf("Inserisci la data di fine (formato yyy/mm/dd): ");
    if(scanf(" %s",data_fine) != 1) return(-1);

    for(int i = 0; i < num_record; i++) {
        if (strcmp(dati[i].data, data_inizio) >= 0 && strcmp(dati[i].data, data_fine) <= 0 && dati[i].ritardo > 0) {
            printRecord(dati[i]);
        }
    }
    return(1);
}

int f_ritardo_tot(record *dati, int num_record) {
    char codice[MAXL + 1];
    int ritardo = 0;
    printf("Inserisci il codice della tratta di cui vuoi verificare il ritardo: ");
    if(scanf(" %s",codice) != 1) return(-1);

    for(int i = 0; i < num_record; i++) {
        if(strcmp(dati[i].codice_tratta,codice) == 0) {
            ritardo += dati[i].ritardo;
        }
    }
    printf("Il ritardo totale accumulato dalla tratta di codice %s è di %d minuti.\n",codice,ritardo);
    return(1);
}