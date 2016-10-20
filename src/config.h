#ifndef GMARK_CONFIG_H
#define GMARK_CONFIG_H

#include "randomgen.h"
#include <iostream>
#include <sstream>

namespace config {

using namespace std;

class type {
public:
    string alias;
    size_t size;
    bool scalable;
    double proportion;

    type () {
        alias = "";
        size = 0;
        scalable = true;
        proportion = 0.0;
    }
    
    type (const string & alias_, size_t size_, bool scalable_, double proportion_) {
        alias = alias_;
        size = size_;
        scalable = scalable_;
        proportion = proportion_;
    }
};

class predicate {
public:
   string alias;
   size_t size;
   double proportion;
   
   predicate () {
       alias = "";
       size = 0;
       proportion = 0;
   }
  
   predicate (const string & alias_, size_t size_, double proportion_) {
        alias = alias_;
        size = size_;
        proportion = proportion_;
    }
};


class edge {
public:
    size_t subject_type;
    size_t predicate;
    size_t object_type;
    char multiplicity; // { '*', '+', '?', '1' }
    int edge_type_id;
    distribution outgoing_distrib;
    distribution incoming_distrib;
    edge (size_t s_type, size_t pred, size_t o_type, char multiplicity, int edgeTypeId,
                const distribution & odistrib, const distribution & idistrib);
};

class schem {
public:
    vector<edge> edges;
    
public:
    void add_edge(size_t subject_type,  size_t predicate, size_t object_type, char multiplicity, int edgeTypeId,
                  const distribution & outgoing_dist, const distribution & incoming_dist);
    
    void add_edge(size_t subject_type, size_t predicate, size_t object_type, char multiplicity, int edgeTypeI,
                  DISTRIBUTION::type type, double arg1, double arg2);
    
    void add_edge(size_t subject_type, size_t predicate, size_t object_type, char multiplicity, int edgeTypeId,
                  DISTRIBUTION::type outgoing_type, double outgoing_arg1, double outgoing_arg2,
                  DISTRIBUTION::type incoming_type, double incoming_arg1, double incoming_arg2);
    
    /*
    vector<config_edge>::iterator begin() {
        return edges.begin();
    }
    
    vector<config_edge>::iterator end() {
        return edges.end();
    }
    */
    
};

class workload_type {
public:
    bool chain;
    bool star;
    bool cycle;
    bool starchain;
};

class workload_output {
public:
    bool sparql;
    bool postgres;
    bool cypher;
};

class workload_selectivity {
public:
    bool constant;
    bool linear;
    bool quadratic;
};

class workload {
public:
    unsigned int id;
    size_t size;
    double multiplicity;
    pair<size_t,size_t> conjuncts;
    pair<size_t,size_t> disjuncts;
    pair<size_t,size_t> length;
    pair<size_t,size_t> arity;
    workload_type type;
    workload_output output;
    workload_selectivity selectivity;
};

class config {
public:
    string input;
    size_t nb_nodes;
    size_t nb_edges;
    vector<type> types;
    distribution node_distribution;     
    vector<predicate> predicates;
    distribution predicate_distribution;
    schem schema;
    vector<workload> workloads;
    bool print_alias;    

    void complete_config();
    void complete_types_config();
    void complete_predicates_config();
};

namespace selectivity {
    enum type {
        CONSTANT,
        LINEAR,
        QUADRATIC
    };
}

ostream & operator << (ostream& stream, selectivity::type type);


}


#endif
