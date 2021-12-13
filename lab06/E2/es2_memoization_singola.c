#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

typedef enum {
	zaffiro,rubino,topazio,smeraldo
} pietre;

int count = 0;

int MAX( int a, int b);

int fZ(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);
int fR(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);
int fT(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);
int fS(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int i,j,k,l,n,n_prove,lenmax,tmp,dati[4],
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

		memoizationZ = malloc((1 + dati[zaffiro]) * sizeof(int ***));
		memoizationR = malloc((1 + dati[zaffiro]) * sizeof(int ***));
		memoizationT = malloc((1 + dati[zaffiro]) * sizeof(int ***));
		memoizationS = malloc((1 + dati[zaffiro]) * sizeof(int ***));


		for(i = 0; i <= dati[zaffiro]; i++) {
			memoizationZ[i] = malloc((1 + dati[rubino]) * sizeof(int **));
			memoizationR[i] = malloc((1 + dati[rubino]) * sizeof(int **));
			memoizationT[i] = malloc((1 + dati[rubino]) * sizeof(int **));
			memoizationS[i] = malloc((1 + dati[rubino]) * sizeof(int **));

			for(j = 0; j <= dati[rubino]; j++) {
				memoizationZ[i][j] = malloc((1 + dati[topazio]) * sizeof(int *));
				memoizationR[i][j] = malloc((1 + dati[topazio]) * sizeof(int *));
				memoizationT[i][j] = malloc((1 + dati[topazio]) * sizeof(int *));
				memoizationS[i][j] = malloc((1 + dati[topazio]) * sizeof(int *));

			
				for(k = 0; k <= dati[topazio]; k++) {
					memoizationZ[i][j][k] = malloc((1 + dati[smeraldo]) * sizeof(int));
					memoizationR[i][j][k] = malloc((1 + dati[smeraldo]) * sizeof(int));
					memoizationT[i][j][k] = malloc((1 + dati[smeraldo]) * sizeof(int));
					memoizationS[i][j][k] = malloc((1 + dati[smeraldo]) * sizeof(int));
			
					for(l = 0; l <= dati[smeraldo]; l++) {
						memoizationZ[i][j][k][l] = -1;
						memoizationR[i][j][k][l] = -1;
						memoizationT[i][j][k][l] = -1;
						memoizationS[i][j][k][l] = -1;
					}
				}
			}
		}

		lenmax = fZ(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		
		tmp = fR(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		lenmax = MAX(lenmax, tmp);

		tmp = fT(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		lenmax = MAX(lenmax, tmp);

		tmp = fS(memoizationZ, memoizationR, memoizationT, memoizationS, dati);
		lenmax = MAX(lenmax, tmp);

		printf("Collana massima di lunghezza %d\n",lenmax);

		for(i = 0; i <= dati[zaffiro]; i++) {
			for(j = 0; j <= dati[rubino]; j++) {
				for(k = 0; k <= dati[topazio]; k++) {
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
	fclose(fp);
	printf("count is %d\n\n",count);
}

int fZ(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	count++;
	if(dati[zaffiro] == 0) return 0;

	if(memoZ[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoZ[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[zaffiro]--;

	a = fZ(memoZ, memoR, memoT, memoS, dati);
	b = fR(memoZ, memoR, memoT, memoS, dati);

	dati[zaffiro]++;
	
	memoZ[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = MAX(a,b) + 1;

	return (MAX(a,b) + 1);
}

int fR(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	count++;
	if(dati[rubino] == 0) return 0;

	if(memoR[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoR[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[rubino]--;

	a = fS(memoZ, memoR, memoT, memoS, dati);
	b = fT(memoZ, memoR, memoT, memoS, dati);

	dati[rubino]++;

	memoR[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = MAX(a,b) + 1;

	return (MAX(a,b) + 1);
}

int fT(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	count++;
	if(dati[topazio] == 0) return 0;

	if(memoT[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoT[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[topazio]--;

	a = fZ(memoZ, memoR, memoT, memoS, dati);
	b = fR(memoZ, memoR, memoT, memoS, dati);
	
	dati[topazio]++;
	
	memoT[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = MAX(a,b) + 1;
	
	return (MAX(a,b) + 1);
}

int fS(int ****memoZ, int ****memoR, int ****memoT, int ****memoS, int *dati) {
	int a,b;
	count++;
	if(dati[smeraldo] == 0) return 0;

	if(memoS[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] != -1)
		return memoS[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]];

	dati[smeraldo]--;

	a = fS(memoZ, memoR, memoT, memoS, dati);
	b = fT(memoZ, memoR, memoT, memoS, dati);
	
	dati[smeraldo]++;

	memoS[dati[zaffiro]][dati[rubino]][dati[topazio]][dati[smeraldo]] = MAX(a,b) + 1;
	
	return (MAX(a,b) + 1);
}

int MAX( int a, int b) {
	if (a > b)
		return a;
	return b;
}
