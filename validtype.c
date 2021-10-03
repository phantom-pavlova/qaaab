/*
validtype.c
pass a string to validtype, it stores the type if valid in **type
returns 0 if nothing found, 
if valid type found, returns length of extension (tiff=4, bmp=3, etc...)
*/

#include "ourdefs.h"
int validtype(const char *filename ,char **type){
// checks if extension is anpg, avif,gif, jpg, jpeg, png, webp, bmp, tif,tiff
// case insensitive - jpeg = JpeG
// returns length of extension if match fount else 0, **type 'returns' extension - type is never more than 4 characters
char str[1024];
int r=-1;
int i;
// look for . and clear str
i=1;
while (r==-1)
{
	str[i]=0;
	if ((filename[strlen(filename)-i]=='.')&&r==-1)
		r=i;

i++;
if (i==strlen(filename))
	r=-2;
	}


if (r>0) // should test filename 'abc.' or '.abc'
{
r--;
for (i=0;i<r;i++)
	str[i]=filename[strlen(filename)-r+i];


//convert to lowercase
//printf("case conversion, (%s) ->",str);
for (i=0;i<r;i++)
	str[i]=tolower(str[i]);



r=0;

// there is a problem with r somewhere, should re-write this

// copy srt to type - no more than 4 characters are needed so type can always be malloced to 4 (* sizeof (char))
strcpy(*type, str);

// now check for valid types

if (strlen(str)==3)
{
if (strncmp(str,"jpg",3)==0)
	r=3;

if (strncmp(str,"gif",3)==0)
	r=3;

if (strncmp(str,"png",3)==0)
	r=3;

if (strncmp(str,"bmp",3)==0)
	r=3;

if (strncmp(str,"tif",3)==0)
	r=3;
}
else
{
if (strncmp(str,"anpg",4)==0)
	r=4;

if (strncmp(str,"avif",4)==0)
	r=4;

if (strncmp(str,"jpeg",4)==0)
	r=4;

if (strncmp(str,"webp",4)==0)
	r=4;

if (strncmp(str,"tiff",4)==0)
	r=4;
}


// so we have a match but it may have been changed to lowercase so..
// actually JPG fails, but jpg is ok - should flag this and rename, maybe one day...
/*
for (i=1;i<r;i++)
*type[strlen(*type)-i]=filename[strlen(filename)-i];
printf("-val-%s-val-",*type);
*/

}
else
	r=0;

return(r);
}
