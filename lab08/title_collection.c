#include <stdlib.h>
#include <string.h>
#include "title_collection.h"
#include "title.h"

typedef struct node *link;

struct node {
	title_t title;
	link next;
};

struct title_collection_s {
	link head;
	int n;
};

title_collection_t title_collection_init() {
	title_collection_t T = malloc(sizeof(struct title_collection_s));
	T->head = NULL;
	T->n = 0;
	return T;
}

void title_collection_free(title_collection_t T) {
	link X;
	for(; T->head != NULL; T->head = X) {
		X = T->head->next;
		title_free(T->head->title);
		free(T->head);
	}
	free(T);
}

void title_collection_read_title(title_collection_t collezione_titoli,FILE *fp) {
	char nome[MAXL_NAME];
	link node;
	title_t title;

	if(fp == NULL) {
		fprintf(stderr, "file non valido!!\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp,"%s",nome);
	title = title_collection_search(collezione_titoli,nome);
	if(title != NULL) {
		title_read(title,nome,fp);
		return;
	}	

	node = malloc(sizeof(struct node));
	node->title = title_init();
	node->next = NULL;
	title_read(node->title, nome, fp);

	if(collezione_titoli->head == NULL) {
		//lista vuota
		collezione_titoli->head = node;
	} else if(title_cmp(node->title,collezione_titoli->head->title)){
		//inserimento in prima posizione
		node->next = collezione_titoli->head;
		collezione_titoli->head = node;
	} else {
		link X,P = NULL;

		for(X = P = collezione_titoli->head; X != NULL && title_cmp(node->title,X->title) > 0; P = X, X = X->next);
		P->next = node;
		node->next = X;
	}
	collezione_titoli->n++;
}

title_t title_collection_search(title_collection_t T, char* name) {
	link l;

	for(l = T->head; l != NULL; l = l->next)
		if(strcmp(title_get_name(l->title),name) == 0)
			return l->title;

	return NULL;
}


void title_collection_print_all(title_collection_t T) {
	link h;
	for(h = T->head; h != NULL; h = h->next)
		title_print(h->title);
}

void title_collection_read_file(title_collection_t T, FILE *fp) {
	int i,n_titoli;

	fscanf(fp,"%d",&n_titoli);

	for(i = 0; i < n_titoli; i++) {
		title_collection_read_title(T,fp);		
	}
}
