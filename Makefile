OBJS = cutFinder.o
CC = g++
DEBUG = -g
RCFLAGS = $(shell root-config --cflags)
RLFLAGS = $(shell root-config --libs)
CFLAGS = -Wall -Werror -fopenmp $(DEBUG)
LFLAGS = -Wall -fopenmp $(DEBUG)

cf : cutFinder.cpp
	$(CC) $(RCFLAGS) $(CFLAGS) $< $(RLFLAGS) $(LFLAGS) -o cf

clean :
	rm -r -f *.d *.o *~ cf *#
