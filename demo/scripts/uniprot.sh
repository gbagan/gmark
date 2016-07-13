#!/bin/bash

folder="../demo/uniprot/"

cd ../../src
./test -c ../use-cases/uniprot.xml -g "$folder"uniprot-graph.txt -w "$folder"uniprot-workload.xml -r "$folder"

cd querytranslate
./test -w ../"$folder"uniprot-workload.xml -o ../"$folder"uniprot-translated

cd ../queryinterface
./test -w ../"$folder"uniprot-workload.xml -t ../"$folder"uniprot-translated -o ../"$folder"uniprot-interface
