qaaab

usage

qaaab
-	asks if you want to copy all image files so that ...
		1 - width (ie largest dimension) resized to SIZE
ni		2 - resize all images using the same ratio, with biggest image dimension SIZE pixels 
		3 - resize so each file is (approx) SIZE Kbytes
ni		4 - change compression ratio so each file is SIZE Kbytes
		5 - like choice 1 but size is 800 - does subdirs 
		6 - quit
ni = not done yet)

if 1-5 entered, you will be prompted for a NAME and SIZE
then subdir created, and progress of conversion shown
maybe do subdirectories as well

qaaab stat
- just shows geometry, compression ratio and filesize of jpegs, then exits.


qaaab help
- show this explanation


qaaab version
- show version number
- and maybe a github link


qaaab batch
- width (ie largest dimension) resized to 800, for current and subdirectories, NAME is always 800




.....................................
Compiling:

a 'make' should do it,
'make clean && make' is generally better
'make install' puts qaaab into ~/bin - so it is not system wide

this was written on 32bit antix 17 with Imagemagick6, 
then ported to 64bit opensuse 15.2 with Iagemagick7.
for version 6 of Imagemagick remove the comment in the makefile so 'CFLAGS += -DIM6' is executed

the apropriate devel packages need to be added if not present (libreadline7-devel libMagickWand-7.Q16HDRI-devel libMagickCore-7.Q16HDRI-devel)
early versions used the xforms library

.......................................
Running:

if you put qaaab in your ~/bin/ dir, 
you may need to run
sudo zypper in libreadline7 libMagickWand-7.Q16HDRI libMagickCore-7.Q16HDRI
- for suse, other distros will have other names.


.....................................
missing / unfinished / features

- jpeg compression not yet done
- resize width/height for target filesize is approximate, may be improved shortly
- feedback to github may result in added features, or other improvements
- gui not really planned as it takes a while and they always seem to change, if EVERYONE asks for it I may do.
 


thanks to:
resize.c from the imagemagick wand examples section, 
and
dgookin from C-for-dummies.com which saved me a bit of time for readdir().


legal stuff

DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
                    Version 2, December 2004 

see http://www.wtfpl.net/about/




