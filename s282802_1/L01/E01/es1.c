/*
 * Laboratorio 1 Esercizio 1
 * Autore: Andrea Grillo
 * Data: Ottobre 2021
 */

/* 
 * compilato e testato con
 * gcc (Debian 8.3.0-6) 8.3.0
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char *cercaRegexp(char *src, char *regexp);

int main() {
	char stringa[] = "strinGa";
	char regex[] = "\\ar.n\\A[^fghjb]";

	printf("found string: %s.\n",cercaRegexp(stringa,regex));
}

char *cercaRegexp(char *src, char *regexp) {
	int found;

	for(int i = 0; i < strlen(src); i++) {
		found = 1;

		for(int regexCount = 0, srcCount = 0; regexCount < strlen(regexp) && found; regexCount++, srcCount++) {

			// '.'
			if(regexp[regexCount] == '.') continue;
			
			// \[A/a]
			else if(regexp[regexCount] == '\\') {
				regexCount++;
				if(regexp[regexCount] == 'A' && islower(src[i+srcCount]))			found = 0;
				else if(regexp[regexCount] == 'a' && isupper(src[i+srcCount]))		found = 0;
				else if(regexp[regexCount] != 'A' && regexp[regexCount] != 'a') 
					fprintf(stderr,"Errore di formato della regex!\n"); return NULL;
			}
			
			else if(regexp[regexCount] == '[') 
			{
				// [^..]
				if(regexp[regexCount+1] == '^')
				{
					for(regexCount+=2;regexp[regexCount] != ']';regexCount++)
						if(regexp[regexCount] == src[i+srcCount])		found = 0;
				}

				// [..]
				else
				{
					for(regexCount++, found = 0; regexp[regexCount] != ']'; regexCount++)
						if(regexp[regexCount] == src[i+srcCount])		found = 1;

				}					
			}
			else if(regexp[regexCount] != src[i+srcCount])					found = 0;
		}
		if(found) return &src[i];
	}
	return NULL;
}
