#ifndef DEBUG_H
#define DEBUG_H

// costante che definisce se sto compilando in modalità DEBUG o no
#define DEBUGGING 0

//macro per stampare solo se DEBUGGING è 1
// stampo su stderr per avere l'output senza buffer
// (è capitato che in caso di segfault non arrivasse
//  la stampa perché ancora nel buffer di stdout)
#if DEBUGGING 
#define DEBUG(arg ...) fprintf(stdout,arg);
#else 
#define DEBUG(arg ...) {}
#endif

#endif
