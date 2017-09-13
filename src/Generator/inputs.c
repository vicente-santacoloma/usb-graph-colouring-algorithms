/*
	Title:	Graph generator input functions
	file:	inputs.c
	Does:	various input functions for outputting useful
		information describing the input parameter required,
		then actually inputting and returning the value the
		user selects
	Source: Joseph Culberson's Coloring Page
		http://web.cs.ualberta.ca/~joe/Coloring/index.html	
	Author:	Joseph Culberson 
		new functions added by Denis Papp in May, 1995
		new functions added by Adam Beacham in May, 1995
	email:	joe@cs.ualberta.ca
		dpapp@cs.ualberta.ca
		abeacham@cs.ualberta.ca

	Compilation: C Code

	Outputs: the values selected by the user

	Copyright: Joseph Culberson. Permission is granted to
		use and distribute freely for non-commercial purposes
		with appropriate attribution.
	Warranty: NONE!
	Special Instructions: Have fun!
	Part of: The k-colorable graph generator program.
*/
#include <string.h>
#include "generate.h"

void welcome()
{
/*
	Welcome message
*/
	printf("\
\tWelcome to Culberson's Quasi-Random Graph Generator.\n\n\
This program generates quasi-random k-colorable graphs from various\n\
classes and with different controlling parameters. Some settings may\n\
make the coloring problem hard, some easy. You will be asked for the\n\
class of graphs and for various parameters interactively.\n\n\
\t\tENJOY!\n\n");

}

void getflatness(int *flatness)
{
	printf("Enter flatness: ");
	scanf("%d",flatness);
	printf("Flatness: %d\n",*flatness);
}


/*
   Get the chosen format (must be a 0 or 1) and store in _format_
   0 = ASCII, 1 = binary
 */
void getformat(format)
int *format;
{
	do {
		printf("Enter output format for the graph\
(0-ASCII, 1-binary): ");
		scanf("%d",format);
	} while (*format!=0 && *format!=1);
	if (*format) printf("Binary\n"); else printf("ASCII\n");
}

void getorder(order)
long *order;
{
	printf("\
The order of a graph is the number of vertices in it.\n\
Please input the order of the graph: ");
	scanf("%ld",order);
	printf("Order = %ld\n",*order);
}

void getwhich(which)
int *which;
{
	printf("\
The size of a graph is the number of edges in it.\n\
You may specify a control on the size by\n\
\t1\tA probability of assigning an edge for each allowed vertex pair\n\
\t2\tAn upper bound on the number of edges\n\
Additional Constraints will further limit the size\n\
Please input 1 or 2 to indicate your choice: ");
	scanf("%d",which);
	if ((*which != 1) && (*which != 2)) {
		printf("Illegal size type\n");
		exit(1);
	}
}

void getprob(probability)
double *probability;
{
	printf("\
This graph class uses probability to assign edges\n\
Please input a floating point number between 0 and 1: ");
	scanf("%lf",probability);
	printf("Probability = %.10f\n", *probability);
	
	if ((*probability < 0.0) || (*probability > 1.0)) {
		printf("Illegal probability\n");
		exit(1);
	}
}

void getsize(size)
long *size;
{
	printf("\
This graph class is edge limited. The program will try to assign up to\n\
but not exceeding the specified number of edges, subject to other\n\
constraints.\n\
Please input an integer limiting the number of edges to be assigned: ");
	scanf("%ld",size);
	printf("The size limit is %ld\n",*size);
}

void getpart(order,part)
long order, *part;
{
	printf("\
The partition number k guarantees that there will be k-coloring\n\
because each partition element will be an independent set.\n\
If k is chosen greater than or equal to %ld then no partitioning\n\
will take place.\n\
Please enter the partition number k: ",order);
	scanf("%ld",part);
	printf("The partition number is %ld\n",*part);
	if (*part < 0) {
		printf("Illegal partition number \n");
		exit(1);
	}
}

void getpartvar(order,partition,vari)
long order,partition,*vari;
{
	printf("\
Partitional variation: If set to 0, each partition element will be\n\
as nearly as possible of equal size. If set larger, then elements\n\
will vary in size in a uniform manner from ((order/part)-pv) to\n\
((order/part)+pv).\n\
Please input Partitional Variation in the range 0 to %ld: ",
			(order/partition)-1);
	scanf("%ld",vari);
	printf("Partitional Variation = %ld\n",*vari);

	if ((*vari >= (order/partition)) || (*vari<0)) {
		printf("Illegal Partitional Variation\n");
		exit(1);
	}
}

