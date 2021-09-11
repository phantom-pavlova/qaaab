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

deps: $(SOURCES)
	grep -n $(TOKEN) Makefile | cut -d: -f1 | tail -n1 | xargs -iRR head -n RR Makefile > Makefile.t
	@cc -MM $(SOURCES.c) | xargs -iRR echo xnewlinexRR | tr -d '\n' | sed 's|xnewlinex|\nobj/|g' >> Makefile.t
	@echo >> Makefile.t
	mv Makefile.t Makefile

clean:
	rm -f $(TARGET) *.o $(objects)

install: CFLAGS= -O2 -Wall $(shell pkg-config --cflags --libs MagickWand)
install: clean
install: all
install: 
	cp $(TARGET) ~/bin/

static: CFLAGS= -Wl,-Bstatic -Wl,-Bdynamic -lpthread -lm -lX11
static: clean
static: all

samples:
	@echo "mkdir -p samples ; locate .jpg | sort -R --random-source=/dev/urandom | tail -n 200 | xargs -d \"\n\" -iRR cp RR samples/"

test:
	@echo $(TOKEN) 




#deps begin

obj/main.o: main.c ourdefs.h prototypes.h globals.h vnum.c
obj/validtype.o: validtype.c ourdefs.h prototypes.h globals.h
obj/vnum.o: vnum.c
