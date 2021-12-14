#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "debug.h"

#define MAXL 255
#define MAX_INV 100


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
	inv_t vettInv[MAX_INV];
	int nInv, maxInv;
} tabInv_t;


tabInv_t acquisisciInventarioFile(char *filename);
void stampaOggetti(tabInv_t tabella);
int ricercaOggettoNome(char *nome, tabInv_t tabella, inv_t **val);
void printOggetto(inv_t *ogg);

void printStat(stat_t stat);
stat_t calcolaStatistiche(stat_t sum, inv_t *vett[], int n);
				
#endif