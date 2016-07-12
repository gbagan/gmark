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
    int arity;
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
