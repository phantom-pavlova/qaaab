
/*
walkexamine.c
goes through a subdir system noting names of image files.
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
const char *name = file + s->base;
static char dupfile[BUFFER];
//static char duppath[BUFFER];
if ((s->level <2)||(message==0)) // skip subdirs?
	{
	if (flag == FTW_F)
	    {
		if (validtype(name, newdir, NULL , 2))
			{
			strcpy(dupfile,file);
			if (strlen(dupfile)>linemax)
			    linemax=strlen(dupfile);

//			strcpy(duppath,dirname(dupfile));
			filestoprocess++;
// this will catch all dirs, but sometimes twice
//			if (strcmp(duppath,oldname)!=0)
//				{
//				dirstomake++;
//				strcpy(oldname,duppath);
//				}
			}
		}
	}
return (0);
}













