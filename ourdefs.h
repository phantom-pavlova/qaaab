/* 
header file for balance
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <libgen.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <readline/readline.h>
#include <sys/dir.h>

#include <sys/types.h> 
#include <errno.h> 
#include <fcntl.h> 

// imagemagick 6:
#ifdef IM6
#include <wand/magick_wand.h>
#define FILTER LanczosFilter,1
#endif

// imagemagick 7:
#ifndef IM6
#include <MagickWand/MagickWand.h>
#define FILTER LanczosFilter
#endif


#include "prototypes.h"

#include "globals.h"

