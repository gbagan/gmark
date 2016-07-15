#ifndef GMARK_REPORT_H
#define GMARK_REPORT_H

#include "randomgen.h"
#include <iostream>

namespace report {

using namespace std;

class predicate {
public:
    size_t size;
    string alias;
};

class report {
public:
    size_t nb_nodes;
    size_t nb_edges;
    double exec_time;
    vector<predicate> predicates;
};

class workload_report {
public:
    double exec_time;
    size_t min_conjuncts;
    size_t max_conjuncts;
    size_t min_disjuncts;
    size_t max_disjuncts;
    size_t min_length;
    size_t max_length;
    size_t min_arity;
    size_t max_arity;
    double percentage_of_stars;
    size_t nb_chains;
    size_t nb_stars;
    size_t nb_starchains;
    size_t nb_cycles;
    size_t nb_arity_2;
    size_t nb_constant;
    size_t nb_linear;
    size_t nb_quadratic;
};
}

#endif
