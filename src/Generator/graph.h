/*
	Title:	Graph.c header file
	file:	graph.h
	Does:	contains various prototypes; these are needed primarily
		for debugging purposes
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Adam Beacham
	email:	abeacham@cs.ualberta.ca

	Compilation: C Code

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include "generate.h"

/**********
  CONSTANTS     
 **********/

#define MAXM 15
#define MAXVERTEX MAX_NR_VERTICES

/********
  Types
 ********/

typedef long BicoefType[MAXM][MAXM];

/***********************
   Function Prototypes
 ***********************/
void k_color(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		int order, long *size, double prob);

void InitBicoef(BicoefType Bicoef);

void gensub(unsigned long pattern, long *where, 
		unsigned long subset[MAXVERTEX], int m, int n, int depth);

void prtbits(unsigned long v, int n);

void Lovastz(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8], 
		int *order, long *size, double prob);

void printgraph(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8], 
		int order);

void vertexperm(int order, int vrd[MAXVERTEX]);


long linprob(long prob, long delta, long r, long c, long k);


