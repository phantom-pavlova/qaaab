// last.c
// checks if last characters the same to length of shortest arg
// returns 1 if same, 0 if different

#include "ourdefs.h"

int last(const char *name1, const char *name2) {
int r;
int c;
int d;
int n1;
int n2;

c=n1=strlen(name1);
n2=strlen(name2);
if (c>n2)
	c=n2;

r=1; //match
for (d=1;d<=c;d++)
	{
	if (name1[n1-d]!=name2[n2-d])
		r=0;
	}




return(r);
}

