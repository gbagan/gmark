#!/bin/bash

folder="../demo/shop-a/"

cd ../../src
./test -c ../use-cases/shop.xml -g "$folder"shop-a-graph.txt -w "$folder"shop-a-workload.xml -r "$folder" -a

cd querytranslate
./test -w ../"$folder"shop-a-workload.xml -o ../"$folder"shop-a-translated

cd ../queryinterface
./test -w ../"$folder"shop-a-workload.xml -t ../"$folder"shop-a-translated -o ../"$folder"shop-a-interface
