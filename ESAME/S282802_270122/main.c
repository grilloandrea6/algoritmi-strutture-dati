/*
 * Autore: Andrea Grillo
 * 
 * Algoritmi e Strutture Dati
 * Politecnico di Torino
 * 27/01/2022
 *
 * Traccia 18 punti
 * 
 * Commenti e correzioni si 
 * trovano nella relazione in allegato.
 */

#include <stdio.h>
#include <stdlib.h>

#define FILE_GRIGLIA "griglia.txt"
#define FILE_PROPOSTA "proposta.txt"

int **readFile(char *nomefile, int *nr, int *nc);
void freeMat(int **mat, int nr);	
int verifica(char *nomefile, int **mat, int nr, int nc, int *n_regioni);
void solve(int **mat, int R, int C);
void solveR(int pos, int k, int **mat, int R, int C, int *sol_i, int *sol_j, int *sol_dim, int *flag_found);
int checkRegione(int s_i, int s_j, int dim, int **mat);
int checkSol(int **mat, int nr, int nc);
void addRegione(int s_i, int s_j, int dim, int **mat);
void removeRegione(int s_i, int s_j, int dim, int **mat);
int min(int a, int b);

int main() {
	int nr,nc,i,j,n_regioni;

	int **matrice = readFile(FILE_GRIGLIA, &nr, &nc);

	printf("Matrice letta:\n");
	for(i = 0; i < nr; i++){
		for(j = 0; j < nc; j++)
			printf("%d ",matrice[i][j]);
		printf("\n");
	}

	if(verifica(FILE_PROPOSTA, matrice, nr, nc, &n_regioni))
		printf("La soluzione presente nel file proposta.txt È valida.\n");
	else
		printf("La soluzione presente nel file proposta.txt NON È valida.\n");

	solve(matrice, nr,nc);

	freeMat(matrice, nr);
}

