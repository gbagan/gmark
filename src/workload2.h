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
