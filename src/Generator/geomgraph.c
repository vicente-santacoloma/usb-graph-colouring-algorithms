#include "generate.h"

/*
        Title:  Geometric Graph Generator (Improved versions - March, May 1995)
	file:	geomgraph.c
        Does:   Generates quasi-random k-colorable graphs that may be used
                to test coloring algorithms.  Randomly places the vertices
		in a dim dimensioned hypercube.  Connects vertex pairs with
		an edge if they are different colors and satisfy a distance
		requirement. 
        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: initial version:  Jonathan Baldwin
		Modified by J. Culberson, March 1995
		Modified by Denis Papp, May 1995
		Modified by Adam Beacham, May 1995
        email:  joe@cs.ualberta.ca
		abeacham@cs.ualberta.ca
		dpapp@cs.ualberta.ca

        Compilation: C Code

        Inputs: <passed as parameters; prompted for in geomcntrl.c>
		graph - the graph
		block - the blocking graph
		order - # of vertices in graph
		dist - distance
		dim - dimension of graph.
		part - number of color partitions (isn't needed?)
		wrap - == 1 if wrapping is on
		dflag - == 1 if looking for distance <= dist; ==0 if
			looking for distance > dist.
		size - # of edges put in graph
		file - the file stream to the gnuplot file.

        Outputs: creates a graph which is returned in graph.  A gnuplot
		file is created (if user picked that option).  Number of
		edges added returned in size.

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/


struct vertex {
  double x;
  double y;
};

double distance(double vert[], int dim ,long i, long j, int depth, int wrap) {
  
  double d1, d2, d=0.0;

  if (depth > 0) {
    d = distance(vert,dim,i,j,depth-1,wrap);
  }

  d1 = (vert[i*dim+depth] - vert[j*dim+depth]);
  if (wrap) {
    if (d1 > 0.0) {
	d2 = 1.0 - d1;
    }
    else {
	d2 = 1.0 + d1;
    }
    d1 *= d1;
    d2 *= d2;

    if (d2 < d1) d1 = d2;

  } else {
    d1 *= d1;
  }
  return(d+d1);
}
  

void gen_geo_graph(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		long order,
		double dist,
		int dim,
		long part,
		int wrap,
		int dflag,
		long *size,
		FILE *file
		)
{

	long i,j,k;
	double d;
	double *vert;

	printf("Generating a geometric graph on %ld vertices.\n\
Distance is %f.\n\
Dimension of %d\n\
There is a %ld colouring\n",order,sqrt(dist),dim,part);
	vert = (double *) malloc(sizeof(double)*dim*order);
	*size = 0;

	if(part > order) part = order;

	/* to randomize assign edges in random order */
	/* Still not completely random order - all edges to a vertex are
	   assigned at once - how to do this without too much memory?? */
	for (i=0;i<dim;i++) 
	  vert[i] = dblrand();

	for(i=1;i<order;i++) {
	  for (j=0;j<dim;j++) 
	    vert[i*dim+j] = dblrand();
	  for (j=0;j<i;j++) {
	    d = distance(vert,dim,i,j,dim-1,wrap);
	    if ( (dflag && (d <= dist) && (!get_edge(i,j,block)))
			|| (!dflag && (d > dist) && (!get_edge(i,j,block)))) {
	      set_edge(i,j,1,graph);
	      (*size)++;
	      if (file != NULL) {
		for(k=0;k<dim;k++)
		  fprintf(file,"%14.12f ",vert[i*dim+k]);
		fprintf(file,"\n");
		for(k=0;k<dim;k++)
		  fprintf(file,"%14.12f ",vert[j*dim+k]);
		fprintf(file,"\n\n");
	      }
	    }
	  }
	}
}
	

