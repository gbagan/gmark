/* gMark
 * Copyright (C) 2015-2016 Guillaume Bagan <guillaume.bagan@liris.cnrs.fr>
 * Copyright (C) 2015-2016 Angela Bonifati <angela.bonifati@univ-lyon1.fr>
 * Copyright (C) 2015-2016 Radu Ciucanu <radu.ciucanu@cs.ox.ac.uk>
 * Copyright (C) 2015-2016 George Fletcher <g.h.l.fletcher@tue.nl>
 * Copyright (C) 2015-2016 Aurélien Lemay <aurelien.lemay@univ-lille3.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/


#ifndef GMARK_CONFIG_H
#define GMARK_CONFIG_H

#include "randomgen.h"
#include <iostream>

namespace config {

using namespace std;

class type {
public:
    string alias;
    size_t size;
    bool scalable;

    type () {
        alias = "";
        size = 0;
        scalable = true;
    }
    
    type (const string & alias_, size_t size_, bool scalable_) {
        alias = alias_;
        size = size_;
        scalable = scalable_;
    }
};

class predicate {
public:
   string alias;
   size_t size;
   
   predicate () {
       alias = "";
       size = 0;
   }
  
   predicate (const string & alias_, double size_) {
        alias = alias_;
        size = size_;
    }
};


class edge {
public:
    size_t subject_type;
    size_t predicate;
    size_t object_type;
    char multiplicity; // { '*', '+', '?', '1' }
    distribution outgoing_distrib;
    distribution incoming_distrib;
    edge (size_t s_type, size_t pred, size_t o_type, char multiplicity,
                const distribution & odistrib, const distribution & idistrib);
};

class schem {
public:
    vector<edge> edges;
    
public:
    void add_edge(size_t subject_type,  size_t predicate, size_t object_type, char multiplicity,
                  const distribution & outgoing_dist, const distribution & incoming_dist);
    
    void add_edge(size_t subject_type, size_t predicate, size_t object_type, char multiplicity,
                  DISTRIBUTION::type type, double arg1, double arg2);
    
    void add_edge(size_t subject_type, size_t predicate, size_t object_type, char multiplicity,
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
    size_t nb_nodes;
    size_t nb_edges;
    vector<type> types;
    distribution node_distribution;     
    vector<predicate> predicates;
    distribution predicate_distribution;
    schem schema;
    vector<workload> workloads;
    
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
