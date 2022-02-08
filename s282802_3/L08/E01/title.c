#include "title.h"


struct title_s{
	char name[MAXL_NAME];
	quotation_collection_t quotations;	
};



title_t title_init() {
	title_t T = malloc(sizeof(struct title_s));
	T->quotations = quotation_collection_init();
	return T;	
}

void title_free(title_t title) {
	quotation_collection_free(title->quotations);
	free(title);
}

void title_read(title_t title, char* nome, FILE *fp) {
	printf("reading new title\n");
	int n_quotations,i;
	if(fp == NULL) {
		fprintf(stderr, "file non valido!!\n");
		exit(EXIT_FAILURE);
	}
	//fscanf(fp,"%s",title->name);
	//title->name = strdup(nome);
	strcpy(title->name,nome);
	title_read_quotations(title, fp);
}

void title_read_quotations(title_t title, FILE *fp) {
	quotation_collection_read_quotations(title->quotations, fp);
}

int title_cmp(title_t a, title_t b) {
	return strcmp(a->name,b->name);
}

char *title_get_name(title_t t) {
	return t->name;
}

void title_print(title_t t) {
	printf("Nome: %s\n", t->name);
	quotation_collection_print(t->quotations);
}


void title_search_quotation(title_t titolo) {
	datetime_t date;

	printf("Inserisci la data del giorno di cui ricercare la quotazione (aaaa/mm/gg): ");
	scanf("%d/%d/%d",&(date.anno),&(date.mese),&(date.giorno));
	quotation_collection_search_print(titolo->quotations, date);
}

void title_search_quotation_interval(title_t t) {
	datetime_t start,end;

	printf("Inserisci la data di inizio (aaaa/mm/gg): ");
	scanf("%d/%d/%d",&(start.anno),&(start.mese),&(start.giorno));
	printf("Inserisci la data di fine (aaaa/mm/gg): ");
	scanf("%d/%d/%d",&(end.anno),&(end.mese),&(end.giorno));
	quotation_collection_search_interval_print(t->quotations, start,end);
}
void title_search_quotation_interval_total(title_t t) {
	datetime_t start = {0,0,0},end = {999999,9999,9999};

	
	quotation_collection_search_interval_print(t->quotations, start,end);
}


void title_balance_tree(title_t title, int soglia) {
	quotation_collection_balance(title->quotations,soglia);
}