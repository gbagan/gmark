CC=g++
CFLAGS=-g -O2 -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -std=c++11

all: test 

test: test.o querytranslate.o pugixml.o
	$(CC) test.o querytranslate.o pugixml.o -o test

test.o: test.cpp querytranslate.h
	$(CC) $(CFLAGS) -c test.cpp

querytranslate.o: querytranslate.cpp querytranslate.h
	$(CC) $(CFLAGS) -c querytranslate.cpp

pugixml.o: pugixml.cpp pugixml.hpp 
	$(CC) $(CFLAGS) -c pugixml.cpp

clean:
	rm -rf *.o test
