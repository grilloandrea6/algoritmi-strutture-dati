#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

typedef enum {
	zaffiro,rubino,topazio,smeraldo
} pietre;

int recursion(pietre prec, int pos, int k, int dati[]);
int compatible(pietre prec, pietre att);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int n,i,k,sol_dim,dati[4],found = 0;

	//printf("Inserisci il nome del file da leggere: ");
	//scanf("%s",filename);
	// just for testing purposes - 
	strcpy(filename,"hard_test_set.txt");

	fp = fopen(filename,"r");
	if(fp == NULL) {
		fprintf(stderr,"Impossibile aprire il file.\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp,"%d",&n);

	for(i = 0; i < n; i++) {
		fscanf(fp,"%d%d%d%d",
			&(dati[zaffiro]),
			&(dati[rubino]),
			&(dati[topazio]),
			&(dati[smeraldo]));

		sol_dim = dati[zaffiro] +
				dati[rubino] +
				dati[topazio] +
				dati[smeraldo];


		printf("TEST #%d\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
			i+1,
			dati[zaffiro],
			dati[rubino],
			dati[topazio],
			dati[smeraldo],
			sol_dim);


		for(k = sol_dim, found = 0; k > 1 && !found; k--) {
			found = recursion(-1,0, k, dati);
		}

		if(found)
			printf("Collana massima di lunghezza %d\n",k + 1);
		else
			printf("Non è stata trovata una collana possibile\n");
	}
	fclose(fp);
}

int recursion(pietre prec, int pos, int k, int dati[]) {
	if (pos >= k)
		return 1;

	for(pietre p = zaffiro; p <= smeraldo; p++) {
		if(dati[p] > 0 && compatible(prec, p)){
			dati[p]--;
			if(recursion(p, pos+1, k, dati)) return 1;
			dati[p]++;
		}
	}
	return 0;
}


int compatible(pietre prec, pietre att) {
	if(	(prec == zaffiro && (att == zaffiro || att == rubino)) ||
		(prec == smeraldo && (att == smeraldo || att == topazio)) ||
		(prec == rubino && (att == smeraldo || att == topazio)) ||
		(prec == topazio && (att == zaffiro || att == rubino)) ||
		 prec == -1)
		return 1;

	return 0;
}
