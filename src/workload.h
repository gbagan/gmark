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

#ifndef GMARK_WORKLOAD_H
#define GMARK_WORKLOAD_H

#include "config.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace workload {

using namespace std;

class Info {
public:
    int id;
    int number;
    config::selectivity::type selectivity;
};


class symbol {
public:
    size_t id;
    bool reverse;
    
    symbol(size_t, bool);
    symbol(long);
};

class disjunct {
public:
    vector<symbol> symbols;
};

class conjunct {
public:
    vector<disjunct> disjuncts;
    bool star;
    string source;
    string target;
};

class body {
public:
    vector<conjunct> conjuncts;
};

class query {
public:
    Info info;
    vector<string> variables;
    vector<body> bodies;
};


class workload {
public:
    vector<query> queries;    
};



void write_xml(const symbol &, ostream &);
void write_xml(const disjunct &, ostream &);
void write_xml(const conjunct &, ostream &);
void write_xml(const body &, ostream &);
void write_xml(const query &, ostream &);
void write_xml(const workload &, ostream &);

class graph {
public:
    vector<unordered_set<size_t>> neighbors;
};

class automaton {
public:
    vector<unordered_map<long,unordered_set<size_t>>> transitions;
    
    
public:
    automaton(const config::config & conf);
};


void distance_matrix_between_types(const config::config &, vector<vector<size_t>> &);
void compute_workload_graph_from_matrix(const vector<vector<size_t>> &, size_t, graph &);
void number_of_paths(const graph &, size_t, vector<vector<size_t>> &);
void generate_random_path(const graph &, const vector<vector<size_t>> &, long, size_t, double, vector<size_t> &);

void generate_random_disjunct(const automaton &, size_t, size_t, size_t, size_t, disjunct &);
void generate_random_conjunct(const config::config &, const config::workload &, size_t, size_t, conjunct &);

void generate_chain(const config::config &, const config::workload &, query &);
void generate_star(const config::config &, const config::workload &, query &);
void generate_cycle(const config::config &, const config::workload &, query &);
void generate_starchain(const config::config &, const config::workload &, query &);
void generate_query(const config::config &, const config::workload &, query &);
void generate_workload(const config::config &, workload &);

}

#endif
