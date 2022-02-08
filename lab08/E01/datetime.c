#include "datetime.h"

datetime_t datetime_read(FILE *fp) {
	datetime_t date;
	if(fp == NULL) {
		fprintf(stderr, "file non valido!!\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp,"%d/%d/%d %d:%d",
			&(date.anno),
			&(date.mese),
			&(date.giorno),
			&(date.ora),
			&(date.minuto));
	return date;
}

void datetime_print(datetime_t date) {
	printf("%04d/%02d/%02d ",//%02d:%02d ",
			(date.anno),
			(date.mese),
			(date.giorno)//,
			//(date.ora),
			//(date.minuto)
			);
}

int datetime_cmp(datetime_t a, datetime_t b) {
	if(a.anno > b.anno) return 1;
	if(a.anno < b.anno) return -1;

	if(a.mese > b.mese) return 1;
	if(a.mese < b.mese) return -1;

	if(a.giorno > b.giorno) return 1;
	if(a.giorno < b.giorno) return -1;

	/*if(a.ora > b.ora) return 1;
	if(a.ora < b.ora) return -1;

	if(a.minuto > b.minuto) return 1;
	if(a.minuto < b.minuto) return -1;
*/
	return 0;
}