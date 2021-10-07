
/*
walkdir.c
goes through a subdir system noting number of image files dirs extracted later.
walk nftw
globals are:

int walkflag;
int dirstomake;
int filestoprocess;
char **dirlist;


*/
#include "ourdefs.h"

int walkdir(const char *file, const struct stat *sb,
        int flag, struct FTW *s)
{
const char *name = file + s->base;
//static char dupfile[BUFFER];
//static char duppath[BUFFER];
//  printf("%*s", s->level * 4, "");    /* indent over */
if ((s->level < 2)||(message==0)) // skip subdirs?
	{
	if (flag == FTW_F)
	    {
		if (validtype(name, newdir, NULL,  2))
			{
			strcpy(filelist[filestoprocess++],file);
//			strcpy(dupfile,file);
//			strcpy(duppath,dirname(dupfile));

//			if (strcmp(duppath,oldname)!=0)
//				{
//				dirstomake++;
//				strcpy(dirlist[dirstomake-1],duppath);
//				strcpy(oldname,duppath);
//				}

			}
		}
	}
return (0);
}













