/*
	Title:	Graph generator main header file
	file:	generate.h
	Does:	included by all graph generator modules.  Has macros
		for limits, macros random functions, and several
		extern statements
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Joseph Culberson 
		Denis Papp
		Adam Beacham	
	email:	joe@cs.ualberta.ca
		dpapp@cs.ualberta.ca
		abeacham@cs.ualberta.ca

	Compilation: C Code

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

/* to avoid conflicts with library functions named rand */
#define rand ourrand

/********
  MACROS 
 ********/

/* xrandom should return a random long - it is used by rand() */
#define xrandom() ((long)lrand48())
/* xdrandom should return a random double - it is used by dblrand() */
#define xdrandom() drand48()
#define xsrandom(x) srand48(x)

/* returns a long random between k1 and k2-1 */
#define rand2(k1,k2)	(rand( (k2)-(k1) ) + (k1) )

/***********
  CONSTANTS 
 ***********/

/* If you change MAX_NR_VERTICES, change MAX_NR_VERTICESdiv8 to be
 * the 1/8th of it */

#define MAX_NR_VERTICES         5000
#define MAX_NR_VERTICESdiv8     625

#define GRAPHSIZE	MAX_NR_VERTICES*MAX_NR_VERTICESdiv8

#define MAXVERTEX	MAX_NR_VERTICES

#define MAXWEIGHT	(2*(MAXLONG / (MAX_NR_VERTICES * (MAX_NR_VERTICES-1))))

/*********
  GLOBALS
 *********/

extern int partset[MAXVERTEX];
extern int partitionflag;
extern int partitionnumber;

/************
  STRUCTURES
 ************/

typedef char graphtype[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8]; 

/************
  PROTOTYPES
 ************/

/* randgraph.c */

/* Values for type parameter */
#define EQUIPARTITE	0
#define KCOLORABLE	1
#define	KSMOOTH		2
#define	DELTA		3

extern void initblock(char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		int order, int numpart, int *perm, double variab, int type);
extern void initblock2(char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
		int order, int numpart, int *perm, double variab, int type,
		int blockflag, int printflag);
extern long firstclass(long c,long order,long numpart,long delta);
extern long lastclass(long c,long order,long numpart,long delta);

/* graph.c */

extern void Lovastz();
extern void k_color();

/* cheat.c */

extern int getcheat();

/* inputs.c */

extern void getflatness();
extern void getformat();
extern void welcome(), getorder(), getwhich(), getprob(), getsize();
extern void getpart(), getpartvar(), getprobvar(), getgirth(), getflat();
extern void getdflag();
extern void getweightedinputs(char *specs,int order, int *initialw, 
			int *addflag1, double *delta1, int *addflag2, 
			double *delta2);
extern void getwrap(), getdimension();
extern void getdistance(), getgnufile();
extern void getmaxattempt(int *maxattempt);

/* whatgraph.c */

extern void whatgraph(char *);

/* girth.c */
void degree_control_graph(graphtype graph, 
		graphtype blocker, 
		int size, double prob, int part, int girth, int delta,
		int delta_max, long *edges);

/* printgraph.c */
extern void write_graph_DIMACS_bin();
extern void write_graph_DIMACS_ascii();

/* verify.c */
extern void verify_color(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8], 
			int size, int part,char *verify);

/* utility.c */

extern void set_edge( register int u, register int v,
	char x, char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8] );
extern char get_edge(  int u,  int v,
        char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8] );
extern void cleargraph(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8] );
extern void get_degree_info(graphtype graph, int order, char *specs);

extern void create_and_shuffle();
extern int initrandom();
extern double dblrand();
extern long rand(long x);

/* weight.c */

void weighted_graph(graphtype graph, graphtype blocker,
		int order, double prob,int initialw,char add,
		double delta, char add2, double deg_delta,long *nedges);


/* library functions */
//extern int printf(),scanf(),fprintf();
extern void exit();

extern double drand48();
extern long lrand48();
extern void srand48();

/* geomcntrl.c */

extern void gen_and_print_geo();

/* geomgraph.c */

extern void gen_geo_graph(char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
                char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8],
                long order,
                double dist,
		int dim,
		long part,
                int wrap,
		int dflag,
                long *size,
                FILE *gnufile
                );

/* flat.c */

extern void create_flat_graph(
	graphtype graph, int size, double prob, int part, int flat, long *edges);

/* clique.c */

extern void gen_clique_graph(graphtype graph, int order, long *edges,
				int parts, int *colors, int size_weighted);

/* cycle.c */

extern void gen_cycle_graph(graphtype graph, int order, long *edges, 
		int parts, int colors[MAX_NR_VERTICES], int maxattempt);

/* evacuate.c */

extern void gen_evacuate_graph(graphtype graph, graphtype block,int order, 
                        double prob,long *nedges, char *specs, char *file);

