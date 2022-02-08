#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "st.h"
#include "debug.h"

typedef struct edge_s {
	int index_a, index_b, weight;
} edge_t;

typedef struct node *link;
typedef struct list_s *list;

struct node {
	int index,weight;
	link next;
};

struct list_s{
	link head;
};

struct graph_s {
	ST st;
	edge_t *edges;
	int n_edges,n_vert;
	list *ladj;
};

static void GRAPHgenerateladj(GRAPH gr);
static int EDGEscan(GRAPH gr, FILE *fp, edge_t *edge);
static int dfsR(GRAPH G, edge_t e, int *time, int *pre, int *post);
static void TSdfsR(GRAPH gr, int v, int *ts, int *pre, int *time);

GRAPH GRAPHload(FILE* fp) {
	int i;
	char name[31];
	edge_t edge;

	GRAPH gr = malloc(sizeof(struct graph_s));

	fscanf(fp, "%d", &(gr->n_vert));

	DEBUG("n vertici = %d\n",gr->n_vert);
	
	gr->st = STinit(gr->n_vert);
	gr->ladj = malloc(gr->n_vert * sizeof(struct list_s));

	gr->edges = malloc(2 * gr->n_vert * sizeof(edge_t));
	gr->n_edges = 0;

	// lettura vertici e inserimento in ST
	for(i = 0; i < gr->n_vert; i++) {
		fscanf(fp,"%s",name);
		STinsert(gr->st, name);

		//inizializzazione lista vuota delle adiacenze
		gr->ladj[i] = malloc(sizeof(struct list_s));
		gr->ladj[i]->head = NULL;
		//gr->ladj[i]->tail = NULL;
	}

	// lettura edges
	while(EDGEscan(gr, fp, &edge)) {
		gr->edges[gr->n_edges++] = edge;
	}

	GRAPHgenerateladj(gr);

	return gr;
}

static int EDGEscan(GRAPH gr, FILE *fp, edge_t *edge) {
	// funzione richiamata tante volte
	// innecessario riallocare ad ogni chiamata
	// le due stringhe rimangono però allocate per tutta
	// l'esecuzione del programma
	static char nomea[31];
	static char nomeb[31];

	int ret = fscanf(fp,"%s %s %d",nomea,nomeb,&(edge->weight));

	edge->index_a = STgetIndex(gr->st,nomea);
	edge->index_b = STgetIndex(gr->st,nomeb);

	return ret == 3;
}

static void GRAPHgenerateladj(GRAPH gr) {
	int i;
	link node;
	for(i = 0; i < gr->n_edges; i++) {
		// inserimento in lista adiacenze
		node = malloc(sizeof(struct node));
		node->next = NULL;
		node->index = gr->edges[i].index_b;
		node->weight = gr->edges[i].weight;

		node->next = gr->ladj[gr->edges[i].index_a]->head;
		gr->ladj[gr->edges[i].index_a]->head = node;
	}
}

void GRAPHfree(GRAPH gr) {
	int i;
	link node,t;	

	//tabella di simboli
	STfree(gr->st);

	//liste di adiacenze
	for(i = 0; i < gr->n_vert; i++) {
		for(node = gr->ladj[i]->head; node != NULL; node = t) {
			t = node->next;
			free(node);
		}

		free(gr->ladj[i]);
	}

	free(gr->ladj);

	// archi
	free(gr->edges);

	// struct wrapper
	free(gr);
}

int GRAPHcheckDAG(GRAPH G, int *mark, int *pre, int *post) {
	int v, time=0, ret = 1;
	edge_t edge;
	
	for (v=0;v<G->n_vert;v++) {
		pre[v]=-1; post[v]=-1;
	}

	// rimuovo i vertici
	GRAPHremoveEdges(G,mark);

#if DEBUGGING
	DEBUG("printing\n");
	GRAPHprintladj(G);
	DEBUG("printed\n\n\n");
#endif

	for (v=0; v < G->n_vert && ret; v++) {
		if (pre[v]==-1) {
			edge.index_a = v;
			edge.index_b = v;
			if(!dfsR(G, edge, &time, pre, post)) {
				ret = 0;
			}
		}
	}

	GRAPHaddEdges(G,mark);

#if DEBUGGING
	printf("printing original ladj\n");
	GRAPHprintladj(G);
	printf("printed\n\n\n");
#endif

	return ret; 
}

