# gMark

**To compile the code:**

    cd demo/scripts
    ./compile-all.sh

In the rest of the readme, suppose that we are in demo/scripts.

**To generate an entire workflow, use the prepared script play.sh:**

    ./play.sh

This executes the following three steps:

**1. Generation of the graph and query workload in internal format, and html reports for both**

    cd ../../src
    ./test -c ../use-cases/test.xml -g ../demo/play/play-graph.txt -w ../demo/play/play-workload.xml -r ../demo/play/

where the parameters are:
- -c : the configuration file
- -g : the output file for the graph instance
- -w : the output file for the query workload generated on this instance (in internal format)
- -r : the output directory for the html reports

and optionally

- -a : to use aliases for the predicates in the generated graph and queries
- -n : to specify the number of nodes in the graph (it overrides the parameter from the config file)

The provided configuration files in the directory *use-cases* are:
- *test.xml* : schema of a bibliographical graph database
- *shop.xml* : schema of an online shop (our gMark encoding of the default schema from WatDiv)
- *social-network.xml* : schema of a social network (our gMark encoding of the schema from LDBC SNB)
- *uniprot.xml* : schema of a protein network (our gMark encoding of the schema extracted from UniProt)

**2. Translation of the queries into the four concrete syntaxes**

    cd querytranslate
    ./test -w ../../demo/play/play-workload.xml -o ../../demo/play/play-translated

where the parameters are:
- -w : the query workload in internal format generated at step 1.
- -o : the output directory for the translations of the queries

**3. Generation of the query workload interface**

    cd ../queryinterface
    ./test -w ../../demo/play/play-workload.xml -t ../../demo/play/play-translated -o ../../demo/play/play-interface

where the parameters are:
- -w : the query workload in internal format generated at step 1.
- -t : the translations of the queries generated at step 2.
- -o : the output directory for the query workload interface


## For more details about the gMark project, please refer to our technical report: http://arxiv.org/abs/1511.08386


## gMark will be demonstrated in VLDB 2016

If you use gMark, please cite:

    @article{BBCFLA16,
      author = {G. Bagan and A. Bonifati and R. Ciucanu and G. H. L. Fletcher and A. Lemay and N. Advokaat},
      title = {Generating Flexible Workloads for Graph Databases},
      journal = {PVLDB},
      volume = {9},
      number = {13},
      year = {2016}
    }


