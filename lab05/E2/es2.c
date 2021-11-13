#include <stdio.h>
#include <stdlib.h>

#define FILENAME_BOARD "board.txt"
#define FILENAME_TILES "tiles.txt"

typedef struct {
	char col1,col2;
	int val1,val2,used;
} tessera_t;

typedef struct {
	int tess,rot;
} cella_t;

int recursion(tessera_t *tessere, cella_t *campo, int nr, int nc, int n_tessere, int pos, int max_punt, cella_t *max_sol);
int calcolaPunteggio(cella_t *campo, tessera_t *tessere, int nr, int nc);
void setUsed(tessera_t *tessere, cella_t *campo, int nr, int nc, int n_tessere);
tessera_t* getTessere(char *filename, int *n_tessere);
cella_t* getCampo(char *filename, int *nr, int *nc);
void printCampo(cella_t *campo, tessera_t *tessere, int nr, int nc);


int main() {
	tessera_t *tessere;
	cella_t *campo,*sol;
	int n_tessere,nr,nc,i;

	tessere = getTessere(FILENAME_TILES,&n_tessere);
	campo = getCampo(FILENAME_BOARD,&nr,&nc);

	sol = malloc(nr*nc*sizeof(cella_t));

	setUsed(tessere,campo,nr,nc,n_tessere);

	printf("Punteggio max = %d\n",recursion(tessere,campo,nr,nc,n_tessere,0,0, sol));
	printf("Soluzione:\n");
	printCampo(sol,tessere,nr,nc);
	printf("\n");


	return 0;
}

int recursion(tessera_t *tessere, cella_t *campo, int nr, int nc, int n_tessere, int pos, int max_punt, cella_t *max_sol) {
	int punteggio,i;
	
	// condizione terminale
	if(pos >= nr*nc) {
		punteggio = calcolaPunteggio(campo,tessere,nr,nc);
		if(punteggio > max_punt) {
			for (i = 0; i < nr * nc; i++)
				max_sol[i] = campo[i];
			max_punt = punteggio;
		}
		return max_punt;
	}

	// se la cella è già occupata
	if(campo[pos].tess != -1) {
		max_punt = recursion(tessere,campo,nr,nc,n_tessere,pos + 1, max_punt, max_sol);
		return max_punt;
	}

	for(i = 0; i < n_tessere; i++) {
		if(!(tessere[i].used)) {
			tessere[i].used = 1;
			campo[pos].tess = i;

			campo[pos].rot = 0;
			max_punt = recursion(tessere,campo,nr,nc,n_tessere,pos + 1, max_punt, max_sol);
			campo[pos].rot = 1;
			max_punt = recursion(tessere,campo,nr,nc,n_tessere,pos + 1, max_punt, max_sol);
			campo[pos].tess = -1;

			tessere[i].used = 0;
		}
	}

	return max_punt;
}

int calcolaPunteggio(cella_t *campo, tessera_t *tessere, int nr, int nc) {
	int punt_totale = 0, punt_parziale;
	int i,j,index,colore;
	int valid;

	// somma punteggi per righe
	for(i = 0; i < nr; i++) {
		valid = 1;
		index = i * nr;

		punt_parziale = campo[index].rot ? tessere[campo[index].tess].val2 : tessere[campo[index].tess].val1;
		colore = campo[index].rot ? tessere[campo[index].tess].col2 : tessere[campo[index].tess].col1;

		for(j = 1; j < nc; j++) {
			index = i * nr + j;
			punt_parziale += campo[index].rot ? tessere[campo[index].tess].val2 : tessere[campo[index].tess].val1;
			if(colore != (campo[index].rot ? tessere[campo[index].tess].col2 : tessere[campo[index].tess].col1)){
				valid = 0;
				break;
			}

		}

		if(valid) punt_totale+=punt_parziale;
	}


	// somma punteggi per colonne
	for(j = 0; j < nc; j++) {
		valid = 1;
		index = j;

		punt_parziale = campo[index].rot ? tessere[campo[index].tess].val1 : tessere[campo[index].tess].val2;
		colore = campo[index].rot ? tessere[campo[index].tess].col1 : tessere[campo[index].tess].col2;

		for(i = 1; i < nr; i++) {
			index = i * nr + j;
			punt_parziale += campo[index].rot ? tessere[campo[index].tess].val1 : tessere[campo[index].tess].val2;
			if(colore != (campo[index].rot ? tessere[campo[index].tess].col1 : tessere[campo[index].tess].col2)){
				valid = 0;
				break;
			}

		}

		if(valid) punt_totale+=punt_parziale;
	}

	return punt_totale;
}

void setUsed(tessera_t *tessere, cella_t *campo, int nr, int nc, int n_tessere) {
	int i,j;
	for(i = 0; i < n_tessere; i++) {
		tessere[i].used = 0;

		for( j = 0; j < nr*nc; j++) {
			if(campo[j].tess == i) {
				tessere[i].used = 1;
				break;
			}
		}
	}
}

tessera_t* getTessere(char *filename, int *n_tessere) {
	FILE *fp;
	tessera_t *ret;
	int i;

	fp = fopen(filename,"r");
	if(fp == NULL) {
		fprintf(stderr,"errore apertura file tiles");
		exit(EXIT_FAILURE);
	}
	fscanf(fp,"%d\n",n_tessere);
	
	ret = malloc((*n_tessere)*sizeof(tessera_t));
	for(i = 0; i < *n_tessere; i++) {
		fscanf(fp,"%c %d %c %d\n",&(ret[i].col1),&(ret[i].val1),&(ret[i].col2),&(ret[i].val2));
	}
	return ret;
}

cella_t* getCampo(char *filename, int *nr, int *nc) {
	FILE *fp;
	int i,j;
	cella_t *campo;

	fp = fopen(filename,"r");
	if(fp == NULL) {
		fprintf(stderr,"errore apertura file tiles");
		exit(EXIT_FAILURE);
	}
	
	fscanf(fp,"%d %d\n",nr,nc);

	campo = (cella_t *) malloc(*nr * *nc * sizeof(cella_t));

	for(i = 0; i < *nr * *nc; i++) {
			fscanf(fp, "%d/%d",&(campo[i].tess),&(campo[i].rot));
	}

	return campo;

}

void printCampo(cella_t *campo, tessera_t *tessere, int nr, int nc) {
	int i,j,index;
	for(i = 0; i < nr; i++) {
		for(j = 0; j < nc; j++) {
			index = i * nr + j;
			printf("%d,%d:(%c,%d,%c,%d) ",campo[index].tess,campo[index].rot,tessere[campo[index].tess].col1,tessere[campo[index].tess].val1,tessere[campo[index].tess].col2,tessere[campo[index].tess].val2);
		}
		printf("\n");
	}
}