#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL_FILE 255

typedef struct {
	int s,f;
} att;

int compatible(att a, att b);
void sortAttivita(int n, att *v);
void attSel(int n, att *val);
int getDuration(att att);
int displaySol(att *val, int *opt, int n);

void main() {
	char filename[MAXL_FILE];
	att *attivita;
	FILE *fp;
	int n_attivita,i;

	printf("Inserisci il nome del file da leggere: ");
	scanf("%s",filename);
	//strcpy(filename,"att1.txt");

	fp = fopen(filename,"r");

	if(fp == NULL) {
		fprintf(stderr,"Errore nell'apertura del file.\n");
		return;
	}

	fscanf(fp,"%d",&n_attivita);

	attivita = (att *) malloc((1+n_attivita)*sizeof(att));

	attivita[0].s=0;
	attivita[0].f=0;

	printf("Data:\n");
	for(i = 1; i <= n_attivita; i++) {
		fscanf(fp,"%d%d",&(attivita[i].s),&(attivita[i].f));
		printf("%d %d\n",(attivita[i].s),(attivita[i].f));
	}
	printf("End data.\n");

	fclose(fp);

	sortAttivita(n_attivita,attivita);

	attSel(n_attivita, attivita);

	free(attivita);
}

void attSel(int n, att *val) {
	int i,j,
		max = 0,max_index,
		*opt = calloc(n+1,sizeof(int));

	opt[1] = getDuration(val[1]);

	for(i = 2; i <= n; i++) {
		for(j = i - 1; j >= 0; j--) {
			if(compatible(val[i], val[j])) {
				if(opt[j] + getDuration(val[i]) > opt[i]) opt[i] = opt[j] + getDuration(val[i]);
			}
		}
	}

	for(i = 1; i <= n; i++) {
		if(opt[i] > max) {
			max = opt[i];
			max_index = i;	
		} 
	}


	printf("Max time: %d\n",max);

	displaySol(val,opt,max_index);

	free(opt);
}

void sortAttivita(int n, att *v) {
	int i,j;
	att temp;
	for(i = 0; i < n; i++) {
		for(j = 0; j < n-i; j++) {
			if((v[j].f == v[j+1].f) ? (v[j].s > v[j+1].s) : (v[j].f > v[j+1].f)) {
				temp = v[j];
				v[j] = v[j+1];
				v[j+1] = temp;
			}
		}
	}
}

int displaySol(att *val, int *opt, int n) {
	int j,i = n;
	int *sol = calloc(n+1,sizeof(int));

	sol[n] = 1;
	while(i>0) {
		for(j = i - 1; j >= 0; j--) {
			if(opt[i] == opt[j] + getDuration(val[i]) && compatible(val[i],val[j])) {
				sol[j] = 1;
				i = j;
				break;

			}
		}
	}

	printf("Solution: ");
	for(i = 1; i <= n; i++) {
		if(sol[i]) printf("(%d,%d) ", val[i].s,val[i].f);
	}
	printf("\n");

	free(sol);
}

int getDuration(att att) {
	return att.f - att.s;
}

int max(int a, int b) {
	if(a>b) return a;
	return b;
}

int compatible(att a, att b) {
	if ((a.s < b.f && b.s < a.f))
			return 0;

	return 1;
}
