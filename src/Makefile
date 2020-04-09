CC=g++
CFLAGS=-g -Wall -std=c++11 -I ../libs

objects = test.o gmark.o configparser.o pugixml.o argparse.o randomgen.o config.o workload.o workload2.o \
			incrementalDeterministicGraphGenerator.o graphNode.o cumulativeDistributionUtils.o \
			nodeGenerator.o processingEdgeTypes.o correlation.o shiftingDegreeDistribution.o

all: test


test: $(objects)
	$(CC) $(objects) -o test

test.o: test.cpp configparser.h config.h gmark.h workload.h monStaGen/incrementalDeterministicGraphGenerator.h monStaGen/processingEdgeTypes.h
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

pugixml.o: ../libs/pugixml/pugixml.cpp ../libs/pugixml/pugixml.hpp
	$(CC) $(CFLAGS) -c ../libs/pugixml/pugixml.cpp

argparse.o: ../libs/argparse/argparse.cpp ../libs/argparse/argparse.hpp
	$(CC) $(CFLAGS) -c ../libs/argparse/argparse.cpp

graphNode.o: monStaGen/graphNode.cpp
	$(CC) $(CFLAGS) -c monStaGen/graphNode.cpp

incrementalDeterministicGraphGenerator.o: monStaGen/incrementalDeterministicGraphGenerator.cpp monStaGen/incrementalDeterministicGraphGenerator.h config.h monStaGen/graphNode.h monStaGen/nodeGenerator.h
	$(CC) $(CFLAGS) -c monStaGen/incrementalDeterministicGraphGenerator.cpp

cumulativeDistributionUtils.o: monStaGen/cumulativeDistributionUtils.cpp monStaGen/graphNode.h config.h
	$(CC) $(CFLAGS) -c monStaGen/cumulativeDistributionUtils.cpp

nodeGenerator.o: monStaGen/nodeGenerator.cpp config.h
	$(CC) $(CFLAGS) -c monStaGen/nodeGenerator.cpp

processingEdgeTypes.o: monStaGen/processingEdgeTypes.cpp monStaGen/incrementalDeterministicGraphGenerator.h config.h configparser.h
	$(CC) $(CFLAGS) -c monStaGen/processingEdgeTypes.cpp

correlation.o: monStaGen/correlation.cpp monStaGen/incrementalDeterministicGraphGenerator.h
	$(CC) $(CFLAGS) -c monStaGen/correlation.cpp

shiftingDegreeDistribution.o: monStaGen/shiftingDegreeDistribution.cpp monStaGen/incrementalDeterministicGraphGenerator.h
	$(CC) $(CFLAGS) -c monStaGen/shiftingDegreeDistribution.cpp
clean:
	rm -rf *.o test
