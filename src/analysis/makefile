CC=g++
CFLAGS=-g -O3 -Wall -std=c++11

objects = analyseCorrelation.o analysisIncrDetGraph.o
			

all: analysis 

analysis: $(objects)
	$(CC) $(objects) -c -o analysis




clean:
	rm -rf *.o test
