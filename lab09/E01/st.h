#ifndef ST_H
#define ST_H

typedef struct st_s* ST;

ST STinit(int maxN);
int STgetIndex(ST st, char *name);
char *STgetName(ST st, int index);
int STinsert(ST st, char *name);
void STfree(ST st);

#endif