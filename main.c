/*
qaaab - image resizer for wordpess

- copyright Robert Hawkin

- copying:

 DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
                    Version 2, December 2004 

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net> 

 Everyone is permitted to copy and distribute verbatim or modified 
 copies of this license document, and changing it is allowed as long 
 as the name is changed. 

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 

  0. You just DO WHAT THE FUCK YOU WANT TO.

*/


#include "ourdefs.h"



int main(int argc,char *argv[]){

// locals
#include "vnum.c"
int r=0;
int i;
int stat=0;
int help=0;
int version=0;
int batch=0;
char ar;
DIR *folder;
struct dirent *entry;
int files = 0;
char *comment;
char *newdir;
int  newsize;
int nwidth;
int nheight;
char *outfile;
FILE *io;
int use=1;

int files_exist=0;

char *batchbase;
char *batchname;
char *tmpname1;
char *tmpname2;
long int totalsize=0;
int largestfile=0;
int maxwidth=0;
int f_d = 0; 
struct stat st; 

char choice;

float ratio;
float x,y;





MagickWand *m_wand = NULL;

int width,height;


comment=malloc(65535);
newdir=malloc(65535);
outfile=malloc(65535);



// process args if any
if (argc >1){
	ar=0;
	for (i=0; i< strlen(argv[1]) ; i++)
		if ( isalpha(argv[1][i]) && (ar==0) ) 
			ar=argv[1][i];

	if (ar=='s') 
		stat=1;

	if (ar=='h') 
		help=1;

	if (ar=='v') 
		version=1;

	if (ar=='b') 
		batch=1;
	}


if (version){
	printf("%s %.2f\n",vname,((float)vmaj)/100);
	printf("latest version from should be on github\n\n");
	exit(r);
	}

if (help){
	printf("%s\n",vname);
	printf("\tlets you batch resize images (current direcory)\n");
	printf("\n");

	printf("%s help\n",vname);
	printf("\t this message\n");
	printf("\n");

	printf("%s stat\n",vname);
	printf("\t shows stats on jpg files in current directory\n");
	printf("\n");

	printf("%s version\n",vname);
	printf("\t shows current program version\n");
	printf("\n");

	printf("%s batch\n",vname);
	printf("\t width (ie largest dimension) resized to 800, for this and all subdirectories\n");
	printf("\t      renaming suffix is 800, previously treated files not resized\n");
	
	printf("\n");
	printf("\n");
	printf("only the first character of argument is parsed, so\n\n");
	printf("%s stat\n    is the same as  \n%s s\n\n\n",vname,vname);

exit(0);
}


if (batch)
{
system("find -name \"*.jpg\" | wc -l > filecount.tmp");
system("find -name \"*.jpg\" | xargs -iRR dirname RR | sort -u | wc -l > dircount.tmp");

io=fopen("filecount.tmp","r");
fgets( comment, 1023,io);
fclose(io);
comment[strlen(comment)-1]=0;
printf("found %s files in ",comment);
if (atoi(comment)>1)
	files_exist=atoi(comment);

io=fopen("dircount.tmp","r");
fgets( comment, 1023,io);
fclose(io);
comment[strlen(comment)-1]=0;
printf("%s directories\n\n",comment);

remove("filecount.tmp");
remove("dircount.tmp");

// use find to list  all jpg's to file
system("find -name \"*.jpg\" > files.tmp");


if (files_exist)
	MagickWandGenesis();
i=0;
// screen out those in */800/
io=fopen("files.tmp","r");
while ((fgets( comment, 8023,io) != NULL)){
i++;
//remove lf
comment[strlen(comment)-1]=0;
tmpname1=strdup(comment);
tmpname2=strdup(comment);
batchbase=dirname(tmpname1);
batchname=basename(tmpname2);

if( (batchbase[strlen(batchbase)-3]=='8') &&
	(batchbase[strlen(batchbase)-2]=='0') &&
	(batchbase[strlen(batchbase)-1]=='0') 
	)
	use=0;

if (use)
	{
	printf("\r");
	printf("%c[2K", 27);
	printf("process: [%i of %i] %s",i,files_exist,comment);
	}
else
	{
	printf("\r");
	printf("%c[2K", 27);
	printf("ignore : [%i of %i] %s \n",i,files_exist,comment);
	}



//printf("%s has basename %s   filename %s\n",comment,batchbase,batchname);

if(use){
// do same resize as option 1 but: newdir is 800, newsize is 800 

m_wand = NewMagickWand();

MagickReadImage(m_wand,comment);
width = MagickGetImageWidth(m_wand);
height = MagickGetImageHeight(m_wand);

// new filename
strcpy(outfile,batchbase);
strcat(outfile,"/");
strcat(outfile,"800");
// create subdir now
r=mkdir(outfile,S_IRWXU|S_IRWXG|S_IRWXO);
strcat(outfile,"/");
strcat(outfile,batchname);
outfile[strlen(outfile)-4]=0;
strcat(outfile,".800");
strcat(outfile,".jpg");
//printf("        newfile is [%s]",outfile);

// width ?
printf("     converting.....");
if (width>height)
        {
        nwidth=800;
        nheight=height*800/width;
        }
    else
        {
        nheight=800;
        nwidth=width*800/height;
        }

MagickResizeImage(m_wand,nwidth,nheight,FILTER);
MagickSetImageCompressionQuality(m_wand,95);
MagickWriteImage(m_wand,outfile);
DestroyMagickWand(m_wand);
printf("written                \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\r");
}
}
remove("files.tmp");
printf("\n\n\nall done\n");

if (files_exist)
    MagickWandTerminus();


exit(0);
}


folder = opendir(".");
if(folder == NULL) {
	perror("Unable to read directory");
	return(1);
	}

while( (entry=readdir(folder)) ) {
//int isaj=0;
// is file? is it a jpg
if(
	(entry->d_type==DT_REG) &&
	(
	  ( (entry->d_name[strlen(entry->d_name)-3]=='j') &&
		(entry->d_name[strlen(entry->d_name)-2]=='p') &&
		(entry->d_name[strlen(entry->d_name)-1]=='g') )
	  ||
	  ( (entry->d_name[strlen(entry->d_name)-3]=='J') &&
		(entry->d_name[strlen(entry->d_name)-2]=='P') &&
		(entry->d_name[strlen(entry->d_name)-1]=='G') )
	)
  )



{


files++;
// get filesize
f_d = open(entry->d_name,O_RDONLY);
if(-1 == f_d) 
    { 
        printf("\n NULL File descriptor\n"); 
        return -1; 
    } 
if(fstat(f_d, &st)) 
    { 
        printf("\nfstat error: [%s]\n",strerror(errno)); 
        close(f_d); 
        return -1; 
    } 

close(f_d); 
// get pic details
MagickWandGenesis();

m_wand = NewMagickWand();

MagickReadImage(m_wand,entry->d_name);
width = MagickGetImageWidth(m_wand);
height = MagickGetImageHeight(m_wand);

if (m_wand)
	m_wand = DestroyMagickWand(m_wand);

MagickWandTerminus();



printf("%s \t%li K bytes  \t\t%i x \t%i\n", entry->d_name, st.st_size/1024, width,height);
totalsize+=(long int)(st.st_size/1024);
if (width>maxwidth)
	maxwidth=width;

if (height>maxwidth)
	maxwidth=height;
if (st.st_size>largestfile) largestfile=st.st_size;
}
}

closedir(folder);
printf("\nfound %i jpg files\n",files);
printf("\ttotal size %li K,\n",totalsize);
printf("\tmax image dimension is %i,\n",maxwidth); 
printf("\tbiggest file is %i K.\n",largestfile/1024); 

if (stat || (files==0))
	exit(0);

printf("choices:\n\n");
printf("1: change max picture dimension (eg 800 px)\n");
printf("2: \n");
printf("3: resize so each file is < X Kb\n");
printf("4: \n");
printf("5: \n");
printf("6: quit\n");
printf(" \n");
strcpy(comment,readline("choice?\n"));
while (atoi(comment)<1)
	strcpy(comment,readline("choice?\n"));

if (comment[0]=='6')
	exit(0);
choice=comment[0];
printf("currently quality of written image is hard coded to 95%% - we may change that later...\n");
strcpy(newdir,readline("name?\n"));
strcpy(comment,readline("new size?\n"));
newsize=atoi(comment);
printf("using %s, %i\n",newdir,newsize);

r=mkdir(newdir,S_IRWXU|S_IRWXG|S_IRWXO);

// second pass
f_d = 0; 

folder = opendir(".");

while( (entry=readdir(folder)) ) {
//int isaj=0;
// is file? is it a jpg
if((entry->d_type==DT_REG) &&
(entry->d_name[strlen(entry->d_name)-3]=='j') &&
(entry->d_name[strlen(entry->d_name)-2]=='p') &&
(entry->d_name[strlen(entry->d_name)-1]=='g') )
{


files++;
// get filesize
f_d = open(entry->d_name,O_RDONLY);
if(-1 == f_d) 
    { 
        printf("\n NULL File descriptor\n"); 
        return -1; 
    } 
if(fstat(f_d, &st)) 
    { 
        printf("\nfstat error: [%s]\n",strerror(errno)); 
        close(f_d); 
        return -1; 
    } 

close(f_d); 
// get pic details
MagickWandGenesis();

m_wand = NewMagickWand();

MagickReadImage(m_wand,entry->d_name);
width = MagickGetImageWidth(m_wand);
height = MagickGetImageHeight(m_wand);
// manip goes here

// new filename
strcpy(outfile,newdir);
strcat(outfile,"/");
strcat(outfile,entry->d_name);
outfile[strlen(outfile)-3]=0;
strcat(outfile,newdir);
strcat(outfile,".jpg");
printf("newfile is [%s]\n",outfile);

// width ?
if(choice=='1'){
printf("converting.....");	
if (width>height)
		{
		nwidth=newsize;
		nheight=height*newsize/width;
		}
	else
		{
		nheight=newsize;
		nwidth=width*newsize/height;
		}

MagickResizeImage(m_wand,nwidth,nheight,FILTER);
MagickSetImageCompressionQuality(m_wand,95);
MagickWriteImage(m_wand,outfile);
printf("written\n");
}
// resize to filesize
if(choice=='3'){
printf("converting.....");

// height width st.st_size newsize(k)

/*
this is a bit aproximate, but needs only one write, better is:
set compression to 95,
get pic size
resize pic to width*newsize/stsize 
get pic size
do a log/log interpolation for actual size
- takes 3 times longer but would be much more accurate (maybe give option fast-approx or 3xslower-quite-accurate)
problem is that jpegs compress according to content 
*/

x=(float)newsize;
x*=1000;
y=(float)st.st_size;
ratio=y/x;
ratio=sqrt(ratio);
ratio=sqrt(ratio);
x=(float)width;
y=(float)height;
x=x/ratio;
y=y/ratio;
nwidth=(int)x;
nheight=(int)y;

printf(" [%f] %i x %i -> %i x %i .......",ratio,width,height,nwidth,nheight);

MagickResizeImage(m_wand,nwidth,nheight,FILTER);
MagickSetImageCompressionQuality(m_wand,95);
MagickWriteImage(m_wand,outfile);
printf("written\n");
}

if (m_wand)
	m_wand = DestroyMagickWand(m_wand);

MagickWandTerminus();



}
}

closedir(folder);






return (r);
}


