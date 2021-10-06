// alreadythere.c
// check if file exists

#include "ourdefs.h"

int alreadythere(const char *filename ){
static FILE *file;

if ((file = fopen (filename, "r")) != NULL)
	{
//	printf("   FILE EXISTS!!!!!!!!!!!!!\n");
	fclose(file);
	return(1);
	}
else
	return (0);

}
