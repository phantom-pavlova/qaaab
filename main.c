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
#define ASSIGN ;
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


float ratio;
float x,y;
time_t t;
//m_wand = NULL;


// allocate strings 
comment=malloc(65535);
newdir=malloc(65535);
outfile=malloc(65535);
dname=malloc(65535);
oldname=malloc(65535);
adddir=malloc(65535);
batchdir=malloc(65535);
batchbase=malloc(65535);
type = malloc(10*sizeof(char));
newdirflag=1;
strcpy(oldname,"owlerror");
// seed random number generator
srand((unsigned) time(&t));

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
	printf("\n%s %.2f\n",vname,((float)vmaj)/100);
	printf("latest version from should be on github\n\n");
	printf("git clone https://github.com/phantom-pavlova/qaaab.git\n\n");
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
	printf("\t      renaming suffix is 1024, peviously treated files should not be resized\n");
	
	printf("\n");
	printf("\n");
	printf("only the first character of argument is parsed, so\n\n");
	printf("%s stat\n    is the same as  \n%s s\n\n\n",vname,vname);

exit(0);
}


if (batch)
{
setbuf(stdout, NULL);

system("find -iname \"*.jpg\" -o -iname \"*.png\" | wc -l > filecount.tmp");
system("find -iname \"*.jpg\" -o -iname \"*.png\" | xargs -d \"\\n\" -iRR dirname RR | sort -u | wc -l > dircount.tmp");

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
system("find -iname \"*.jpg\" -o -iname \"*.png\" > files.tmp");


if (files_exist)
	MagickWandGenesis();
i=0;
// screen out those in */800/
io=fopen("files.tmp","r");
while ((fgets( comment, 1023,io) != NULL)){
i++;
//remove lf
comment[strlen(comment)-1]=0;
tmpname1=strdup(comment);
tmpname2=strdup(comment);
batchbase=dirname(tmpname1);
batchname=basename(tmpname2);
use=1;
if( (batchbase[strlen(batchbase)-4]=='1') &&
	(batchbase[strlen(batchbase)-3]=='0') &&
	(batchbase[strlen(batchbase)-2]=='2') &&
	(batchbase[strlen(batchbase)-1]=='4') 
	)
	use=0;

if ((i%10)==0)
	printf("\033[10A"); // go up 10 lines

if (use)
	{
	printf("%c[2K", 27); // clear entire line
	printf("process: [%i of %i] %s",i,files_exist,comment);
	}
else
	{
	printf("%c[2K", 27); // clear entire line
	printf("ignore : [%i of %i] %s \n",i,files_exist,comment);
	}



//printf("%s has basename %s   filename %s\n",comment,batchbase,batchname);

if(use){
// do same resize as option 1 but: newdir is 1024, newsize is 1024 

// new filename
strcpy(outfile,batchbase);
strcat(outfile,"/");
strcat(outfile,"1024");
// create subdir now
r=mkdir(outfile,S_IRWXU|S_IRWXG|S_IRWXO);
strcat(outfile,"/");
strcat(outfile,batchname);
outfile[strlen(outfile)-4]=0;
strcat(outfile,".1024");
strcat(outfile,".jpg");
//terrible fix but..
outfile[strlen(outfile)-1]=comment[strlen(comment)-1];
outfile[strlen(outfile)-2]=comment[strlen(comment)-2];
outfile[strlen(outfile)-3]=comment[strlen(comment)-3];

//printf("        newfile is [%s]",outfile);
if ((testpointer=fopen(outfile,"r"))==NULL)
	{
	m_wand = NewMagickWand();

	MagickReadImage(m_wand,comment);
	width = MagickGetImageWidth(m_wand);
	height = MagickGetImageHeight(m_wand);
	// width ?
	if (	(width ==0)	||
			(height == 0) ||
			((width<1024)&&(height<1024)) )
        {
        printf(" -!- image zero size or too small  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\n");    
        }
    else
        {




		printf("     converting.....");
		if (width>height)
				{
				nwidth=1024;
				nheight=height*1024/width;
				}
			else
				{
				nheight=1024;
				nwidth=width*1024/height;
				}

			MagickResizeImage(m_wand,nwidth,nheight,FILTER);
			MagickSetImageCompressionQuality(m_wand,95);
			MagickWriteImage(m_wand,outfile);
			printf("written                \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\n");
			}
		DestroyMagickWand(m_wand);


		}
	else
        {
        fclose(testpointer);
        printf(" - already done\n");
        }




}
}
remove("files.tmp");

while ((i%10)!=0)
	{
	i++;
	printf("\n");
	}

printf("\n\n\nall done\n");

if (files_exist)
    MagickWandTerminus();


exit(0);
}


printf("\n");



// pre read - maybe get rid of this 

#ifdef COMPILE_OLD_CODE

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


printf("\r");
printf("%c[2K", 27);

printf("%s \t%li K bytes  \t\t%i x \t%i\r", entry->d_name, st.st_size/1024, width,height);
totalsize+=(long int)(st.st_size/1024);
if (width>maxwidth)
	maxwidth=width;

if (height>maxwidth)
	maxwidth=height;
if (st.st_size>largestfile) largestfile=st.st_size;
}
}

closedir(folder);
printf("\n\nfound %i jpg files\n",files);
printf("\ttotal size %li K,\n",totalsize);
printf("\tmax image dimension is %i,\n",maxwidth); 
printf("\tbiggest file is %i K.\n",largestfile/1024); 
#endif


//if (stat || (files==0))
//	exit(0);

