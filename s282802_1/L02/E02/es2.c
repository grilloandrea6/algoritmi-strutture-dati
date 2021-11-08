/*
 * Laboratorio 2 Esercizio 2
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

#include <stdio.h>
#include <stdlib.h>

#define FILENAME "mat.txt"


int openFile(FILE **fp, char* filename);
int **malloc2dR(char* filename, int *nr, int *nc);
void separa(int **mat, int nr, int nc, int **v_bianchi, int **v_neri);

void main() {
	int **matrice, nr, nc, *v_bianchi, *v_neri, i, j;

	matrice = malloc2dR(FILENAME, &nr, &nc);

	separa(matrice, nr, nc, &v_bianchi, &v_neri);

	// stampo matrice
	printf("Matrice di dimensioni %dx%d:\n",nr,nc);

	for( i = 0; i < nr; i++) {
		for( j = 0; j < nc; j++)
			printf("%d ",matrice[i][j]);
		printf("\n");
	}

	// stampo vettori
	printf("\nVettore 1: ");
	for( i = 0; i < (nr * nc + 1) / 2 - 1; i++)
		printf("%d ",v_bianchi[i]);
	
	printf("\nVettore 2: ");
	for( i = 0; i < (nr * nc + 1) / 2; i++)
		printf("%d ",v_neri[i]);
	printf("\n");

	// free memoria
	for(i = 0; i < nr; i++)
		free(matrice[i]);
	free(matrice);
	free(v_bianchi);
	free(v_neri);
}

int openFile(FILE **fp, char* filename){
    *fp = fopen(filename,"r");
    return(*fp != NULL);
}

 
int **malloc2dR(char* filename, int *nr, int *nc) {
	FILE *fp;
	int **matrice;

	// apertura file
	if(!openFile(&fp,filename)) {
    	fprintf(stderr,"Errore nell'apertura del file!\n");
    	exit(EXIT_FAILURE);
	}

	if(fscanf(fp,"%d %d ",nr,nc) != 2) {
		fprintf(stderr,"Errore lettura file!\n");
       	exit(EXIT_FAILURE);
    }

    // allocazione memoria
	matrice = (int **) malloc(*nr * sizeof(int *));

	if(matrice == NULL) {
		fprintf(stderr,"Errore allocazione!\n");
    	exit(EXIT_FAILURE);
	}

	for(int i = 0; i < *nr; i++) {
		// allocazione memoria
		matrice[i] = (int *) malloc((*nc) * sizeof(int));

		if(matrice[i] == NULL) {
			fprintf(stderr,"Errore allocazione!\n");
        	exit(EXIT_FAILURE);
    	}
		for(int j = 0; j < *nc; j++) 
			fscanf(fp,"%d ",&matrice[i][j]);
	}

	fclose(fp);

	return matrice;	
}

void separa(int **mat, int nr, int nc, int **v_bianchi, int **v_neri) {
	int riga_pari = 0, j = 0,bianchi_counter = 0, neri_counter = 0;

	(*v_bianchi) = (int *) malloc( ((nr * nc + 1) / 2 - 1) * sizeof(int));
	(*v_neri)    = (int *) malloc( (nr * nc + 1) / 2 * sizeof(int));

	for(int i = 0; i < nr; i++)
	{
		if(riga_pari) (*v_bianchi)[bianchi_counter++] = mat[i][j++];
		while(j < nr) {

			(*v_neri)[neri_counter++] = mat[i][j++];
			if(j < nr) (*v_bianchi)[bianchi_counter++] = mat[i][j++];
		}

		riga_pari = !riga_pari;
		j = 0;
	}
}

