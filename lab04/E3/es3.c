#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

#define PRUNING

typedef enum {
        zaffiro,rubino,topazio,smeraldo
} pietre;

int checkSol(int *sol, int to_check, int sol_dim);
int recursion(int *sol, int pos, int max_dim, int count, int dati[],int);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int n,i,sol_dim,*sol,dati[4];

	printf("Inserisci il nome del file da leggere: ");
	scanf("%s",filename);
	// just for testing purposes - strcpy(filename,"hard_test_set.txt");

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

		sol_dim =	dati[zaffiro] + 
					dati[rubino] + 
					dati[topazio] + 
					dati[smeraldo];

		sol = (int*) malloc(sol_dim * sizeof(int));

		printf("TEST #%d\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
			i+1,
			dati[zaffiro],
			dati[rubino],
			dati[topazio],
			dati[smeraldo],
			sol_dim);


		//recursion
		printf("Collana massima di lunghezza %d\n",recursion(sol,0, sol_dim, 0, dati,0));

		// freeing solution
		free(sol);
	}
	fclose(fp);
}


int checkSol(int *sol, int to_check, int sol_dim) {
	for(int i = 0; i < to_check; i++) {
		if((sol[i] == zaffiro && !(sol[(i+1)%sol_dim] == zaffiro || sol[(i+1)%sol_dim] == rubino)) ||
			(sol[i] == smeraldo && !(sol[(i+1)%sol_dim] == smeraldo || sol[(i+1)%sol_dim] == topazio)) ||
			(sol[i] == rubino && !(sol[(i+1)%sol_dim] == smeraldo || sol[(i+1)%sol_dim] == topazio)) ||
			(sol[i] == topazio && !(sol[(i+1)%sol_dim] == zaffiro || sol[(i+1)%sol_dim] == rubino)) ) {
				return 0;
			}
	}
	return 1;
}

int recursion(int *sol, int pos, int max_dim, int max_found, int dati[], int prova) {
	pietre p;
	if(pos > 0 && checkSol(sol,pos,pos))
	{
		if(pos + 1 > max_found) max_found = pos + 1;
		//return max_found;
	}


#ifdef PRUNING
	if(pos < max_dim && checkSol(sol,pos-1,pos)) {
#else
	if(pos < max_dim) {
#endif
		for(p = zaffiro; p <= smeraldo; p++) {
			if(dati[p] > 0) {

#ifdef PRUNING
				// pruning dinamico
				if( ((p == rubino || p == zaffiro)		&& !(sol[pos-1] == zaffiro	|| sol[pos-1] == topazio)) ||
					((p == smeraldo || p == topazio)	&& !(sol[pos-1] == smeraldo	|| sol[pos-1] == rubino)))
						continue;
#endif
				sol[pos] = p;
				dati[p]--;
				max_found = recursion(sol,pos + 1, max_dim, max_found, dati, prova + 1);
				dati[p]++;
			}
		}
	}

	return max_found;
}