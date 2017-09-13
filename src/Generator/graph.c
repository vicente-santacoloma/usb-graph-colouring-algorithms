/* 
  IID graphs 
 */

#include "graph.h"
#include <string.h>

/*
        Title:  IID Graph Generator (Improved version - May 1995)
	file:	graph.c
        Does:   Generates quasi-random k-colorable graphs that may be used
                to test coloring algorithms.  In an IID, independent random
		edge assignment is used.  The hidden coloring is passed in
		a blocking graph.  (A graph where each color set appears as
		a clique).

		NOTE:  contains some code for generating Lovasz graphs,
			but these functions are not used by the main
			program.  The only vital function is k_color()
      
	Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Joseph Culberson (original version)
		Numerous modifications:
			Adam Beacham, May 1995
			Denis Papp, May 1995
        email:  joe@cs.ualberta.ca
		abeacham@cs.ualberta.ca
		dpapp@cs.ualberta.ca

        Compilation: C Code

        Inputs: <Passed as parameters to k_color() >
		graph - the graph generated
		block - the blocking graph
		order - number of vertices
		size - number of edges
		prob - edge probability
	
        Outputs: Returns the graph in graph, number of edges in size.

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/



#define DEBUGVV 1
#define DEBUG 1

/********
  MACROS
 ********/

#define setedge(i,j)	set_edge(i,j,1,graph)
#define clearedge(i,j)	set_edge(i,j,0,graph)
#define edge(i,j)	get_edge(i,j,graph)

/********
  TYPES
 ********/

typedef unsigned int colortype;


/*
 * k_color
 *
 * Creates a general k-colorable graph.  It is assumed that
 * block has been initialized in a way such that the different
 * color classes are cliques in the graph block.
 *
 * An edge is assigned between two vertices with probability prob
 * provided it is not blocked.
 *
 */  
void k_color(graph,block,order,size,prob)
graphtype graph;
graphtype block;
int order; 
long *size;
double prob; /* congruency classes */
{
	int i, j;

	*size=0;

	for(i=0;i<order; i++) {
		/* record partitioning */
		for(j=0;j<i;j++) {
			if ( get_edge(i,j,block) == 0
			   && ( dblrand() < prob)) {
				/* Put the edge in the graph */
				setedge(i,j);
				(*size)++;
			}
			else {
				clearedge(i,j);
			}

		}
	}

	for(i=0;i<order;i++)
		clearedge(i,i);

	printf("The number of edges set is %ld\n",*size);
}


