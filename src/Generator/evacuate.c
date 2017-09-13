/*
        Title:  Evacuation graph generator
        file:   evacuate.c
        Does:   all the menus for inputting the evacuation options
                are here.  This looks like the graph generator
		all over again.
        Source: Joseph Culberson's Coloring Page
                http://web.cs.ualberta.ca/~joe/Coloring/index.html
        Author: Original by Joseph Culberson
                Modified by Denis Papp and Adam Beacham in May, 1995
        email:  joe@cs.ualberta.ca
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

#define MAXPERLINE  20

#include "generate.h"
#include <string.h>

extern int cheatflag;


/* This procedure splits the partset into numd sets of size numd
   and equally divides the remaining vertices into numpart-numd parts */
void specialpart(int order, int numpart,int *perm, int numd,int sized, 
		int printflag)
{
	int i,j,k,p,rem,smsize,lgnum;
	k = 0;
	i = 0;
	j = 0;
	p = 0;
	while (k < numd) {
		partset[perm[i]] = k;
		i++; j++; p++;
		if (j==sized) { 
			k++; j=0;
			if (printflag) {
				printf("%3d ", p);
			}
			 p=0;
		}
	}

	rem = order -(numd * sized);
	smsize = rem / (numpart - numd);
	lgnum = numd + (rem - (smsize * (numpart-numd)));

	j=0;
	p=0;
	while (k < lgnum) {
		partset[perm[i]] = k;
		i++; j++; p++; 
		if (j==(smsize+1)) {
			k++;j=0;
			if (printflag) {
				printf("%3d ", p);
			}
			 p=0;
		}
	}

	j=0;
	while (k < numpart) {
		partset[perm[i]] = k;
		i++; j++; p++;
		if (j==smsize) {
			k++;j=0;
			if (printflag) {
				printf("%3d ", p);
			}
			 p=0;
		}
	}
	
	if (printflag) printf("\n");
}

void get_scheme(int order,int *what, int *numpart,double *variab, char *specs,
	int *numd, int *sized) 
{
	char temp[80];
	

	printf("Now, enter the coloring scheme for the deceptive colorings\n");
	strcat(specs,"c   Deceptive colorings:\n");

	printf("K-coloring schemes:\n\
\t1 Equi-partitioned\n\
\t2 k-colorable\n\
\t3 k-colorable(smooth)\n\
\t4 k-colorable with delta variation\n\
\t5 Special partitioning\n\
?");

	scanf("%d",what);
	printf("%d\n", *what);

	getpart(order,numpart);

	switch (*what) {
		case 1: 
			/* Equipartite coloring */
			/* build specs */
			sprintf(temp,
			    "c   Equipartitioned Coloring, %d partitions\n",
			    *numpart);
			strcat(specs,temp);
			break;
		case 2:
			/* k-colorable (with variability) */
			printf("Enter variability (0-%d) ", *numpart-1);
			scanf("%lf",variab);
			printf("%f\n",*variab);
			/* build specs */
			strcat(specs,"c   k-colorable, ");
			sprintf(temp,
				"%d partitions, %d variability\n",
				*numpart,(int)(*variab));
			strcat(specs,temp);
			break;
		case 3:
			/* k-colorable (smooth) */
			printf("Enter variability (0.0-1.0) ");
			scanf("%lf",variab);
			printf("%f\n",*variab);
			/* build specs */
			strcat(specs,"c   k-colorable (smooth), ");
			sprintf(temp,
				"%d partitions, %f variability\n",
				*numpart,*variab);
			strcat(specs,temp);
                        break;
		case 4:
			/* k-colorable, delta variation */
                        printf("Enter delta (0-%d) ", (order/(*numpart))-1);
                        scanf("%lf",variab);
                        printf("%f\n",*variab);
			/* build specs */
			strcat(specs,"c   k-colorable (delta), ");
			sprintf(temp,
				"%d partitions, %d variability\n",
				*numpart,(int)(*variab));
			strcat(specs,temp);
			break;
		case 5: 
			/* Special */
			printf("Enter number and size of special sets\n");
			printf("Remaining will be equi-distributed\n");
			scanf("%d %d",numd,sized);
			printf("%d, %d\n",*numd,*sized);
			if ( (*numd > *numpart) || (*numd * *sized > order))
			{
				printf("Error too many or big special parts\n");
				exit(1);
			}
			sprintf(temp,
			 "c            Special Partition, %d partitions\n",
					*numpart);
			strcat(specs,temp);
			sprintf(temp,
			 "c            %d sets of size %d specified\n",
				*numd, *sized);
			strcat(specs,temp);
			break;
		default: 
			printf("Not implemented\n");
			exit(1);
	}
}



/*
 * gen_evacuate_graph
 *
 * Generates an evacuation graph.
 * 
 * NOTE:  Uses global variable partset.
 */
