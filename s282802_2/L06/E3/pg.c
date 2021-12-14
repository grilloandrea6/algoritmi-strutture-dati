#include "pg.h"

/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pers) {
	int ret;
	
	ret = fscanf(fp,"%s %s %s",
		pers->cod,
		pers->nome,
		pers->classe);

	stat_read(fp,&(pers->b_stat));
	
	pers->eq_stat = pers->b_stat;

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

	stat_print(fp,&(pgp->eq_stat),1);

	equipArray_print(fp,pgp->equip,invArray);
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
	equipArray_update(pgp->equip,invArray);
	pg_updateStat(pgp,invArray);
}


/* somma alle statistiche del personaggio le statistiche dell'ultimo oggetto nell'equipaggiamento */
void pg_updateStat(pg_t *pgp, invArray_t invArray) {
	stat_t stat = equipArray_getLastStat(pgp->equip,invArray);

	pgp->eq_stat.hp += stat.hp;
	pgp->eq_stat.mp += stat.mp;
	pgp->eq_stat.atk += stat.atk;
	pgp->eq_stat.def += stat.def;
	pgp->eq_stat.mag += stat.mag;
	pgp->eq_stat.spr += stat.spr;
}