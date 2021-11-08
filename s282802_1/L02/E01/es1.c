/*
 * Laboratorio 2 Esercizio 1
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

#include <stdio.h>

int gcd(int a, int b);
int gcd_euclide(int a, int b);

void main() {
	int i = 25, j = 5;
	printf("%d,%d,%d\n",i,j,gcd(i,j));
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