#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"

#define N_SCELTE 4

#define DEBUGGING 1

enum { falso, vero };
typedef int bool;

void stampaMenu(char *scelte[], int *selezione){
	int i=0;
	printf("\nMENU'\n");
	for(i=0;i<N_SCELTE;i++)
		printf("%2d > %s\n",i,scelte[i]);
	scanf(" %d",selezione);
}

int main(int argc, char **argv) {
	char *scelte[] = {
		"Uscita",
		"Stampa elenco ordinato",
		"Verifica sottografo completo",
		"Genera lista adiacenze"
	};
	int matrice_lista;

	char nodoA[LEN],nodoB[LEN],nodoC[LEN];
	int selezione;
	FILE *fin;
	bool fineProgramma;

	fin = fopen("grafo.txt","r");
	if (fin == NULL) {
		fprintf(stderr, "Errore apertura file.\n");
		exit(EXIT_FAILURE);
	}

	graph_t graph = graph_load(fin);
#if DEBUGGING
	graph_print_matrix(graph);
#endif
	fclose(fin);

	do {
		stampaMenu(scelte, &selezione);
		switch(selezione){

			case 0: {
				fineProgramma = vero;
			} break;

			case 1: { // stampa elenco ordinato
				graph_print_sorted_table(graph);
			} break;

			case 2: { // verifica sottografo completo
				printf("Vuoi eseguire il comando utilizzando la lista delle adiacenze [1] o la matrice [2]? ");
				scanf("%d",&matrice_lista);

				if(matrice_lista == 1 && !graph_is_lista_generated(graph)) {
					fprintf(stderr,"Non hai ancora generato la lista delle adiacenze!\n");
					break;
				}

				if(matrice_lista != 1 && matrice_lista != 2) {
					fprintf(stderr, "Scelta non valida.\n");
					break;
				}
				
				printf("Inserire nome primo nodo: ");
				scanf("%s", nodoA);
				printf("Inserire nome secondo nodo: ");
				scanf("%s", nodoB);
				printf("Inserire nome terzo nodo: ");
				scanf("%s", nodoC);

				if(graph_complete_subgraph(graph, nodoA, nodoB, nodoC, matrice_lista))
					printf("Il sottografo inserito è completo.\n");
				else
					printf("Il sottografo inserito NON è completo.\n");
			} break;

			case 3: {// genera lista adiacenze
				if(!graph_is_lista_generated(graph)) {
					graph_generate_adjacency_list(graph);
					graph_print_list(graph);
				} else fprintf(stderr,"Hai già generato la lista delle adiacenze!\n");
				break;
			} break;
			default:{
				printf("Scelta non valida\n");
			} break;
		}
	} while(!fineProgramma);

	graph_free(graph);

	return 0;
}
