CC = g++
SKLIB = ..
CFLAGS = -Wall -g -static -O3 -I$(SKLIB)/include
#LIBS   = -lsk -lpthread
LIBS   = -L$(SKLIB)/lib -lsk -lpthread
#LIBS   = $(SKLIB)/lib/libsk.a

all: test

test: main.cpp Makefile
	$(CC) $(CFLAGS) main.cpp $(LIBS) -o $@ $(LIBS)

clean:
	rm -f testSKLib
