/*
 * Laboratorio 3 Esercizio 1
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

#include <stdio.h>

int majority( int *a, int N);
int conta(int x,int *A,int N);

int main() {
	int a[] = {0, 1, 0, 2, 3, 4, 0, 5};
	printf("maj %d\n", majority(a,8));
}

int majority( int *a, int N) {
	int q = N/2;

	if(N==1) return a[0];

	int majLeft = majority(a,q);
	int majRight = majority(a+q,q);

	if(majLeft == majRight) return majLeft;
	if(conta(majLeft,a,N) > N/2) return majLeft;
	if(conta(majRight,a,N) > N/2) return majRight;

	return -1;
}

int conta(int x,int *A,int N) {
	int count = 0;
	for(int i = 0; i < N; i++)
		if(A[i] == x) count++;
	return count;
}

