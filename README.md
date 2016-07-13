# gMark

**To compile the code:**

    cd demo/scripts
    ./compile-all.sh

**To generate an entire workflow, use the prepared script play.sh:**
(suppose we are currently in demo/scripts)

    ./play.sh

This executes the following three steps:

1. Generation of the graph and query workload in internal format, and html reports for both

    ./test -c ../use-cases/test.xml -g ../demo/play/play-graph.txt -w ../demo/play/play-workload.xml -r ../demo/play/

where the parameters are:
- -c : the configuration file
- -g : the output file for the graph instance
- -w : the output file for the queries generated on this instance
- -r : the output directory for the html reports
and optionally
- -a : to use aliases for the predicates in the generated graph and queries
- -n : to specify the number of nodes in the graph (it overrides the parameter from the config file)

2. Translation of the queries into the four concrete syntaxes

    ./test -w ../../demo/play/play-workload.xml -o ../../demo/play/play-translated

3. Generation of the query workload interface

    ./test -w ../../demo/play/play-workload.xml -t ../../demo/play/play-translated -o ../../demo/play/play-interface


**For more details about the gMark project, please refer to our technical report:
http://arxiv.org/abs/1511.08386**


**gMark will be demonstrated in VLDB 2016**

If you use gMark, please cite:

    @article{BBCFLA16,
      author = {G. Bagan and A. Bonifati and R. Ciucanu and G. H. L. Fletcher and A. Lemay and N. Advokaat},
      title = {Generating Flexible Workloads for Graph Databases},
      journal = {PVLDB},
      volume = {9},
      number = {13},
      year = {2016}
    }


