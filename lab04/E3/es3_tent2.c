#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

#define PRUNING

typedef enum {
        zaffiro,rubino,topazio,smeraldo
} pietre;

int checkSol(int *sol, int to_check, int sol_dim);
int recursion(int *sol, int pos, int max_dim, int count, int dati[]);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int n,i,sol_dim,*sol,dati[4];

	printf("Inserisci il nome del file da leggere: ");
	scanf("%s",filename);
	// just for testing purposes - strcpy(filename,"very_very_easy_test_set.txt");

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
		int *vect = (int*) malloc(sol_dim * sizeof(int));
		pietre p;
		int counter = 0;

		for(p = zaffiro; p <= smeraldo; p++) {
			for(int alla = 0; alla < dati[p]; alla++)
			{
				vect[counter] = p;
				counter++;
			}	
		}
		for(counter = 0; counter < sol_dim; counter++) printf("%d ",vect[counter]);
			printf("\n");


		printf("TEST #%d\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
			i+1,
			dati[zaffiro],
			dati[rubino],
			dati[topazio],
			dati[smeraldo],
			sol_dim);

continue;
		//recursion
		printf("Collana massima di lunghezza %d\n",recursion(sol,0, sol_dim, 0, dati));

		// freeing solution
		free(sol);
	}
	fclose(fp);
}


int checkSol(int *sol, int to_check, int sol_dim) {
	for(int i = 0; i < to_check; i++) {
		//printf("to_check=%d,comparing %d %d indexes\n",to_check,i,(i+1)%sol_dim);

		if((sol[i] == zaffiro && !(sol[(i+1)%sol_dim] == zaffiro || sol[(i+1)%sol_dim] == rubino)) ||
			(sol[i] == smeraldo && !(sol[(i+1)%sol_dim] == smeraldo || sol[(i+1)%sol_dim] == topazio)) ||
			(sol[i] == rubino && !(sol[(i+1)%sol_dim] == smeraldo || sol[(i+1)%sol_dim] == topazio)) ||
			(sol[i] == topazio && !(sol[(i+1)%sol_dim] == zaffiro || sol[(i+1)%sol_dim] == rubino)) ) {
				return 0;
			}
	}
	return 1;
}

// TODO partire dai più grandi
// fare un for nel richiamo della recursion
// 	if(pos < max_dim && checkSol(sol,pos-1,pos)) {
// cambiare con if(pos > qualcosa)
// il qualcosa viene diminuito nel for
// così si generano prima quelle più grandi


int powerset(int pos,int*val,int*sol,int k,int start,int cnt) {
	int i;
	if(start >= k) {
		for (i = 0; i < pos; i++)
			printf("%d ", sol[i]);
		printf("\n");
		return cnt+1;
	}
	for (i = start; i < k; i++) {
		sol[pos] = val[i];
		cnt = powerset(pos+1, val, sol, k, i+1, cnt);
	}
	cnt = powerset(pos, val, sol, k, k, cnt);
	return cnt;
}



int recursion(int *sol, int pos, int max_dim, int max_found, int dati[]) {
	pietre p;
	if(pos > 0)// && checkSol(sol,pos,pos))
	{
		if(checkSol(sol,pos,pos)){
			if(pos + 1 > max_found) max_found = pos + 1;
			//printf("VALID!\n");
		}
		//for(int i = 0; i < pos; i++) printf("%d ", sol[i]);
		//printf("\n"); 
		//return max_found;
	}


	if(pos < max_dim) {
		for(p = zaffiro; p <= smeraldo; p++) {
			if(dati[p] > 0) {
				sol[pos] = p;
				dati[p]--;
				max_found = recursion(sol,pos + 1, max_dim, max_found, dati);
				dati[p]++;
			}
		}
	}
	return max_found;
}