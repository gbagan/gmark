#!/bin/bash

folder="../demo/scenarios/test/"

cd ../../src
./test -c ../use-cases/test.xml -g "$folder"test-graph.txt -w "$folder"test-workload.xml -r "$folder"

cd querytranslate
./test -w ../"$folder"test-workload.xml -o ../"$folder"test-translated

cd ../queryinterface
./test -w ../"$folder"test-workload.xml -t ../"$folder"test-translated -o ../"$folder"test-interface
