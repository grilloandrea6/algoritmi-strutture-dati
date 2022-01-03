#ifndef TITLE_H
#define TITLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quotation_collection.h"


#define MAXL_NAME 21


typedef struct title_s *title_t;


title_t title_init();
void title_free(title_t title);
void title_read(title_t title, char *nome, FILE *fp);

int title_cmp(title_t a, title_t b);
char *title_get_name(title_t t);
void title_print(title_t t);
void title_read_quotations(title_t title, FILE *fp);
void title_search_quotation(title_t title);
void title_search_quotation_interval(title_t title);
void title_search_quotation_interval_total(title_t title);
void title_balance_tree(title_t title, int soglia);
#endif