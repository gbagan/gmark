#!/bin/bash

folder="../demo/social/"

cd ../../src
./test -c ../use-cases/social-network.xml -g "$folder"social-graph.txt -w "$folder"social-workload.xml -r "$folder"

cd querytranslate
./test -w ../"$folder"social-workload.xml -o ../"$folder"social-translated

cd ../queryinterface
./test -w ../"$folder"social-workload.xml -t ../"$folder"social-translated -o ../"$folder"social-interface
