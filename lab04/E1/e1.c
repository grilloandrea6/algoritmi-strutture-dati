/*
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>


#define FILENAME "grafo.txt"

void getData(int ***matrice_incidenza, int *n_vertici, int *n_archi);
void freeMatrice(int **mat, int N);

int isSolution(int *sol, int N,int **matrice_incidenza,int n_archi){
	int i, j,
		*archi = calloc(n_archi,sizeof(int));

	for(i = 0; i < N; i++) {
		for(j = 0; j < n_archi;j++)
			if(matrice_incidenza[j][sol[i]] == 1)
				archi[j] = 1;
	}

	for(i = 0; i < n_archi; i++)
		if(archi[i] == 0)
			return 0;

	return 1;
}

int powerset(int pos, int *sol, int n, int start, int count, int n_archi, int **matrice) {
	int i;
	if (start >= n) {
		if(pos != 0 && isSolution(sol,pos,matrice,n_archi)) {
			printf("( ");
			for(i = 0; i < pos; i++)
				printf("%d ",sol[i]);
			printf(")\n");
			return count+1;
		}
		return count;
	}
	
	for (i = start; i < n; i++) {
		sol[pos] = i;
		count = powerset(pos+1, sol, n, i+1, count,n_archi,matrice);
	}
	count = powerset(pos, sol, n, n, count,n_archi,matrice);
	return count;
}


int main(){
	int **matrice_incidenza, *sol, *used, n_archi, n_vertici;

	getData(&matrice_incidenza,&n_vertici,&n_archi);
	
	/* 
	DEBUG - stampa matrice
	for(int i = 0; i < n_archi; i++) {
		for(int j = 0; j < n_vertici; j++)
			printf("%d ",matrice_incidenza[i][j]);
		printf("\n");
	}

	printf("\n\n");
	*/

	sol = (int*) malloc(n_vertici*sizeof(int));
	used = (int*) malloc(n_archi*sizeof(int));

	powerset(0, sol, n_vertici, 0, 0,n_archi,matrice_incidenza);

	freeMatrice(matrice_incidenza,n_archi);
}


void getData(int ***matrice_incidenza, int *n_vertici, int *n_archi) {
	int i,a,b;
	FILE *fp = fopen(FILENAME,"r");

	fscanf(fp,"%d%d",n_vertici,n_archi);

	*matrice_incidenza = (int **) malloc(*n_archi * sizeof(int *));
	for(i = 0; i < *n_archi; i++) {
		(*matrice_incidenza)[i] = (int *) malloc(*n_vertici * sizeof(int));
	
		fscanf(fp,"%d%d",&a,&b);
		(*matrice_incidenza)[i][a] = 1;
		(*matrice_incidenza)[i][b] = 1;
	}
}


void freeMatrice(int **mat,int N) {
	for(int i = 0; i < N; i++) {
		free(mat[i]);
	}
	free(mat);
}



/*int recursion(int *sol, int pos, int *used, int **matrice, int n_archi, int n_vertici,int cnt) {
	int i;
	if(isSolution(sol,pos,matrice,n_vertici,n_archi)) {
		for(i = 0; i < pos; i++)
			printf("%d ",sol[i]);
		printf("\n");
		return cnt+1;
	}
	if(pos > n_vertici) return cnt;

	for(i = pos; i < n_vertici; i++) {
		if(!used[i]) {
			used[i] = 1;
			sol[pos] = i;
			cnt = recursion(sol,pos+1,used,matrice,n_archi,n_vertici,cnt);
			used[i] = 0;	
		}
			//cnt = recursion(sol,pos,used,matrice,n_archi,n_vertici,cnt);


	}
}
*/