void gen_evacuate_graph(graphtype graph, graphtype block,int order, 
			double prob,long *nedges, char *specs, char *file)
{
	int color[MAX_NR_VERTICES];
	long target;		/* Target # of edges */
	int iter;		/* Number of iterations */
	int frac;
	double fltiters;
	int i,j;
	int perm[MAXVERTEX];
	int what;		/* Type of deceptive coloring */
	int numpart;		/* Number of partitions in dec. coloring [k'] */
	double variab;
	int originalnumpart = partitionnumber;
	char temp[80];
	int printflag;
	int numd, sized;

	double fraction;	/* fraction of edges to leave behind on
				   decceptive colorings */
	int missedcnt;

	char decpfile[100];
	FILE *fp;

	/* Copy global partset into local array */
	for( i=0; i<order; i++ ) color[i] = partset[i];

	/* Put all possible edges into graph */
	*nedges=0;
	for( i=0; i<order; i++ ) {
		for( j=0; j<i; j++ ) {
			if( color[i] != color[j] ) {
				set_edge(i,j,1,graph);
				(*nedges)++;
			}
		}
	}

	/* Calculate target;  target / Num_possible_edges == rho */
	target = (long) (*nedges * prob);

	printf("Max possible edges:  %ld\nNum wanted:  %ld\n",*nedges,target);

	printf("Print out the hidden color counts? (0=no,1=yes) ");
	scanf("%d",&printflag);
	printf("%d\n",printflag);

	printf("Fraction of edges to leave behind ");
	scanf("%lf",&fraction);
	printf("%f\n",fraction);

	/* Now, get parameters for the deceptive colorings */
	/* Set specs as well				   */
	get_scheme(order,&what,&numpart, &variab, specs, &numd, &sized);
	
	strcpy(decpfile,file);
	if (NULL == (fp = fopen(strcat(decpfile,".dcp"),"w")) ) {
		printf("Cannot open file for deceptive colorings\n");
		exit(1);
	}

	if (originalnumpart < order) { /* if there is a hidden coloring */
		fprintf(fp,"CLRS %d HIDDEN COLORING\n",originalnumpart);
		
		j=0;
		for (i=0;i<order;i++) {
			fprintf(fp,"%3d ",color[i]+1);
			j++;
			if (j>=MAXPERLINE) {
				fprintf(fp,"\n");
				j=0;
			}
		}
		if (j != 0) fprintf(fp,"\n");
	}
		

	/* Now, create paritions and remove edges until target reached */
	iter = 1;
	frac =0;
	while( *nedges > target ) {
		/* Produce another partitioning of the vertices */
		create_and_shuffle(perm,order);

		/* Clear the blocking graph. */
		cleargraph(block);

		switch(what) {
		case 1:	/* Equipartite */
			initblock2(block,order,numpart,perm,0,EQUIPARTITE,1,
				printflag);
			break;
		case 2: /* k-colorable (with variability */
			initblock2(block,order,numpart,perm,variab,KCOLORABLE,1,
				printflag);
			break;
		case 3:	/* k-colorable (smooth) */
			initblock2(block,order,numpart,perm,variab,KSMOOTH,1,
				printflag);
			break;
		case 4:	/* k-colorable (delta variation) */
			initblock2(block,order,numpart,perm,variab,DELTA,1,
				printflag);
			break;
		case 5: /* special */
			specialpart(order,numpart,perm,numd,sized,printflag);
			break;
		default:/* This should never happen */
			fprintf(stderr,"Unknown type.  SHOULD NOT OCCUR.\n");
		}

		/* Remove edges */
		frac = 0;
		missedcnt = 0;
		for( i=0; i<order; i++ ) {
			for( j=0; j<i; j++ ) {
				if ( *nedges >target) frac++;
				if( (*nedges > target) &&
				    (partset[i] == partset[j]) &&
				    (get_edge(i,j,graph) == 1) ) {
					if (dblrand() > fraction ){
					  set_edge(i,j,0,graph);
					  (*nedges)--;
					} else missedcnt++;
				}
			}
		}
		if (frac == ( (order * (order -1)) /2 ) ) {
			fprintf(fp,"CLRS %d DECEPTIVE COLORING(%f,%d)\n",
					numpart,fraction,missedcnt);
			
			j=0;
			for (i=0;i<order;i++) {
				fprintf(fp,"%3d ",partset[i]+1);
				j++;
				if (j>=MAXPERLINE) {
					fprintf(fp,"\n");
					j=0;
				}
			}
			if (j != 0) fprintf(fp,"\n");
		}
				

		printf("After iteration = %d; Num edges =   %ld\n",
			iter,*nedges);

		iter++;

	}

	fclose(fp);

	/* Restore original partset */
	for( i=0; i<order; i++ ) partset[i] = color[i];

	partitionnumber=originalnumpart;

	fltiters = ((double) (iter-2) ) + 
			(( (double) frac) / ( (double) (order *(order-1) /2)));

	sprintf(temp,"c   %.2f deceptive colorings generated.\n",fltiters);
	strcat(specs,temp);
	sprintf(temp,"c   %f fraction edges left.\n",fraction);
	strcat(specs,temp);
}

