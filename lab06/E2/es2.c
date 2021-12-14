#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

typedef enum {
	zaffiro,rubino,topazio,smeraldo
} pietre;

int max(int a, int b);

int fZ(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);
int fR(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);
int fT(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);
int fS(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int i,j,k,l,n,n_prove,lenmax,tmp,dati[4],
		maxZ = 0,
		maxR = 0,
		maxT = 0,
		maxS = 0,
		****memoizationZ,
		****memoizationR,
		****memoizationT,
		****memoizationS;

	//printf("Inserisci il nome del file da leggere: ");
	//scanf("%s",filename);
	// just for testing purposes - 
	strcpy(filename,"hard_test_set.txt");

	fp = fopen(filename,"r");
	if(fp == NULL) {
		fprintf(stderr,"Impossibile aprire il file.\n");
		exit(EXIT_FAILURE);
	}

	// first reading of the file to get 
	// the maximum dimensions for the matrix
	fscanf(fp,"%d",&n_prove);
	for(n = 0; n < n_prove; n++) {
		fscanf(fp,"%d%d%d%d",
			&(dati[zaffiro]),
			&(dati[rubino]),
			&(dati[topazio]),
			&(dati[smeraldo]));
		maxZ = max(maxZ,dati[zaffiro]);
		maxR = max(maxR,dati[rubino]);
		maxT = max(maxT,dati[topazio]);
		maxS = max(maxS,dati[smeraldo]);

	}

	// allocate matrices
	memoizationR = malloc((1 + maxZ) * sizeof(int ***));
	memoizationZ = malloc((1 + maxZ) * sizeof(int ***));
	memoizationT = malloc((1 + maxZ) * sizeof(int ***));
	memoizationS = malloc((1 + maxZ) * sizeof(int ***));


	for(i = 0; i <= maxZ; i++) {
		memoizationZ[i] = malloc((1 + maxR) * sizeof(int **));
		memoizationR[i] = malloc((1 + maxR) * sizeof(int **));
		memoizationT[i] = malloc((1 + maxR) * sizeof(int **));
		memoizationS[i] = malloc((1 + maxR) * sizeof(int **));

		for(j = 0; j <= maxR; j++) {
			memoizationZ[i][j] = malloc((1 + maxT) * sizeof(int *));
			memoizationR[i][j] = malloc((1 + maxT) * sizeof(int *));
			memoizationT[i][j] = malloc((1 + maxT) * sizeof(int *));
			memoizationS[i][j] = malloc((1 + maxT) * sizeof(int *));

		
			for(k = 0; k <= maxT; k++) {
				memoizationZ[i][j][k] = malloc((1 + maxS) * sizeof(int));
				memoizationR[i][j][k] = malloc((1 + maxS) * sizeof(int));
				memoizationT[i][j][k] = malloc((1 + maxS) * sizeof(int));
				memoizationS[i][j][k] = malloc((1 + maxS) * sizeof(int));
		
				for(l = 0; l <= maxS; l++) {
					memoizationZ[i][j][k][l] = -1;
					memoizationR[i][j][k][l] = -1;
					memoizationT[i][j][k][l] = -1;
					memoizationS[i][j][k][l] = -1;
				}
			}
		}
	}

	// re-read the file
	rewind(fp);
	// just to skip first number
	fscanf(fp,"%d",&n_prove);

	for(n = 0; n < n_prove; n++) {
		fscanf(fp,"%d%d%d%d",
			&(dati[zaffiro]),
			&(dati[rubino]),
			&(dati[topazio]),
			&(dati[smeraldo]));

		printf("TEST #%d\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
			n+1,
			dati[zaffiro],
			dati[rubino],
			dati[topazio],
			dati[smeraldo],
			dati[zaffiro] + dati[rubino] + dati[topazio] + dati[smeraldo]);

		lenmax = fZ(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		
		tmp = fR(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		lenmax = max(lenmax, tmp);

		tmp = fT(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		lenmax = max(lenmax, tmp);

		tmp = fS(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		lenmax = max(lenmax, tmp);

		printf("Collana massima di lunghezza %d\n",lenmax);
	}

	fclose(fp);
	
	// free matrices
	for(i = 0; i <= maxZ; i++) {
		for(j = 0; j <= maxR; j++) {
			for(k = 0; k <= maxT; k++) {
				free(memoizationZ[i][j][k]);
				free(memoizationR[i][j][k]);
				free(memoizationT[i][j][k]);
				free(memoizationS[i][j][k]);
			}
			free(memoizationZ[i][j]);
			free(memoizationR[i][j]);
			free(memoizationT[i][j]);
			free(memoizationS[i][j]);
		}
		free(memoizationZ[i]);
		free(memoizationR[i]);
		free(memoizationT[i]);
		free(memoizationS[i]);
	}
	free(memoizationZ);
	free(memoizationR);
	free(memoizationT);
	free(memoizationS);
}

int fZ(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	if(dati[zaffiro] == 0) return 0;

	if(memoZ[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoZ[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[zaffiro]--;

	a = fZ(memoZ, memoR, memoT, memoS, dati);
	b = fR(memoZ, memoR, memoT, memoS, dati);

	dati[zaffiro]++;
	
	memoZ[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = max(a,b) + 1;

	return (max(a,b) + 1);
}

int fR(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	if(dati[rubino] == 0) return 0;

	if(memoR[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoR[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[rubino]--;

	a = fS(memoZ, memoR, memoT, memoS, dati);
	b = fT(memoZ, memoR, memoT, memoS, dati);

	dati[rubino]++;

	memoR[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = max(a,b) + 1;

	return (max(a,b) + 1);
}

int fT(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	if(dati[topazio] == 0) return 0;

	if(memoT[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoT[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[topazio]--;

	a = fZ(memoZ, memoR, memoT, memoS, dati);
	b = fR(memoZ, memoR, memoT, memoS, dati);
	
	dati[topazio]++;
	
	memoT[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = max(a,b) + 1;
	
	return (max(a,b) + 1);
}

int fS(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	if(dati[smeraldo] == 0) return 0;

	if(memoS[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoS[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[smeraldo]--;

	a = fS(memoZ, memoR, memoT, memoS, dati);
	b = fT(memoZ, memoR, memoT, memoS, dati);
	
	dati[smeraldo]++;

	memoS[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = max(a,b) + 1;
	
	return (max(a,b) + 1);
}

int max( int a, int b) {
	if (a > b)
		return a;
	return b;
}
