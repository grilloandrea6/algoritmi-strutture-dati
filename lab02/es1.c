#include <stdio.h>

int gcd(int a, int b);
int gcd_euclide(int a, int b);

void main() {
	printf("gcd %d %d\n",gcd(30,15),gcd_euclide(30,15));
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