void getprobvar(probability,vari)
double probability,*vari;
{
/* This routine does not apply if the edge limit version is used */

	double lim;
	/* make sure we limit the range */
	lim = probability;
	if (lim > (1.0-probability)) lim = 1.0 - probability;
	printf("\
Probability variation: If set to 0 then each edge is assigned with equal\n\
probability. Otherwise, for each pair of partition elements,\n\
a random positive or negative quantity is chosen to perturb the\n\
edge probability for all edges between that pair. This quantity\n\
is limited by the probability variation\n\
Please enter a probability variation in the range 0.0 to %f: ",
		lim);
	scanf("%lf",vari);
	printf("Probability Variation = %f\n",*vari);
	if ((*vari < 0.0) || (*vari > lim)) {
		printf("Value error on probability variation\n");
		exit(1);
	}
}

void getgirth(girth)
long *girth;
{
	printf("\
The girth limit g will ensure that the graph has no simple cycle of size\n\
less than g.  The minimum simple cycle is g. Thus, setting g to 3 or less\n\
has no effect.  Note that large values of g (i.e. g>=5) tend to force the\n\
graph to be quite sparse, and to consume a lot of CPU time.\n\
Please enter girth limit g: ");
	scanf("%ld",girth);
	printf("The girth limit is %ld\n",*girth);
}


void randchoice(long *which)
{
	printf("\n\t\t***********\n\
   Due to the interactions of the inhibitors and generators for edges,\n\
the order in which edges are assigned WILL influence the characteristics\n\
of the graph generated. To randomize the result, an attempt will be made\n\
to randomize the order in which the edges are assigned.\n\n\
   You have two choices: \
      1 - The n choose 2 possibilities are generated and uniformly\n\
          shuffled. This requires 2*(n choose 2) words of memory.\n\
          For n=5000 you had better have a LOT of memory.\n\
          For n=1000 it requires about 4 megabytes.\n\n\
      2 - Vertices are taken in a random order, and from each\n\
          all potential neighbors of lower index are generated in random\n\
          order (different for each v). This is NOT equivalent to (1)!!\n");
   printf("Please enter your edge generation choice: ");
	scanf("%ld",which);
	printf("Edge generation choice is ");
	if (*which == 1) printf("Fully random\n");
	else printf("Semi-Random\n");
}


/****** Allow for >1 distance? */
/* YES:  In a unit hypercube, you can easily have distances >= 1 */
void getdistance(double *distance) {
    printf("\n\
Two vertices are determined to be connected if they are within\n\
a certain distance of each other. Enter this distance between 0\n\
and 1.0 (it will be used as an absolute distance.\n\
   Please enter your distance: ");
    scanf("%lf",distance);
    /* if (*distance < 1.0) printf("The distance is %f\n",*distance);
       else printf("The expected degree is %f\n",*distance);
     */
    printf("The distance is %f\n",*distance);
  }


/* dflag == 1 if user is checking d <= dist; dflag == 0 for d > dist */
void getdflag(int *dflag, double distance)
{
	printf("\n\
Insert edges if they are at a distance d where:\n\
\t0:   d >  %f   or\n\
\t1:   d <= %f\n\
Choose (0 - 1):  ",distance,distance);
	scanf("%d",dflag);

	printf("Edge if d ");
	if( *dflag ) {
		printf("<= %f\n",distance);
	} else {
		printf("> %f\n",distance);
	}
}


void getdimension(int *dim) {
  printf("\nEnter dimension for embedding (at least 1): ");
  scanf("%d",dim);
/*  if (*dim < 2) *dim = 2; */
    if (*dim < 1) *dim = 1;
    printf("Dimension = %d\n",*dim);
}

void getwrap(int *wrap) {
  *wrap = 0;
  printf("\nDo you want to have wrapping of edges included in the distance\n\
calculation? (0 for no, 1 for yes): ");
  scanf("%d",wrap);
  if( *wrap ) {
	printf("Wrapping\n");
  } else {
	printf("No wrapping\n");
  }
}

