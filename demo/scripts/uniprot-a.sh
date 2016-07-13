#!/bin/bash

folder="../demo/uniprot-a/"

cd ../../src
./test -c ../use-cases/uniprot.xml -g "$folder"uniprot-a-graph.txt -w "$folder"uniprot-a-workload.xml -r "$folder" -a

cd querytranslate
./test -w ../"$folder"uniprot-a-workload.xml -o ../"$folder"uniprot-a-translated

cd ../queryinterface
./test -w ../"$folder"uniprot-a-workload.xml -t ../"$folder"uniprot-a-translated -o ../"$folder"uniprot-a-interface