printf("choices:\n\n");
printf("1:y  change max picture dimension (eg 800 px)\n");
printf("2:  watermark\n");
printf("3:  resize so each file is < X Kb\n");
printf("4:  stat\n");
printf("5:y  auto\n");
printf("6:y  quit\n");
printf(" \n");
strcpy(comment,readline("choice?\n"));
while (((atoi(comment)<1)||(atoi(comment)>6))&&(comment[0]!='q'))
	strcpy(comment,readline("choice?\n"));
//if((rand() % 10)>7) "that's not very polite, say please"
if ((atoi(comment)==6)||(comment[0]=='q'))
	exit(0);
if (atoi(comment)==5)
{
choice=5|8;
newsize=800;
strcpy(newdir,"800");
}
else
{
choice=atoi(comment);
printf("choice was %i\n",choice);
printf("currently quality of written image is hard coded to 95%% - we may change that later...\n");
strcpy(comment,readline("recurse into subdirectories(y/n)?\n"));
if((comment[0]=='y')||(comment[0]=='Y'))
	choice |=8;

printf("choice was %c, (%i)\n",comment[0],choice);
strcpy(comment,readline("new size?\n"));
while (atoi(comment)==0)
	strcpy(comment,readline("new size (number)?\n"));

strcpy(newdir,comment);
newsize=atoi(comment);
}



if (choice>7)
	{
	strcpy(comment," recursing into subdirs");
	message=0;
	}
else
	{
	strcpy(comment," no recursing, just current dir");
	message=1;
	}

printf("using %s, %i %s\n",newdir,newsize,comment);
//newsize *=1024; // idiot, NEVER use same variable for two different tasks!
printf("hit a key to go or \"q\" to abort\n");



struct termios info;
int tc1=info.c_cc[VMIN];
int tc2=info.c_cc[VTIME];
tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
info.c_lflag &= ~ICANON;      /* disable canonical mode */
info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
info.c_cc[VTIME] = 0;         /* no timeout */
tcsetattr(0, TCSANOW, &info); /* set immediately */



char waitc = getchar();
if ((waitc=='q')||(waitc=='Q'))
	exit(0);

info.c_cc[VTIME]=tc2;
info.c_cc[VMIN]=tc1;
info.c_lflag |= ICANON;
tcsetattr(0, TCSANOW, &info);
printf("\n");


// nftw stuff now...



//int c, nfds;
int errors = 0;
int flags = FTW_PHYS | FTW_CHDIR | FTW_DEPTH;
char start[PATH_MAX], finish[PATH_MAX];

//nfds = getdtablesize() - SPARE_FDS; /* leave some spare descriptors */

getcwd(dname,4095);

filesdone=0;
filestoprocess=0;
walkflag=1;
printf("Examining directories ........\n");
if (nftw(dname, walkexamine, 15, flags) != 0)
        {
        fprintf(stderr, "nftw(): stopped early\n");
        errors++;
        }
printf("found %i image files\n",filestoprocess);
printf("in %i directories,\n",dirstomake);
printf("\n\ncreating new directories........\n");
// mallocing in nftw affects nftw's structures - go figure...
dirlist=malloc(dirstomake*sizeof(char *));
for (i=0;i<dirstomake;i++)
	dirlist[i]=malloc(4096*sizeof(char));
//this time get their names
dirstomake=0;
if (nftw(dname, walkdir, 15, flags) != 0)
        {
        fprintf(stderr, "nftw(): stopped early\n");
        errors++;
        }

printf("got names......\n");

for (i=0;i<dirstomake;i++)
	{
// if dirlist[i] already end with newdir, dont add it
int chkn=0;
int j;

	for (j=0;j<strlen(newdir);j++)
		if (dirlist[i][strlen(dirlist[i])-j]==newdir[strlen(newdir)-j])
			chkn+=(2<<j);


	strcpy(comment,dirlist[i]);
	strcat(comment,"/");
	strcat(comment,newdir);
	if (chkn!=((2<<j)-2))
		if(mkdir(comment,S_IRWXU|S_IRWXG|S_IRWXO)==0)
			printf("%s created  %i %i\n",comment,chkn,(2<<j)-1);
	}



printf("processing files ........\n\n\n\n\n\n\n\n\n\n\n\n\n");


if (nftw(dname, walkresize, 15, flags) != 0)
        {
        fprintf(stderr, "nftw(): stopped early\n");
        errors++;
        }


while(((filesdone++)%10)!=0)
	printf("\n");

printf("ftw() finished\n");






/*
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
int wrote=0;
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
if(MagickWriteImage(m_wand,outfile)==MagickFalse)
	printf("failed\n");
else
	{
	printf("written\n");
	wrote=1;
	}

}
// resize to filesize
if(choice=='3'){
printf("converting.....");

// height width st.st_size newsize(k)

//
//this is a bit aproximate, but needs only one write, better is:
//set compression to 95,
//get pic size
//resize pic to width*newsize/stsize 
//get pic size
//do a log/log interpolation for actual size
//- takes 3 times longer but would be much more accurate (maybe give option fast-approx or 3xslower-quite-accurate)
//problem is that jpegs compress according to content 


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
if (MagickWriteImage(m_wand,outfile)==MagickFalse)
	printf("failed\n");
else
	{
	printf("written\n");
	wrote=1;
	}

}

if (m_wand)
	m_wand = DestroyMagickWand(m_wand);

MagickWandTerminus();



}
}

closedir(folder);
// gcc is great, but nested comments would be nice
*/
return (r);
}


