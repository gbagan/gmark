CC=g++
CFLAGS=-g -O3 -Wall -std=c++11

objects = graphNode.o incrementalDeterministicGraphGenerator.o cumulativeDistributionUtils.o \
			nodeGenerator.o processingEdgeTypes.o
			

all: analysis 

analysis: $(objects)
	$(CC) $(objects) -c -o analysis


graphNode.o: graphNode.cpp
	$(CC) $(CFLAGS) -c graphNode.cpp
	
incrementalDeterministicGraphGenerator.o: incrementalDeterministicGraphGenerator.cpp incrementalDeterministicGraphGenerator.h ../config.h graphNode.h nodeGenerator.h
	$(CC) $(CFLAGS) -c incrementalDeterministicGraphGenerator.cpp

cumulativeDistributionUtils.o: cumulativeDistributionUtils.cpp graphNode.h ../config.h
	$(CC) $(CFLAGS) -c cumulativeDistributionUtils.cpp

nodeGenerator.o: nodeGenerator.cpp ../config.h
	$(CC) $(CFLAGS) -c nodeGenerator.cpp

processingEdgeTypes.o: processingEdgeTypes.cpp incrementalDeterministicGraphGenerator.h ../config.h ../configparser.h
	$(CC) $(CFLAGS) -c processingEdgeTypes.cpp


clean:
	rm -rf *.o test
