# gMark

**To generate graph instances and query workloads using a graph configuration:**

    cd src
    make
    ./test -c ../use-cases/test.xml -g ../graph.txt -w ../xml/queries.xml -s

where the parameters are:
- -c : the configuration file
- -q : the output file for the graph instance
- -w : the output file for the queries generated on this instance
- -s : to enforce selectivity estimation on binary queries

The provided configuration file *use-cases/test.xml* encodes the schema of a bibliographical graph database.

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
