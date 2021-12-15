#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "symbol_table.h"

typedef struct node *link;
struct node {
	int v, flusso;
	link next;
};

struct graph_s {
	st_t tab;
	int V,E,is_list_generated;
	link *ladj;
	int **madj;
};

// static prototypes
static graph_t graph_init(int V);
static void graph_insert_edge(graph_t G, int id1, int id2, int flusso);
static int **matrix_init(int v);
static int file_count_lines(FILE *fp);
static int graph_get_index(graph_t graph, char *name, char *subnet);
static int graph_complete_subgraph_matrix(graph_t graph, int a, int b, int c);
static int graph_complete_subgraph_list(graph_t graph, int a, int b, int c);


graph_t graph_load(FILE *fp) {
	int i,flusso, E = file_count_lines(fp);

	char nomeA[LEN],nomeB[LEN],subnetA[LEN],subnetB[LEN];
	graph_t graph = graph_init(2 * E);
	
	rewind(fp);
	
	for(i = 0; i < E; i++) {
		fscanf(fp,"%s %s %s %s %d",nomeA,subnetA,nomeB,subnetB,&flusso);
		graph_insert_edge(
			graph, 
			graph_get_index(graph,nomeA,subnetA),
			graph_get_index(graph,nomeB,subnetB),
			flusso);
	}
#if DEBUGGING
	st_print_table(graph->tab);
#endif
	return graph;
}

void graph_free(graph_t graph) {
	int i;
	link node, nodeA;
	for(i = 0; i < st_get_n(graph->tab); i++) {
		free(graph->madj[i]);
		for(node = graph->ladj[i]; node != NULL; node = nodeA) {
			nodeA = node->next;
			if(node != NULL)
				free(node);
		}
	}
	free(graph->ladj);
	st_free(graph->tab);
	free(graph->madj);
	free(graph);
}

void graph_print_sorted_table(graph_t graph) {
	int i,j,k, *vect_totale = malloc(st_get_n(graph->tab) * sizeof(int)),
			*vect_parziale = malloc(st_get_n(graph->tab) * sizeof(int));

	for(i = 0; i < st_get_n(graph->tab); i++) {
		vect_totale[i] = i;
	}

	st_get_sorted_indexes(graph->tab, vect_totale, st_get_n(graph->tab));
	
	for(i = 0; i < st_get_n(graph->tab); i++) {		
		printf("%2d - %-10s %s\n",vect_totale[i],st_get_name(graph->tab,vect_totale[i]),st_get_subnet(graph->tab,vect_totale[i]));
		k = 0;

		for(j = 0; j < st_get_n(graph->tab); j++) {
			if(graph->madj[vect_totale[i]][j] != 0) vect_parziale[k++] = j;
		}
		
		st_get_sorted_indexes(graph->tab, vect_parziale, k);
		
		for(j = 0; j < k; j++) {
			printf("\t\t\t%2d - %-10s %s %5d\n",vect_parziale[j],st_get_name(graph->tab,vect_parziale[j]),st_get_subnet(graph->tab,vect_parziale[j]),graph->madj[vect_totale[i]][vect_parziale[j]]);
		}
		printf("\n");
	}

	free(vect_totale);
	free(vect_parziale);
}

void graph_generate_adjacency_list(graph_t graph) {
	int i,j;
	link tail;

	for(i = 0; i < st_get_n(graph->tab); i++) {
		tail = graph->ladj[i];
		for(j = 0; j < st_get_n(graph->tab); j++) {
			if(graph->madj[i][j] != 0) {
				link node = malloc(sizeof(*node));
				node->v = j;
				node->flusso = graph->madj[i][j];
				node->next = NULL;
				
				if(tail == NULL) {
					graph->ladj[i] = tail = node;
				} else {
					tail->next = node;
					tail = tail->next;
				}
			}
		}
	}
	graph->is_list_generated = 1;
}

int graph_complete_subgraph(graph_t graph, char* a, char *b, char *c, int matrice_lista) {
	int a_ind,b_ind,c_ind;
	a_ind = st_get_index(graph->tab, a);
	b_ind = st_get_index(graph->tab, b);
	c_ind = st_get_index(graph->tab, c);

	if(matrice_lista == 1)
		return graph_complete_subgraph_list(graph,a_ind,b_ind,c_ind);
	else if(matrice_lista == 2)
		return graph_complete_subgraph_matrix(graph,a_ind,b_ind,c_ind);

	//never reached, matrice_lista value is already checked in main
	return -1;
}

void graph_print_list(graph_t graph) {
	link node;
	for(int i = 0; i < st_get_n(graph->tab); i++) {
		printf("%2d - %-10s %s\n",i,st_get_name(graph->tab,i),st_get_subnet(graph->tab,i));
		for(node = graph->ladj[i]; node != NULL; node = node->next) {
			printf("\t\t\t%2d - %-10s %s %5d\n",node->v,st_get_name(graph->tab,node->v),st_get_subnet(graph->tab,node->v),node->flusso);

		}
	}
}

void graph_print_matrix(graph_t graph) {
	for(int i = 0; i < st_get_n(graph->tab); i++) {
		for(int j = 0; j < st_get_n(graph->tab); j++) {
			printf("%4d ",graph->madj[i][j]);
		}
		printf("\n");
	}
}

int graph_is_lista_generated(graph_t graph) {
	return graph->is_list_generated;
}

//static functions
static graph_t graph_init(int V) {
	graph_t G = malloc(sizeof(struct graph_s));
	G->V = V;
	G->E = 0;
	G->madj = matrix_init(V);
	G->tab = st_init(V);
	G->ladj = malloc(2 * V * sizeof(struct node));
	G->is_list_generated = 0;
	return G;
}

static void graph_insert_edge(graph_t G, int id1, int id2, int flusso) {
	if(G->madj[id1][id2] == 0) G->E++; 
	G->madj[id1][id2] = flusso;
	G->madj[id2][id1] = flusso;
}

static int **matrix_init(int v) {
	int ** mat;
	mat = malloc(v * sizeof(int *));
	for (int i = 0; i < v; i++) {
		mat[i] = calloc(v,sizeof(int));
	}
	return mat;
}

static int file_count_lines(FILE *fp) {
	int count = 1;
	char temp;
	while(fscanf(fp,"%c",&temp) == 1){
	    if(temp == '\n') count++;
	}
	return count;
}

static int graph_get_index(graph_t graph, char *name, char *subnet) {
	int id;
	id = st_get_index(graph->tab, name);

	if (id == -1) {
		id = st_add(graph->tab, name, subnet);
	}
	return id;
}

static int graph_complete_subgraph_matrix(graph_t graph, int a, int b, int c) {
	if(graph->madj[a][b] == 0 ||
		graph->madj[a][c] == 0 ||
		graph->madj[b][c] == 0)
		return 0;
	return 1;
}

static int graph_complete_subgraph_list(graph_t graph, int a, int b, int c) {
	int foundB = 0,foundC = 0;
	link node;

	// a connected to b and c
	for(node = graph->ladj[a]; node != NULL && !(foundB && foundC); node = node->next) {
		if(node->v == b)
			foundB = 1;
		if(node->v == c)
			foundC = 1;
	}

	if(!foundB || !foundC)
		return 0;
	
	// b connected to c
	for(node = graph->ladj[b]; node != NULL; node = node->next) {
		if(node->v == c)
			return 1;
	}

	return 0;
}