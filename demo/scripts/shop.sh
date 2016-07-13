#!/bin/bash

folder="../demo/shop/"

cd ../../src
./test -c ../use-cases/shop.xml -g "$folder"shop-graph.txt -w "$folder"shop-workload.xml -r "$folder"

cd querytranslate
./test -w ../"$folder"shop-workload.xml -o ../"$folder"shop-translated

cd ../queryinterface
./test -w ../"$folder"shop-workload.xml -t ../"$folder"shop-translated -o ../"$folder"shop-interface
