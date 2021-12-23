#ifndef TITLE_COLLECTION_H
#define TITLE_COLLECTION_H

#include "title.h"

typedef struct title_collection_s *title_collection_t;

title_collection_t title_collection_init();
void title_collection_free(title_collection_t T);

void title_collection_read_title(title_collection_t collezione_titoli,FILE *fp);
title_t title_collection_search(title_collection_t T, char* name);
void title_collection_print_all(title_collection_t T);
void title_collection_read_file(title_collection_t T, FILE *fp);

#endif