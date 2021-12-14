#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAXN_INITIAL 5

typedef struct st_s *st_t;

st_t st_init();
void st_free(st_t st);
int st_add(st_t st, char *name, char *subnet);
char* st_get_name(st_t st, int index);
char* st_get_subnet(st_t st, int index);

int st_get_index(st_t st, char *name);

#endif