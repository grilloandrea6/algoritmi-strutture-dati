#include "inv.h"

/* funzioni di input/output delle statistiche */
void stat_read(FILE *fp, stat_t *statp) {
	fscanf(fp,"%d %d %d %d %d %d",
 		&(statp->hp),
		&(statp->mp),
		&(statp->atk),
		&(statp->def),
		&(statp->mag),
		&(statp->spr));
}

void stat_print(FILE *fp, stat_t *statp, int soglia) {
	// soglia == 0 -> stampo statp
	// soglia == 0 -> stampo statp o 1 se i valori sono minori di 0
	stat_t toPrint;
	
	toPrint.hp = !soglia ? statp->hp : (statp->hp > 1 ? statp->hp : 1);
	toPrint.mp = !soglia ? statp->mp : (statp->mp > 1 ? statp->mp : 1);
	toPrint.atk = !soglia ? statp->atk : (statp->atk > 1 ? statp->atk : 1);
	toPrint.def = !soglia ? statp->def : (statp->def > 1 ? statp->def : 1);
	toPrint.mag = !soglia ? statp->mag : (statp->mag > 1 ? statp->mag : 1);
	toPrint.spr = !soglia ? statp->spr : (statp->spr > 1 ? statp->spr : 1);

	fprintf(fp,"%5d %5d %5d %5d %5d %5d\n",
		toPrint.hp,
		toPrint.mp,
		toPrint.atk,
		toPrint.def,
		toPrint.mag,
		toPrint.spr);

}

/* funzioni di input/output di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *invp) {
	fscanf(fp,"%s%s",
		invp->nome,
		invp->tipo);
	stat_read(fp,&(invp->stat));
}

void inv_print(FILE *fp, inv_t *invp) {
	fprintf(fp,"%-20s %-20s\t",
		invp->nome,
		invp->tipo);
	stat_print(fp,&(invp->stat),0);

}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp) {
	return invp->stat;
}
