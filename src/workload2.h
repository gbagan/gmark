#ifndef GMARK_WORKLOAD2_H
#define GMARK_WORKLOAD2_H

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "config.h"
#include "workload.h"

namespace workload2 {

using namespace std;

namespace SELECTIVITY {
    enum type {
        ONEONE,
        NONE,
        ONEN,
        EQUALS,
        LESS,
        GREATER,
        LESS_GREATER,
        CROSS
    };
};

class matrix {
public:
    vector<vector<map<SELECTIVITY::type,size_t>>> data;
};

ostream & operator << (ostream&, const matrix &);

class graph {
public:
    vector<map<SELECTIVITY::type, set<pair<SELECTIVITY::type,pair<size_t,SELECTIVITY::type>>>>> neighbors;
};

class matrix_of_paths {
public:
    vector<vector<map<SELECTIVITY::type,size_t>>> data;
};

class triple {
public:
    size_t start;
    SELECTIVITY::type selectivity;
    size_t end;
    bool star;

    triple(size_t st, SELECTIVITY::type sel, size_t e, bool star_) {
        start = st;
        selectivity = sel;
        end = e;
        star = star_;
    }
};

ostream & operator << (ostream&, const triple &);

class automaton {
public:
    vector<map<SELECTIVITY::type, set<pair<long,pair<size_t,SELECTIVITY::type>>>>> transitions;

    automaton(const config::config & conf);
};

void distance_matrix_between_types(const config::config &, matrix &);
void compute_graph_from_matrix(const matrix &, size_t, graph &);
void number_of_paths(const graph &, config::selectivity::type, size_t, matrix_of_paths &);
void generate_random_path(const graph &, const matrix_of_paths &, long, size_t, double, vector<triple> &);

void generate_workload(const config::config & conf, workload::workload & wl);    


}




#endif