#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "debug.h"

#define MAXL 255


typedef struct {
	int hp;
	int mp;
	int atk;
	int def;
	int mag;
	int spr;
} stat_t;


typedef struct {
	char nome[MAXL];
	char tipo[MAXL];
	stat_t stat;
} inv_t;

typedef struct {
	inv_t *vettInv;
	int nInv, maxInv;
} tabInv_t;




tabInv_t acquisisciInventarioFile(char *filename);
void stampaOggetti(tabInv_t tabella);
int ricercaOggettoNome(char *nome, tabInv_t tabella, inv_t *val);

void printStat(stat_t stat);
stat_t calcolaStatistiche(stat_t stat, inv_t *vett, int n);
				



#endif