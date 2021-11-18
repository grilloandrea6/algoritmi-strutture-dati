#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

void solveDP(int *val, int n);
void displaySol(int *opt, int *val, int n);
int max(int a, int b);

int solveR(int *val, int *opt, int k);
void solve(int *val, int n);



int main() {
	char filename[255];
	FILE *fp;
	int *val,n=8,i;
	struct timeval stop, start;


	printf("Inserisci il nome del file da leggere: ");
	scanf("%s",filename);

	fp = fopen(filename,"r");
	if(fp == NULL) {
		fprintf(stderr, "Errore nell'apertura del file");
		exit(-1);
	}

	fscanf(fp,"%d",&n);

	val = malloc((n + 1) * sizeof(int));

	val[0] = 0;

	for(i = 1; i <= n; i++) {
		fscanf(fp,"%d",&val[i]);
	}

	gettimeofday(&start, NULL);
	solveDP(val,n);
	gettimeofday(&stop, NULL);
	printf("Dynamic solution took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

	gettimeofday(&start, NULL);
	solve(val,n);
	gettimeofday(&stop, NULL);
	printf("Recursive solution took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
	
}


int solveR(int *val, int *opt, int k) {
	if (k==0)
		return 0;
	if (k==1)
		return val[1];
	return max(solveR(val,opt,k-1), solveR(val,opt,k-2) + val[k]);
} 

void solve(int *val, int n) {
	int *opt;
	opt = calloc((n+1),sizeof(int));
	
	printf("Recursive solution: ");
	printf("maximum population covered %d\n", solveR(val, opt, n));
}


void solveDP(int *val, int n) {
	int i, *opt;

	opt = calloc((n+1),sizeof(int));
	opt[1] = val[1];
	
	for (i=2; i<=n; i++) {
		if (opt[i-1] > opt[i-2]+val[i])
			opt[i] = opt[i-1];
		else
			opt[i] = opt[i-2] + val[i];
	} 

	printf("Dynamic programming  solution: ");
	printf("maximum population covered %d\n", opt[n]);
	displaySol(opt, val, n);
}



void displaySol(int *opt, int *val, int n){
	int i, j, *sol;
	sol = calloc((n+1),sizeof(int));
	sol[1]=1;
	i=n;
	while (i>=2) {
		printf("i=%d\n", i);
		if (opt[i] == opt[i-1]){
			sol[i] = 0;
			i--;
		} else if (opt[i] == opt[i-2] + val[i]) {
			sol[i] = 1;
			sol[i-1] = 0;
			i -=2;
		}
	}
	for (i=1; i<=n; i++)
		if (sol[i])
			printf("%d ", val[i]);
	printf("\n");
}



int max(int a, int b) {
	return a > b ? a : b;
}
