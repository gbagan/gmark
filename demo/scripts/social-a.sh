#!/bin/bash

folder="../demo/social-a/"

cd ../../src
./test -c ../use-cases/social-network.xml -g "$folder"social-a-graph.txt -w "$folder"social-a-workload.xml -r "$folder" -a

cd querytranslate
./test -w ../"$folder"social-a-workload.xml -o ../"$folder"social-a-translated

cd ../queryinterface
./test -w ../"$folder"social-a-workload.xml -t ../"$folder"social-a-translated -o ../"$folder"social-a-interface
