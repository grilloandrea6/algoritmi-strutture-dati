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

void st_get_sorted_indexes(st_t st, int *v, int n){
	int i,j,temp;

	for(i = 0; i < n - 1; i++) {
		for(j = 0; j < n - 1 - i; j++) {
			//printf("comparing %s and %s, ret = %d\n",st->vect_name[v[j]],st->vect_name[v[j+1]], strcmp(st->vect_name[j],st->vect_name[j+1]));
			if(strcmp(st->vect_name[v[j]],st->vect_name[v[j+1]]) >= 0) {
				//printf("swapping j=%d and %d\n",j,j+1);
				temp = v[j];
				v[j] = v[j+1];
				v[j+1] = temp;
			}
		}
	}
}

char* st_get_name(st_t st, int index) {
	return st->vect_name[index];
}

char* st_get_subnet(st_t st, int index) {
	return st->vect_subnet[index];
}

void st_print_table(st_t st) {
	printf("Symbol Table:\n");
	for(int i = 0; i < st->n; i++) {
		printf("Index: %d\tName: %s\t\tSubnet: %s\n",i,st->vect_name[i],st->vect_subnet[i]);
	}
}

int st_get_index(st_t st, char *name) {
	for(int i = 0; i < st->n; i++) {
		if(strcmp(name, st->vect_name[i]) == 0)
			return i;
	}

	//fprintf(stderr,"Nome non trovato.\n");
	return -1;
}

int st_get_n(st_t st) {
	return st->n;
}