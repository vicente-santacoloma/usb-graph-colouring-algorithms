#include "generate.h"

#define TEST


#ifdef TEST
/* test routine for partition boundaries */
void testbnd(order,numpart,part_var)
long order,numpart,part_var;
{
	long i,t;

	printf("\
Due to truncations and descretization the distribution of sizes may\n\
not be exactly what you expect.\n\
Do you wish to see the distribution (1=YES)?: ");
	scanf("%ld",&t);
	if (t == 1) {
	   for(i=0;i<numpart;i++)
		printf("c = %ld\tfirst = %ld\tsize = %ld\n",
			i, firstclass(i,order,numpart,part_var),
			lastclass(i,order,numpart,part_var)
			  - firstclass(i,order,numpart,part_var));
	}
}
#endif

void gen_and_print_k(file)
char *file;
{
	char graph[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
	char block[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];

	char parm[50];

	long order, size;
	double probability;

	long numpart,part_var;
	double prob_var;

	long perm[MAX_NR_VERTICES];

	long girth_lim;
	extern void something();

	getorder(&order);
	printf("\n----------\n");

	getpart(order,&numpart);
	if (numpart > order) numpart = order;
	if (numpart < order) {
		getpartvar(order,numpart,&part_var);
	}
	printf("\n----------\n");


	getprob(&probability);
	getprobvar(probability,&prob_var);
	printf("\n----------\n");

	getgirth(&girth_lim);
	printf("\n----------\n");
	/* get set limited girths, cycle limits */
	something("get set limited girths, cycle limits");

#ifdef TEST
	testbnd(order,numpart,part_var);
#endif

	/* get flatness degree limits */
	something("get flatness degree limits");

	gen_graph(graph,block,order,numpart,part_var,probability,prob_var,
		girth_lim, &size, perm);

	sprintf(parm,"p edge %ld %ld\n",order,size);

	write_graph_DIMACS_bin(file,order,"c no specs\n",
		"c no verify\n",parm,"c no cheat\n",graph);

}
