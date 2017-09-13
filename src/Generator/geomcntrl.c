#include "generate.h"
#include <string.h>
/*
        Title:  Geometric Graph Control (Improved version May 1995)
	file:	geomcntrl.c
        Does:   Prompts user for input needed for generating a geometric
		graph.  Calls the geometric graph generator.
        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Original version:  Jonathan Baldwin
		Modified by Joe Culberson, March 1995
		Modified by Adam Beacham, May 1995
		Modified by Denis Papp, May 1995
        email:  joe@cs.ualberta.ca
		abeacham@cs.ualberta.ca
		dpapp@cs.ualberta.ca

        Compilation: C Code

        Inputs: <from stdin - prompted; uses functions in inputs.c>
                 Parameters:
			graph - the graph
			block - the blocking graph (ind. sets in graph
				are cliques in block)
			order - # of vertices
			part - number of color partitions
			size - number of edges
		 Input from StdIn:
			dim - dimension of graph
			wrap - == 1 if wrapping on
			dflag - == 1 if adding an edge when vertices within
				distance of each other; == 0 if edge added
				when vertices > distance from each other
			gnufile - name of gnuplot file.
        Outputs:  Prompts to stdout.  The file geomgraph.c actually
		  creates the graph and gnu file.

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/


void gen_and_print_geo(specs,graph,block,order,part,size)
char *specs;
char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
int order;
int part;
long *size;
{
	double distance;
	int wrap = 0, dim = 0;
	int dflag;
	FILE *gnufile;
	char temp[80];

	getdimension(&dim);
	sprintf(temp,"c   Dimension = %d\n",dim);
	strcat(specs,temp);
	printf("\n----------\n");	

	getdistance(&distance);
	printf("\n----------\n");
	distance *= distance;

	/* See if user wants d <= dist or d > dist */
	getdflag(&dflag, sqrt(distance));

	strcat(specs,"c   Edge if dist ");
	if( dflag ) {
		sprintf(temp,"<= %f\n",sqrt(distance));
	} else {
		sprintf(temp,"> %f\n",sqrt(distance));
	}
	strcat(specs,temp);

	getwrap(&wrap);
	if( wrap ) {
		strcat(specs,"c   Wrapping\n");
		if ( distance >= 1.0 ) 
	  		fprintf(stderr,"Warning: graph will be k-complete \
because distance > 1.0 and wrapping on.\n"); 
	} else {
		strcat(specs,"c   No wrapping\n");
		if( distance >= dim )
			fprintf(stderr,"Warning: graph will be k-complete \
because distance > sqrt(dimension).\n");
	}
	printf("\n----------\n");

	getgnufile(specs,&gnufile);
	printf("\n----------\n");

	gen_geo_graph(graph,block,order,distance,dim,part,wrap,dflag,size,gnufile);

	if (gnufile != NULL) fclose(gnufile);
}
