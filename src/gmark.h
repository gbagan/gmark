#ifndef GMARK_H
#define GMARK_H

#include <cstdlib>
#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <ostream>
#include <iostream>
#include <cmath>

#include "randomgen.h"
#include "config.h"

namespace graph {

using namespace std;

vector<size_t> generate_random_slots(pair<size_t,size_t> range, const distribution & distrib);

class abstract_graph_writer {
private:
    size_t nb_nodes;
    vector<pair<size_t,size_t>> node_ranges_per_type;
    vector<size_t> created_edges;
    
    //unordered_map<size_t, unordered_map<size_t, unordered_set<size_t>>> neighbors;
    void add_random_edges1(config::edge & c_edge);
    void add_random_edges2(config::edge & c_edge);
    void add_remaining_edges(size_t predicate, size_t nb_edges);

protected:
    config::config * conf;
    
public:
    void build_graph (config::config & conf);
    void add_vertices(size_t type, size_t size);
    void add_edge(size_t subject, size_t predicate, size_t object);
    virtual void print_edge(size_t subject, size_t predicate, size_t object) = 0;
    void add_random_edges(config::edge & c_edge);
};   

class ntriple_graph_writer : public abstract_graph_writer {
private:
    ostream * stream;

public:  
    ntriple_graph_writer (ostream & s);
    void print_edge(size_t subject, size_t predicate, size_t object);
};


}

#endif
