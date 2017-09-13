/*
	Title:	Graph generator cheat output function
	file:	cheat.c
	Does:	given the hidden coloring it first asks the user if the
		cheat is to be used then creates the cheat comment to
		be placed in the DIMACS graph
		The format is:
		c cheat <x> <y>			
		cx c0 c1 c2 c3 c4 ...
		cx cy c(y+1) ...
		cx c(2y) c(2y+1) ...
		...

		x = the order of the graph (# of values in the cheat string)
		y = number of values per line in the cheat
		ci = hidden color for vertex #i

	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Denis Papp
	email:	dpapp@cs.ualberta.ca

	Compilation: C Code

	Inputs:  given the cheat (hidden coloring)

	Outputs: returns whether the user selected to have the cheat or
		 not and returns a string which contains the DIMACS comment
		 string containing the cheat

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include <string.h>
#include "generate.h"

/*********
  GLOBALS
 *********/

int partset[MAXVERTEX];
int partitionflag;
int partitionnumber;

/* how many color values to put per comment line */
#define VALUESPERLINE 10

/*
 * getcheat()
 * GIVEN: the vertex hidden coloring (partset), the order, and cheat string
 * DESC: asks the user if teh cheat should be added to the comments
 *       if so - allocates memory for the cheat string, then builds it
 *	 if not - does not allocate memory for the cheat string
 * RETURNS: true if the user selected to have a cheat
 */
int getcheat(int partset[MAXVERTEX], int order, char **cheat) 
{
	int i;
	int cheatflag;
	char temp[10];

	printf("Do you want to output the cheat with the graph (0-no)? ");
	scanf("%d",&cheatflag);

	if (cheatflag) {
		/* estimate how much space will be needed */
		*cheat = malloc(order*5);
		if (*cheat==0) {
			fprintf(stderr,"Not enough memory for cheat string\n");
		  	cheatflag = 0;
		} else {
			sprintf(*cheat,"c cheat %d %d",order,VALUESPERLINE);
			for (i=0; i<order; i++) {
				if (i % VALUESPERLINE == 0)
					strcat(*cheat,"\ncx");
				sprintf(temp," %d",partset[i]);
				strcat(*cheat,temp);
			}
			strcat(*cheat,"\n");
		}
	}
	return (cheatflag);
}
		
