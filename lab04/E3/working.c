#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

typedef enum {
        zaffiro,rubino,topazio,smeraldo
} pietre;

int checkSol(int *sol, int sol_dim);
int recursion(int *sol, int pos, int max_dim, int count, int dati[],int);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int n,i,sol_dim,*sol,dati[4];

	//printf("Inserisci il nome del file da leggere: ");
	//scanf("%s",filename);
	// just for testing purposes - 
	strcpy(filename,"easy_test_set.txt");

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


int checkSol(int *sol, int sol_dim) {
	if((sol[sol_dim-1] == zaffiro && !(sol[0] == zaffiro || sol[0] == rubino)) ||
		(sol[sol_dim-1] == smeraldo && !(sol[0] == smeraldo || sol[0] == topazio)) ||
		(sol[sol_dim-1] == rubino && !(sol[0] == smeraldo || sol[0] == topazio)) ||
		(sol[sol_dim-1] == topazio && !(sol[0] == zaffiro || sol[0] == rubino)) ) 
	{
		return 0;
	}
	
	return 1;
}



int recursion(int *sol, int pos, int max_dim, int max_found, int dati[], int prova) {
	if(pos + 1 > max_found && checkSol(sol,pos)) max_found = pos + 1;

	if(pos == 0) {
		for(pietre p = zaffiro; p <= smeraldo; p++) {
			if(dati[p] > 0) {
				sol[pos] = p;
				dati[p]--;
				max_found = recursion(sol,pos + 1, max_dim, max_found, dati, prova + 1);
				dati[p]++;
			}
		}
	}

	else if(pos < max_dim - 1) {
		switch (sol[pos-1]) {
			case rubino:
			case smeraldo:
				if(dati[smeraldo] > 0) {
					sol[pos] = smeraldo;
					dati[smeraldo]--;
					max_found = recursion(sol,pos + 1, max_dim, max_found, dati, prova + 1);
					dati[smeraldo]++;
				}

				if(dati[topazio] > 0) {
					sol[pos] = topazio;
					dati[topazio]--;
					max_found = recursion(sol,pos + 1, max_dim, max_found, dati, prova + 1);
					dati[topazio]++;
				}
				break;

			case zaffiro:
			case topazio:
				if(dati[zaffiro] > 0) {
					sol[pos] = zaffiro;
					dati[zaffiro]--;
					max_found = recursion(sol,pos + 1, max_dim, max_found, dati, prova + 1);
					dati[zaffiro]++;
				}

				if(dati[rubino] > 0) {
					sol[pos] = rubino;
					dati[rubino]--;
					max_found = recursion(sol,pos + 1, max_dim, max_found, dati, prova + 1);
					dati[rubino]++;
				}
				break;

		}

	}

	return max_found;
}