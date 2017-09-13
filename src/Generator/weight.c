#include<stdio.h>
#include<math.h>
#include "generate.h"


/*
        Title:  Weight-Biased Graph Generator (Improved version - May 1995)
	file:	weight.c
        Does:   Generates quasi-random k-colorable graphs that may be used
                to test coloring algorithms.  Edges are given an initial
		weight, and weights are changed as edges are added to the
		graph.
        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Joseph Culberson
		additions by Adam Beacham, May 1995
        email:  joe@cs.ualberta.ca
		abeacham@cs.ualberta.ca

        Compilation: C Code

	Inputs:	Parameters to weighted_graph():
			graph (graph generated)
			blocker (blocking graph; independent sets in graph
				 are cliques in blocker)
			order (# of vertices in graph & blocker)
			prob  (edge probability)
			initialw (initial edge weight)
			add ( 1 - add alpha; 0 - multiply weight by alpha)
			alpha (variability parameter)
			add2 ( 0 - multiply weight by gamma; 1 - add gamma)
			gamma (second variability parameter)
			nedges (number of edges)
        Outputs:  Returned in parameters passed:
			graph - the graph generated
			nedges - number of edges placed in graph

        Copyright: Joseph Culberson. Permission is granted to
                 use and distribute freely for non-commercial purposes	
		 with appropriate attribution.
        Warranty: NONE!
        Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/


/*
 * NOTE:
 *
 * Each edge pair is stored in a node in a heaplike structure.  (The array
 * weight, in most cases).
 *
 * Thus, all references to a "node" in this file refers to an edge pair,
 * not a vertex in the graph.
 *
 */

void update(long weight[],long node,long delta);
void initweights(graphtype block,long weight[], long maxnode, long delta);
long weightedSelect(long weight[],long maxnode);
long actualWeight(long weight[], long node, long maxnode);
long index(int i, int j);
void deindex(long k, int *i, int*j);
long getdelta(long weight[], long node, long maxnode, char add, double delta);


/*
	Change the weight of node by delta
	and propagate the change upwards through the tree
	updating the total weight also.
*/
void update(weight,node,delta)
long weight[];
long node;
long delta;
{
	long parent;
	/* Debugging stuff */
	/* printf("Update1:  Node %ld + %ld\n",node,delta); */

	weight[node] += delta;
	while (node > 1) {
		parent = node/2;
		if (parent*2 == node) {
			weight[parent] += delta;
			/* Debugging stuff */
			/*printf("Update2:  Node %ld + %ld\n",parent,delta);*/
		}
		node = parent;
	}
	weight[0] += delta;
}

/*
	initialize the weight set to delta each and the total weight 
	NOTE: should be possible to initialize in O(n) time?
	This takes O(nlogn)
*/
void initweights(graphtype block,long weight[], long maxnode, long delta)
{
	long i;
	int k,l;
	weight[0] = 0;
	for (i=1; i<= maxnode; i++){
		weight[i] = 0;
		deindex(i,&k,&l);
		if( get_edge(k,l,block) == 0 ) {
			/* edge not blocked; give nonzero weight */
			update(weight,i,delta);
		}
	}
}

/*
	select a node at random using proportional weights
*/
long weightedSelect(weight,maxnode)
long weight[];
long maxnode;
{
	long leftnode, rightnode, searchwt;
	searchwt = rand(weight[0]); /* searchwt should be passed?*/

	/* The following assignment is to remove a compiler warning */
	rightnode = 0;

	leftnode = 1;
	while (1) { 
		/* we exit on return statement when leftnode exits tree*/
		/* required cell lies between (or =) the leftmost node of 
		   subtree rooted at leftnode and rightnode if set */
		if (searchwt >= weight[leftnode]) {
			searchwt -= weight[leftnode];
			leftnode = (2*leftnode)+1;
		}
		else {
			rightnode = leftnode;
			leftnode = 2*leftnode;
		}
		/* since searchwt < total weight in weight[0],
		   rightnode must have been set
		   at least once before following occurs
		*/
		if (leftnode > maxnode) return(rightnode);
	}
}


