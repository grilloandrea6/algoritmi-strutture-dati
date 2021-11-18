
#include <stdio.h>
#include <string.h>

#define LUN_MAX 8+1
#define BASE 10
#define N 10

int lett_dist = 0;

typedef struct {
	int num;
	char val;
}alpha;


alpha * init_alpha() {
	alpha ret[N];
	for(int i = 0; i < N; i++) {
		ret[i].val = '\0';
		ret[i].num = -1;
	}

	return ret;
}

int trova_indice(alpha *lettere, char c) {
	for(int i = 0; i < N; i)
		if(lettere[i].val == c) return i;

	return -1;
}

void setup(alpha *lettere, char *st1, char *st2, char *st3) {
	int i, l1 = strlen(st1), l2 = strlen(st2), l3 = strlen(st3);
	for(i = 0; i < l1; i++) {
		if(trova_indice(lettere,st1[i]) == -1)
			lettere[lett_dist++].val = st1[i];
	}

	for(i = 0; i < l2; i++) {
		if(trova_indice(lettere,st2[i]) == -1)
			lettere[lett_dist++].val = st2[i];
	}

	for(i = 0; i < l3; i++) {
		if(trova_indice(lettere,st3[i]) == -1)
			lettere[lett_dist++].val = st3[i];
	}
}


int w2n(alpha *lettere, char *st) {
	int i, v = 0, l=strlen(st);
	if(lettere[trova_indice(lettere,st[0])].val == 0)
		return -1;

	for(i = 0; i < l; i++)
		v = v*10 + lettere[trova_indice(lettere,st[i])].val;

	return v;
}

int c_sol(alpha *lettere, char *st1, char *st2, char *st3) {
	int a = w2n(lettere,st1),
		b = w2n(lettere,st2),
		c = w2n(lettere,st2);

	if(a == -1 || b == -1 || c == -1)
		return 0;

	return ((a + b) == c);
}

int main(void) {
	char str1[LUN_MAX],str2[LUN_MAX],str3[LUN_MAX + 1];
	int mark[BASE] = {0};
	int i;

	alpha *lettere = init_alpha();

}