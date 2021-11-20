#ifndef DEBUG_H
#define DEBUG_H

// costante che definisce se sto debuggando o no
#define DEBUGGING

//macro per stampare solo se DEBUGGING è definita
// stampo su stderr per avere l'output senza buffer
// (è capitato che in caso di segfault non arrivasse
//  la stampa perché ancora nel buffer di stdout)
#ifdef DEBUGGING 
#define DEBUG(arg ...) fprintf(stderr,arg);
#else 
#define DEBUG(arg ...) {}
#endif

#endif
