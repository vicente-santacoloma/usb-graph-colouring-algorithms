/*
	Title:	Flat Graph Generator (Improved version - June 1994, April, 1995)
	file:	flat.c
	Does:	Generates quasi-random k-colorable graphs that may be used
		to test coloring algorithms. These may or may not be harder
		than equi-partite graphs, but for values near the ridge
		they do give iterated greedy a rougher time.
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html
	Author: Joseph Culberson
		Integrated with graph generator by Denis Papp May, 1995		
	email:	joe@cs.ualberta.ca
		dpapp@cs.ualberta.ca
	
	Compilation: C Code

	Inputs: various parameters:
			the graph,
			size (# vertices),
			edge probability (0.0 - 1.0),
			flatness (0 = max difficulty (??) ),
			pointer to long to contain # edges assigned

	Outputs: the generated flat graph

	Copyright: Joseph Culberson. Permission is granted to
		 use and distribute freely for non-commercial purposes.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of:  The k-colorable graph generator program.
*/
#include "generate.h"
#include <stdio.h>

void assign1(graph,vset,size,nedges,b1,b2,large,part,flat)
graphtype graph;
int *vset,size,nedges,b1,b2,large,part;
int flat;
{

	int nv, npairs,f1,f2,l1,l2;
	int i,j,v,w,vp;
	int *psetx, *psety;

	int deg1,deg2;
	int *dg1, *dg2;

	nv = (int) (size/part);

	/* compute first and last vertices of block b1 */
	if (b1 < large) 
		f1 = (nv+1) * b1;
	else 
		f1 = (nv+1)*large + (b1-large)*nv;

	if ((b1+1) < large)
		l1 = (nv+1)*(b1+1);
	else
		l1 = (nv+1)*large +((b1+1-large)*nv);

	/* compute first and last vertices of block b2 */
	if (b2 < large) 
		f2 = (nv+1) * b2;
	else 
		f2 = (nv+1)*large + (b2-large)*nv;

	if ((b2+1) < large)
		l2 = (nv+1)*(b2+1);
	else
		l2 = (nv+1)*large +((b2+1 -large)*nv);

	npairs = (l1-f1)*(l2-f2);
	psetx = (int *) calloc(npairs, sizeof(int));
	psety = (int *) calloc(npairs, sizeof(int));
	dg1 = (int *) calloc(l1,sizeof(int));
	dg2 = (int *) calloc(l2,sizeof(int));

	deg1 = nedges/(l1-f1);
	if ( (deg1*(l1-f1)) < nedges ) deg1++;
	deg1 += flat;

	deg2 = nedges/(l2-f2);
	if ( (deg2*(l2-f2)) < nedges ) deg2++;
	deg2 += flat;

	for(i=f1;i<l1;i++) dg1[i] =0;
	for(i=f2;i<l2;i++) dg2[i] =0;

	for(i=f1;i<l1;i++) for(j=f2;j<l2;j++) {
		psetx[(i-f1)*(l2-f2)+(j-f2)] = i;
		psety[(i-f1)*(l2-f2)+(j-f2)] = j;
	}

	for(i=0;i<nedges; ){
		vp = rand(npairs);
		v = psetx[vp];
		w = psety[vp];
		if ( (dg1[v] <= deg1) && (dg2[w] <= deg2) ) {
			set_edge(vset[v],vset[w],1,graph);
			i++;
			dg1[v]++;
			dg2[w]++;
		}
		/* reduce the set of pairs */
		npairs--;
		if (npairs <= 0) {
			if ( i < nedges) {
				printf("ERROR: Cannot allocate edges\n");
				exit(10);
			}
		} else {
			psetx[vp] = psetx[npairs];
			psety[vp] = psety[npairs];
		}
	}

	free((char *)psetx);
	free((char *)psety);
	free((char *)dg1);
	free((char *)dg2);
	
}

void create_flat_graph(graph,size,prob,part,flat,edges)
graphtype graph;
int size,part,flat;
double prob;
long *edges;
{
	int large,nvert,npairs,nedges,residue;
	int i,j;
	int vset[MAX_NR_VERTICES];
	int first,last;
	int nv = (int) (size/part);

	create_and_shuffle(vset,size);

	large = size % part; /* for 0<= i< large, each partition element 
				has ceil(size/part) vertices,
				and large <=i<part has floor(size/part) */

	/*************************************/
	/* assign edges between large blocks */
	/*************************************/

   if (large > 1) {
	/* number of vertices in large block */
	nvert = 1+ (int) (size/part);

	/* number of pairs of blocks */
	npairs = (large*(large-1))/2;

	/* Expected number of edges assigned between large pairs */
	nedges = (int) ( npairs *nvert *nvert *prob);

	/* to distribute, need to know how many take ceiling */
	residue = nedges % npairs;
	nedges = nedges / npairs;

	for(i=0;i<large;i++) for(j=i+1;j<large;j++) {
		if ( rand(npairs) < residue){
		  assign1(graph,vset,size,nedges+1,i,j,large,part,flat);
		  npairs--;
		  residue--;
		  *edges += nedges+1;
		} else {
		  assign1(graph,vset,size,nedges,i,j,large,part,flat);
		  npairs--;
		  *edges += nedges;
		}
	}
   }
   if (large > 0) {
		
	/*************************************/
	/*assign edges large and small blocks */
	/*************************************/

	/* number of vertices in large block */
	nvert = (int) (size/part);

	/* number of pairs of blocks */
	npairs = large*(part-large);

	/* Expected number of edges assigned between all pairs */
	nedges = (int) ( npairs *nvert *(nvert+1) *prob);

	/* to distribute, need to know how many take ceiling */
	residue = nedges % npairs;

	/* and how many per pair (floor) */
	nedges = nedges / npairs;

	for(i=0;i<large;i++) for(j=large;j<part;j++) {
		if ( rand(npairs) < residue){
		  assign1(graph,vset,size,nedges+1,i,j,large,part,flat);
		  npairs--;
		  residue--;
		  *edges += nedges+1;
		} else {
		  assign1(graph,vset,size,nedges,i,j,large,part,flat);
		  npairs--;
		  *edges += nedges;
		}
	}
   }

   if ((part > 1) && (large<(part-1))) {
	/*************************************/
	/* assign edges between small blocks */
	/*************************************/

	/* number of vertices in large block */
	nvert = (int) (size/part);

	/* number of pairs of blocks */
	npairs = ((part-large)*(part-1-large))/2;

	/* Expected number of edges assigned between all pairs */
	nedges = (int) ( npairs *nvert *nvert *prob);

	/* to distribute, need to know how many take ceiling */
	residue = nedges % npairs;

	/* and how many per pair (floor) */
	nedges = nedges / npairs;

	for(i=large;i<part-1;i++) for(j=i+1;j<part;j++) {
		if ( rand(npairs) < residue){
		  assign1(graph,vset,size,nedges+1,i,j,large,part,flat);
		  npairs--;
		  residue--;
		  *edges += nedges+1;
		} else {
		  assign1(graph,vset,size,nedges,i,j,large,part,flat);
		  npairs--;
		  *edges += nedges;
		}
	}
  }
  /* set up partset for verification */
  partitionflag = 1;
  /* Force applying greedy in vset order for verification */
  for (i=0; i<part; i++) {
     if( (i) < large )
	first = (nv+1)*(i);
     else
	first = (nv+1)*large +((i-large)*nv);
     if( (i+1) < large )
	last = (nv+1)*(i+1);
     else
	last = (nv+1)*large +((i+1-large)*nv);
     for( j=first; j<last; j++ )
	partset[vset[j]] = i;
  }
}

