/*
	Title:	clique driven graph generator
	file:	clique.c
	Does:	inputs the user for an amount of variously-sized cliques
		and places those cliques randomly in the graph.  A fair
		number of cliques is required since multiple cliques
		may share multiple vertices and there is no guarantee
		all vertices will be chosen
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Denis Papp
	email:	dpapp@cs.ualberta.ca

	Compilation: C Code

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include "generate.h"

/*#define DEBUG */  

#ifdef DEBUG
int degree[MAX_NR_VERTICES];
#endif


/* clique()
 * DESC: clique generator.  Increments in the given edges parameter
 * GIVEN: the graph, size of the clique, permutation of vertices,
 *        list of first vertex of each color class, order, 
 *        number of partitions, pointer to number of edges
 */
void clique(graph,size,perm,firstv,order,parts,edges,size_weighted)
graphtype graph;
int size,*firstv,order,parts;
int perm[MAX_NR_VERTICES];
long *edges;
int size_weighted;
{
	int i,j,k,t;
	int maxweight;
	int chosen[parts];
	int class[parts]; 

	for (i=0;i<parts;i++) class[i] = i;

	if (!size_weighted) {
		/* choose classes */
		for (i=0;i<size;i++) {
			/* choose ith class */
			j = rand2(i,parts);
			/* swap */
			t = class[j];
			class[j] = class[i];
			class[i] = t;
		}
	} else {
		maxweight = order;
		for (i=0; i<size; i++) {
			/* choose classes - weight by class size */
			j = rand(maxweight);
			for (k=i;k<parts;k++) {
				j-=firstv[class[k]+1]-firstv[class[k]];
				if (j<0) break;
			}
			/* swap */
			t = class[k];
			class[k] = class[i];
			class[i] = t;
			maxweight -= firstv[class[i]+1] - firstv[class[i]];
		}
	}
			
	#ifdef DEBUG
	printf("CHC\n");
	for (i=0;i<size;i++) printf("%d ",class[i]);
	printf("\n");
	#endif

	/* choose vertices */
	for (i=0;i<size;i++) 
		chosen[i] = perm[rand2(firstv[class[i]],firstv[class[i]+1])];

	for (i=1;i<size;i++)
	for (j=0;j<i;j++) {
		if (!get_edge(chosen[i],chosen[j],graph)) {
			#ifdef DEBUG
			degree[chosen[i]]++;
			degree[chosen[j]]++;
			#endif
			set_edge(chosen[i],chosen[j],1,graph);
			(*edges)++;
		}
	}
}

/* gen_clique_graph()
 * DESC: generator clique driven graph, places #edges in given edges param
 * GIVEN: graph, order, pointer to edges, number of partitions, vertex
 *        coloring
 */
void gen_clique_graph(graph,order,edges,parts,colors,size_weighted) 
graphtype graph;
int order;
long *edges;
int parts,size_weighted;
int colors[MAX_NR_VERTICES];
{
	int perm[MAX_NR_VERTICES];
	int *firstv;
	int i,j;
	int num,size;

	#ifdef DEBUG
	for (i=0;i<order;i++) degree[i]=0;
	#endif

	/* require that the # of partitions is <= to the order */
	if (parts>order)  parts = order;

	firstv = (int *) calloc((unsigned)parts+1,sizeof(int));

	/* get a list of vertices sorted by color */
	for (i=0;i<order;i++) {
		num = colors[i];
		/* shift over all vertexes */
		/* bcopy wasnt working well */
		for (j=order-1; j>firstv[num]; j--)
			perm[j] = perm[j-1];
		perm[firstv[num]] = i;
		/* shift over the other classes starting places */
		for (j=num+1;j<=parts;j++) firstv[j]++;
	}

	#ifdef DEBUG
	for (i=0;i<=order;i++) printf("%d ",colors[perm[i]]);
	printf("\n");
	for (i=0;i<=parts;i++) printf("%d ",firstv[i]);
	printf("\n");
	#endif

	printf( "Enter (# size) pair for cliques to generate, terminate with 0,0.\n");
	do {
	  scanf("%d %d",&num,&size);
	  printf("%d %d",num,size);
	  if (size>1 && size<=parts) {
	    for (i=0;i<num;i++)  {
	      clique(graph,size,perm,firstv,order,parts,edges,0);
            }
	    printf(" edges:%ld\n",*edges);
          } else if (size>parts) printf("Illegal clique size\n");
	} while (num>0 || size>0);

	printf("Assigned %ld edges\n",*edges);

	#ifdef DEBUG
	for (i=0;i<parts;i++) {
		num=0;
		for (j=firstv[i];j<firstv[i+1];j++) num+=degree[j];
		printf("CLASS:%d  TOTAL:%d  AVGDEG:%1.4f\n",i,num,
		    (float)num/(float)(firstv[i+1]-firstv[i]) );
	}
	#endif

	free((char *)firstv);
}


