#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph_s* GRAPH;


GRAPH GRAPHload(FILE* fp);
void GRAPHfree(GRAPH gr);

int GRAPHcheckDAG(GRAPH gr, int *mark, int *pre, int *post);
int GRAPHsumWeight(GRAPH gr, int *mark);

void GRAPHaddEdges(GRAPH gr, int *archidariaggiungere);
void GRAPHremoveEdges(GRAPH gr, int *archidarimuovere);
void GRAPHdagMaximumPath(GRAPH gr);

void GRAPHprintladj(GRAPH G);
void GRAPHprintEdge(GRAPH gr, int index);
int GRAPHgetn_edges(GRAPH gr);
int GRAPHgetn_vert(GRAPH gr);

#endif