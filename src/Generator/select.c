/*
	This is a selection that will allow degree flatness control
	and place on hold edges not yet usuable because of 
	degree variation constraint.
*/

#include<malloc.h>
#include<stdio.h>
typedef struct pairstr {
                short i,j;
        } pairtype;


void initselect( pairtype **pair, int *availpair, int *waitpair, int size  )
{
	short i,j;

        *pair = (struct pairstr *) calloc((unsigned) 1+(size*(size-1))/2,
                                        (unsigned) sizeof(struct pairstr));

	*availpair=0;
	for(i=1;i<(short) size;i++) for(j=0;j<i;j++){
		(*pair)[*availpair].i = i;
		(*pair)[*availpair].j = j;
		(*availpair)++;
	}

	*waitpair = *availpair;
	
}

void main() {
	int a,w;
	pairtype *p;
	int size;
	
	int i;
	size = 10;
	initselect(&p,&a,&w,size);

	for(i=0;i<a;i++)
		printf("%d %d\n",(short)p[i].i,(short)p[i].j);
}


