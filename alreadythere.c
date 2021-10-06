// alreadythere.c
// check if file exists

#include "ourdefs.h"

int alreadythere(const char *filename ){
int r=0;
FILE *file;

if ((file = fopen (filename, "r")) != NULL)
	{
	r=1;
//	printf("   FILE EXISTS!!!!!!!!!!!!!\n");
	}



return (r);
}
