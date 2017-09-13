/*
	Title:	Graph generator main function
	file:	main.c
	Does:	inputs the filename, prints the welcome message, and
		starts the generator
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Joseph Culberson 
		some minor modifications made in May, 1995 
	email:	joe@cs.ualberta.ca

	Compilation: C Code

	Inputs:  graph output filename (command line)

	Outputs: the generated graph in DIMACS standard format

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include "generate.h"

int main(argc,argv)
int argc;
char *argv[];
{


        if ( argc != 2 ) 
          {printf("Usage: %s outfile\n",argv[0]);  exit(10); }

	welcome();

	whatgraph( argv[1] );

	return(0);
}
