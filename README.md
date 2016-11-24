# gMark

gMark is a domain- and query language-independent framework targeting highly tunable generation of both graph instances and graph query workloads based on user-defined schemas.

**For more details about gMark, please refer to our technical report: http://arxiv.org/abs/1511.08386**

**gMark was demonstrated in VLDB 2016. The gMark research paper has been accepted in the TKDE journal.** 

If you use gMark, please cite:

    @article{BBCFLA17,
      author = {Bagan, G. and Bonifati, A. and Ciucanu, R. and Fletcher, G. H. L. and Lemay, A. and Advokaat, N.},
      title = {{gMark}: Schema-Driven Generation of Graphs and Queries},
      journal = {IEEE Transactions on Knowledge and Data Engineering},
      year = {2017 (to appear)}
    }

and/or

    @article{BBCFLA16,
      author = {Bagan, G. and Bonifati, A. and Ciucanu, R. and Fletcher, G. H. L. and Lemay, A. and Advokaat, N.},
      title = {Generating Flexible Workloads for Graph Databases},
      journal = {PVLDB},
      volume = {9},
      number = {13},
      pages = {1457--1460},
      year = {2016}
    }



# How to use gMark

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

**Provided examples**

We provide several examples of generated graphs, query workloads in internal format, html reports, translated queries, and query workload interfaces.

You can find them in the directory *demo*, subdirectories *test*, *test-a*, *shop*, *shop-a*, *social*, *social-a*, *uniprot*, *uniprot-a*. 

These scenarios basically correspond to the aforementioned four configuration files from *use-cases*, and for each of them we generated versions without or with aliases (i.e., using integers as predicates or using real-world predicates specified in the configuration file, respectively).
