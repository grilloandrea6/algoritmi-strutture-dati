#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAXL_COMANDO 255
#define MAXL_FILENAME 255
#define MAXL_STR 51
#define L_CODICE 6
#define L_DATA 11


typedef struct Item {
	char codice[L_CODICE],
		nome[MAXL_STR],
		cognome[MAXL_STR],
		data[L_DATA],
		via[MAXL_STR],
		citta[MAXL_STR],
		cap[MAXL_STR];
} Item;

typedef struct node node_t, *link;
struct node {
	Item val;
	link next;
};

typedef enum {
	c_nuovo, c_leggi, c_ricerca, c_cancellacodice, c_cancelladate, c_stampa, c_fine, c_errore
} comandi;


comandi leggiComando ();
void convertData(char * data, char *converted);
int itemCmp(Item a, Item b);
int itemPrint(Item item, FILE *fp);
link newNode(Item val, link next);
int scanNode(FILE *fp, Item *item);
link getData(link h,char *filename);
link SortListIns(link h, Item val);
Item listExtrHeadP(link *hp);
int estraiData(link *h,Item start,Item end);

link f_nuovo(link head);
link f_leggi(link head);
void f_ricerca(link head);
link f_cancellacodice(link h);
void f_stampa(link head);
link f_cancelladate(link h);


void main() {
	link head = NULL;

	link t;
	int exit = 0;
	comandi cmd;
	while(!exit) {
		cmd = leggiComando();

		switch (cmd) {
			case c_nuovo:
				head = f_nuovo(head);
				break;
			case c_leggi:
				head = f_leggi(head);
				break;
			case c_ricerca:
				f_ricerca(head);
				break;
			case c_cancellacodice:
				head = f_cancellacodice(head);
				break;
			case c_cancelladate:
				head = f_cancelladate(head);
				break;
			case c_stampa:
				f_stampa(head);
				break;

			case c_fine:
				printf("Arrivederci!\n");
				exit = 1;
				break;

			case c_errore:
			default:
				printf("Errore nella lettura del comando.\n");
 				break;
		}
	}
}

comandi leggiComando () {
	comandi c;
	char cmd[MAXL_COMANDO];
	char tabella[c_errore][MAXL_COMANDO] = {"nuovo","leggifile","ricerca","cancella","cancellaintervallo","stampafile","fine"};

	printf("Menù comandi:\n\t- nuovo: crea nuovo record leggendo da tastiera\n\t- leggifile: leggi dati da file\n\t- ricerca: ricerca un record per codice\n\t- cancella: cancella un record per codice\n\t- cancellaintervallo: cancella un intervallo di record compresi fra due date\n\t- stampafile: stampa su file i dati\n\t- fine: terminare il programma\nInserisci il comando: ");
	scanf("%s",cmd);
	c=c_nuovo;

	while(c<c_errore && strcmp(cmd,tabella[c])!=0)
		c++;

	return(c);
}

link f_nuovo(link head){
	Item item;
	printf("Inserisci i dati del nuovo record: ");
	if(!scanNode(stdin,&item)) {
		printf("Errore nella lettura del record!\n");
		return head;
	}
	return SortListIns(head,item); 
}

link f_leggi(link head) {
	char nomefile[MAXL_FILENAME];
	FILE *fp;
	printf("Inserisci il nome del file da leggere: ");
	scanf("%s",nomefile);
	
	head = getData(head,nomefile);
	
	return head;
}

void f_ricerca(link head) {
	link t;
	char codice[L_CODICE];
	int found = 0;

	printf("Inserisci codice da ricercare: ");
	scanf("%s",codice);

	
	if(head == NULL) {
		printf("Non ci sono dati in memoria!\n");
		return;
	}

	for(t = head; t != NULL; t = t->next)
		if(strcmp(codice, t->val.codice) == 0) {
			found = 1;
			itemPrint(t->val,stdout);
		}

	if(!found) printf("Codice non trovato in memoria!\n");
}

