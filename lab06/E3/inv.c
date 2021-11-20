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
	// TODO what does soglia means?
	fprintf(fp,"%5d %5d %5d %5d %5d %5d\n", 
		statp->hp,
		statp->mp,
		statp->atk,
		statp->def,
		statp->mag,
		statp->spr);

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
	stat_print(fp,&(invp->stat),/*TODO soglia?*/0);

}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp) {
	return invp->stat;
}
