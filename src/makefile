CC=g++
CFLAGS=-g -O3 -Wall -std=c++11

objects = test.o gmark.o configparser.o pugixml.o randomgen.o config.o workload.o workload2.o \
			incrementalDeterministicGraphGenerator.o graphNode.o cumulativeDistributionUtils.o \
			analysisIncrDetGraph.o nodeGenerator.o processingEdgeTypes.o analyseCorrelation.o

all: test 

test: $(objects)
	$(CC) $(objects) -o test

test.o: test.cpp configparser.h config.h gmark.h workload.h incrementalDeterministicGraphGenerator.h analysisIncrDetGraph.h processingEdgeTypes.h
	$(CC) $(CFLAGS) -c test.cpp

config.o: config.cpp config.h randomgen.h
	$(CC) $(CFLAGS) -c config.cpp

gmark.o: gmark.cpp gmark.h config.h randomgen.h
	$(CC) $(CFLAGS) -c gmark.cpp

workload.o: workload.cpp workload.h config.h randomgen.h
	$(CC) $(CFLAGS) -c workload.cpp

workload2.o: workload2.cpp workload2.h config.h randomgen.h
	$(CC) $(CFLAGS) -c workload2.cpp

randomgen.o: randomgen.cpp randomgen.h
	$(CC) $(CFLAGS) -c randomgen.cpp

configparser.o: configparser.cpp config.h configparser.h
	$(CC) $(CFLAGS) -c configparser.cpp

	
pugixml.o: pugixml.cpp pugixml.hpp 
	$(CC) $(CFLAGS) -c pugixml.cpp

incrementalDeterministicGraphGenerator.o: incrementalDeterministicGraphGenerator.cpp config.h graphNode.h nodeGenerator.h
	$(CC) $(CFLAGS) -c incrementalDeterministicGraphGenerator.cpp

cumulativeDistributionUtils.o: cumulativeDistributionUtils.cpp graphNode.h config.h
	$(CC) $(CFLAGS) -c cumulativeDistributionUtils.cpp

graphNode.o: graphNode.cpp
	$(CC) $(CFLAGS) -c graphNode.cpp
	
analysisIncrDetGraph.o: analysisIncrDetGraph.cpp config.h
	$(CC) $(CFLAGS) -c analysisIncrDetGraph.cpp

nodeGenerator.o: nodeGenerator.cpp config.h
	$(CC) $(CFLAGS) -c nodeGenerator.cpp

processingEdgeTypes.o: processingEdgeTypes.cpp incrementalDeterministicGraphGenerator.h config.h configparser.h analyseCorrelation.h
	$(CC) $(CFLAGS) -c processingEdgeTypes.cpp

analyseCorrelation.o: analyseCorrelation.cpp incrementalDeterministicGraphGenerator.h
	$(CC) $(CFLAGS) -c analyseCorrelation.cpp

clean:
	rm -rf *.o test
