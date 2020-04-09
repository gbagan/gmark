CC=g++
CFLAGS=-g -O2 -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -std=c++11 -I ../../libs

all: test

test: test.o querytranslate.o pugixml.o argparse.o
	$(CC) test.o querytranslate.o pugixml.o argparse.o -o test

test.o: test.cpp querytranslate.h
	$(CC) $(CFLAGS) -c test.cpp

querytranslate.o: querytranslate.cpp querytranslate.h
	$(CC) $(CFLAGS) -c querytranslate.cpp

pugixml.o: ../../libs/pugixml/pugixml.cpp ../../libs/pugixml/pugixml.hpp
	$(CC) $(CFLAGS) -c ../../libs/pugixml/pugixml.cpp

argparse.o: ../../libs/argparse/argparse.cpp ../../libs/argparse/argparse.hpp
	$(CC) $(CFLAGS) -c ../../libs/argparse/argparse.cpp

clean:
	rm -rf *.o test
