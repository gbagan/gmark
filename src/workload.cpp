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

#include "workload.h"

#include <iostream>

namespace workload {

symbol::symbol(size_t id_, bool reverse_) {
    id = id_;
    reverse = reverse_;
}

symbol::symbol(long id_) {
    if (id_ >= 0) {
        id = id_;
        reverse = false;
    }
    else {
        id = -id_ - 1;
        reverse = true;
    }
}


void write_xml(const symbol & symb, ostream & stream) {
    stream << "          <symbol";
    if (symb.reverse) {
        stream << " inverse=\"true\"";
    }
    stream << ">" << symb.id << "</symbol>\n";
}    

void write_xml(const disjunct & disj, ostream & stream) {
    stream << "        <concat>\n";
    for (const auto & symb : disj.symbols) {
        write_xml(symb, stream);
    }
    stream << "        </concat>\n";
}    
    
void write_xml(const conjunct & conj, ostream & stream) {
    stream << "      <conjunct src=\"" << conj.source << "\" trg=\"" << conj.target << "\">\n";
    if (conj.star) {
        stream << "      <star>\n";
    }
    stream << "      <disj>\n";
    for (const auto & disj : conj.disjuncts) {
        write_xml(disj, stream);
    }
    stream << "      </disj>\n";
    if (conj.star) {
        stream << "      </star>\n";
    }
    stream << "      </conjunct>\n";
}

void write_xml(const body & body, ostream & stream) {
    stream << "    <body>\n";
    for (const auto & conj : body.conjuncts) {
        write_xml(conj, stream);
    }
    stream << "    </body>\n";
}

void write_xml(const query & query, ostream & stream) {
    stream << "<query>\n";
    stream << "  <metadata>\n";
    stream << "    <id>" << query.info.id << "</id>\n";
    stream << "    <number>" << query.info.number << "</number>\n";
    stream << "    <selectivity>" << query.info.selectivity << "</selectivity>\n";
    stream << "  </metadata>\n";

    stream << "  <head>\n";
    for (auto & var : query.variables) {
        stream << "    <var>" << var << "</var>\n";
    }
    stream << "  </head>\n";
    stream << "  <bodies>\n";
    for (const auto & body : query.bodies) {
        write_xml(body, stream);
    }
    
    stream << "  </bodies>\n";
    stream << "</query>\n";
    
}

void write_xml(const workload & wl, ostream & stream) {
    stream << "<queries>\n";
    for (auto & query : wl.queries) {
        write_xml(query, stream);
        stream << "\n";
    }
    stream << "</queries>\n";
}


automaton::automaton(const config::config & conf) {
    size_t size = conf.types.size();
    transitions.resize(size);
    for (const auto & edge : conf.schema.edges) {
        transitions[edge.subject_type][edge.predicate].insert(edge.object_type);
        transitions[edge.object_type][-edge.predicate-1].insert(edge.subject_type);
    }
}

void distance_matrix_between_types(const config::config & conf, vector<vector<size_t>> & matrix) {
    size_t size = conf.types.size();
    int log_size = (int) (log(size) + 1);
    
    vector<vector<size_t>> temp_matrix;
    
    matrix.resize(size);
    temp_matrix.resize(size);
    for (size_t i = 0; i < size; i++) {
        matrix[i].resize(size);
        temp_matrix[i].resize(size);
    }
    
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            matrix[i][j] = SIZE_MAX;
        }
    }
    
    for (const auto & edge : conf.schema.edges) {
        matrix[edge.subject_type][edge.object_type] = 1;
        matrix[edge.object_type][edge.subject_type] = 1;
    }
    
    for (int step = 0; step < log_size; step++) {
        // copy matrix into temp_matrix
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                temp_matrix[i][j] = matrix[i][j];
            }
        }
        
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                size_t m = matrix[i][j];
                for (size_t k = 0; k < size; k++) {
                    if(temp_matrix[i][k] < SIZE_MAX && temp_matrix[k][j] < SIZE_MAX) {
                        m = min(m, temp_matrix[i][k]+temp_matrix[k][j]);
                    }
                }
                matrix[i][j] = m;
            }
        }
    }
}

void compute_workload_graph_from_matrix(const vector<vector<size_t>> & matrix, size_t threshold, graph & g) {
    size_t n = matrix.size();
    g.neighbors.resize(n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if(matrix[i][j] <= threshold)
                g.neighbors[i].insert(j);
        }
    }
}

/*
 * computes a matrix m such that m[i][j] is the number of paths of length i and starting from the node j
 */
void number_of_paths(const graph & g, size_t n, vector<vector<size_t>> & matrix) {
    size_t m = g.neighbors.size();
    matrix.resize(n+1);
    for (size_t i = 0; i <= n; i++) {
        matrix[i].resize(m);
    }
    
    for (size_t j = 0; j < m; j++) {
        matrix[0][j] = 1;
    }
    
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (size_t k : g.neighbors[j]) {
                matrix[i][j] += matrix[i-1][k];
            }
        }
    }
}

