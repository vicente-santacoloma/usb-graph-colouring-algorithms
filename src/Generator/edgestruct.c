#include "generate.h"
#include<stdio.h>
#include<malloc.h>
#include<math.h>

void update(weight,node,delta)
/*
	Change the weight of node by delta
	and propagate the change upwards through the tree
	updating the total weight also.
*/
int weight[];
int node;
int delta;
{
	int parent;

	weight[node] += delta;
	while (node > 1) {
		parent = node/2;
		if (parent*2 == node) {
			weight[parent] += delta;
		}
		node = parent;
	}
	weight[0] += delta;
}

void initweights(int weight[], int maxnode, int delta)
/*
	initialize the weight set to delta each and the total weight 
	NOTE: should be possible to initialize in O(n) time?
	This takes O(nlogn)
*/
{
	int i;
	weight[0] = 0;
	for (i=1; i<= maxnode; i++){
		weight[i] = 0;
		update(weight,i,delta);
	}
}

int weightedSelect(weight,maxnode)
/*
	select a node at random using proportional weights
*/
int weight[];
int maxnode;
{
	int leftnode, rightnode, searchwt;
	searchwt = (int)(dblrand() * weight[0]); /* searchwt should be passed?*/

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


int actualWeight(int weight[], int node, int maxnode)
/*
	Get the actual weight of the node. Recall that weight[node]
	includes the total of the (inclusive) left subtree.
	Thus, we must subtract the weights on the predecessor path.
*/
{
	int pnode;
	int actwt;
	
	actwt = weight[node];
	pnode = 2* node;
	while (pnode <= maxnode) {
		actwt -= weight[pnode];
		pnode = 2*pnode+1;
	}
	return(actwt);
}


int index(int i, int j)
{
	if (j>i ) {
		return((j*(j+1)/2)-i);
	} else if (j==i) {
		printf("ERROR IN INDEX\n");
		exit();
	} else {
		return((i*(i+1)/2)-j);
	}
}

void deindex(int k, int *i, int*j)
{
	int t;
	*j = (int) sqrt( (double) 2*k);
	t = ((*j) * ((*j)+1))/2;
	if (t < k){
		t += (++(*j));
	}
	*i = t -k;
}

void main(){
	/* test */
	int i,j,ir,jr,order, node,maxnode;

	int *weight,delta;
	int nedges;
	int seed;
	int degree[1000];
	int maxdeg, mindeg;


	printf("Seed ");
	scanf("%d",&seed);
	xsrandom(seed);

	printf("Enter number of nodes ");
	scanf("%d",&order);
	maxnode = order*(order-1)/2;

	printf("Initial weight ");
	scanf("%d",&delta);


	weight = (int *) calloc(maxnode+1,sizeof(int));
	if (weight==NULL) {
		printf("Error not allocated\n");
        }

	initweights(weight,maxnode,delta);
	printf("total weight = %d\n",weight[0]);

	for(i=0;i<order;i++) degree[i] =0;

	nedges = 0;
	while(weight[0] >0) {
		nedges++;
		node = weightedSelect(weight,maxnode);
		delta = actualWeight(weight,node,maxnode);
		deindex(node,&ir,&jr);
		/*printf("node = %d edge=(%d,%d) wt=%d\n",node,ir,jr,delta);*/
		degree[ir]++;
		degree[jr]++;
	
		update(weight,node,-delta);

		/*printf("**total weight =%d\n",weight[0]); */

		for(i=0;i<order;i++) {
		     if (i != ir) {
			node = index(i,ir);
			delta = actualWeight(weight,node,maxnode);
			delta = delta - ((int) (delta*0.5));
			if (delta >0) delta = 1; 
			update(weight,node,-delta);
			/* printf("ir-loop**total weight =%d\n",weight[0]); */
		    }
		}
		/*printf("\n"); */
		for(i=0;i<order;i++) {
		     if (i != jr) {
			node = index(i,jr);
			delta = actualWeight(weight,node,maxnode);
			delta = delta - ((int) (delta*0.5));
			if (delta >0) delta = 1;
			update(weight,node,-delta);
			/*printf("jr-loop**total weight =%d\n",weight[0]); */
		    }
		}
	}

	maxdeg = 0; mindeg = order;
	for(i=0;i<order;i++) {
		if (degree[i] > maxdeg) maxdeg = degree[i];
		if (degree[i] < mindeg) mindeg = degree[i];
	}
	printf("\nNumber of edges = %d\n",nedges);
	printf("maxdeg = %d,  mindeg = %d\n",maxdeg,mindeg);
}
