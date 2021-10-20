/*
 * Laboratorio 3 Esercizio 2
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX 256

int princ_molt(int pos, int *val,int *sol,int n, int cnt, char ***data);


void main() {
	int n;
	FILE *fp;
	int *val;
	char ***data;
	char str[MAX];

	// apertura file
	fp = fopen("brani.txt","r");

	// lettura numero di amici
	fscanf(fp,"%d\n",&n);

	// allocazione memoria
	val = (int *)malloc(n*sizeof(int));
	data = (char ***) malloc(n*sizeof(char *));

	// lettura dati da file
	for(int i=0; i<n; i++) {
		// lettura numero di scelte per amico
		fscanf(fp,"\t%d\n",&(val[i]));

		// allocazione memoria
		data[i] = (char **) malloc(val[i]*sizeof(char*));

		// lettura delle scelte
		for(int j = 0; j < val[i]; j++) {
			fscanf(fp,"\t\t%s\n",str);
			data[i][j] = malloc((strlen(str) + 1) * sizeof(char));
			strcpy(data[i][j],str);

		}
		
	}

	int *sol = malloc(n*sizeof(int));

	printf("Numero combinazioni: %d.\n",princ_molt(0, val,sol, n, 0,data));

}

int princ_molt(int pos, int *val,int *sol,int n, int cnt, char ***data) {
	int i;
	if (pos >= n) {
		printf("Combinazione: ");
		for (i = 0; i < n; i++)
			printf("%s ", data[i][sol[i]]);
		printf("\n");
		return cnt+1;
	}

	for (i = 0; i < val[pos]; i++) {
		sol[pos] = i;
		cnt = princ_molt(pos+1, val, sol, n, cnt,data);
	}
	return cnt;
}
