#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILENAME 255

typedef enum {
	zaffiro,rubino,topazio,smeraldo
} pietre;

int MAX( int a, int b);

int fZ(int *dati);
int fR(int *dati);
int fT(int *dati);
int fS(int *dati);

void main() {
	char filename[MAXL_FILENAME];
	FILE* fp;
	int n,i,k,sol_dim,dati[4],found = 0;

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


		int parameter[4];
		memcpy(parameter,dati,4*sizeof(int));
		int lenmax = fZ(dati);

		memcpy(dati,parameter,4*sizeof(int));
		int a = fR(dati);
		lenmax = lenmax > a ? lenmax : a;
		memcpy(dati,parameter,4*sizeof(int));
		a = fT(dati);
		lenmax = lenmax > a ? lenmax : a;
		memcpy(dati,parameter,4*sizeof(int));
		a = fS(dati);
		lenmax = lenmax > a ? lenmax : a;
		

		printf("Collana massima di lunghezza %d\n",lenmax);
		}
	fclose(fp);
}

int fZ(int *dati) {
	int a,b;
	if(dati[zaffiro] == 0) return 0;

	dati[zaffiro]--;

	a = fZ(dati);
	b = fR(dati);
	return (MAX(a,b) + 1);
}

int fR(int *dati) {
	int a,b;
	if(dati[rubino] == 0) return 0;

	dati[rubino]--;

	a = fS(dati);
	b = fT(dati);
	return (MAX(a,b) + 1);
}

int fT(int *dati) {
	int a,b;
	if(dati[topazio] == 0) return 0;

	dati[topazio]--;

	a = fZ(dati);
	b = fR(dati);
	return (MAX(a,b) + 1);
}

int fS(int *dati) {
	int a,b;
	if(dati[smeraldo] == 0) return 0;

	dati[smeraldo]--;

	a = fS(dati);
	b = fT(dati);
	return (MAX(a,b) + 1);
}

int MAX( int a, int b) {
	if (a > b)
		return a;
	return b;
}
