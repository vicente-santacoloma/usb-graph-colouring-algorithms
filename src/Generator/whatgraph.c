/*
	Title:	Core of graph generator
	file:	whatgraph.c
	Does:	all the menus for inputting the graph generation options
		are here.  The parameter inputting is done here as well.
		Then the actual graph generating functions are called.
		And finally the graph is written to file.
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Original by Joseph Culberson
		Modified by Denis Papp and Adam Beacham in May, 1995
	email:	joe@cs.ualberta.ca
		dpapp@cs.ualberta.ca
		abeacham@cs.ualberta.ca

	Compilation: C Code

	Inputs:  requires the filename for the output

	Outputs: saves the graph to the specified file

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include "generate.h"
#include <string.h>

void whatgraph_others(char *file);

int binary;	/* Flag.  0 if output is ASCII, 1 if output binary */

int seed;	/* Randomizer seed */
double prob;	/* Edge probability */
long size;	/* Number of edges in graph */
int order;	/* Number of vertices */
double variab;	/* Variability.  Used in some k-colorings */
int numpart;	/* Number of color partitions (ie: # of colors hidden */
int flat;	/* Flatness (for flatgraphs) */

/* Variables used in the output file */
int cheatflag;		/* == 1 if a cheat is to be included in the output file */
char *cheat;		/* The cheat info string */
char parm[80];		/* String containing edge and order info */
char specs[500];	/* Specification string */
char verify[300];	/* Verification string */
char temp[150];		/* Temporary string; various uses */

graphtype graph;	/* The graph itself */
graphtype block;	/* The blocking graph */

/*
	Ask the user for the specifications of the graph wanted 

	file is the output file.
*/
void whatgraph(file)
char *file;
{
	/* initialize specs */
	strcpy(specs,"c Specifications: \n");

	cleargraph(graph);

	getformat(&binary);
	seed = initrandom();
	/* specs */
	sprintf(temp,"c   Random seed: %d\n",seed);
	strcat(specs,temp);

	partitionflag = 0;
	size = 0;

	/* Call the k-colorable graph stuff */
	whatgraph_others( file);

#ifdef DEBUGPRINT
	printgraph();
#endif

	/* Generate edge info string */
	sprintf(parm,"p edge %d %ld\n",order,size);

	/* check if cheat is desired */
	cheatflag = 0;
	if (partitionflag) {
		verify_color(graph, order, numpart, verify);
		/* input dependent question - 
		 * "Do you want the cheat?" only appears if
		 * partitionflag is on
		 */
		cheatflag = getcheat(partset,order,&cheat);	
	} else {
		sprintf(verify, "c No verification required.\n");
	}


	/* now get the vertex degree specs */
	printf("Getting degree statistics\n");
	get_degree_info(graph,order,specs);

	/* if cheat string was not allocated, do so now */
	if (!cheatflag) {
		cheat = malloc(20);
		strcpy(cheat,"c no cheat\n");
	}

	if (binary) 
		write_graph_DIMACS_bin(file, order, specs,
		verify,parm,cheat,graph);
	else
		write_graph_DIMACS_ascii(file, order, specs,
		verify,parm,cheat,graph);

	/* free up memory allocated for cheat text */
	free(cheat);

	printf("Done.\n");
}	


/*
 * whatgraph_others( file )
 *
 * Generates different types of k-colorable graphs in a variety of ways.
 *
 * The blocking graph, block, is used to indicate which edges are not
 * permitted in the graph being generated.  After initialization with
 * a k-colorable scheme, there will be an edge (i,j) in block iff i and j
 * are in the same color partition.
 *
 * file is passed to gen_evacuate_graph if that is used.
 *
 */
