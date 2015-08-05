# Makefile for libtempltext
# Copyright (C) 2014 Sergey Kolevatov

###################################################################

VER := 0

MODE ?= debug

###################################################################

BOOST_PATH := $(shell echo $$BOOST_PATH)

ifeq (,$(BOOST_PATH))
    $(error 'please define path to boost $$BOOST_PATH')
endif

###################################################################

BOOST_INC=$(BOOST_PATH)
BOOST_LIB_PATH=$(BOOST_PATH)/stage/lib

BOOST_LIB_NAMES := regex
BOOST_LIBS = $(patsubst %,$(BOOST_LIB_PATH)/libboost_%.a,$(BOOST_LIB_NAMES))


###################################################################

EXT_LIBS=$(BOOST_LIBS)

###################################################################

PROJECT := templtext

LIBNAME=libtempltext

###################################################################

ifeq "$(MODE)" "debug"
    OBJDIR=./DBG
    BINDIR=./DBG

    CFLAGS := -Wall -std=c++0x -ggdb -g3 -Wl,--no-as-needed
    LFLAGS := -Wall -lstdc++ -lrt -ldl -lm -g
#    LFLAGS_TEST := -Wall -lstdc++ -lrt -ldl -g -L. $(BINDIR)/$(LIBNAME).a $(BINDIR)/libutils.a -lm
    LFLAGS_TEST := -Wall -lstdc++ -lrt -ldl -g -L. -lm $(LFLAGS_STAT)

    TARGET=example
else
    OBJDIR=./OPT
    BINDIR=./OPT

    CFLAGS := -Wall -std=c++0x -Wl,--no-as-needed
    LFLAGS := -Wall -lstdc++ -lrt -ldl -lm
#    LFLAGS_TEST := -Wall -lstdc++ -lrt -ldl -L. $(BINDIR)/$(LIBNAME).a $(BINDIR)/libutils.a -lm
    LFLAGS_TEST := -Wall -lstdc++ -lrt -ldl -L. -lm $(LFLAGS_STAT)

    TARGET=example
endif

###################################################################

CC=gcc

LDSHARED=gcc
CPP=gcc -E
INCL = -I$(BOOST_INC) -I.


STATICLIB=$(LIBNAME).a
SHAREDLIB=
SHAREDLIBV=
SHAREDLIBM=
LIBS=$(STATICLIB) $(SHAREDLIBV)

AR=ar rc
RANLIB=ranlib
LDCONFIG=ldconfig
LDSHAREDLIBC=-lc
TAR=tar
SHELL=/bin/sh
EXE=

#vpath %.cpp .

SRCC = index_matches.cpp parser.cpp renderer.cpp templ.cpp

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCC)) $(patsubst %.c,$(OBJDIR)/%.o,$(SRCC))

LIB_NAMES =
LIBS = $(patsubst %,$(BINDIR)/lib%.a,$(LIB_NAMES))

all: static

static: $(TARGET)

check: test

test: all teststatic

teststatic: static
	@echo static test is not ready yet, dc10

$(BINDIR)/$(STATICLIB): $(OBJS)
	$(AR) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

$(OBJDIR)/%.o: %.cpp
	@echo compiling $<
	$(CC) $(CFLAGS) -DPIC -c -o $@ $< $(INCL)

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	$(CC) $(CFLAGS) -DPIC -c -o $@ $< $(INCL)

$(TARGET): $(BINDIR) $(BINDIR)/$(TARGET) 
	ln -sf $(BINDIR)/$(TARGET) $(TARGET)
	@echo "$@ uptodate - ${MODE}"

$(BINDIR)/$(TARGET): $(LIBS) $(OBJDIR)/$(TARGET).o $(OBJS) $(BINDIR)/$(STATICLIB)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$(TARGET).o $(BINDIR)/$(LIBNAME).a $(LIBS) $(EXT_LIBS) $(LFLAGS_TEST)

# somehow this rule doesn't work
#$(BINDIR)/lib%.a: %
#	cd ../$<; make; cd $(project)
#	ln -sf ../$</$@ $(BINDIR)

$(LIBS):
	for s in $(LIB_NAMES); do \
		cd ../$$s; make; cd ../$(PROJECT); \
		ln -sf ../../$$s/$(BINDIR)/lib$$s.a $(BINDIR); \
		done;

$(BINDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean:
	#rm $(OBJDIR)/*.o *~ $(TARGET)
	rm $(OBJDIR)/*.o $(TARGET) $(BINDIR)/$(TARGET) $(BINDIR)/$(STATICLIB)
