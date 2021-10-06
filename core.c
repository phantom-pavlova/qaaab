/*
core.c
dispatcher and forks
this does the work.
FIXME close unneeded file descriptors (there are a lot)
*/

#include "ourdefs.h"
#include <sys/wait.h>


int
core (void)
{
int r = 0;
int flag;
int i, j, k;						// local, global may exist
int status;
int counter;
int justwrite;
int filesprocessed = 0;
char infile[BUFFER];			// local, global may exist
char outfile[BUFFER];			// local, global may exist
char *string;					// local, global may exist
char needle[255];				// null, just for validtype()
char *hillary;				// null, just for validtype()
pid_t pids[cpus];
pid_t wpid;
int tochildfd[cpus][2];
int toparentfd[cpus][2];
int childcounter;

int height, width;
int nheight, nwidth;

// nanosleep does not work on old (2.x) kernels
struct timespec ctim, ctim2;
struct timespec ptim, ptim2;
ctim.tv_sec  = 0;
ctim.tv_nsec = 5000L;
ptim.tv_sec  = 0;
ptim.tv_nsec = 500000L;

//MagickWand *magick_wand;
// locals declared ...

//cpus = 1;

hillary = malloc (1024);
string = malloc (BUFFER);
childcounter=0;

//mainly for parent
for (i = 0; i < BUFFER; i++)
	infile[i] = 0;
	

for (i = 0; i < cpus; i++)
	if (pipe (tochildfd[i]) < 0)
		{
		printf ("pipe error\n");
		exit (1);
		}


for (i = 0; i < cpus; i++)
	if (pipe (toparentfd[i]) < 0)
		{
		printf ("pipe error\n");
		exit (1);
		}

for (i = 0; i < cpus; ++i)
{
if ((pids[i] = fork ()) < 0)
	{
	perror ("fork");
	exit (1);
	}
else if (pids[i] == 0)
	{
	/* child */

	for (j = 0; j < cpus; j++)
		{
		close(tochildfd[j][1]);
		close(toparentfd[j][0]);
		if (j!=i)
			{
			close(tochildfd[j][0]);
			close(toparentfd[j][1]);
			}
		}

//	printf ("child %i active...\n", i);
	MagickWandGenesis();
	while (1)
		{
		childcounter++;
		// clean up from time to time
		if ((childcounter%100)==27)
			{
			MagickWandTerminus();
			MagickWandGenesis();
			}

		// wait for signal
		for (j = 0; j < BUFFER; j++)
			infile[j] = 0;
		flag=0;

		while (read (tochildfd[i][0], infile, BUFFER) == 0)
				nanosleep(&ctim , &ctim2);

		if (strncmp (infile, "stop", 4) == 0)	// all done
			{
//			printf ("child %i has stopped\n", i);
			exit (0);
			}
		// we have a filename, so
//		printf ("child %i got %s\n", i, infile);
		// check it is valid
		string = strdup (infile);
		needle[0] = '/';
		needle[1] = 0;
		strcat (needle, newdir);
		if (strstr (string, needle) == NULL)
//			printf ("invalid infile\n");
//		else
			{
			// calculate outfile
			// should do a function for this: 
			string = strdup (infile);
			strcpy (outfile, dirname (string));
			strcat (outfile, "/");
			strcat (outfile, newdir);
			strcat (outfile, "/");
			string = strdup (infile);
			strcat (outfile, basename (string));
			string = strdup (infile);
			outfile[strlen (outfile) - validtype (string, &hillary,1)] = 0;
			strcat (outfile, newdir);
			strcat (outfile, ".");
			strcat (outfile, hillary);
			//terrible fix but ... seems to work! - form batch
			outfile[strlen (outfile) - 1] = infile[strlen (infile) - 1];
			outfile[strlen (outfile) - 2] = infile[strlen (infile) - 2];
			outfile[strlen (outfile) - 3] = infile[strlen (infile) - 3];
			outfile[strlen (outfile) - 4] = infile[strlen (infile) - 4];	
				
//			printf ("(%s)(%s)\n", infile, outfile);
			
			// make sure it does not exist
			//if ((checkpointer = fopen (outfile, "r")) != NULL)
			if (alreadythere(outfile))
				{
//				printf ("file exists\n");
				flag=0;
			//	fclose (testpointer);
				}
			else
				{
//				printf ("opening..\n");
				// open infile
				m_wand = NewMagickWand ();
				MagickReadImage (m_wand, infile);
				width = MagickGetImageWidth (m_wand);
				height = MagickGetImageHeight (m_wand);	
				
				// is it bigger than newsize in at least 1 diection
				justwrite=0;
				if ((width < newsize) && (height < newsize)&& (alwaysresize==0))
//					{
//					printf (" image (%i x %i)less than %i - unchanged\n", width,height,newsize);
					justwrite=1;
//					}
				// and the other is not zero
				//else
					{
					if ((width != 0) && (height != 0))
//						printf (" image has zero size\n");
						{
						// ok, then resize and write.
						if (width > height)
							{
							nwidth = newsize;
							nheight = height * newsize;
							nheight = nheight / width;
							}
						else
							{
							nheight = newsize;
							nwidth = width * newsize;
							nwidth = nwidth / height;
							}
					if (justwrite==0)
						{
						MagickResizeImage (m_wand, nwidth, nheight, FILTER);
						MagickSetImageCompressionQuality (m_wand, 95);
						}
						// close
						if (MagickWriteImage (m_wand, outfile) == MagickFalse)
							flag=0;
						else
							flag=1;
					
//						printf ("\n");
						}
					}
				if(m_wand) m_wand = DestroyMagickWand(m_wand);
				}			// end of file exists
			}
		// tell parent we are done.
		
//		printf ("telling parent we are done \n");
		if (flag)
			write (toparentfd[i][1], "done", 4);
		else
			write (toparentfd[i][1], "failed", 6);
		}
	MagickWandTerminus();
	}

}

/* parent */
// you could let user up + down CPUs from here

for (j = 0; j < cpus; j++)
	{
	close(tochildfd[j][0]);
	close(toparentfd[j][1]);
	}


printf ("I am the parent \n");

//int flag = (2 << (cpus + 1)) - 1;	//  ??
counter = 0;
// send off first batch
for (i = 0; i < cpus; i++)
	{
	if (counter < filestoprocess)	// you never know
		write (tochildfd[i][1], filelist[counter], strlen (filelist[counter]));
	else
		write (tochildfd[i][1], "stop", 4);

	counter++;
	}
// loop
i = 0;
while (counter < (filestoprocess + cpus))
	{
	nanosleep(&ptim , &ptim2);
	// check for children done
	if ((k=read (toparentfd[i][0], infile, BUFFER)) != 0)
	// more to do ? send another 
		{
		if (strncmp(infile,"done",4)==0)
			filesprocessed++;	

		if (counter >= filestoprocess)
			{
			write (tochildfd[i][1], "stop", 4);
			printf ("\nchild %i has been sent stop ", i);
			}
		else
			{
			write (tochildfd[i][1], filelist[counter], strlen (filelist[counter]));
			printf("\033[1A"); // go up 1 lines
			printf("%c[2K", 27); // clear entire line
			printf("processed %i, written %i of %i\n",counter,filesprocessed,filestoprocess);
			}

		counter++;
		}
	// else tell child to stop, and flag
	i++;
	if (i == cpus)
		i = 0;
	}
// make sure all finished
while ((wpid = wait (&status)) > 0)
	;
printf ("\n\n%i files examined, %i converted\n", filestoprocess,
filesprocessed);
printf ("bye from parent \n");

// cleanup
free(hillary);
free(string);

return (r);
}
