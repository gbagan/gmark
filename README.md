# gMark

**To compile the code:**

    cd demo/scripts
    ./compile-all.sh

**To generate an entire workflow, use the prepared script play.sh:**
(suppose we are currently in demo/scripts)

    ./play.sh

This executes the following three steps:

- Generation of the graph and query workload in internal format, and html reports for both
- Translation of the queries into the four concrete syntaxes
- Generation of the query workload interface




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


