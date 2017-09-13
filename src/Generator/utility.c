/*
	Title:	Graph generator utility functions
	file:	utility.c
	Does:	various functions: handle the edges in the adjacency matrix,
		random numbers, get graph degree statistics, shuffle
		a vertex permutation
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Joseph Culberson 
		new functions added by Denis Papp in May, 1995 
	email:	joe@cs.ualberta.ca
		dpapp@cs.ualberta.ca

	Compilation: C Code

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include "generate.h"
#include <string.h>
#include <math.h>

#ifndef linux
extern void bzero(char *,int);
#endif

/*
	EDGES IN GRAPHS
*/

char masks[ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

/* set_edge()
 * GIVEN: vertices u/v, value to set x, graph
 * DESC: sets the appropriate bit in the graph to x
 *       since the graph is symmetric it only sets the  lower triangle
 */
void set_edge( register int u, register int v, char x, graphtype graph  )
{
        register int i,j, byte, bit, mask;

	if (u> v) { i= u; j=v;}
	else {j=u;i=v;}

        bit  = 7 - (j & 0x00000007);
        byte = j >> 3;

        mask = masks[bit];
        if ( x == 1 ) graph[i][byte] |= mask;
        else  graph[i][byte] &= ~mask;
}

/* get_edge()
 * GIVEN: vertices u/v, graph
 * RETURNS: what the appropriate bit in the graph is set to
 * NOTE: since the graph is symmetric it only looks at the lower triangle
 */
char get_edge(  int u,  int v, graphtype graph ) {
        int i,j, byte, bit;
        char mask;

	if (u>v) {i=u; j=v;}
	else {j=u; i=v;}

        bit  = 7-(j & 0x00000007);
        byte = j >> 3;

        mask = masks[bit];
        return( (graph[i][byte] & mask)==mask );
}

/* cleargraph()
 * GIVEN: a graph
 * DESC: sets the graph to all 0s
 */
void cleargraph(graphtype graph )
{
	bzero((char *)graph,MAX_NR_VERTICES*MAX_NR_VERTICESdiv8);
}


/*
 * initrandom()
 * DESC: inputs a random seed and seeds the generator with xsrandom
 * RETURNS: the seed used
 */
int initrandom()
{
	int seed;

	printf("Please enter a seed for the random number generator: ");
	scanf("%d",&seed);
	printf("The seed is %d\n",seed);
	xsrandom(seed);

	return(seed);
}

/*
 * dblrand()
 * RETURNS: uses xdrandom to return a double random between 0.0 and 1.0 
 */
double dblrand()
{
	return (double) xdrandom();
}

/*
 * rand()
 * RETURNS: users xrandom to return a long random between 0 and k-1 
 */
long rand(long k)
{
	return (long) (xrandom() % k);
}

/* 
 * create_and_shuffle()
 * GIVEN: an int array vset, the number of vertices (order)
 * DESC: creates a random order of vertices and stores in vset
 */
void create_and_shuffle(vset,order)
int *vset,order;
{
        long i,k,t;


        for(i=0;i<order;i++) {
                vset[i] = i;
        }
        /* shuffle */
        /*for(i=0;i<order-1;i++) {*/
	for( i=0; i<order; i++ ) {	
                k = i+ rand(order-i);
                t = vset[k];
                vset[k] = vset[i];
                vset[i] = t;
        }
}

void something(s)
char *s;
{
	printf("\n\n***********\n");
	printf("%s NOT IMPLEMENTED\n",s);
	printf("***********\n\n");
}


/*
 * get_degree_info()
 * GIVEN: a graph, order and a string
 * ASSUMES: string has been allocated and is null-terminated
 * DESC: calculates the min/max/avg degree of the vertices in the
 *       the graph and concatenates a descriptive line on to the string
 *	eg. "c Min:2 Avg:3.2 Max:6 Std:1.23"
 */
void get_degree_info(graphtype graph, int order, char *string)
{
	double min=order;
	double max=0;
	double total=0;
	int i,j;
	int degree[order];
	double avg,std,totalsqr;
	double intermediate;
	char temp[100];

	/* clear degree array */
	for (i=0; i<order; i++) degree[i]=0;

	/* count degrees */
	for (i=0; i<order; i++)
	for (j=0; j<i; j++) 
		if (get_edge(i,j,graph)) {
			degree[i]++;
			degree[j]++;
		}

	/* statistics */
	totalsqr = total = 0;
	for (i=0; i<order; i++) {
		if (degree[i]<min) 
			min = degree[i]; 
		if (degree[i]>max)
			max = degree[i];
		total += degree[i];
		totalsqr += degree[i] * degree[i];
	}

	avg = total / order;

	intermediate =(totalsqr - (2*avg*total) + (avg * total))/order; 
	if( intermediate >= 0.0 ) {
		std = sqrt(intermediate);
	} else {
		std = 0.0;
	}
	sprintf(temp,
		"c   Degree Information:\nc   Min:%d Avg:%f Max:%d Std:%f\n",
		     (int)min,avg,(int)max,std);
	printf("%s\n",temp);
	strcat(string,temp);
}


