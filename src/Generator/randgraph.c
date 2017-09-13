#include "generate.h"

/*
        Title:  Graph Color Hider
	file:	randgraph.c
        Does:  Generates a quasi-random k-coloring.  Each vertex is assigned
		a certain color (several different methods of assigning
		colors may be used).  Calling functions must pass a
		permutation of the vertices to prevent easy finding of
		the coloring.
        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Joseph Culberson (original algorithms)
		Adam Beacham.  Putting them all in one place, May 1995
		Denis Papp.  Numerous other vital modifications, May 1995 
        email:  joe@cs.ualberta.ca
		abeacham@cs.ualberta.ca
		dpapp@cs.ualberta.ca

        Compilation: C Code

        Inputs: <passed as parameters to initblock>
		block - the blocking graph.  Color sets in the graph to
			be genenerated appear as cliques in this graph.
		order - number of vertices
		numpart - number of color sets
		perm - a permutation of vertices.
		variab - variability, a parameter used in several types
			of k-colorings.  (double)
		type - type of k-coloring to hide.  (See comment for
			function initblock() )

        Outputs: Returns the blocking graph in the parameter block.
		 Stores the color of each vertex in the global array
		 partset.

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/



/*
 * firstclass()
 * GIVEN: partition # (c), order, number of partitions (numpart), delta value
 * RETURNS: the index of the first vertex of the partition-class c
 */
long firstclass(long c,long order,long numpart,long delta)
{
        double  m,v;

        if (c==0) return((long) 0);

	m = (double) (order *c)/((double) numpart);

	v = (double) (delta * c *(numpart-c)) / ((double) (numpart-1));

	if (v > m) v=m;

	return((long) (m-v));

}

/*
 * lastclass()
 * GIVEN: partition # (c), order, number of partitions (numpart), delta value
 * RETURNS: the index of the last+1 vertex of the partition-class c 
 */
long lastclass(long c,long order,long numpart,long delta)
{
        return(firstclass(c+1,order,numpart,delta));
}

/*
 * initblock()
 *
 * Calls initblock2() with blockflag set to 1
 *
 */
void initblock(char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		int order, int numpart, int *perm, double variab,
		int type)
{
	initblock2(block,order,numpart,perm,variab,type,1,1);
}

/* 
 * initblock2()
 * GIVEN: block graph, order, numpartitions, vertex permutation (perm),
 *        variability, type of hidden coloring, blockflag, printflag
 * DESC: creates a hidden coloring and assigns blocking edges to block 
 *
 * If blockflag = 1, block is assigned.  Otherwise, it is ignored.
 *
 * type:	0 - equipartite (also used for random)
 *		1 - k-colorable
 *		2 - k-colorable (smooth)
 *		3 - delta 
 *
 * NOTE:	The global variable partset[], (defined in the
 *		file cheat.c) is set by this function.
 *
 *		Global variable partitionflag is set.
 *
 */
void initblock2(char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		int order, int numpart, int *perm, double variab,
		int type, int blockflag, int printflag)
{
	int i, j, r;
	int firstv,lastv;
	double x, z, z1;	        /* Used in k-colorable (smooth) */
	int colorcount[MAX_NR_VERTICES];	/* A count of colors used */

	switch( type ) {
	case EQUIPARTITE:	
		/* equipartite */
		for( i=0; i<order; i++ ) {
			partset[ perm[i] ] = i % numpart;
		}
		/* don't use partset if there isn't a hidden coloring */
		if( numpart < order ) {
			partitionflag = 1;
		}
		break;
		
	case KCOLORABLE:	
		/* k-colorable (variab used) */
		/* here variab is from 0 to numpart */
	        if (variab > numpart) {
                	printf("Error - variability greater than number \
of colors\n");
                	exit(1);        
        	}  

        	if (variab > 0)
                	for(i=0;i<order;i++){
                        	r = rand(variab);
                        	partset[ perm[i] ] = r + rand(numpart-r);
        		}
        	else for(i=0;i<order;i++) { 
                	partset[ perm[i] ] = rand(numpart);
        	}
        	partitionflag = 1;

		break;

	case KSMOOTH:	
		/* k-colorable (smooth) */
		/* here variab is from 0 to 1 */
	        if ((variab < 0) || (variab > 1.0)) {
	                printf( "ERROR: Variability out of range\n");
        	        exit(1);
        	}
 
        	z = variab;
        	z1 = 1.0 -z;

        	for(i=0;i<order;i++) {
                	x = dblrand();
                	partset[ perm[i] ] = 
				(int) (numpart * x * (z*x + z1));
        	}
		partitionflag = 1;
		
		break;

	case DELTA:
		/* here variab is from 0 to numpart */
		for(r=0;r<numpart;r++) {
			firstv = firstclass(r,order,numpart,(long)variab);
			lastv = lastclass(r,order,numpart,(long)variab);
			#ifdef DEBUG
			printf("F:%d L:%d\n",firstv,lastv);
			#endif
	 		for(i=firstv;i<lastv;i++) {
				partset[ perm[i] ] = r;
			}
		}

		partitionflag = 1;

		break;

	default:
		fprintf(stderr,"Unknown graph type, %d\n",type);
		exit(1);
	}

	/* Compute and print color count, if required */
	if (partitionflag && blockflag) {
		if (printflag) {
			printf("Color count :\n");
			for( i=0; i<order; i++ ) {
				colorcount[ partset[i] ]=0;
			}
			for( i=0; i<order; i++ ) {
				colorcount[ partset[i] ]++;
			}
			for( i=0; i<numpart; i++ ) {
				printf("   %2d",colorcount[i]);
			}
			printf("\n");
		}

		/* Set the edges in the block */
		for( i=0; i<order; i++ )
		for( j=0; j<=i; j++ )
			if( partset[i] == partset[j] ) /* same color */
				set_edge(i,j,1,block);
	}

	partitionnumber=numpart;
}

