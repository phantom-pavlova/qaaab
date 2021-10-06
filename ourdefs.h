#define _XOPEN_SOURCE 500                 /* Required under GLIBC for nftw() */
#define _XOPEN_SOURCE_EXTENDED 1        /* Same */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
//#include <getopt.h>
//#include <dirent.h>
#include <ftw.h>        /* gets <sys/types.h> and <sys/stat.h> for us */
#include <limits.h>     /* for PATH_MAX */
#include <unistd.h>     /* for getdtablesize(), getcwd() declarations */

#define SPARE_FDS 5     /* fds for use by other functions, see text */


#include <sys/dir.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <libgen.h>

#include <readline/readline.h>
#include <termios.h>
#include <MagickWand/MagickWand.h>
#define FILTER LanczosFilter
#define BUFFER 65535
#include "prototypes.h"
#include "globals.h"

