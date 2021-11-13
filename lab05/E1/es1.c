#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILE 255

// costante che definisce se sto debuggando o no
//#define DEBUGGING

//macro per stampare solo se DEBUGGING è definita
// stampo su stderr per avere l'output senza buffer
// (è capitato che in caso di segfault non arrivasse
//  la stampa perché ancora nel buffer di stdout)
#ifdef DEBUGGING 
#define DEBUG(arg ...) fprintf(stderr,arg);
#else 
#define DEBUG(arg ...) {}
#endif




typedef struct {
	int s,f;
} att;

void attSel(int N, att *v);
int recursion(att* att, int *sol, int pos, int max_sol,int max_found,int*,int*, int *mark);
int checkCompatible(att *vet, int *sol, int N, int to_check);
int getTime(att *vet, int *sol, int N);

void main() {
	char filename[MAXL_FILE];
	att *attivita;
	FILE *fp;
	int n_attivita,i;

	//printf("Inserisci il nome del file da leggere: ");
	//scanf("%s",filename);
	strcpy(filename,"att.txt");

	fp = fopen(filename,"r");

	if(fp == NULL) {
		fprintf(stderr,"Errore nell'apertura del file.\n");
		return;
	}

	fscanf(fp,"%d",&n_attivita);

	attivita = (att *) malloc(n_attivita*sizeof(att));

	printf("Data:\n");
	for(i = 0; i < n_attivita; i++) {
		fscanf(fp,"%d%d",&(attivita[i].s),&(attivita[i].f));
		printf("%d %d\n",(attivita[i].s),(attivita[i].f));
	}
	printf("End data.\n");

attSel(n_attivita,attivita);
}


void attSel(int N, att *v){
	int *mark,*sol,*max_sol,i,max_found,max_sol_dim;
	mark = malloc(N * sizeof(int));
	sol = malloc(N * sizeof(int));
	max_sol = malloc(N * sizeof(int));
	for( i = 0; i < N; i++) mark[i] = 1;
max_found = recursion(v,sol,0,N,0,max_sol,&max_sol_dim,mark);
	printf("Max time: %d\nSolution: ",max_found);

for(i = 0; i < max_sol_dim; i++)
	printf("(%d,%d) ", v[max_sol[i]].s,v[max_sol[i]].f);
printf("\n");
}
int recursion(att* att, int *sol, int pos, int N,int max_found, int *max_sol,int *max_sol_dim, int *mark){
	int adding=0,i;
	for( i = 0; i < N; i++) {
		if(mark[i] && checkCompatible(att,sol,pos,i))
		{	
			adding = 1;
			DEBUG("adding %d to: ",i);
			int j;
			for( j = 0; j < pos; j++)
				DEBUG("%d ",sol[j]);
			DEBUG("\n");
			mark[i]--;
			sol[pos] = i;
			//printf("added %d %d\n",j+1,pos);
			max_found = recursion(att,sol,pos + 1, N,max_found,max_sol,max_sol_dim,mark);
			mark[i]++;
		}
	}
	if(!adding) {
		for(i = 0; i < pos; i++)
			DEBUG("%d ",sol[i]);
		DEBUG("\n");
		/*if(pos > max_found) {
			max_found = pos;
			for(i = 0; i < pos; i++)
				max_sol[i] = sol[i];
		}*/
		int tempo = getTime(att,sol,pos);
		if(tempo > max_found) {
			max_found = tempo;
			*max_sol_dim = pos;
			for(i = 0; i < pos; i++)
				max_sol[i] = sol[i];
		}
	}
	return max_found;

}

int checkCompatible(att *vet, int *sol, int N, int to_check) {
	int i;
	for(i = 0; i < N; i++) {
		DEBUG("comparing %d %d with %d %d\n",vet[i].s,vet[i].f,vet[to_check].s,vet[to_check].f);
		if (vet[sol[i]].s < vet[to_check].f && vet[to_check].s < vet[sol[i]].f)
		{
			DEBUG("returning 0\n");
			return 0;
		}
	}
	DEBUG("returning 1\n");
	return 1;
}


int getTime(att *vet, int *sol, int N) {
	int ret = 0;
	for(int i = 0; i - N; i++) {
		ret += vet[sol[i]].f - vet[sol[i]].s;
	}
	return ret;
}