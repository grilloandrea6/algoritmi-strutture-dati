#ifndef GRAPH_H
#define GRAPH_H

#define LEN 31

typedef struct graph_s *graph_t;

graph_t graph_load(FILE *fp);
void graph_free(graph_t graph);

void graph_print_sorted_table(graph_t graph);
void graph_generate_adjacency_list(graph_t graph);
int graph_complete_subgraph(graph_t graph, char* a, char *b, char *c, int matrice_lista);
void graph_print_list(graph_t graph);
void graph_print_matrix(graph_t graph);
int graph_is_lista_generated(graph_t graph);

#endif