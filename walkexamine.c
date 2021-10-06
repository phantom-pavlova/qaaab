
/*
walkexamine.c
goes through a subdir system noting number of image files, and subdirs to create.
walk nftw
globals are:

int walkflag;
int dirstomake;
int filestoprocess;
char **dirlist;


*/
#include "ourdefs.h"

int walkexamine(const char *file, const struct stat *sb,
        int flag, struct FTW *s)
{
int retval = 0;
const char *name = file + s->base;
static char dupfile[5000];
static char duppath[5000];
if ((s->level == 0)||(message==0)) // skip subdirs?
	{
	if (flag == FTW_F)
	    {
		if (validtype(name ,&type))
			{
			strcpy(dupfile,file);
			if (strlen(dupfile)>linemax)
			    linemax=strlen(dupfile);

			strcpy(duppath,dirname(dupfile));
			filestoprocess++;
// this will catch all dirs, but sometimes twice
			if (strcmp(duppath,oldname)!=0)
				{
				dirstomake++;
				strcpy(oldname,duppath);
				}
			}
		}
	}
return retval;
}













