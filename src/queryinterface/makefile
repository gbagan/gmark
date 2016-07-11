CC=g++
CFLAGS=-g -O2 -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -std=c++11

all: test 

test: test.o queryinterface.o pugixml.o
	$(CC) test.o queryinterface.o pugixml.o -o test

test.o: test.cpp queryinterface.h
	$(CC) $(CFLAGS) -c test.cpp

queryinterface.o: queryinterface.cpp queryinterface.h
	$(CC) $(CFLAGS) -c queryinterface.cpp

pugixml.o: pugixml.cpp pugixml.hpp 
	$(CC) $(CFLAGS) -c pugixml.cpp

clean:
	rm -rf *.o test
