// Quick Sort

#include <stdio.h>

void Swap(int *A, int a, int b);
int partition(int *A, int l, int r);
void QuickSortR(int *A, int l, int r);
void QuickSort(int A[], int N);

int main() {
	int A[] = {3, 4, 5, 1, 2, 7, 9, 8, 0, 6};

	QuickSort(A,10);

	for(int i = 0; i < 10; i++)
		printf("%d ", A[i]);

	printf("\n");

}

void QuickSort(int A[], int N) {
	int l = 0, r = N - 1;

	QuickSortR(A, l, r);
}

void QuickSortR(int *A, int l, int r) {
	int q;
	if(l >= r) return;
	q = partition(A,l,r);
	QuickSortR(A,l,q-1);
	QuickSortR(A,q+1,r);
	return;
}

int partition(int *A, int l, int r) {
	int i = l - 1, j = r;
	int x = A[r]; //Pivot

	for(;;) {
		while(A[++i] < x);
		while(A[--j] > x)
			if(j == l)
				break;

		if(i>=j)
				break;
		Swap(A,i,j);
	}
	Swap(A,i,r);
	return(i);
}

void Swap(int *A, int a, int b) {
	int temp = A[a];
	A[a] = A[b];
	A[b] = temp;
	return;
}