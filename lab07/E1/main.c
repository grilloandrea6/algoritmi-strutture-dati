#include <stdio.h>
#include <stdlib.h>

#define FILENAME "elementi.txt"
#define MAXL 101
#define MAX_ELEMENTI_DIAGONALE 5

typedef struct elemento_s {
	char nome[MAXL];
	int tipologia,
		direzione_ingresso,
		direzione_uscita,
		requisito_precedenza,
		finale,
		difficolta;

	float valore;
} elemento_t;


elemento_t elemento_read(FILE *fp);
void elemento_print(elemento_t elemento, FILE *fp);
void ordinaElementi(elemento_t *elementi, int n_elementi);
float elemento_cmp(elemento_t a, elemento_t b);
void algGreedy(elemento_t *A, elemento_t *B, elemento_t *C, elemento_t *elementi, int n_elementi, int dd, int dp);



int main() {
	FILE *fp;
	int i,n_elementi;
	elemento_t *elementi;
	elemento_t  diagonaleA[MAX_ELEMENTI_DIAGONALE],
				diagonaleB[MAX_ELEMENTI_DIAGONALE],
				diagonaleC[MAX_ELEMENTI_DIAGONALE];

	int dd = 10;
	int dp = 30;


	fp = fopen(FILENAME,"r");

	fscanf(fp,"%d",&n_elementi);

	elementi = malloc(n_elementi * sizeof(*elementi));

	for (i = 0; i < n_elementi; i++) {
		elementi[i] = elemento_read(fp);
		elemento_print(elementi[i],stdout);
	}

	ordinaElementi(elementi,n_elementi);

	printf("\n\n");
	for (i = 0; i < n_elementi; i++) {
		elemento_print(elementi[i],stdout);
	}

	algGreedy(diagonaleA,diagonaleB,diagonaleC,elementi,n_elementi,dd,dp);
}

/*
	VINCOLI:
		1 elemento acrobatico per diagonale
		1 avanti e 1 indietro acrobatico in tutto il programma
		almeno due acrobatici in sequenza
		se c'è finale >=8 si moltiplica punteggio diagonale per 1.5
		max 5 elementi per diagonale
		difficoltà diagonale < DD
		difficoltà totale < DP
*/

void algGreedy(elemento_t *A, elemento_t *B, elemento_t *C, elemento_t *elementi, int n_elementi, int dd, int dp) {
	int n = n_elementi;
	int nA = 0, nB = 0, nC = 0;
	int direzioneA = 1,
		direzioneB = 1,
		direzioneC = 1;
	int difficoltaA = 0,
		difficoltaB = 0,
		difficoltaC = 0;

	int added = 0;

// while posso aggiungere un elemento
// aggiungo a quella che ha punteggio minore
// devo ordinare dinamicamente
// parametro iniziale rapporto valore difficolta
// 


	while(nA < MAX_ELEMENTI_DIAGONALE && nB < MAX_ELEMENTI_DIAGONALE && nC < MAX_ELEMENTI_DIAGONALE) {

		for(n = n_elementi; n > 0 && !added; n--) {
			if(nA < MAX_ELEMENTI_DIAGONALE &&
			 (nA == 0 ? !elementi[n-1].requisito_precedenza : 1) &&
			 (direzioneA == elementi[n-1].direzione_ingresso) &&
			 // TODO ((nA == MAX_ELEMENTI_DIAGONALE - 1) ? !elementi[n-1].finale: 1) &&
			 (difficoltaA + elementi[n-1].difficolta <= dd) &&
			 (difficoltaA + difficoltaB + difficoltaC + elementi[n-1].difficolta <= dp)) {
				
				A[nA++] = elementi[n-1];
				direzioneA = elementi[n-1].direzione_uscita;
				difficoltaA+= elementi[n-1].difficolta;

				n--;
				printf("added elementi[%d] to A[%d]\n",n,nA-1);
				added = 1;
			}
		}

		added = 0;

		for(n = n_elementi; n > 0 && !added; n--) {
			if(nB < MAX_ELEMENTI_DIAGONALE &&
			 (nB == 0 ? !elementi[n-1].requisito_precedenza : 1) &&
			 (direzioneB == elementi[n-1].direzione_ingresso) &&
			 ((nB == MAX_ELEMENTI_DIAGONALE - 1) ? !elementi[n-1].finale: 1) &&
			 (difficoltaB + elementi[n-1].difficolta <= dd) &&
			 (difficoltaA + difficoltaB + difficoltaC + elementi[n-1].difficolta <= dp)) {
				
				B[nB++] = elementi[n-1];
				direzioneB = elementi[n-1].direzione_uscita;
				difficoltaB+= elementi[n-1].difficolta;

				n--;
				printf("added elementi[%d] to B[%d]\n",n,nB-1);
				added = 1;
			}
		}
		added = 0;


		
		for(n = n_elementi; n > 0 && !added; n--) {
			if(nC < MAX_ELEMENTI_DIAGONALE &&
			 (nC == 0 ? !elementi[n-1].requisito_precedenza : 1) &&
			 (direzioneC == elementi[n-1].direzione_ingresso) &&
			 ((nC == MAX_ELEMENTI_DIAGONALE - 1) ? !elementi[n-1].finale: 1) &&
			 (difficoltaC + elementi[n-1].difficolta <= dd) &&
			 (difficoltaA + difficoltaB + difficoltaC + elementi[n-1].difficolta <= dp)) {
				
				C[nC++] = elementi[n-1];
				direzioneC = elementi[n-1].direzione_uscita;
				difficoltaC+= elementi[n-1].difficolta;

				n--;
				printf("added elementi[%d] to C[%d]\n",n,nC-1);
				added = 1;
			}
		}
		added = 0;


		
	}
}



void ordinaElementi(elemento_t *elementi, int n_elementi) {
	int i,j;
	elemento_t a;
	for(i = 0; i < n_elementi; i++) {
		for(j = i; j < n_elementi - 1; j++) {
			if(elemento_cmp(elementi[j],elementi[j+1]) > 0.) {
				a = elementi[j];
				elementi[j] = elementi[j+1];
				elementi[j+1] = a;
			}
		}
	}
}

float elemento_cmp(elemento_t a, elemento_t b) {
	return a.valore - b.valore;
}

elemento_t elemento_read(FILE *fp) {
	elemento_t elemento;

	fscanf(fp,"%s %d %d %d %d %d %f %d",
			elemento.nome,
			&(elemento.tipologia),
			&(elemento.direzione_ingresso),
			&(elemento.direzione_uscita),
			&(elemento.requisito_precedenza),
			&(elemento.finale),
			&(elemento.valore),
			&(elemento.difficolta));

	return elemento;
}

void elemento_print(elemento_t elemento, FILE *fp) {
	fprintf(fp, "%s %d %d %d %d %f %d\n",
			elemento.nome,
			elemento.tipologia,
			elemento.direzione_ingresso,
			elemento.direzione_uscita,
			elemento.requisito_precedenza,
			elemento.finale,
			elemento.valore,
			elemento.difficolta);
}