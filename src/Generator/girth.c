/*
	Title:	Girth and Degree inhibited graph generator
	file:	girth.c
	Does:	generates a graph inhibited by girth (minimum
		cycle size) and/or degree variance from the average,
		which can aid in making the graph flatter.    
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Original Girth code by Joseph Culberson 
		Degree inhibiting code added by Denis Papp in May, 1995 
	email:	joe@cs.ualberta.ca
		dpapp@cs.ualberta.ca

	Compilation: C Code

	Inputs:  several parameters describing the graph

	Outputs: generated graph

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include "generate.h"

/*#define DEBUG */ 

extern void bzero();
extern void bcopy();

/* degree_check()
 * DESC:  checks if both vertices are of acceptable degree
 * GIVEN: the degrees array, the 2 vertices, and the max degree delta
 *        from the average to permit
 * RETURNS: true if i,j are of acceptable degree
 */
int degree_check(degrees,i,j,avg,delta)
int *degrees,i,j,delta;
float avg;
{
	return !(degrees[i]>=avg+delta || degrees[j]>=avg+delta); 
}

void dfsother(blocker,graph,u,pu,v,pv,girth,size)
graphtype blocker,graph;
int u,pu,v,pv;
int girth,size;
{
	int i;
	set_edge(u,v,1,blocker);
	if (girth>1) {
		for(i=0;i<size;i++){
			if ((get_edge(i,u,graph)) && (i!=pu)) {
				dfsother(blocker,graph,i,u,v,pv,girth-1,size);
			}
		}
	}
}
	
void dfsblock(blocker,graph,u,pu,v,pv,girth,size)
graphtype blocker,graph;
int u,pu,v,pv;
int girth,size;
{
	int i;
	dfsother(blocker,graph,u,pu,v,pv,girth,size);
	if (girth>1) {
		for(i=0;i<size;i++){
			if ((get_edge(i,v,graph)) && (i!=pv)) {
				dfsblock(blocker,graph,u,pu,i,v,girth-1,size);
			}
		}
	}
	
}

void shuffle(int *array, int size)
{
	int i,j,temp;
		
	for (i=0; i<size; i++) {
		j = i + rand(size - i);
		temp = array[j];
		array[j] = array[i];
		array[i] = temp;
	}
}

/* for larger girths, e.g. > 7, this is quite inefficient. 
   Given sparseness of such graphs it might be worthwhile choosing a 
   data structure... */

/* degree_control_graph
 * DESC: generates grith and degree inhibited graph, sets edges parameter
 * GIVEN: graph, blocked edges matrix, number of vertices (size),
 *        edge probability, number of partitions, 
 *        girth (minimum cycle size)
 *        delta (max degree allowed beyond the average)
 *        delta_max (maximum amount delta can increase to)
 *        pointer to edges
 */
void degree_control_graph(
		graph,blocker,size,prob,part,girth,delta,delta_max,edges)
graphtype graph;
graphtype blocker;
int size,part,girth,delta,delta_max;
double prob;
long *edges;
{
	int i,j;
	long npair;
	int *queue;
	int qsize,wsize,initsize,next;
	int quit;

	int *degrees;
	int total;
	float avg;

	struct pairstr {
		int i,j;
	} *pair,temp;

	pair = (struct pairstr *) calloc((unsigned) 1+(size*(size-1))/2,
					(unsigned) sizeof(struct pairstr));
	/* create all possible pairs */
	npair=0;
	for(i=1;i<size;i++) for(j=0;j<i;j++){
		pair[npair].i = i;
		pair[npair].j = j;
		npair++;
	}

	/* The beginning of the queue is the wait queue */
	queue = (int *) calloc((unsigned)npair, sizeof(int));
	initsize = qsize = npair;
	next = 0;	

	/* wsize is size of wait queue, guaranteed wsize < qsize */
	wsize = 0;
	
	/* initialize queue - list of pairs waiting to be looked at */
	for (i=0;i<npair;i++)  queue[i] = i;

	/* random order */
	shuffle(queue,qsize);

	degrees = (int *) calloc((unsigned)size, sizeof(int));
	total = 0; avg = 0.0;

	/* main loop */
	quit = 0;
	while (!quit) {

		#ifdef DEBUG
		printf("Qsize: %d\n",qsize);
		for (j=0;j<qsize;j++) { 
			if (wsize==j) printf("*");
			if (next==j) printf("#");
			printf("%d ",queue[j]); 
		}
		#endif

		i = queue[next];
		temp = pair[i];
		if (get_edge(temp.i,temp.j,blocker)) {
		   /* throw away this pair (do nothing) */
		   #ifdef DEBUG
		      printf("B");
		   #endif
		} else {
		   /* check the degrees */
		   if (degree_check(degrees,temp.i,temp.j,avg,delta)) {
		      if (dblrand() < prob) {
		         /* apply girth limit to add blocking edges */
		         dfsblock(blocker,graph,temp.i,size,temp.j,size,
				girth-2,size);
		         set_edge(temp.i,temp.j,1,graph);
			 (*edges)++;
			 /* modify degrees */
			 total += 2;
			 avg = total/size;
			 degrees[temp.i]+=1;
			 degrees[temp.j]+=1;
		   	 #ifdef DEBUG
		            printf("+");
		         #endif
 		      } else {
			/* edge is not added */
			#ifdef DEBUG
			  printf("-");
		        #endif
 		      }
  		   } else {
		      /* move this edge to the wait list */
		      queue[wsize] = queue[next];
		      wsize++;
		      #ifdef DEBUG
		            printf("W");
		      #endif
 		   }
		}  
		/* now advance the list (remove the pair) */
		next++;
		/* If the queue is empty use the wait queue */
		if (next==qsize) {
		   qsize = wsize;
		   /* now reshuffle */
		   shuffle(queue,qsize);
		   next = wsize = 0;
		   /* if no change happened in that run through the
		      queue increase delta */
  		   if (qsize==initsize) {
		      if (delta>=delta_max) {
			 printf("Delta hit max.\n");
			 quit = 1;
		      } else {
		         printf("Problem: no change, increasing delta by 1\n");
		         delta++;
		      }
		   }
		   initsize = qsize;
		   if (qsize==0) quit=1;
		}
		#ifdef DEBUG
		printf("\n");
		#endif
	}
	printf("Final delta: %d   Edges not considered: %d\n",delta,qsize);

	free((char *)degrees);
	free((char *)pair);
	free((char *)queue);
}
	
