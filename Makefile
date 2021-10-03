# standard Makefile:
# SOURCES.c output from 'ls*.c'
# TARGET is pwd
# 'mkdir obj' -- not automatic
# check cflags and includes 
#  
# bugs:
# cannot (quite) set itself up automatically
#  -mkdir obj 
#
# deps, clean, ... should be declaired phoney
#
# no help or comments
#
CFLAGS:=-g -pg -Wall -Wno-unused-variable -Wno-unused-but-set-variable $(shell pkg-config --cflags --libs MagickWand)
#
# imgmagick 6:
#CFLAGS += -DIM6
#
#
LDLIBS:=-lm -L/usr/X11R6/lib -lX11  -lreadline $(shell pkg-config --libs MagickWand)
ifeq ($(shell @grep fl_do_forms * 2>/dev/null),1)
LDLIBS +=-lforms
endif
OBJDIR:=obj
#TOKEN:="deps start"
#TOKEN:=$(subst start,begin,$(TOKEN))
TOKEN:="deps begin"
#this file is now cut from LAST occurance 
SOURCES.c=$(shell ls *.c)


TARGET=$(shell basename $(CURDIR))

objects = $(addprefix $(OBJDIR)/, $(SOURCES.c:.c=.o))



all:	$(objects)
	$(CC) $(CFLAGS) -o $(TARGET) $(objects) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

help: # Print help on Makefile
	@grep '^[^.#]\+:\s\+.*#' Makefile | \
    sed "s/\(.\+\):\s*\(.*\) #\s*\(.*\)/`printf "\033[93m"`\1`printf "\033[0m"` \3 [\2]/" | \
    expand -t20


deps: # redo dependencies  
	$(SOURCES)
	grep -n $(TOKEN) Makefile | cut -d: -f1 | tail -n1 | xargs -iRR head -n RR Makefile > Makefile.t
	@cc -MM $(SOURCES.c) | xargs -iRR echo xnewlinexRR | tr -d '\n' | sed 's|xnewlinex|\nobj/|g' >> Makefile.t
	@echo >> Makefile.t
	mv Makefile.t Makefile

clean: # for new target machine
	mkdir -p $(OBJDIR)
	rm -f $(TARGET) *.o $(objects)
	sync

install: CFLAGS= -O2 -Wall $(shell pkg-config --cflags --libs MagickWand)
install: clean
install: all
install: 
	cp $(TARGET) ~/bin/

static: # static build for distribution ie to machines without xforms libraries 
	CFLAGS= -Wl,-Bstatic -Wl,-Bdynamic -lpthread -lm -lX11
static: clean
static: all

samples: # make a dir called samples, fill it with assorted images for testing
	FORCE
	mkdir -p samples 
	-locate -l 300 .jpg | sort -R --random-source=/dev/urandom | tail -n 30 | xargs -d "\n" -iRR cp RR samples/
	-locate -l 300 .JPG | sort -R --random-source=/dev/urandom | tail -n 30 | xargs -d "\n" -iRR cp RR samples/
	-locate -l 300 .png | sort -R --random-source=/dev/urandom | tail -n 30 | xargs -d "\n" -iRR cp RR samples/

FORCE: ;

test: # for trial end error makefile modifications
	@echo $(TOKEN) 




#deps begin

obj/main.o: main.c ourdefs.h prototypes.h globals.h vnum.c
obj/validtype.o: validtype.c ourdefs.h prototypes.h globals.h
obj/vnum.o: vnum.c
obj/walkdir.o: walkdir.c ourdefs.h prototypes.h globals.h
obj/walkexamine.o: walkexamine.c ourdefs.h prototypes.h globals.h
obj/walkresize.o: walkresize.c ourdefs.h prototypes.h globals.h
