/*
validtype.c
takes a string, and checks the suffix
if changetype=1, returns suffix in **suffix
if changetype=2, also checks second suffix
returns 0 if nothing found, 
or if valid type found, returns length of extension (tiff=4, bmp=3, etc...)
*/

#include "ourdefs.h"
int validtype(const char *filename, const char *ssufixx ,char **suffix, int changetype){
int r;
int s;
int i;
char copy[BUFFER];

r=0;
s=strlen(filename);
for(i=0;i<s;i++)
	copy[i]=tolower(filename[i]);

copy[i]=0;


if ((s>4)&&(last(filename,"jpg")))
	r=3;
if ((s>4)&&(last(filename,"gif")))
	r=3;
if ((s>4)&&(last(filename,"png")))
	r=3;
if ((s>4)&&(last(filename,"bmp")))
	r=3;
if ((s>4)&&(last(filename,"tif")))
	r=3;

if ((s>5)&&(last(filename,"anpg")))
	r=4;
if ((s>5)&&(last(filename,"avif")))
	r=4;
if ((s>5)&&(last(filename,"jpeg")))
	r=4;
if ((s>5)&&(last(filename,"webp")))
	r=4;
if ((s>5)&&(last(filename,"tiff")))
	r=4;


if ((r>0)&&(changetype==1)) // also copy to suffix
	{
	strcpy(*suffix,filename+s-r);

//	for (i=0;i<r;i++)
//		*suffix[i]=filename[s-r+i];

//	*suffix[i]=0;
	}

if ((r>0)&&(changetype==2)) // also check presuffix
	{
	strcpy(copy,filename);
	copy[s-r-1]=0;
	r*=(1-last(ssufixx,copy));
	}



return(r);
}
