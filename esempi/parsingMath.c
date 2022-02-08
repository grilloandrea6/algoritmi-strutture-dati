#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STR 255
char *a;
int i = 0;
int eval(){
	int x = 0;
	while (a[i] == ' ') i++;

	if(a[i] == '+') {i++; return eval() + eval();}
	
	if(a[i] == '*') {i++; return eval() * eval();}
	if(a[i] == '-') {i++; return eval() - eval();}
	if(a[i] == '/') {i++; return eval() / eval();}

	while((a[i] >= '0') && a[i] <= '9')
		x = 10*x + (a[i++]-'0');

	return x;
}


int main(){
a = strdup("/-*+5 12 + 2 3 11 5"); 
printf("%d\n",eval());
free(a);
}