void getgnufile(char *specs,FILE **file) {
  int yes = 0;
  char filename[80];

  printf("\n\
Do you wish to generate an output file readable by gnuplot?\n\
(gnuplot can only handle 2 and 3 dimensional graphs)\n\
0 for no, 1 for yes : ");

  scanf("%d",&yes);
  
  if (yes) {
    printf("Enter file name: ");
    scanf("%s",filename);
    *file = fopen(filename,"w");
    printf("Gnuplot file:  %s\n",filename);
    strcat(specs,"c   Gnuplot file:  ");
    strcat(specs,filename);
    strcat(specs,"\n");
  } else {
    *file = NULL;
    printf("No gnuplot\n");
    strcat(specs,"c   No gnuplot\n");
  }
}


/*
 * Get input for weight-biased graphs.
 *
 */
void getweightedinputs(char *specs,int order, int *initialw, int *addflag1,
			double *alpha, int *addflag2, double *gamma)
{
	char temp[50];

	long maxweight = ((LONG_MAX) / (order*(order-1))) * 2; 

	printf("The edge weight w limits clique size <= w+1.\n");
	printf("w = 0 will result in no edges.\n");
	printf("Enter initial edge weight w (0-%ld): \n",maxweight);

        scanf("%d",initialw);
	printf("Initial weight = %d\n",*initialw);        
	sprintf(temp,"c   Initial weight = %d\n",*initialw);	
	strcat(specs,temp);

        printf("\n-------------------------------------\n");
	printf("Alpha changes the weight of incident\n");
	printf("pairs which would form triangles if\n");
	printf("added.  Reducing the weight inhibits\n");
	printf("clique formation, while increasing it\n");
	printf("increases clustering.\n");
        printf("-------------------------------------\n");
        printf("\nChange weights by (0) Multiplying or (1) Adding to old \
weight?  ");
        scanf("%d",addflag1);
	if( *addflag1 ) {
		printf("Adding\n");
	} else {
		printf("Multiplying\n");
	}
        printf("\nEnter Alpha from the range ");
        if( *addflag1 ) {
        	printf("[%d, %ld]: \n",-*initialw, maxweight-*initialw);
        } else {
                printf("[0.0, %0.1f]: \n", 
		  (*initialw==0 ? 0.0 : ((double)maxweight/(*initialw))) );
        }
        scanf("%lf",alpha);
	printf("Alpha = %f\n",*alpha);
	strcat(specs,"c   Alpha:  ");
	if( *addflag1 ) {
		sprintf(temp,"add %f\n",*alpha);
	} else {
		sprintf(temp,"mult by %f\n",*alpha);
	}
	strcat(specs,temp);

        printf("\n-------------------------------------\n");
	printf("Gamma changes the weight of all incident\n");
	printf("pairs.  Reducing the weight reduces average\n");
	printf("degree, increasing it increases clustering.\n");
        printf("-------------------------------------\n");

        printf("\nChange weights by (0) Multiplying or (1) Adding to old \
weight?  ");
        scanf("%d",addflag2);
	if( *addflag2 ) {
		printf("Adding\n");
	} else {
		printf("Multiplying\n");
	}
        printf("\nEnter Gamma from the range ");
        if( *addflag2 ) {
        	printf("[%d, %ld]: \n",-*initialw, maxweight-*initialw);
        } else {
                printf("[0.0, %0.1f]: \n", 
		   (*initialw==0 ? 0.0 : ((double)maxweight/(*initialw))) );
	}

	scanf("%lf",gamma);
	printf("Gamma = %f\n",*gamma);
	strcat(specs,"c   Gamma:  ");
	if( *addflag2 ) {
		sprintf(temp,"add %f\n",*gamma);
	} else {
		sprintf(temp,"mult by %f\n",*gamma);
	}
	strcat(specs,temp);
}

/*
 * getmaxattempt
 *
 * Gets the max # of attempts program will make to create a cycle.
 *
 */
void getmaxattempt(int *maxattempt)
{
	printf("The generator will do its best to create simple\n");
	printf("cycles which obey the hidden coloring.  However,\n");
	printf("this is not always possible.\n");
	printf("Enter the maximum number of attempts to create\n");
	printf("a cycle before giving up:\n");
	scanf("%d",maxattempt);

	printf("%d\n",*maxattempt);

	if( *maxattempt < 1 ) {
		printf("Too small.  Changing to 1 try.\n");
		*maxattempt = 1;
	}
	
}