void whatgraph_others(char *file)
{
	int perm[MAXVERTEX];
	int what;
	int initialw;
	int add, add2;		/* used in weight biased graphs */
	double alpha,gamma;	/* used in weight biased graphs */
	/* degree control variables */
	int girth,degree_delta,delta_max;
	/* Cycle-driven stuff:  */
	int maxattempt;
	/* Clique-driven */
	int size_weighted;

	printf("Which k-colorable graph?\n");
	printf("K-coloring schemes:\n\
\t1 No hidden coloring\n\
\t2 Equi-partitioned\n\
\t3 k-colorable\n\
\t4 k-colorable(smooth)\n\
\t5 k-colorable with delta variation\n\
Alternate graph:\n\
\t6 Flatgraph\n\
?");

	scanf("%d",&what);
	printf("%d\n", what);

	/* Get order (# of vertices) */
	getorder(&order);
	if ((order <2) || (order> MAXVERTEX)) {
        	printf("Illegal number of vertices .. must be 2-%d\n",
					MAXVERTEX);
		exit(1);
	}

	/* Clear the blocking graph. */
	cleargraph(block);

	/* Get number of partitions */
	if (what!=1) 
		getpart(order,&numpart);

	/* randomize vertex order */
	if (what!=6)
		create_and_shuffle(perm,order);

	switch (what) {
		case 1: 
			/* no hidden coloring */
			numpart = order+1;
			initblock(block,order,order+1,perm,0,EQUIPARTITE);
			/* build specs */
			strcat(specs,"c   No hidden coloring\n");
			break;
		case 2: 
			/* Equipartite coloring */
			initblock(block,order,numpart,perm,0,EQUIPARTITE);
			/* build specs */
			sprintf(temp,
			    "c   Equipartitioned Coloring, %d partitions\n",
			    numpart);
			strcat(specs,temp);
			break;
		case 3:
			/* k-colorable (with variability) */
			printf("Enter variability (0-%d) ", numpart-1);
			scanf("%lf",&variab);
			printf("%f\n",variab);
			initblock(block,order,numpart,perm,variab,KCOLORABLE);
			/* build specs */
			strcat(specs,"c   k-colorable, ");
			sprintf(temp,
				"%d partitions, %d variability\n",
				numpart,(int)variab);
			strcat(specs,temp);
			break;
		case 4:
			/* k-colorable (smooth) */
			printf("Enter variability (0.0-1.0) ");
			scanf("%lf",&variab);
			printf("%f\n",variab);
			initblock(block,order,numpart,perm,variab,KSMOOTH);
			/* build specs */
			strcat(specs,"c   k-colorable (smooth), ");
			sprintf(temp,
				"%d partitions, %f variability\n",
				numpart,variab);
			strcat(specs,temp);
                        break;
		case 5:
			/* k-colorable, delta variation */
                        printf("Enter delta (0-%d) ", (order/numpart)-1);
                        scanf("%lf",&variab);
                        printf("%f\n",variab);
			initblock(block,order,numpart,perm,variab,DELTA);
			/* build specs */
			strcat(specs,"c   k-colorable (delta), ");
			sprintf(temp,
				"%d partitions, %d variability\n",
				numpart,(int)variab);
			strcat(specs,temp);
			break;
		case 6:
			/* Flat graphs */
			printf("Flatgraph\n");
			getprob(&prob);
			getflatness(&flat);
			create_flat_graph(graph,order,prob,numpart,flat,&size);
			/* build specs */
			sprintf(temp,"c   Flat graph\n\
c   Probability: %f\n\
c   Partitions: %d\n\
c   Flatness: %d\n", prob, numpart,flat);
			strcat(specs,temp);
			break;	

		default: 
			printf("Not implemented\n");
			exit(1);
	}

  if (what!=6) {

	/*							  */
	/* Next level of the hierarchy, the actual graph creation */
	/*							  */

	printf("Which graph type?\n");
	printf("\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n ?",
		"1 IID (independant random edge assignment)",
		"2 Girth and Degree Inhibited",
		"3 Geometric",
		"4 Weight Biased Graph (encourages or inhibits cliques)",
		"5 Clique driven",
		"6 Cycle driven",
		"7 Evacuation Graph"
		);
	scanf("%d",&what);
	printf("%d\n", what);

	if (what!=3 && what!=5 && what!=6) {
		printf("Enter edge probability in percent (0.0-1.0) ");
		scanf("%lf",&prob);
		printf("%f\n",prob);
	
		/* specs */
		sprintf(temp,"c   Probability: %f\n",prob);
		strcat(specs,temp);
	}

	switch (what) {
		case 1:
			k_color(graph,block,order,&size,prob);
			/* specs */
			strcat(specs,"c   random IID graph\n");
			break;
		case 2:
			printf("Only Girth/Degree-limited graphs implemented\n");
			getgirth(&girth);
			printf("Enter delta for degree limiting (0-%d) \n\
low delta makes the graph flat and high delta has little effect: \n", order);
			scanf("%d",&degree_delta);
			printf("Enter max delta (%d-%d): \n", 
							degree_delta,order);
			scanf("%d",&delta_max);
			degree_control_graph(graph,block,order,prob,
				         numpart,girth,degree_delta,
					 delta_max,&size);
			sprintf(temp, "c   girth/degree-limited graph, girth %d, delta %d, max %d\n", girth,degree_delta,delta_max);
			strcat(specs,temp);
			break;
		case 3:
			/* got order, prob, binary, need size */
			strcat(specs,"c   Geometric graph\n");
			gen_and_print_geo(specs,graph,block,order,numpart,&size);
			break;
		case 4:
			/* have order, prob, block */
			strcat(specs,"c   Weighted Graph\n");
			getweightedinputs(specs,order, &initialw, &add, &alpha,
				&add2, &gamma);
			/* Generate the graph */
			weighted_graph(graph,block,order,prob,initialw,
				(char) add,alpha,(char) add2, gamma,&size);
			break;
		case 5:
			printf ("Do you wish to weight color class selection\n\
by color class size? (0-no, 1-yes) ");
			scanf("%d",&size_weighted);
			printf("%d\n",size_weighted);

			gen_clique_graph(graph,order,&size,
					  numpart,partset,size_weighted);
			/* specs */
			strcat(specs,"c   Clique driven graph\n");
			if (size_weighted) {
			      strcat(specs,
				"c   Class Selection: weighted by size\n");
			} else {
			      strcat(specs,
				"c   Class Selection: equal weight\n");
			}
			break;
		case 6:
			strcat(specs,"c   Cycle driven graph\n");
			getmaxattempt(&maxattempt);
			gen_cycle_graph(graph,order,&size,
					numpart,partset,maxattempt);
			break;
		case 7:
			strcat(specs,"c   Evacuation graph\n");
			gen_evacuate_graph(graph,block,order,prob,
					   &size,specs,file);
			break;
		default:
			printf("Not implemented\n");
			exit(1);
	}		
  }
}

