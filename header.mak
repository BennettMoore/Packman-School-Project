#
# NOTE: Makefile does not support ~/ for -L or -I; using $(HOME) 
#
INCLUDEPATH = /home/course/csci243/pub/projects/02/include
#
# CPPFLAGS= -I $(INCLUDEPATH)
CFLAGS= -std=c99 -ggdb -Wall -Wextra -pedantic -I $(INCLUDEPATH)
#
# public project2 archive
#
LIBPATH = /home/course/csci243/pub/projects/02/lib 

#
CLIBFLAGS = -lm -L$(LIBPATH) -lp2

