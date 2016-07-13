#!/bin/sh

cd src
make
./test -c ../use-cases/test.xml -g ../graph.txt -w ../xml/queries.xml

cd querytranslate
make
./test

cd ../queryinterface
make
test