int **readFile(char *nomefile, int *nr, int *nc) {
	int **mat,i,j;
	FILE *fp;
	
	fp = fopen(nomefile, "r");
	if(fp == NULL) {
		fprintf(stderr,"Errore apertura file.\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp,"%d%d",nr,nc);
	
	mat = malloc(*nr * sizeof(int*));
	
	for(i = 0; i < *nr; i++) {
		mat[i] = malloc(*nc * sizeof(int));
		for(j = 0; j < *nc; j++)
			fscanf(fp,"%d",&(mat[i][j]));
	}
	
	fclose(fp);
	return mat;
}

void freeMat(int **mat, int nr) {
	int i;
	for(i = 0; i < nr; i++)
		free(mat[i]);
	free(mat);
}

int verifica(char *nomefile, int **mat, int nr, int nc, int *n_regioni) {
	FILE *fp;
	int **mat_,i,j,n,s_i,s_j,dim1,dim2,ret = 1;
	fp = fopen(nomefile, "r");
	if(fp == NULL) {
		fprintf(stderr,"Errore apertura file.\n");
		exit(EXIT_FAILURE);
	}

	// allocazione memoria e copia dei dati
	// nella nuova matrice
	mat_ = malloc(nr * sizeof(int*));
	for(i = 0; i < nr; i++) {
		mat_[i] = malloc(nr * sizeof(int));
		for(j = 0; j < nc; j++) {
			mat_[i][j] = mat[i][j];
		}
	}

	// leggo numero di regioni
	fscanf(fp,"%d",n_regioni);

	// per ogni regione
	for(n = 0; n < *n_regioni; n++) {
		// leggo dati della regione
		fscanf(fp, "%d %d %d %d", &s_i, &s_j, &dim1, &dim2);
	
		// se le sue dimensioni sono diverse
		if(dim1 != dim2) {
			ret = 0;
			break;
		}

		// incremento la matrice in ogni elemento della sottoregione
		for(i = s_i; i < dim1 + s_i; i++) {
			for(j = s_j; j < dim1 + s_j; j++) {
				mat_[i][j]++;
			}
		}
	}

	// if non necessario, però migliora il caso medio
	if(ret) { 
		for(i = 0; i < nr && ret; i++) {
			for(j = 0; j < nc && ret; j++) {
				if(mat_[i][j] != 1) {
					ret = 0;
				}
			}
		}
	}

	// free memoria matrice
	for(i = 0; i < nr; i++)
		free(mat_[i]);
	free(mat_);

	// chiusura file
	fclose(fp);

	return ret;
}


void solve(int **mat, int R, int C) {
	int k,i,*sol_i, *sol_j, *sol_dim,flag_found = 0;
	
	// allocazione memoria vettori soluzione
	sol_i = malloc(R*C*sizeof(int));
	sol_j = malloc(R*C*sizeof(int));
	sol_dim = malloc(R*C*sizeof(int));
	
	// esecuzione algoritmo ricorsivo
	for(k = 0; k < R*C && !flag_found; k++)
		solveR(0, k, mat, R, C, sol_i, sol_j, sol_dim,&flag_found);
	
	// stampa soluzione
	printf("Soluzione migliore utilizza %d sottoregioni.\n",k - 1);
	for(i = 0; i < k - 1; i++) {
		printf("Regione:\t(%d,%d) -> %dx%d\n",
			sol_i[i],sol_j[i],sol_dim[i],sol_dim[i]);
	}

	// free memoria vettori soluzione
	free(sol_i);
	free(sol_j);
	free(sol_dim);
}

void solveR(int pos, int k, int **mat, int R, int C, int *sol_i, int *sol_j, int *sol_dim, int *flag_found) {
	int dim, i, j;

	// caso terminale, ho aggiunto k regioni
	if(pos >= k) {	
		if(checkSol(mat,R,C)) {
			*flag_found = 1;
		}
		return;
	}

	// aggiunta di una regione
	// di ogni dimensione valida
	for(dim = 1; dim < min(R,C) && !(*flag_found); dim++) {

		// in ogni posizione all'interno della matrice
		for(i = 0; i <= R - dim && !(*flag_found); i++) { 
			for(j = 0; j <= C - dim && !(*flag_found); j++) {

				// se è libera
				if(checkRegione(i,j,dim,mat)) {
					
					// aggiungo in matrice
					addRegione(i,j,dim,mat);
		
					// aggiungo in soluzione
					sol_i[pos] = i;
					sol_j[pos] = j;
					sol_dim[pos] = dim;
		
					// ricorro
					solveR(pos + 1, k, mat, R, C, sol_i, sol_j, sol_dim,flag_found);
					
					// backtrack, rimuovo in matrice
					removeRegione(i,j,dim,mat);
				}
			}
		}
	}
	return;
}

int checkSol(int **mat, int nr, int nc) {
	int i,j;

	// controllo che tutti gli elementi della matrice siano 1

	for(i = 0; i < nr; i++) {
		for(j = 0; j < nc; j++) {
			if(mat[i][j] != 1)
				return 0;
		}
	}
	return 1;
}

void addRegione(int s_i, int s_j, int dim, int **mat) {
	int i,j;

	// incremento unitario di ogni elemento della matrice della regione

	for(i = s_i; i < s_i + dim; i++) {
		for(j = s_j; j < s_j + dim; j++) {
			mat[i][j]++;
		}
	}
}

void removeRegione(int s_i, int s_j, int dim, int **mat) {
	int i,j;

	// decremento unitario di ogni elemento della matrice della regione

	for(i = s_i; i < s_i + dim; i++) {
		for(j = s_j; j < s_j + dim; j++) {
			mat[i][j]--;
		}
	}
}

int checkRegione(int s_i, int s_j, int dim, int **mat) {
	int i,j;

	// controllo che lo spazio dove aggiungo la regione sia libero
	// non mi serve controllare che stiamo nelle dimensioni massime nr e nc, 
	// è garantito dai for

	for(i = s_i; i < s_i + dim; i++) {
		for(j = s_j; j < s_j + dim; j++) {
			if(mat[i][j] != 0)
				return 0;
		}
	}
	return 1;
}

int min(int a, int b) {
	if(a < b)
		return a;
	return b;
}