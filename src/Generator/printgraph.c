/*
	Title:	graph writing functions
	file:	printgraph.c
	Does:	writes the graph in DIMACS standard to file (either
		ASCII or Binary)
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Original code by Tamas Badics
		using the technique of Marcus Peinado, Boston University
		Modified to be integrated with the graph generator by:
			Joseph Culberson (joe@cs.ualberta.ca) and
			Denis Papp (dpapp@cs.ualberta.ca)
	email:	badics@rutcor.rutgers.edu

	Included by permission since this is given in the DIMACS
	original source.  

	Compilation: C Code

	Inputs:  file pointer, graph, and several other strings for
		 the preamble

	Part of: The k-colorable graph generator program.
*/
#include "generate.h"
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

extern char *strcat();
//extern int fprintf(); 
FILE *fopen();
time_t time();

#define MAXPREAMBLE 50000

void write_graph_DIMACS_ascii(file,Nr_vert,specs,verify,parm,cheat,Bitmap)
char *file;		/* file name for output */
int Nr_vert;		/* the order of the graph */
char	*specs, 	/* comments specifying graph characteristics */
	*verify,	/* comments verifying coloring exists */
	*parm,  	/* the string "p edge # #" used by DIMACS */
	*cheat; 	/* permutation if requested which yields coloring
				under simple greedy */
char Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
			 /* the bit matrix representing  the graph*/
{
	int i,j;
	FILE *fp;
	char preamble[MAXPREAMBLE];
	char temp[100];
        time_t clock;

	/* header info */
	sprintf(preamble,"c DESCRIPTION: Quasi-random coloring problem\n");
	strcat(preamble,
		"c CODE SOURCE: Joseph Culberson (joe@cs.ualberta.ca)\n");

	strcat(preamble,specs);

	strcat(preamble,verify);

	strcat(preamble,"c Creation Date: ");
	clock = time((time_t *) NULL);
	sprintf(temp,"%s",ctime(&clock));
	strcat(preamble,temp);

	strcat(preamble,parm);

	strcat(preamble,cheat);

        if ( (fp=fopen(file,"w"))==NULL )
          { printf("ERROR: Cannot open outfile\n"); exit(10); }

	fprintf(fp,preamble);

        for ( i = 0; i<Nr_vert; i++) {
	  for (j=0; j<=i; j++) {
		if (get_edge(i,j,Bitmap) ) fprintf(fp, "e %d %d\n",i+1,j+1);
	  }
	}

	fclose(fp);

}



void write_graph_DIMACS_bin(file,Nr_vert,specs,verify,parm,cheat,Bitmap)
char *file;		/* file name for output */
int Nr_vert;		/* the order of the graph */
char	*specs, 	/* comments specifying graph characteristics */
	*verify,	/* comments verifying coloring exists */
	*parm,  	/* the string "p edge # #" used by DIMACS */
	*cheat; 	/* permutation if requested which yields coloring
				under simple greedy */
char Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
			 /* the bit matrix representing  the graph*/
{
	int i;
	FILE *fp;
	char preamble[MAXPREAMBLE];
	char temp[100];
        time_t clock;


	/* header info */
	sprintf(preamble,"c DESCRIPTION: Quasi-random coloring problem\n");
	strcat(preamble,
		"c CODE SOURCE: Joseph Culberson (joe@cs.ualberta.ca)\n");

	strcat(preamble,specs);

	strcat(preamble,verify);

	strcat(preamble,"c Creation Date: ");
	clock = time((time_t *) NULL);
	sprintf(temp,"%s",ctime(&clock));
	strcat(preamble,temp);

	strcat(preamble,parm);

	strcat(preamble,cheat);
	
        if ( (fp=fopen(file,"w"))==NULL )
          { printf("ERROR: Cannot open outfile\n"); exit(10); }

	fprintf(fp,"%d\n",(int)strlen(preamble));
	fprintf(fp,preamble);

        for ( i = 0
                 ; i < Nr_vert && fwrite(Bitmap[i], 1, (int)((i + 8)/8), fp)
                 ; i++ );

	fclose(fp);

}
