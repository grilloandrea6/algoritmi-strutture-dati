#include <stdio.h>

int majority( int *a, int N);
int isMajority(int,int *a, int x, int N);

int main() {
	int a[] = {0, 1, 0, 2, 3, 4, 0, 5};
	printf("maj %d\n", majority(a,8));
}

int conta(int x,int *A,int N) {
	int count = 0;
	for(int i = 0; i < N; i++)
		if(A[i] == x) count++;
	return count;
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

/*int majorityR(int *a, int N, int candidate, int counter) {
	if(N == 0) return candidate;
	if(*a == candidate) counter++;
	else
		if(counter > 0)
			counter --;
		else {
			candidate = *a;
			counter = 1;
		}
	return majorityR(a+1,N-1,candidate,counter);
}

int majority( int *a, int N) {
	int maj = majorityR(a,N,a[0],0);
	// se il maggioritario esiste si trova in maj
	int count = 0;
	for(int i = 0; i < N && count <= N/2; i++)
		if(maj = a[i]) count++;

	if(count >= N/2) return maj;
	return -1;
	
}*/


/*
int majority( int *a, int N) {
	for(int i = 0; i < N; i++) {
		if(isMajority(0,a,a[i],N)) return a[i];
	}
	return -1;
}

int isMajority(int i, int *a, int x, int N) {
	int q = N/2;
	printf("called isMaj i=%d a[0]=%d x=%d, N=%d\n",i,a[0],x,N);

	if(N <= 2) return (a[0] == a[1] == x)?1:0;
	
	if( isMajority(i,a,x,q)||isMajority(i+q,a+q,x,q)) return 1;
	return 0;
}*/