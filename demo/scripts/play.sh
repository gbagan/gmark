#!/bin/bash

cd ../../src
./test -c ../use-cases/test.xml -g ../demo/play/play-graph.txt -w ../demo/play/play-workload.xml -r ../demo/play/

cd querytranslate
./test -w ../../demo/play/play-workload.xml -o ../../demo/play/play-translated

cd ../queryinterface
./test -w ../../demo/play/play-workload.xml -t ../../demo/play/play-translated -o ../../demo/play/play-interface
