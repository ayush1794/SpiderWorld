CC = g++
CFLAGS = -Wall
PROG = game
UTIL = utility

LIBS = -lglut -lGL -lGLU -lopenal -lalut

all: $(PROG)

$(PROG):	$(PROG).o $(UTIL).o 
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).o $(UTIL).o  $(LIBS)

$(PROG).o:	$(PROG).cpp
	$(CC) -c $(PROG).cpp

$(UTIL).o:	$(UTIL).cpp
	$(CC) -c $(UTIL).cpp


clean:
	rm -rf  *.o $(PROG)
