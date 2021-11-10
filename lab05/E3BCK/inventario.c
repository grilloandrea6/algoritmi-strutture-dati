#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inventario.h"

#define MAX_INV 100

// funzioni interne al modulo
void printOggetto(inv_t ogg);
inv_t leggiOggetto(FILE *fp);
stat_t sommaStat(stat_t a, stat_t b);
stat_t azzeraNegativi(stat_t stat);


void stampaOggetti(tabInv_t tabella) {
	for(int i = 0; i < tabella.nInv; i++)
		printOggetto(tabella.vettInv[i]);
}

tabInv_t acquisisciInventarioFile(char *filename) {
	FILE *fp;
	tabInv_t tabella;
	int i = 0;

	tabella.vettInv = (inv_t*) malloc(MAX_INV * sizeof(inv_t));
	tabella.maxInv = MAX_INV;

	if(strcmp(filename, "stdout") == 0)
		fp = stdout;
	else
		fp = fopen(filename,"r");


	fscanf(fp,"%d",&(tabella.nInv));

	DEBUG("Numero oggetti in inventario: %d\n",tabella.nInv);

	for(i = 0; i < tabella.nInv; i++) {
		tabella.vettInv[i] = leggiOggetto(fp);
	}

	if(strcmp(filename, "stdout") != 0)
		fclose(fp);

	return tabella;
}


int ricercaOggettoNome(char *nome, tabInv_t tabella, inv_t *val) {
	for(int i = 0; i < tabella.nInv; i++) {
		if(strcmp(tabella.vettInv[i].nome,nome) == 0) {
			*val = tabella.vettInv[i];
			return 1;
		}

	}
	return 0;
}


void printStat(stat_t stat) {
	printf("hp = %d, mp = %d, atk = %d, def = %d, mag = %d, spr = %d\n", 
		stat.hp,
		stat.mp,
		stat.atk,
		stat.def,
		stat.mag,
		stat.spr);
}

stat_t calcolaStatistiche(stat_t sum, inv_t *vett, int n) {
	int i;
	
	for(i = 0; i < n; i++) {
		sum = sommaStat(sum,vett[i].stat);
	}
	return azzeraNegativi(sum);
}


void printOggetto(inv_t ogg) {
	printf("%s %s %d %d %d %d %d %d\n",
		ogg.nome,
		ogg.tipo,
		ogg.stat.hp,
		ogg.stat.mp,
		ogg.stat.atk,
		ogg.stat.def,
		ogg.stat.mag,
		ogg.stat.spr);
}




inv_t leggiOggetto(FILE *fp) {
	inv_t ogg;
	fscanf(fp,"%s %s %d %d %d %d %d %d",
		ogg.nome,
		ogg.tipo,
		&(ogg.stat.hp),
		&(ogg.stat.mp),
		&(ogg.stat.atk),
		&(ogg.stat.def),
		&(ogg.stat.mag),
		&(ogg.stat.spr));
	
	return ogg;
}

stat_t sommaStat(stat_t a, stat_t b) {
	stat_t ret = {a.hp + b.hp, a.mp + b.mp, a.atk + b.atk, a.def + b.def, a.mag + b.mag, a.spr + b.spr};
	return ret;	
}

stat_t azzeraNegativi(stat_t stat) {
	if(stat.hp < 0) stat.hp = 0;
	if(stat.mp < 0) stat.mp = 0;
	if(stat.atk < 0) stat.atk = 0;
	if(stat.def < 0) stat.def = 0;
	if(stat.mag < 0) stat.mag = 0;
	if(stat.spr < 0) stat.spr = 0;

	return stat;
}