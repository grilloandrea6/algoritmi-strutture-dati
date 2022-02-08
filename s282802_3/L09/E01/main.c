#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "debug.h"

#define FILENAME "grafo4.txt"

void genera_insiemi_archi(int pos, int *mark, GRAPH gr,int n_edges, int k, int *found, int *max_wt, int *max_sol, int start, int *pre, int *post);

int main() {
	int *mark, *archidarimuovere, found = 0, max_wt = 0,i, *pre,*post,n_edges,n_vert;
	FILE *fp;
	GRAPH gr;
 	
	fp = fopen(FILENAME,"r");
	if(fp == NULL) {
		fprintf(stderr,"Errore apertura file.\n");
		exit(EXIT_FAILURE);
	}

	gr = GRAPHload(fp);
	fclose(fp);


#if DEBUGGING
	DEBUG("ORIGINAL\n\n");
	GRAPHprintladj(gr);
	DEBUG("END ORIGINAL\n\n");
#endif 

	n_edges = GRAPHgetn_edges(gr);
	n_vert = GRAPHgetn_vert(gr);

	mark 				= calloc(n_edges, sizeof(int));
	archidarimuovere	= malloc(n_edges * sizeof(int));

	// allocati nel main per non essere riallocati per ogni chiamata alla funzione checkdag
	pre					= malloc(n_vert * sizeof(int));
	post				= malloc(n_vert * sizeof(int));

	for(i = 0; i < n_edges && !found; i++) {
		DEBUG("calling recursion with k = %d\n",i);
		genera_insiemi_archi(0,mark,gr,n_edges,i,&found,&max_wt,archidarimuovere,0,pre,post);
	}

	free(pre);
	free(post);
	if(i != 1) {
		printf("\nSoluzione a somma massima:\n\t\t\t");
		for(i = 0; i < n_edges; i++) {
			if(archidarimuovere[i])
				GRAPHprintEdge(gr,i);
		}
		printf("\tsomma pesi = %3d\n\n\n", max_wt);

		GRAPHremoveEdges(gr,archidarimuovere); 
	}
#if DEBUGGING
	GRAPHprintladj(gr);
#endif

	GRAPHdagMaximumPath(gr);

	GRAPHfree(gr);
	free(mark);
	free(archidarimuovere);
}


void genera_insiemi_archi(int pos, int *mark, GRAPH gr,int n_edges, int k, int *found, int *max_wt, int *max_sol, int start, int *pre, int *post) {
	int i, sum;
	if(pos == k) {
		if(GRAPHcheckDAG(gr,mark,pre,post)) {
			if(!(*found)) {
				printf("Il numero minimo di archi da rimuovere è %d.\n",k);
				*found = 1;	
			}
			if(k != 0) {
				printf("Possibile soluzione:\t");

				for(i = 0; i < n_edges; i++) {
					if(mark[i])
						GRAPHprintEdge(gr,i);
				}

				sum = GRAPHsumWeight(gr,mark);
				printf("\tsomma pesi = %3d\n",sum);
			
				if(sum > *max_wt) {
					DEBUG("updating max_sol\n");
					*max_wt = sum;
					for(int i = 0; i < n_edges; i++)
						max_sol[i] = mark[i];
				}
			}
		}
		return;
	}

	for(i = start; i < n_edges; i++) {
		mark[i] = 1;
		genera_insiemi_archi(pos + 1, mark, gr, n_edges, k, found, max_wt, max_sol, i + 1, pre, post);
		mark[i] = 0;		
	}
}
