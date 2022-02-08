#ifndef DATETIME_H
#define DATETIME_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int anno,mese,giorno,ora,minuto;
} datetime_t;

datetime_t datetime_read(FILE *fp);
int datetime_cmp(datetime_t a, datetime_t b);
void datetime_print(datetime_t date);
#endif