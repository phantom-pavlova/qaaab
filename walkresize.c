
/*
walkresize.c
goes through a subdir system resizing files, subdirs must have already been created.
walk nftw
*/
#include "ourdefs.h"

int walkresize(const char *file, const struct stat *sb,
        int flag, struct FTW *s)
{
int retval = 0;
const char *name = file + s->base;
int chkn,j;

//  printf("%*s", s->level * 4, "");    /* indent over */
if ((s->level == 0)||(message==0)) // skip subdirs?
{
if (flag == FTW_F)
    {
	if 	(validtype(name ,&type))	//&&(strcmp(newdir,dname)!=0))// dont go into dirs we have just created 
		//&& (strstr(cpname,cpnewdir)==NULL)	) // or something like that

	{
	// choice
	// do magick read
	// do magick write
    strcpy(outfile,file);
	outfile=dirname(outfile);
	strcat(outfile,"/");
	strcat(outfile,newdir);
	strcat(outfile,"/");
	strcat(outfile,name);
	outfile[strlen(outfile)-strlen(type)-1]=0;
	chkn=0;
	for (j=0;j<strlen(newdir);j++)
        if (outfile[strlen(outfile)-j]==newdir[strlen(newdir)-j])
            chkn+=(2<<j);

	if (chkn!=((2<<j)-2))
	{

	strcat(outfile,".");
	strcat(outfile,newdir);
	strcat(outfile,".");
	strcat(outfile,type);
    if ((filesdone%10)==0)
	    printf("\033[10A"); // go up 10 lines
	printf("%c[2K", 27);
	printf("%i of %i:%s   ",filesdone++,filestoprocess,outfile);
	if (filestoprocess<filesdone) 
		filesdone=filestoprocess;

	if ((testpointer=fopen(outfile,"r"))==NULL)
	    {

// resize
		MagickWandGenesis();

		m_wand = NewMagickWand();
		MagickReadImage(m_wand,file);
		width = MagickGetImageWidth(m_wand);
		height = MagickGetImageHeight(m_wand);
		if ((width !=0)&&(height !=0))
			{
			nwidth=newsize;
			nheight=height*newsize/width;
//printf("MagickResizeImage(m_wand,%i,%i,FILTER);\n",nwidth,nheight);
			MagickResizeImage(m_wand,nwidth,nheight,FILTER);
			MagickSetImageCompressionQuality(m_wand,95);
			if(MagickWriteImage(m_wand,outfile)==MagickFalse)
				printf("failed\n");
			else
			printf("written\n");

			}
		else
			printf("failed - zero width or height\n");

		if (m_wand)
		    m_wand = DestroyMagickWand(m_wand);

		MagickWandTerminus();
		}
    else
        {
        fclose(testpointer);
        printf(" - already done\n");
        }
	}

//	else
//		printf (" - wrong name\r");
}
}

}

return retval;
}













