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
