#include <stdio.h>
typedef enum {
        zaffiro,rubino,topazio,smeraldo
} comandi;

int checkSol(int *sol, int sol_dim) {
	// 0 z - 1 r - 2 t - 3 s
	
	for(int i = 0; i < sol_dim; i++) {
		if((sol[i] == zaffiro && !(sol[(i+1)%sol_dim] == zaffiro || sol[(i+1)%sol_dim] == rubino)) ||
			(sol[i] == smeraldo && !(sol[(i+1)%sol_dim] == smeraldo || sol[(i+1)%sol_dim] == topazio)) ||
			(sol[i] == rubino && !(sol[(i+1)%sol_dim] == smeraldo || sol[(i+1)%sol_dim] == topazio)) ||
			(sol[i] == topazio && !(sol[(i+1)%sol_dim] == zaffiro || sol[(i+1)%sol_dim] == rubino)) ) {
				return 0;
			}
	}
	return 1;
}


int main(){
	int sol[] = {zaffiro,zaffiro,rubino,topazio,rubino,topazio,zaffiro,rubino,smeraldo,topazio};
	int sol_dim = 10;
	printf("%d\n",checkSol(sol,sol_dim));
}