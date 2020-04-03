CC=g++
CFLAGS=-g -O2 -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -std=c++11 -I ../../libs

all: test

test: test.o queryinterface.o pugixml.o argparse.o
	$(CC) test.o queryinterface.o pugixml.o argparse.o -o test

test.o: test.cpp queryinterface.h
	$(CC) $(CFLAGS) -c test.cpp

queryinterface.o: queryinterface.cpp queryinterface.h
	$(CC) $(CFLAGS) -c queryinterface.cpp

pugixml.o: ../../libs/pugixml/pugixml.cpp ../../libs/pugixml/pugixml.hpp
	$(CC) $(CFLAGS) -c ../../libs/pugixml/pugixml.cpp

argparse.o: ../../libs/argparse/argparse.cpp ../../libs/argparse/argparse.hpp
	$(CC) $(CFLAGS) -c ../../libs/argparse/argparse.cpp

clean:
	rm -rf *.o test