link f_cancellacodice(link h) {
	char codice[L_CODICE];
	int found = 0;
	printf("Inserisci codice da eliminare: ");
	scanf("%s",codice);

	link x, p;
	if(h == NULL) {
		printf("Non ci sono dati in memoria!\n");
		return h;
	}

	for (x=h, p=NULL; x!=NULL; p=x, x=x->next) {
		if(strcmp(x->val.codice,codice) == 0) {
			found = 1;
			if(x==h)
				h = x->next;
			else
				p->next = x->next;
			itemPrint(x->val,stdout);
			free(x);
			break;
		}
	}
	if(!found) printf("Codice non trovato in memoria!\n");
	return h;
}

void f_stampa(link head) {
	char filename[MAXL_FILENAME];
	link t;
	FILE *fp;

	printf("Inserisci il nome del file su cui stampare i dati (scrivi 'video' per stampare a video): ");
	scanf("%s",filename);
	if(strcmp(filename,"video") == 0)
		fp = stdout;
	else
		fp = fopen(filename,"w");

	if(fp == NULL) {
		printf("Errore nell'apertura del file.");
		return;
	}

	for(t = head; t != NULL; t = t->next)
		itemPrint(t->val,fp);
	
	if(fp != stdout) fclose(fp);
}

link f_cancelladate(link h) {
	int found = 0;
	Item start,end;
	printf("Inserisci data iniziale: ");
	scanf("%s",start.data);
	printf("Inserisci data finale: ");
	scanf("%s",end.data);

	if(h == NULL) {
		printf("Non ci sono dati in memoria!\n");
		return h;
	}

	while(estraiData(&h,start,end)) found = 1;	

	if(!found) printf("Non ci sono record in memoria tra quelle date!\n");
	return h;
}

void convertData(char * data, char *converted) {
	// conversione data da formato italiano
	// gg/mm/aaaa a aaaammgg per fare confronto
	// con strcmp
	int i;
	for(i = 0; i < 4; i++)
		converted[i] = data[i + 6];

	converted[4] = data[3];
	converted[5] = data[4];

	converted[6] = data[0];
	converted[7] = data[1];
	converted[8] = '\0';
}

int itemCmp(Item a, Item b) {
	char a_data[L_DATA], b_data[L_DATA];
	convertData(a.data, a_data);
	convertData(b.data, b_data);
	
	return(strcmp(a_data,b_data));
}

int itemPrint(Item item, FILE* fd) {
	fprintf(fd, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", 
			item.codice,
			item.nome,
			item.cognome,
			item.data,
			item.via,
			item.citta,
			item.cap);
}

link newNode(Item val, link next) {
	link x = malloc(sizeof *x);
	if(x==NULL)
		return NULL;
	else {
		x->val = val;
		x->next = next;
	}
	return x;
}

int scanNode(FILE *fp, Item *item) {
	if(fscanf(fp,"%s %s %s %s %s %s %s",
		item->codice,
		item->nome,
		item->cognome,
		item->data,
		item->via,
		item->citta,
		item->cap) == 7)
		return 1;
	return 0;
}

link getData(link head, char *filename) {	
	Item item;

	FILE *fp = fopen(filename,"r");
	if(fp == NULL) {
		fprintf(stderr,"errore apertura file.\n");
		exit(-1);
	}	

	while(scanNode(fp,&item) && !feof(fp)) {
		itemPrint(item,stdout);
		head = SortListIns(head,item);
	}

	fclose(fp);

	return head;
}

link SortListIns(link h, Item val) {
	link x, p;

	if(h==NULL || itemCmp(h->val,val) > 0) {
		return newNode(val, h);
	}

	for (x=h->next, p=h; x!=NULL && itemCmp(val,x->val) > 0; p=x, x=x->next);

	p->next = newNode(val, x);
	return h;
}

Item listExtrHeadP(link *hp) {
	link t = *hp;
	Item tmp;
	if(t == NULL)
		return tmp;
	tmp= t->val;
	*hp= t->next;

	free(t);
	return tmp;
}
	
int estraiData(link *h,Item start,Item end) {
	link x, p;

	for (x=*h, p=NULL; x!=NULL; p=x, x=x->next) {
		if(itemCmp(x->val,start) >= 0 && itemCmp(x->val,end) <= 0) {
			if(x==*h)
				*h = x->next;
			else
				p->next = x->next;
			itemPrint(x->val,stdout);
			free(x);
			return 1;
		}
	}
	return 0;
}