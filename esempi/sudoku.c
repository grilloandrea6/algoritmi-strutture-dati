#include <stdio.h>
#include <math.h>
#include <stdlib.h>



void disp_ripet(int**schema, int dim, int pos);

int controlla(int**schema, int dim, int pos, int val);

int num_sol;




int main() {
	int**schema, dim, i;
	char nomefile[20];

	printf("Inserire il nome del file: ");
	scanf("%s", nomefile);
	schema = acquisisci(nomefile, &dim);



	disp_ripet(schema, dim, 0);

	printf("\n Numero di soluzioni = %d\n", num_sol);

	for(i=0; i<dim; i++)
		free(schema[i]);
	free(schema);
	return 0;
}



void disp_ripet(int**schema, int dim, int pos) {
	int i, j, k;

	if(pos >= dim*dim) {
		num_sol++;
		stampa(schema, dim); 
		return;
	}

	i = pos / dim;
	j = pos % dim;

	if(schema[i][j] != 0) {
		disp_ripet(schema, dim, pos+1);
		return;
	}

	for(k=1; k<=dim; k++) {
		schema[i][j] = k;
		if(controlla(schema, dim, pos, k))
			disp_ripet(schema, dim, pos+1);
		schema[i][j] = 0;
	}
	return;
}




int controlla(int**schema, int dim, int pos, int val) {
	int i, j, r, c, n=floor(sqrt(dim));

	i = pos/dim;
	j = pos % dim;

	for(c=0; c<dim; c++) {
		if(c!=j)
			if(schema[i][c]==val)
				return 0;
	}
	
	for (r=0; r<dim; r++) {
		if(r!=i)
			if(schema[r][j]==val)
				return 0;
	}

	for(r=(i/n)*n; r<(i/n)*n+n; r++)
		for(c=(j/n)*n; c<(j/n)*n+n; c++) {
			if((r!=i) || (c!=j))
				if(schema[r][c]==val)
					return 0;
			}

	return 1;
}

