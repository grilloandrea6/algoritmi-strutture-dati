/*
 * Laboratorio 2 Esercizio 1
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

/*
 * Testa che i valori trovati dalla funzione gcd
 * secondo l'algoritmo proposto, e dalla funzione
 * che sfrutta l'algoritmo di euclide siano coincidenti
 * per ogni combinazione di valori interi ammissibili.
 */

#include <stdio.h>
#include <limits.h>

int gcd(int a, int b);
int gcd_euclide(int a, int b);

void main() {
	for(int i = 0; i < INT_MAX; i++)
		for(int j = 0; j < INT_MAX; j++){
			if(gcd(i,j) != gcd_euclide(i,j)) printf("OPS %d,%d,%d,%d",i,j,gcd_euclide(i,j),gcd(i,j));
			return;
		}
}

int gcd(int a, int b) {
	if (a < b) {
		int temp = a;
		a = b;
		b = temp;
	}

	if (a == b) return a;

	if (a % 2 == 0) {
		if(b % 2 == 0)
			return 2*gcd(a/2,b/2);

		return gcd(a/2,b);
	}

	if(b % 2 == 0)
		return gcd(a,b/2);

	return gcd((a-b)/2,b);
}

int gcd_euclide(int a, int b) {
	if(a == b) return a;
	if(a > b) return gcd_euclide(a-b,b);
	return gcd_euclide(a,b-a);
}
