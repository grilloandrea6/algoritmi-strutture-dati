#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N_SCELTE 5
#define LEN 31

enum { falso, vero };
typedef int bool;

void stampaMenu(char *scelte[], int *selezione){
  int i=0;
  printf("\nMENU'\n");
  for(i=0;i<N_SCELTE;i++)
    printf("%2d > %s\n",i,scelte[i]);
  scanf(" %d",selezione);
}

int main(int argc, char **argv) {
  char *scelte[] = {
    "Uscita",
    "Stampa elenco ordinato",
    "Verifica sottografo completo - lista",
    "Verifica sottografo completo - matrice",
    "Genera lista adiacenze"
  };
  int isListaGenerated = 0;

  char nodoA[LEN],nodoB[LEN],nodoC[LEN];
  int selezione;
  FILE *fin;
  bool fineProgramma;

  fin = fopen("pg.txt","r");
//TODO
  fclose(fin);

  do {
    stampaMenu(scelte, &selezione);
    switch(selezione){

      case 0: {
        fineProgramma = vero;
      } break;

      case 1: { // stampa elenco ordinato
      } break;

      case 2: { // verifica sottografo completo - lista
        if(!isListaGenerated) {
          fprintf(stderr,"Non hai ancora generato la lista delle adiacenze!\n");
          break;
        }
        
        printf("Inserire nome primo nodo: ");
        scanf("%s", nodoA);
        printf("Inserire nome secondo nodo: ");
        scanf("%s", nodoB);
        printf("Inserire nome terzo nodo: ");
        scanf("%s", nodoC);

      } break;

      case 3: { // verifica sottografo completo - matrice
        printf("Inserire nome primo nodo: ");
        scanf("%s", nodoA);
        printf("Inserire nome secondo nodo: ");
        scanf("%s", nodoB);
        printf("Inserire nome terzo nodo: ");
        scanf("%s", nodoC);

        
      } break;

      case 4: {// genera lista adiacenze

        isListaGenerated = 1;
      } break;
      default:{
        printf("Scelta non valida\n");
      } break;
    }
  } while(!fineProgramma);

  //TODO FREE

  return 0;
}
