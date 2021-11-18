#include <stdio.h>
#include <limits.h>


int minCostR(int*p, int i, int j, int minCost) {
	int k, cost;
	if(i ==j) return 0;

	for(k=i; k<j; k++) {
		cost = minCostR(p, i, k, minCost) + 
				minCostR(p, k+1, j, minCost) + 
				p[i-1]*p[k]*p[j];

		if(cost < minCost)
			minCost = cost;
	}

	return minCost; 
}

int matrix_chainR(int *p, int n) {
	return minCostR(p, 1, n, INT_MAX);
}

int main() {
	int p[] = {10,100,50,12,32,54};
	printf("%d\n",matrix_chainR(p,6));

}