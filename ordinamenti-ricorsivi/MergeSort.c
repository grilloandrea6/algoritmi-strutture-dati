// Merge Sort

#include <stdio.h>

void MergeSort(int A[], int B[], int N);
void MergeSortR(int A[], int B[], int l, int r);
void Merge(int A[], int B[], int l, int q, int r);

int main() {
	int A[] = {3, 4, 5, 1, 2, 7, 9, 8, 0, 6};
	int B[10];

	MergeSort(A,B,10);
	for(int i = 0; i < 10; i++)
		printf("%d ",A[i]);

	printf("\n");

}

void MergeSort(int A[], int B[], int N) {
	int l = 0, r = N - 1;
	MergeSortR(A,B,l,r);
}

void MergeSortR(int A[], int B[], int l, int r) {
	int q = (r + l) / 2;
	if(r <= l) return;

	MergeSortR(A,B,l,q);
	MergeSortR(A,B,q+1,r);
	Merge(A,B,l,q,r);
}

void Merge(int A[], int B[], int l, int q, int r) {
	int left_count = l; // i
	int right_count = q + 1; // j
int i; // k
	for(i = l; i <= r; i++) {
		if(left_count > q) 
			B[i] = A[right_count++];
		else if (right_count > r)
			B[i] = A[left_count++];
		else if(A[left_count] <= A[right_count])
				B[i] = A[left_count++];
		else
				B[i] = A[right_count++];	
	}
	for (i = l; i <= r; i++)
		A[i] = B[i];

	return;
}