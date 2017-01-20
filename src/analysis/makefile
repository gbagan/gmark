CC=g++
CFLAGS=-g -O3 -Wall -std=c++11

objects = analyseCorrelation.o analysisIncrDetGraph.o
			

all: analysis 

analysis: $(objects)
	$(CC) $(objects) -c -o analysis


analyseCorrelation.o: analyseCorrelation.cpp
	$(CC) $(CFLAGS) -c analyseCorrelation.cpp ../incrementalDeterministicGraphGenerator.h

analysisIncrDetGraph.o: analysisIncrDetGraph.cpp ../config.h
	$(CC) $(CFLAGS) -c analysisIncrDetGraph.cpp
	

clean:
	rm -rf *.o test
