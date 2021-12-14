#include <stdlib.h>
#include "graph.h"
#include "symbol_table.h"


typedef struct node *link;
struct node {
	int v; link next;
};

int **matrix_init(int v);
static edge_t edge_create(int v, int w, int flusso);

struct graph_s {
	st_t tab;
	int V,E;
	link *ladj;
	int **madj;
};

graph_t graph_init(int V) {
	graph_t G = malloc(sizeof *G);
	G->V = V;
	G->E = 0;
	G->madj = matrix_init(V);
	G->tab = st_init(V);
	return G;
}

void graph_free(graph_t G) {
	int i;
	
	for (i=0; i<G->V; i++)
		free(G->madj[i]);
	
	free(G->madj);
	
	st_free(G->tab);

	free(G);
}


int **matrix_init(int v) {
	int ** mat;
	mat = malloc(v * sizeof(int *));
	for (int i = 0; i < v; i++) {
		mat[i] = calloc(v,sizeof(int));
	}
	return mat;
}
void graph_insert_edge(graph_t G, int id1, int id2, int flusso) {
	if(G->madj[id1][id2] == 0) G->E++; 
	G->madj[id1][id2] = flusso;
	G->madj[id2][id1] = flusso;
}

int GRAPHgetIndex(Graph G, char *name, *subnet) {
	int id;
	id = st_get_index(G->tab, label);

	if (id == -1) {
		id = st_add(G->tab, label, subnet);
	}
	return id;
}