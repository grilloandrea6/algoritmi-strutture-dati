#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph_s *graph_t;
typedef struct {int v,w,flusso;} edge_t;


graph_t graph_init();
graph_t graph_readFile(char *filename);
graph_t graph_generateAdjacencyList(graph_t graph);

int graph_matrix_complete_subgraph(graph_t graph, char* a, char *b, char *c);
int graph_list_complete_subgraph(graph_t graph, char* a, char *b, char *c);




#endif