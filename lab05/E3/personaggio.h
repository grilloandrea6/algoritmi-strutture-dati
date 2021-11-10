#ifndef PERSONAGGIO_H
#define PERSONAGGIO_H

#include <stdlib.h>
#include "inventario.h"
#include "debug.h"

#define MAXL		255
#define MAX_OGGETTI	100

typedef struct {
	int inUso;
	inv_t *vettEq[MAX_OGGETTI];
} tabEquip_t;

typedef struct {
	char codice[MAXL];
	char nome[MAXL];
	char classe[MAXL];
	tabEquip_t equip;
	stat_t stat;
} pg_t;

typedef struct nodoPg_s *linkPg, nodoPg_t;

struct nodoPg_s {
	pg_t val;
	linkPg next;
};

typedef struct {
	nodoPg_t *head;
	nodoPg_t *tail;
	int nPg;
} tabPg_t;

tabPg_t *allocaTabellaPersonaggi();
void acquisisciPersonaggiFile(char *filename, tabPg_t *tabella);
void stampaPersonaggi(tabPg_t *tabella);
void aggiungiPersonaggio(tabPg_t *tabella);
void eliminaPersonaggio(tabPg_t *tabella);
void stampaEquipaggiamento(tabPg_t *tabella);
void aggiungiOggetto(tabPg_t *tabella, tabInv_t tabella_inventario);
void rimuoviOggetto(tabPg_t *tabella);
void stampaStatistiche(tabPg_t *tabella);
void freeTabella(tabPg_t *tabella);

#endif