/*
 * generates a random path of the form [a_1, s_1, a_2, s_2, ... s_{len}, a_{len+1}]
 * where a_i are node types and s_i indicates the presence or not of a star in the conjunct between a_i and a_{i+1}
 * matrix is the resut of number_of_paths()
 */

void generate_random_path(const graph & g, const vector<vector<size_t>> & matrix,
                          long first_node, size_t len, double star, vector<size_t> & path) {
    size_t current_node = 0;
    
    if (first_node >= 0) {
        current_node = first_node;
    }
    else {
        size_t m = g.neighbors.size();
        size_t nb_paths = 0;
        for(size_t i = 0; i < m; i++) {
            nb_paths += matrix[len][i];
        }
    
        // TODO fails if nb_paths == 0
    
        size_t rnd = uniform_random_generator(1, nb_paths).next(); //TODO averifier
        size_t acc = 0;
        current_node = 0;
        for(size_t i = 0; i < m; i++) {
            acc += matrix[len][i];
            if (acc >= rnd) {
                current_node = i;
                break;
            }
        }
    }
    path.push_back(current_node);
    for (size_t i = len; i > 0; i--) {
        if (g.neighbors[current_node].count(current_node) && random_0_1() <= star) {
            path.push_back(1);
            path.push_back(current_node);
        }
        else {
            size_t rnd = uniform_random_generator(1, matrix[i][current_node]).next(); // TODO averifier
            size_t acc = 0;
            for(size_t j : g.neighbors[current_node]) {
                acc += matrix[i-1][j];
                if (acc >= rnd) {
                    current_node = j;
                    break;
                }
            }
            path.push_back(0);
            path.push_back(current_node);
        }
    }
}

    
    
/*
computes a matrix m such that m[i][j] is the number of paths of length i and starting from the node j
and ending with v
*/
void number_of_paths2(const automaton & automat, size_t n, size_t v, vector<vector<size_t>> & matrix) {
    size_t m = automat.transitions.size();
    matrix.resize(n+1);
    for (size_t i = 0; i <= n; i++) {
        matrix[i].resize(m);
    }
    
    matrix[0][v] = 1;
    
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (const auto & transitions : automat.transitions[j]) {
                for (size_t target : transitions.second) {
                    matrix[i][j] += matrix[i-1][target];
                }
            }
        }
    }
    
    /*for(size_t i = 0; i <= n; i++) {
        for (size_t j = 0; j < m; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
    cout << endl;
    */
    
}

void generate_random_path_aux2(const automaton & automat, const vector<vector<size_t>> & matrix,
                          size_t start, size_t len, size_t max_len, vector<long> & path) {
    size_t acc;
    size_t rnd;
    size_t current_node = start;
    
    //cout << "starting from " << start << endl;
    
    size_t i = len;
    while (i <= max_len && matrix[i][current_node] == 0) {
        i++;
    }
    
    if (i > max_len) {
        i = len;
        while (i > 0 && matrix[i][current_node] == 0) {
            i--;
        }
    }
    
    if (i == 0) {
        cerr << "error: generate_random_path_aux2" << endl;
        exit(EXIT_FAILURE);
    }
        
    for (; i > 0; i--) {
        rnd = uniform_random_generator(1, matrix[i][current_node]).next(); // TODO averifier
        acc = 0;
        for (const auto & transitions : automat.transitions[current_node]) {
            for (size_t target : transitions.second) {
                acc += matrix[i-1][target];
                //cout << "acc: " << acc << " " << rnd << " " << matrix[i][current_node] << endl;
                if (acc >= rnd) {
                    current_node = target;
                    path.push_back(transitions.first);
                    //cout << "putting edge " << transitions.first << " " << target << endl;
                    break;
                }
            }
            if (acc >= rnd)
                break;
        }
         
    }
}

/*
 * generate a random disjunct
 * source: source type
 * target: target type
 * len: number of symbols in the disjunct
 */

void generate_random_disjunct(const automaton & automat, size_t source, size_t target,
                              size_t len, size_t max_len, disjunct & disj) {
    vector<vector<size_t>> matrix;
    vector<long> path;
    number_of_paths2(automat, max_len, target, matrix);
    generate_random_path_aux2(automat, matrix, source, len, max_len, path);

    for (long pred : path) {
        symbol s(pred);
        disj.symbols.push_back(s);
    }
    
}    


