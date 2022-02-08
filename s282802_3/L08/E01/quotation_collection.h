#ifndef QUOTATION_COLLECTION_H
#define QUOTATION_COLLECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "datetime.h"


typedef struct quotation_collection_s *quotation_collection_t;
/*typedef struct {
	datetime_t date;
	float value;
	int total_number;
} quotation_t;*/

quotation_collection_t quotation_collection_init();
void quotation_collection_free(quotation_collection_t Q);

void quotation_collection_read_quotations(quotation_collection_t Q, FILE* fp);
void quotation_collection_print(quotation_collection_t Q);

void quotation_collection_search_print(quotation_collection_t Q, datetime_t date);
void quotation_collection_search_interval_print(quotation_collection_t Q, datetime_t start,datetime_t end);
void quotation_collection_balance(quotation_collection_t Q, int soglia);

#endif