/*
	Get the actual weight of the node. Recall that weight[node]
	includes the total of the (inclusive) left subtree.
	Thus, we must subtract the weights on the predecessor path.
*/
long actualWeight(long weight[], long node, long maxnode)
{
	long pnode;
	long actwt;
	
	actwt = weight[node];
	pnode = 2* node;
	while (pnode <= maxnode) {
		actwt -= weight[pnode];
		pnode = 2*pnode+1;
	}
	return(actwt);
}


/* Compute the edge (node), given the vertices i and j */
long index(int i, int j)
{
	if (j>i ) {
		return((j*(j+1)/2)-i);
	} else if (j==i) {
		printf("ERROR IN INDEX\n");
		exit(1);
	} else {
		return((i*(i+1)/2)-j);
	}
}

/* Compute the vertices, given the edge (node) index k */
void deindex(long k, int *i, int*j)
{
	long t;
	*j = (int) sqrt( (double) 2*k);
	t = ((*j) * ((*j)+1))/2;
	if (t < k){
		t += (++(*j));
	}
	*i = t -k;
}


/*
 * getdelta
 *
 * Given a node and the parameters for altering weights (add, delta),
 * computes and returns the value by which the weight of the node will be
 * adjusted.
 *
 * add = 1 --> change weight by adding delta.
 * add = 0 --> change weight by multiplying by delta
 *
 * This function ensures that edges with 0 weight (ie: they have been
 * removed from consideration) will not have their weights changed.  It also
 * prevents negative weights and weights that are too large.
 *
 */
long getdelta(long weight[], long node, long maxnode, char add, double delta)
{
	long actw, neww;	/* Actual and new weights */
	long maxwt;

	/* Compute the current weight of the node */
	actw = actualWeight(weight,node,maxnode);

	/* If edge already removed, do not change its weight. */
	/* If user makes weight positive, it could be selected again */
	/* which could lead to an infinite loop */
	if( actw == 0 ) {
		return 0;
	}

	/* Max possible weight is LONG_MAX  / (order CHOOSE 2) */
	maxwt = (LONG_MAX) / maxnode;

	/* Make sure that modifying this weight by delta will not cause */
	/* it to exceed the maxwt.  If it would, do not modify */
	if( add ) {	/* we are adding to modify */
		if( delta > 0 && maxwt-(long)delta < actw ) {
			/* Change this to neww = maxwt? */
			return 0;
		}
	} else {	/* we are multiplying */
		if( delta != 0 && maxwt / delta < actw ) {
			/* Change this to neww = maxwt? */
			return 0;
		}
	}

	/* Compute new weight */
	if( add ) {
		neww = (long) (actw + delta);
	} else {
		neww = (long) (actw * delta);
	}

	/* Do not allow weights to drop below 0 */
	if( neww < 0 ) { neww = 0; }

	/* Debugging stuff */
	/* printf("node %ld + %ld\n",node,neww-actw); */

	return (neww - actw);
}

#ifdef DEBUG
/* Debugging function.  Prints the heap that stores the weights */
void weighttree(long weight[], long maxnode)
{
	long i;
	long j=1;

	for( i=1; i<=maxnode; i++ ) {
		printf("%ld ",weight[i]);
		if( i==j ) {
			printf("\n");
			j = (j*2) +1;
		}
	}
	printf("\n");
}
#endif

/*
 * weighted_graph
 *
 * Produces a "weighted" graph.  When an edge is added, the weights of
 * nearby edges are altered.  Edges are randomly chosen with a probability
 * proportional to their weights.  The two "deltas" (alpha and gamma) are
 * applied to different incident edges.  Alpha only applies to incident
 * edges that would create triangles.  The gamma change applies to all
 * incident edges.
 *
 * If add = 1, the new weight is found by adding delta to the old weight.
 * If add = 0, new weight found by multiplying by delta.
 *
 * Weights are stored as longs.  Any fraction is truncated.  Weights
 * are not allowed to drop below zero.  The deltas (alpha & gamma) 
 * are doubles to make multiplication meaningful.
 *
 * graph - graph that is being created.
 * blocker - corresponding graph with all vertices of the same color
 *		connected by edges.  (Assumed that it has been set up this
 *		way by the caller).
 * order - number of vertices.
 * prob - edge probability.  No more than (prob*(max possible edges))
 *		edges will be put in the graph.
 * nedges - number of edges. 
 *
 */