void generate_random_conjunct(const config::config & conf, const config::workload & wconf, size_t source, size_t target, conjunct & conj) {
    automaton automat(conf);
    size_t nb_disjuncts = uniform_random_generator(wconf.disjuncts.first, wconf.disjuncts.second).next();
    conj.disjuncts.resize(nb_disjuncts);
    auto length_gen = uniform_random_generator(wconf.length.first, wconf.length.second);
    for (auto & disjunct : conj.disjuncts) {
        size_t len = length_gen.next();
        generate_random_disjunct(automat, source, target, len, wconf.length.second, disjunct);
    }
}

void generate_chain(const config::config & conf, const config::workload & wconf, query & q) {
    size_t nb_conjs = uniform_random_generator(wconf.conjuncts.first, wconf.conjuncts.second).next();
    q.bodies.emplace_back();
    auto & body = q.bodies[0];
    body.conjuncts.resize(nb_conjs);
    
    vector<vector<size_t>> distmat;
    distance_matrix_between_types(conf, distmat);
    graph g;
    compute_workload_graph_from_matrix(distmat, wconf.length.second, g);
    vector<vector<size_t>> pathmat;
    number_of_paths(g, nb_conjs, pathmat);
    vector<size_t> path;
    generate_random_path(g, pathmat, -1, nb_conjs, wconf.multiplicity, path);
    
    for(size_t i = 0; i < nb_conjs; i++) {
        auto & conjunct = body.conjuncts[i];
        conjunct.source = "?x" + to_string(i);
        conjunct.target = "?x" + to_string(i+1);
        conjunct.star = path[2*i+1];
        generate_random_conjunct(conf, wconf, path[2*i], path[2*i+2], conjunct);
    }
    
    size_t arity = uniform_random_generator(wconf.arity.first, wconf.arity.second).next();
    if (arity == 1) {
        q.variables.push_back("?x0");
    }
    else if (arity >= 2) {
        q.variables.push_back("?x0");
        q.variables.push_back("?x" + to_string(nb_conjs));
    }
}

void generate_star(const config::config & conf, const config::workload & wconf, query & q) {
    size_t nb_conjs = uniform_random_generator(wconf.conjuncts.first, wconf.conjuncts.second).next();
    q.bodies.emplace_back();
    auto & body = q.bodies[0];
    body.conjuncts.resize(nb_conjs);
    
    vector<vector<size_t>> distmat;
    distance_matrix_between_types(conf, distmat);
    graph g;
    compute_workload_graph_from_matrix(distmat, wconf.length.second, g);
    vector<vector<size_t>> pathmat;
    number_of_paths(g, 1, pathmat);
    
    vector<size_t> path;
    generate_random_path(g, pathmat, -1, 1, wconf.multiplicity, path);
    auto & first_conjunct = body.conjuncts[0];
    first_conjunct.source = "?x0";
    first_conjunct.target = "?x1";
    first_conjunct.star = path[1];
    generate_random_conjunct(conf, wconf, path[0], path[2], first_conjunct);
    
    size_t central_type = path[0];
    
    for(size_t i = 1; i < nb_conjs; i++) {
        auto & conjunct = body.conjuncts[i];
        generate_random_path(g, pathmat, central_type, 1, wconf.multiplicity, path);
        conjunct.source = "?x0";
        conjunct.target = "?x" + to_string(i+1);
        conjunct.star = path[1];
        generate_random_conjunct(conf, wconf, central_type, path[2], conjunct);
    }
    
    size_t arity = uniform_random_generator(wconf.arity.first, wconf.arity.second).next();
    if (arity >= 1) {
        q.variables.push_back("?x0");
    }
}

void generate_cycle(const config::config & conf, const config::workload & wconf, query & q) {
}


void generate_starchain(const config::config & conf, const config::workload & wconf, query & q) {
}


void generate_query(const config::config & conf, const config::workload & wconf, query & q) {
    int sum = wconf.type.chain + wconf.type.star + wconf.type.cycle + wconf.type.starchain;
    int n = uniform_random_generator(1, sum).next();
    int i = 0;
    
    if (wconf.type.chain) {
        i++;
        if(i == n) {
            generate_chain(conf, wconf, q);
            return;
        }
    }
    
    if (wconf.type.star) {
        i++;
        if(i == n) {
            generate_star(conf, wconf, q);
            return;
        }
    }
    
    if (wconf.type.cycle) {
        i++;
        if(i == n) {
            generate_cycle(conf, wconf, q);
            return;
        }
    }
    
    if (wconf.type.starchain) {
        i++;
        if(i == n) {
            generate_starchain(conf, wconf, q);
            return;
        }
    }
}

void generate_workload(const config::config & conf, workload & wl) {
    size_t size = 0;
    size_t c = 0;
    
    for (const auto & wconf : conf.workloads) {
        size += wconf.size;
    }
    wl.queries.resize(size);
    
    for (const auto & wconf : conf.workloads) {
        for (size_t i = 0; i < wconf.size; i++) {
            generate_query(conf, wconf, wl.queries[c]);
            c++;
        }
    }
}

}
