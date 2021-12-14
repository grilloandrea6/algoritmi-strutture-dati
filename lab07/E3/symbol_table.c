#include <stdio.h>
#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

struct st_s {
	int maxN,n;
	char **vect_name;
	char **vect_subnet;
};

st_t st_init() {
	st_t st = malloc(sizeof(struct st_s));
	st->maxN = MAXN_INITIAL;
	st->n = 0;
	st->vect_name = malloc(MAXN_INITIAL * sizeof(char *));
	st->vect_subnet = malloc(MAXN_INITIAL * sizeof(char *));

	return st;
}

void st_free(st_t st) {
	for(int i = 0; i < st->n; i++) {
		free(st->vect_name[i]);
		free(st->vect_subnet[i]);
	}
	free(st->vect_name);
	free(st->vect_subnet);
	free(st);
}

int st_add(st_t st, char *name, char *subnet) {
	if(st->n == st->maxN) {
		st->maxN *= 2;
		st->vect_name = realloc(st->vect_name, st->maxN * sizeof(char *));
		st->vect_subnet = realloc(st->vect_subnet, st->maxN * sizeof(char *));
	}
	st->vect_name[st->n] = strdup(name);
	st->vect_subnet[st->n] = strdup(subnet);
	st->n++;
	return st->n - 1;
}

char* st_get_name(st_t st, int index) {
	return st->vect_name[index];
}

char* st_get_subnet(st_t st, int index) {
	return st->vect_subnet[index];
}

int st_get_index(st_t st, char *name) {
	for(int i = 0; i < st->n; i++) {
		if(strcmp(name, st->vect_name[i]) == 0)
			return i;
	}

	fprintf(stderr,"Nome non trovato.\n");
	return -1;
}