static int dfsR(GRAPH G, edge_t e, int *time, int *pre, int *post){
	link t;
	int v, w = e.index_b;
	edge_t x;
	pre[w] = (*time)++;
	for (t = G->ladj[w]->head; t != NULL; t = t->next) {
		DEBUG("checking edge %d-%d\n",w,t->index);
		if (pre[t->index] == -1) {
			x.index_a = w;
			x.index_b = t->index;
			if(!dfsR(G, x, time, pre, post))
				return 0;
		}
		else {
			v = t->index;
			if (post[v] == -1) {
				DEBUG("found back edge %d,%d\n",v,w);
				return 0;
			}
		}
	}
	post[w] = (*time)++;
	return 1;
}

int GRAPHsumWeight(GRAPH gr, int *mark) {
	int sum = 0,i;
	for(i = 0; i < gr->n_edges; i++) {
		if(mark[i])
			sum += gr->edges[i].weight;
	}
	return sum;
}

void GRAPHaddEdges(GRAPH gr, int *archidariaggiungere) {
	int i;
	link node;
	for(i = 0; i < gr->n_edges; i++) {
		if(archidariaggiungere[i]) {
			node = malloc(sizeof(struct node));
			node->weight = gr->edges[i].weight;
			node->index = gr->edges[i].index_b;
			node->next = gr->ladj[gr->edges[i].index_a]->head;
			gr->ladj[gr->edges[i].index_a]->head = node;
		}
	}
}

void GRAPHremoveEdges(GRAPH gr, int *archidarimuovere) {
	int i;
	link node,prec;
	for(i = 0; i < gr->n_edges; i++) {
		if(archidarimuovere[i]) {
			for(node = gr->ladj[gr->edges[i].index_a]->head, prec = NULL; node != NULL; prec = node, node = node->next) {
				if(gr->edges[i].index_b == node->index) {
					if(node == gr->ladj[gr->edges[i].index_a]->head)
						gr->ladj[gr->edges[i].index_a]->head = node->next;
					else
						prec->next = node->next;

					free(node);
					break;
				}
			}
		}
	}
}

void GRAPHdagMaximumPath(GRAPH gr) {
	int v, time = 0, *pre, *ts, *maximumPaths;
	link t;

	pre = malloc(gr->n_vert * sizeof(int));
	ts = malloc(gr->n_vert * sizeof(int));
	maximumPaths = malloc(gr->n_vert * sizeof(int));

	for (v=0; v < gr->n_vert; v++) { pre[v] = -1; ts[v] = -1; maximumPaths[v] = 0;}

	// ordinamento dei vertici in ordine topologico
	for (v=0; v < gr->n_vert; v++)
		if (pre[v]== -1) TSdfsR(gr, v, ts, pre, &time);
	
	// calcolo dei cammini massimo attraverso relaxation inversa
	for (v=gr->n_vert - 1; v >= 0; v--) {
		for (t = gr->ladj[ts[v]]->head; t != NULL; t = t->next) {
			if(maximumPaths[ts[v]] + t->weight > maximumPaths[t->index])
				maximumPaths[t->index] = maximumPaths[ts[v]] + t->weight;
		}
	}

	printf("Nodi in ordine topologico:\n\t\t\t");
	for (v=gr->n_vert - 1; v >= 0; v--)
		printf("%s ", STgetName(gr->st,ts[v]));

	printf("\n\nCammini massimi a partire dalle sorgenti:\n");
	for (v=gr->n_vert - 1; v >= 0; v--) {
		printf("Nodo:\t\t\t%s",STgetName(gr->st,ts[v]));
		if(maximumPaths[ts[v]] == 0)
			printf("\t\t\tsorgente\n");
		else
			printf("\t\t\t%3d\n",maximumPaths[ts[v]]);
	}
	
	free(pre);
	free(ts);
	free(maximumPaths);
}

static void TSdfsR(GRAPH gr, int v, int *ts, int *pre, int *time) {
	link t; pre[v] = 0;
	for (t = gr->ladj[v]->head; t != NULL; t = t->next)
		if (pre[t->index] == -1)
			TSdfsR(gr, t->index, ts, pre, time);
	ts[(*time)++] = v;
}

void GRAPHprintladj(GRAPH gr) {
	int i;
	link node;
	for(i = 0; i < gr->n_vert; i++) {
		printf("starting from node %s\n",STgetName(gr->st,i));
		for(node = gr->ladj[i]->head; node != NULL; node = node->next)
			printf("	\t%s\n",STgetName(gr->st,node->index));
	}
}

void GRAPHprintEdge(GRAPH gr, int index) {
	printf("%s-%s %2d\t",STgetName(gr->st,gr->edges[index].index_a),STgetName(gr->st,gr->edges[index].index_b),gr->edges[index].weight);
}

int GRAPHgetn_edges(GRAPH gr) {
	return gr->n_edges;
}

int GRAPHgetn_vert(GRAPH gr) {
	return gr->n_vert;
}