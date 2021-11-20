#include "pg.h"

/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pers) {
	int ret;
	
	ret = fscanf(fp,"%s %s %s",
		pers->cod,
		pers->nome,
		pers->classe);

	stat_read(fp,&(pers->b_stat));
	
	// se non ho letto non alloco - memory leakage subdolo!
	if(ret == 3) {
		pers->equip = equipArray_init();
	}

	return (ret == 3);
}

/* non essendo struct dinamica, pulisce chiamando il distruttore di equipArray */
void pg_clean(pg_t *pgp) {
	equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
	fprintf(fp,"%-7s %-20s %-20s\t",
		pgp->cod,
		pgp->nome,
		pgp->classe); 

	stat_print(fp,&(pgp->b_stat),/*TODO soglia?*/0);

	equipArray_print(fp,pgp->equip,invArray);
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
	equipArray_update(pgp->equip,invArray);
}