void weighted_graph(graphtype graph, graphtype blocker,
                int order, double prob,int initialw,char add,
                double alpha, char add2, double gamma,long *nedges)
{
	int i,ir,jr;
	long node, maxnode;
	long del;		/* delta change in a weight */
	long *weight;
	long maxedge;		/* Max # of edges to add */
	maxnode = order*(order-1)/2;

	#ifdef DEBUG
	char c;
	#endif

	weight = (long *) calloc(maxnode+1,sizeof(long));
	if (weight==NULL) {
		fprintf(stderr,"Error: Weight not allocated\n");
		exit(1);
        }

	/* Edges between vertices of same color will have weight 0 */
	/* All others have weight initialw			   */
	initweights(blocker,weight,maxnode,initialw);
	printf("total weight = %ld\n",weight[0]);

	/* Calculate number of expected edges to add.  The total */
	/* number of possible edges is weight[0] / initialw	 */
	maxedge = (initialw==0) ? 0 : (long) ((weight[0] / initialw) * prob);
	printf("expected number of edges = %ld\n",maxedge);

	*nedges = 0;
	/* Loop until all edges gone or reached max # of edges */
	while(weight[0] >0 && *nedges < maxedge ) {
		/* Debugging stuff */
		if( *nedges % 1000 == 0 ) {
			/* Progress report */
			fprintf(stderr,".");
		}

		#ifdef DEBUG
		/* Debugging stuff */
		if( order <= 10 ) {
			printf("-------------------------\n");
			printf("Weights:  ");
			for( node=1; node<=maxnode; node++) {
				printf("%5ld",actualWeight(weight,node,maxnode));
			}
			printf("\nTotal Weight:  %ld\n",weight[0]);

			weighttree(weight,maxnode);
			scanf("%c",&c);
		} 
		#endif

	
		/* Select a new edge at random  */	
		(*nedges)++;
		node = weightedSelect(weight,maxnode);

		#ifdef DEBUG
		/* Debugging */
		if( actualWeight(weight,node,maxnode) <=0 ) {
			printf("Error:  Weight of selected node <=0\n");
			exit(1);
		}
		#endif

		/* Remove this edge from contention; don't pick it again */
		del = actualWeight(weight,node,maxnode);
		deindex(node,&ir,&jr);
		update(weight,node,-del);

		/* set the edge */
		set_edge(ir,jr,1,graph);

		/* Debugging */
		/*printf("Added edge (%d, %d)\n",ir,jr);*/

		/* Change weights on nearby "clique" edges */
		if( (add==1 && alpha != 0) || (add==0 && alpha != 1) ) {
			/* User wishes to modify clique edges; do so */
			for(i=0;i<order;i++) {
			     if( i != ir && i != jr && get_edge(i,ir,graph) ) { 
				/* Alter weight on edge (i,jr) */
				node = index(i,jr);
				/* Compute weight change */
				del=getdelta(weight,node,maxnode,add,alpha);
				if( del != 0 ) {
					update(weight,node,del);
				}
			    }
			}

			for(i=0;i<order;i++) {
			     if (i != jr && i != ir && get_edge(i,jr,graph) ) {
				/* Alter weight of edge (i, ir) */
				node = index(i,ir);
				/* Compute weight change */
				del=getdelta(weight,node,maxnode,add,alpha);
				if( del != 0 ) {
					update(weight,node,del);
				}
			     }
			}
		}

		/* change weights on incident edges, if required. */
		if( (add2==1 && gamma!=0) || (add2==0 && gamma!=1)) {
			/* User wishes to change incident edges */
			for(i=0;i<order;i++) {
			     if( i != ir && i != jr ) { 
				/* Alter weight on edge (i,ir) */
				node = index(i,ir);
				del=getdelta(weight,node,maxnode,add2,
					gamma);
				if( del != 0 ) {
					update(weight,node,del);
				}
			    }
			}

			for(i=0;i<order;i++) {
			     if (i != jr && i != ir ) {
				/* Alter weight of edge (i, jr) */
				node = index(i,jr);
				del=getdelta(weight,node,maxnode,add2,
					gamma);
				if( del != 0 ) {
					update(weight,node,del);
				}
			     }
			}
		}

		
	}

	printf("\nNumber of edges = %ld\n",*nedges);
}
