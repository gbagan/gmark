# gMark

**To generate graph instances and query workloads using a graph configuration:**

    cd src
    make
    ./test -c ../use-cases/test.xml -g ../graph.txt -w ../xml/queries.xml

where the parameters are:
- -c : the configuration file
- -g : the output file for the graph instance
- -w : the output file for the queries generated on this instance
- -a : to use aliases for the predicates in the generated graph and queries
- -n : to specify the number of nodes in the graph (it overrides the parameter from the config file)

The provided configuration files in the directory *use-cases* are:
- *test.xml* : schema of a bibliographical graph database
- *shop.xml* : schema of an oline shop (our gMark encoding of the default schema from WatDiv)
- *social-network.xml* : schema of a social network (our gMark encoding of the schema from LDBC SNB)

**To translate the generated queries from the internal XML encoding to the concrete query languages:**
(suppose we are currently in src)

    cd querytranslate
    make
    ./test

This will read the file 

	../../xml/queries.xml

and translate the queries from there in four files: 

- a.sql 
- a.sparql
- a.cypher
- a.logicblox

The file *xml/queries-examples.xml* illustrates our XML encoding of UCRPQ.

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


