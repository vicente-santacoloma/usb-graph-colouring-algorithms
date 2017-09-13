#include "generate.h"
#include <string.h>
/*
        Title:  Hidden Coloring Verifier
	file:	verify.c
        Does:   Given a graph and an array containing the coloring given to
		each vertex, it verifies the existence of the specified
		hidden coloring by first sorting the vertices in order
		of color and then trying to color them with a simple
		greedy algorithm.
        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Joseph Culberson (original simple greedy algorithm)
		Adam Beacham (sorting, and modifications of greedy to
				work with new data structures)
        email:  joe@cs.ualberta.ca
		abeacham@cs.ualberta.ca

        Compilation: C Code

        Inputs: <passed as parameters - prompted>
		partset - a GLOBAL variable (initialized in randgraph.c)
			 that details the color of each vertex (as hidden
			 by the graph generator)
		graph - the graph to be verified
		size - number of vertices
		part - number of color partitions.
		verify - verification string which is printed in the
			output file
		
        Outputs: A verification string returned in verify.

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/

typedef int (*compfunc)();


/*
 * Verify
 *
 * Verifies that generated coloring is valid with a simple greedy.
 *
 */

void verify_color(graphtype graph, int size, int part, char *verify);

extern	int partset[MAX_NR_VERTICES];
extern char get_edge(int u, int v, graphtype graph );


/*
 * colorcompare
 *
 * Used by qsort to sort vertices by color.
 *
 * Note that array is sorted based on colors contained in partset,
 * not on what is in the array of vertices.
 *
 * NOTE:  partset must be global for this to work.
 *
 */
int colorcompare( const int *i, const int *j )
{
	return (partset[(*i)]-partset[(*j)]);
}




/*
 * Verifies that given coloring is valid.
 *
 * graph - the graph
 * size -  number of vertices
 * part -  number of partitions in graph produced by program
 * verify - verification string
 *
 */
void verify_color(graph,size,part,verify)
graphtype graph;
int size,part;
char *verify;
{

        int *start,*vertex,*next;
	int *vset;		/* Permutation of vertices */
        int i,k,clr,v,maxclr,freeid;
	char temp[100];
	
	/* Permute vertices so that they are in order by color */
	/* This requires that the array partset is global */
	vset = (int *) calloc( size+1 , sizeof(int) );
	for( i=0; i<size; i++ ) {
		vset[i] = i;
	}

	qsort( (int *) vset, (int) size, sizeof(int), (compfunc)colorcompare );

        start = (int *) calloc(size+1,sizeof(int));
        vertex = (int *) calloc(size+1,sizeof(int));
        next = (int *) calloc(size+1,sizeof(int));

        /* All color sets are empty initially */
        for(i=1; i<=size;i++) start[i]=0;
        freeid = 1; /* next free position */
                                
        maxclr = 0;

        /* greedy color in the order of vset */
        for(i=0;i<size;i++) {
                clr = 1;
                k = start[clr];
                v = vset[i]; /* choosing in permuted order */

                /* initnbr(x,v); */

                while (k) {
                        /* no conflicts with clr but vertex at k untested */
			if( get_edge(v, vertex[k], graph) == 1 ) {
			        /* conflict with clr */
                                clr++; /* next clr */
                                k = start[clr];
                        }
                        else k = next[k];  /* next vertex position to test */
                }
                /* k=0 means clr had no conflicts so use it */
                if (clr >maxclr) maxclr = clr;
        
                /* add vertex to appropriate color list */
                vertex[freeid] = v;
                next[freeid] = start[clr];
                start[clr] = freeid;
                freeid++;
      }


        if (maxclr < part) {
                printf("WARNING: Coloring better than partition\n");
                printf("\t This means that due to the choice of probability\n");
                printf("\t and partition there exists better colorings than\n");
                printf("\t the one specified\n");
		printf("Color = %d specified partitions = %d\n",maxclr,part);
		sprintf(verify, "c WARNING:  Coloring better than partition\n");
                strcat(verify, "c\t This means that due to the choice of probability\n");
                strcat(verify,"c\t and partition there exists better colorings than\n");
                strcat(verify,"c\t the one specified\n");
		sprintf(temp,"c Color = %d specified partitions = %d\n",maxclr,part);
		strcat(verify,temp);
		
        } else if (maxclr == part) {
                printf("c COLOR VERIFICATION: Using the permuted order\n");
                printf("c under simple greedy yields the specified\n");
                printf("c coloring number\n");
                printf("c Color = %d specified partitions = %d\n",
                        maxclr,part);
                sprintf(verify,"c COLOR VERIFICATION: Using the permuted order\n");
                strcat(verify,"c under simple greedy yields the specified\n");
                strcat(verify,"c coloring number\n");
                sprintf(temp,"c Color = %d specified partitions = %d\n",
                        maxclr,part);
		strcat(verify,temp);
        } else {
                printf("ERROR: Coloring greater than partition number\n");
                printf("This indicates a program error and should not occur\n");
                printf("Color = %d Partition = %d\n",maxclr,part);
		sprintf(verify,"c ERROR:  Coloring greater than partition number\n");
                strcat(verify,"c This indicates a program error and should not occur\n");
                sprintf(temp,"c Color = %d Partition = %d\n",maxclr,part);
		strcat(verify,temp);
        }
        free((char *)start);
        free((char *)vertex);
        free((char *)next);
}

