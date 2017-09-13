#include "generate.h"

/*
 * cycle driven graph generator
 */


/*
        Title:  Cycle-Driven Graph Generator (1st Version - June 1995)
	file:	cycle.c
        Does:   Generates quasi-random k-colorable graphs by hiding
		numerous simple cycles in the graph.

        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Adam Beacham, June 1995
		with some code taken from Denis Papp
        email:  abeacham@cs.ualberta.ca
		dpapp@cs.ualberta.ca

        Compilation: C Code

        Inputs: <Parameters to gen_cycle_graph() >
		graph - the graph generated
		order - # of vertices
		edges - # of edges
		colors - array of vertex colors
		maxattempt - # of tries to make to create a cycle

		<prompted from stdin >
		number (num) and size of cycles.

        Outputs: Returns a graph in graph.

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/



/* #define DEBUGCYCLE */
/* #define DEBUGCYCLEQ */


/*
 * swap
 *
 * swaps array[i] and array[j]
 *
 */
static void swap(int array[], int i, int j)
{
	int t;

	t=array[i];
	array[i]=array[j];
	array[j]=t;
}





/*
 * cycle
 *
 * Generates a cycle of length size in graph.
 *
 * The queue has the following parts:
 *	[0,first-1] : already used vertices
 *	[first, last-1] : vertices that might be chosen
 *	[last,order) : the "wait queue"; vertices that could
 *			not be used this time (due to color), but
 *			not completely rejected.
 *
 * RETURNS:  The graph in graph
 *           Number of edges in edges
 *           Return value is 1 if cycle could be created, 0 otherwise
 *
 */
int cycle(graphtype graph, int size, int order, 
		int color[MAX_NR_VERTICES], long *edges)
{
	int queue[MAX_NR_VERTICES];	/* Queue of vertices */
	int first=0, last=order;
	int fvert, pvert;	/* First vertex, previous vertex */
	int v;			/* Current vertex */
	int fcol, pcol;		/* Color of 1st vertex, color of prev */
	int i,j;
	#ifdef DEBUGCYCLEQ
	 int k;
	#endif


	/* Initialize queue */
	for( i=0; i<order; i++ ) queue[i]=i;

	/* Randomize first vertex */
	fvert = queue[ rand2(first,last) ];
	fcol = color[fvert];
	pcol = fcol;	/* Previous color for 2nd vertex is fcol */
	pvert = fvert;	/* Prev vert for 2nd vertex is 1st vertex */

	/* Remove this vertex from queue */
	swap(queue,fvert,first);
	first++;


	/* Initializations to make compiler happy:  */
	v = 0;
	j = 0;

	for( i=2; i<=size; i++ ) {
		/* reset the wait queue */
		last = order;

		while( first < last ) {
			j = rand2(first,last);
			#ifdef DEBUGCYCLEQ
			 for( k =0; k<order; k++ ) printf("%d ",queue[k]);
			 printf("\nfirst = %d, last = %d, j = %d\n",
					first, last, j);
		        #endif
			v = queue[ j ];
			/* Check for color conflicts.  On last vertex, */
			/* ( i = size ), color of 1st vertex checked as well */
			if( color[v] == pcol || 
			    (i == size && color[v]==fcol) ) {
				/* Put v in wait queue */
				last--;
				swap(queue,j,last);
			} else {
				break;
			}
		}

		if( first >= last ) {
			/*fprintf(stderr,"Unable to create cycle\n");*/
			/* Unable to create cycle.  Return error */
			return 0;
		}


		/* Remove v from the queue */
		swap(queue,first,j);
		first++;
		pvert=v;
		pcol=color[v];
	}


	#ifdef DEBUGCYCLE
	 printf("(v%d,c%d)-",queue[0],color[queue[0]]);
	#endif

	/* Put in the edges.  Note that at this point, queue contains */
	/* the cycle in elements [0..first-1]			      */
	for( i=1; i<first; i++ ) {
		#ifdef DEBUGCYCLE
		printf("(v%d,c%d)-",queue[i],color[queue[i]]);
		#endif
		/* Put in edge, if needed */
		if( get_edge(queue[i],queue[i-1],graph) == 0 ) { 
			set_edge(queue[i],queue[i-1],1,graph);
			(*edges)++;
		}
	}
	if( get_edge(queue[first-1],fvert,graph) == 0 ) {
		set_edge(queue[first-1],fvert,1,graph);
		(*edges)++;
	}

	#ifdef DEBUGCYCLE
	printf("\n");
	#endif

	return 1;	/* SUCCESS!! */
}


void gen_cycle_graph(graphtype graph, int order, long *edges, int parts,
	int colors[MAX_NR_VERTICES], int maxattempt)
{
	int i;
	#ifdef DEBUGCYCLE
	 int j
	#endif
	int num, size;
	int attempt;
	int success;

	/* require # of partitions <= to the order */
        if (parts>order)  parts = order;
	if (parts<2) {
		printf("Cannot add edges to a graph that is all one color\n");
		return;
	}


	printf("Enter (# size) pair for cycles to generate, terminate with 0 0.\n");
	do {
          scanf("%d %d",&num,&size);
          printf("%d %d\n",num,size);
	  /* Disallow cycles that are too big or small, as well as */
	  /* odd cycles in a 2-colorable graph.			   */
          if (size>2 && size<=order && !(size % 2 == 1 && parts == 2) ) {
            for (i=0;i<num;i++)  {
	      /* Make several attempts until success  */
	      success=0;
	      for( attempt=maxattempt; 
		   attempt>0 && !(success=cycle(graph,size,order,colors,edges));
		   attempt-- )
			; 	/* empty */
	      if( !success ) {
			printf("Unable to create cycle\n");
	      }
              #ifdef DEBUG
              printf("D%d: ",i);
              for (j=0;j<order;j++) printf("%d ",degree[j]);
              printf("\n");
              #endif
            }
          } else if( size != 0 && num != 0 ) { 
              printf("Illegal cycle size\n");
	  }
        } while (num>0 || size>0);

	#ifdef DEBUGCYCLE
	 for( i=0; i<order-1; i++ )
		for( j=i; j<order; j++ )
			if( get_edge(i,j,graph) && colors[i]==colors[j] ) {
				fprintf(stderr,"ERROR!!!!!\n");
				exit(1);
			}
	#endif 

}

