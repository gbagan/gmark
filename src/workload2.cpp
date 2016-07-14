#include "workload2.h"

#include <iostream>
#include <sys/time.h>
#include <algorithm>

namespace workload2 {

size_t MIN_CONJUNCTS = 100000000;
size_t MAX_CONJUNCTS = 0;
size_t MIN_DISJUNCTS = 100000000;
size_t MAX_DISJUNCTS = 0;
size_t MIN_LENGTH = 100000000;
size_t MAX_LENGTH = 0;

size_t NUMBER_OF_DISJUNCTS = 0;
size_t NUMBER_OF_STARS = 0;

size_t NB_CHAINS = 0;
size_t NB_STARS = 0;
size_t NB_STARCHAINS = 0;
size_t NB_CYCLES = 0;


namespace SELECTIVITY {
    vector<type> types = { ONEONE, NONE, ONEN, EQUALS, LESS, GREATER, LESS_GREATER, CROSS };
}

SELECTIVITY::type operator * (SELECTIVITY::type a, SELECTIVITY::type b) {
    switch(a) {
        case SELECTIVITY::ONEONE:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::ONEONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::ONEN;
                case SELECTIVITY::LESS:         return SELECTIVITY::ONEN;
                case SELECTIVITY::GREATER:      return SELECTIVITY::ONEN;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::ONEN;
                case SELECTIVITY::CROSS:        return SELECTIVITY::ONEN;
            }
        case SELECTIVITY::ONEN:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::ONEONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::ONEN;
                case SELECTIVITY::LESS:         return SELECTIVITY::ONEN;
                case SELECTIVITY::GREATER:      return SELECTIVITY::ONEN;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::ONEN;
                case SELECTIVITY::CROSS:        return SELECTIVITY::ONEN;
            }
        case SELECTIVITY::NONE:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::NONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::CROSS;      // averifier
                case SELECTIVITY::NONE:         return SELECTIVITY::NONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::NONE;
                case SELECTIVITY::LESS:         return SELECTIVITY::NONE;
                case SELECTIVITY::GREATER:      return SELECTIVITY::NONE;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::NONE;
                case SELECTIVITY::CROSS:        return SELECTIVITY::NONE;
            }       
        case SELECTIVITY::EQUALS:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::NONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::EQUALS;
                case SELECTIVITY::LESS:         return SELECTIVITY::LESS;
                case SELECTIVITY::GREATER:      return SELECTIVITY::GREATER;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::LESS_GREATER;
                case SELECTIVITY::CROSS:        return SELECTIVITY::CROSS;
            }
        case SELECTIVITY::LESS:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::NONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::LESS;
                case SELECTIVITY::LESS:         return SELECTIVITY::LESS;
                case SELECTIVITY::GREATER:      return SELECTIVITY::LESS_GREATER;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::LESS_GREATER;
                case SELECTIVITY::CROSS:        return SELECTIVITY::CROSS;
            }
        case SELECTIVITY::GREATER:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::NONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::GREATER;
                case SELECTIVITY::LESS:         return SELECTIVITY::CROSS;
                case SELECTIVITY::GREATER:      return SELECTIVITY::GREATER;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::CROSS;
                case SELECTIVITY::CROSS:        return SELECTIVITY::CROSS;
            }
        case SELECTIVITY::LESS_GREATER:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::NONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::LESS_GREATER;
                case SELECTIVITY::LESS:         return SELECTIVITY::CROSS;
                case SELECTIVITY::GREATER:      return SELECTIVITY::LESS_GREATER;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::CROSS;
                case SELECTIVITY::CROSS:        return SELECTIVITY::CROSS;
            }
        case SELECTIVITY::CROSS:
            switch(b) {
                case SELECTIVITY::ONEONE:       return SELECTIVITY::ONEONE;
                case SELECTIVITY::ONEN:         return SELECTIVITY::ONEN;
                case SELECTIVITY::NONE:         return SELECTIVITY::NONE;
                case SELECTIVITY::EQUALS:       return SELECTIVITY::CROSS;
                case SELECTIVITY::LESS:         return SELECTIVITY::CROSS;
                case SELECTIVITY::GREATER:      return SELECTIVITY::CROSS;
                case SELECTIVITY::LESS_GREATER: return SELECTIVITY::CROSS;
                case SELECTIVITY::CROSS:        return SELECTIVITY::CROSS;
            }
    }
    return SELECTIVITY::EQUALS;
}

SELECTIVITY::type operator - (SELECTIVITY::type a) {
    if (a == SELECTIVITY::NONE) {
        return SELECTIVITY::ONEN;
    }
    else if (a == SELECTIVITY::ONEN) {
        return SELECTIVITY::NONE;
    }
    else if (a == SELECTIVITY::LESS) {
        return SELECTIVITY::GREATER;
    }
    else if (a == SELECTIVITY::GREATER) {
        return SELECTIVITY::LESS;
    }
    else {
        return a;
    }
}

ostream & operator << (ostream& stream, SELECTIVITY::type type) {
    switch(type) {
        case SELECTIVITY::ONEONE:
            stream << "1=1";
            break;
        case SELECTIVITY::NONE:
            stream << "N>1";
            break;
        case SELECTIVITY::ONEN:
            stream << "1<N";
            break;
        case SELECTIVITY::EQUALS:
            stream << "N=N";
            break;
        case SELECTIVITY::LESS:
            stream << "N<N";
            break;
        case SELECTIVITY::GREATER:
            stream << "N>N";
            break;
        case SELECTIVITY::LESS_GREATER:
            stream << "N<>N";
            break;
        case SELECTIVITY::CROSS:
            stream << "NXN";
        default:
            stream << (int) type;
    }
    return stream;
}


SELECTIVITY::type get_selectivity_from_config_edge(const config::edge & edge, const config::config & conf) {
    if(!conf.types[edge.subject_type].scalable && !conf.types[edge.object_type].scalable)
        return SELECTIVITY::ONEONE;
    else if (conf.types[edge.subject_type].scalable && !conf.types[edge.object_type].scalable)
        return SELECTIVITY::NONE;
    else if (!conf.types[edge.subject_type].scalable && conf.types[edge.object_type].scalable)
        return SELECTIVITY::ONEN;
    else if (edge.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edge.incoming_distrib.type == DISTRIBUTION::ZIPFIAN)
        return SELECTIVITY::LESS_GREATER;
    else if (edge.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edge.incoming_distrib.type != DISTRIBUTION::ZIPFIAN)
        return SELECTIVITY::LESS;
    else if (edge.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edge.incoming_distrib.type == DISTRIBUTION::ZIPFIAN)
        return SELECTIVITY::GREATER;
    else
        return SELECTIVITY::EQUALS;
}

bool is_subclass_of(SELECTIVITY::type type, config::selectivity::type type2) {
    switch (type2) {
        case config::selectivity::CONSTANT:
            return (type == SELECTIVITY::ONEONE);
        case config::selectivity::LINEAR:
            return (type != SELECTIVITY::ONEONE && type != SELECTIVITY::CROSS);
        case config::selectivity::QUADRATIC:
            return (type == SELECTIVITY::CROSS);
        default:
            return false;
    }
}

ostream & operator << (ostream& stream, const matrix & mat) {
    size_t size = mat.data.size();
    
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            stream << "(";
            for (auto pair : mat.data[i][j]) {
                stream << pair.first << " " << pair.second << " "; 
            }
            stream << ") ";
        }
        stream << "\n";
    }
    return stream;
}    

ostream & operator << (ostream& stream, const triple & t) {
    stream << "(" << t.start << " " << t.selectivity << " " << t.end << ")"; 
    
    return stream;
}

void distance_matrix_between_types(const config::config & conf, matrix & mat) {
    size_t size = conf.types.size();
    int log_size = (int) (log(size) + 4);
    
    matrix temp_matrix;

    mat.data.resize(size);
    temp_matrix.data.resize(size);
    for (size_t i = 0; i < size; i++) {
        mat.data[i].resize(size);
        //temp_matrix.data[i].resize(size);
    }
    
    for (const auto & edge : conf.schema.edges) {
        auto selectivity = get_selectivity_from_config_edge(edge, conf);
        mat.data[edge.subject_type][edge.object_type][selectivity] = 1;
        mat.data[edge.object_type][edge.subject_type][-selectivity] = 1;
    }
    
    for (int step = 0; step < log_size; step++) {
        // copy matrix into temp_matrix
        temp_matrix.data = mat.data;
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                for (size_t k = 0; k < size; k++) {
                    for (auto pair1 : temp_matrix.data[i][k]) {
                        for (auto pair2 : temp_matrix.data[k][j]) {
                            auto selectivity = pair1.first * pair2.first;
                            size_t length = pair1.second + pair2.second;
                            if (mat.data[i][j].count(selectivity) > 0) {
                                mat.data[i][j][selectivity] = min(mat.data[i][j][selectivity], length);
                            }
                            else {
                                mat.data[i][j][selectivity] = length;
                            }
                        }
                    }
                }
            }
        }
    }
}

void compute_graph_from_matrix(const matrix & matrix, size_t threshold, graph & graph) {
    size_t n = matrix.data.size();
    graph.neighbors.resize(n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            for (auto sel1 : SELECTIVITY::types) {
                for (auto sel2 : SELECTIVITY::types) {
                    if (matrix.data[i][j].count(sel2) > 0 && matrix.data[i][j].at(sel2) <= threshold) {
                        graph.neighbors[i][sel1].insert(make_pair(sel2,make_pair(j,sel1*sel2)));
                    }
                }
            }
        }
    }
}


void number_of_paths(const graph & graph, config::selectivity::type type, size_t n, matrix_of_paths & matrix) {
    size_t m = graph.neighbors.size();
    matrix.data.resize(n+1);
    for (size_t i = 0; i <= n; i++) {
        matrix.data[i].resize(m);
    }
    
    for (size_t j = 0; j < m; j++) {
        for (auto sel : SELECTIVITY::types) {
            if (is_subclass_of(sel, type)) {
                matrix.data[0][j][sel] = 1;
            }
        }
    }
    
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (auto sel : SELECTIVITY::types) {
                if (graph.neighbors[j].count(sel) > 0) {
                    for (auto pair : graph.neighbors[j].at(sel)) {
                        matrix.data[i][j][sel] += matrix.data[i-1][pair.second.first][pair.second.second];
                    }
                }
            }
        }
    }
}

void generate_random_path(const graph & graph, const matrix_of_paths & matrix, long first_node, size_t len, double star, vector<triple> & path) {
    size_t current_node = 0;
    auto current_sel = SELECTIVITY::EQUALS;
    
    if (first_node >= 0) {
        current_node = first_node;   
    }
    else {
        size_t m = graph.neighbors.size();
        size_t nb_paths = 0;
        for(size_t i = 0; i < m; i++) {
            if (matrix.data[len][i].count(SELECTIVITY::EQUALS)) {
                nb_paths += matrix.data[len][i].at(SELECTIVITY::EQUALS);
            }
        }
    
        if (nb_paths == 0) {
            throw string("error: generate_random_path ") + to_string(__LINE__);
            //cerr << "error: generate_random_path " << __LINE__ << endl;
            //exit(-1);
        }
    
        size_t rnd = uniform_random_generator(1, nb_paths).next();
        size_t acc = 0;
        current_node = 0;
        for(size_t i = 0; i < m; i++) {
            if (matrix.data[len][i].count(SELECTIVITY::EQUALS)) {
                acc += matrix.data[len][i].at(SELECTIVITY::EQUALS);
            }
            if (acc >= rnd) {
                current_node = i;
                break;
            }
        }
    }
    
    if(matrix.data[len][current_node].count(current_sel) == 0 || matrix.data[len][current_node].at(current_sel) == 0) {
        throw string("error: generate_random_path ") + to_string(__LINE__);
        //cerr << "error: generate_random_path " << __LINE__ << endl;
        //exit(0);
    }
    
    bool has_star = false;
    
    for (size_t i = len; i > 0; i--) {
        bool test = false;
        
        for (const auto & pair : graph.neighbors[current_node].at(SELECTIVITY::EQUALS)) {
            if(pair.first == SELECTIVITY::EQUALS && pair.second.first == current_node) {
                test = true;
                break;
            }
        }
        
        if (/*!has_star && */ test && random_0_1() <= star) {
            path.emplace_back(current_node, SELECTIVITY::EQUALS, current_node, true);
            NUMBER_OF_DISJUNCTS++;
            NUMBER_OF_STARS++;
            has_star = true;
        }
        else {
            size_t previous_node = current_node;
            SELECTIVITY::type sel;
        
            size_t rnd = uniform_random_generator(1, matrix.data[i][current_node].at(current_sel)).next();
            size_t acc = 0;
            for(auto & pair : graph.neighbors[current_node].at(current_sel)) {
                acc += matrix.data[i-1][pair.second.first].at(pair.second.second);
                if (acc >= rnd) {
                    current_node = pair.second.first;
                    current_sel = pair.second.second;
                    sel = pair.first;
                    break;
                }
            }
            path.emplace_back(previous_node, sel, current_node, false);
            NUMBER_OF_DISJUNCTS++;
        }
    }
}

automaton::automaton(const config::config & conf) {
    size_t size = conf.types.size();
    transitions.resize(size);
    for (const auto & edge : conf.schema.edges) {
        auto sel2 = get_selectivity_from_config_edge(edge, conf);
        for (auto sel1 : SELECTIVITY::types) {
            transitions[edge.subject_type][sel1].insert(make_pair(edge.predicate, make_pair(edge.object_type, sel1*sel2)));
            transitions[edge.object_type][sel1].insert(make_pair(-edge.predicate-1, make_pair(edge.subject_type, sel1*(-sel2))));
        }
    }
}


void number_of_paths2(const automaton & automat, size_t n, size_t v, SELECTIVITY::type sel, matrix_of_paths & matrix) {
    size_t m = automat.transitions.size();
    matrix.data.resize(n+1);
    for (size_t i = 0; i <= n; i++) {
        matrix.data[i].resize(m);
    }

    matrix.data[0][v][sel] = 1;
    
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (auto sel2 : SELECTIVITY::types) {
                if (automat.transitions[j].count(sel2) > 0) {
                    for (auto pair : automat.transitions[j].at(sel2)) {
                        matrix.data[i][j][sel2] += matrix.data[i-1][pair.second.first][pair.second.second];
                    }
                }
            }
        }
    }
}

void generate_random_path_aux2(const automaton & automat, const matrix_of_paths & matrix,
                          size_t start, size_t len, size_t max_len, vector<long> & path) {
    size_t acc;
    size_t rnd;
    size_t current_node = start;
    SELECTIVITY::type current_selectivity = SELECTIVITY::EQUALS;
    
    //cout << "starting from " << start << endl;
    
    size_t i = len;
    
    
    while (i <= max_len && (matrix.data[len][current_node].count(current_selectivity) == 0 || matrix.data[i][current_node].at(current_selectivity) == 0)) {
        i++;
    }
    
    if (i > max_len) {
        i = len;
        while (i > 0 && (matrix.data[len][current_node].count(current_selectivity) == 0 || matrix.data[i][current_node].at(current_selectivity) == 0)) { 
            i--;
        }
    }
    
    if (i == 0) {
        cerr << "error: generate_random_path_aux2: " << __LINE__ << endl;
        exit(EXIT_FAILURE);
    }

    
    if (i < MIN_LENGTH)
        MIN_LENGTH = i;
    if (i > MAX_LENGTH)
        MAX_LENGTH = i;
        
    for (; i > 0; i--) {
        rnd = uniform_random_generator(1, matrix.data[i][current_node].at(current_selectivity)).next();
        acc = 0;
        for (const auto & transition : automat.transitions[current_node].at(current_selectivity)) {
            size_t next_node = transition.second.first;
            auto next_selectivity = transition.second.second;
            if (matrix.data[i-1][next_node].count(next_selectivity) > 0) {
                acc += matrix.data[i-1][next_node].at(next_selectivity);
            }
            //cout << "acc: " << acc << " " << rnd << " " << matrix[i][current_node] << endl;
            if (acc >= rnd) {
                current_node = next_node;
                current_selectivity = next_selectivity;
                path.push_back(transition.first);
                //cout << "putting edge " << transitions.first << " " << target << endl;
                break;
            }
        }
    }
}

void generate_random_disjunct(const automaton & automaton, size_t source, SELECTIVITY::type sel, size_t target,
                              size_t len, size_t max_len, workload::disjunct & disj) {
    matrix_of_paths mat;
    vector<long> path;
    number_of_paths2(automaton, max_len, target, sel, mat);
    generate_random_path_aux2(automaton, mat, source, len, max_len, path);

    for (long pred : path) {
        workload::symbol s(pred);
        disj.symbols.push_back(s);
    }
    
} 

void generate_random_conjunct(const config::config & conf, const config::workload & wconf, size_t source,
                              SELECTIVITY::type sel, size_t target, workload::conjunct & conj) {
    automaton automat(conf);
    size_t nb_disjuncts = uniform_random_generator(wconf.disjuncts.first, wconf.disjuncts.second).next();
    conj.disjuncts.resize(nb_disjuncts);

    if (nb_disjuncts < MIN_DISJUNCTS)
        MIN_DISJUNCTS = nb_disjuncts;
    if (nb_disjuncts > MAX_DISJUNCTS)
        MAX_DISJUNCTS = nb_disjuncts;


    auto length_gen = uniform_random_generator(wconf.length.first, wconf.length.second);
    for (auto & disjunct : conj.disjuncts) {
        size_t len = length_gen.next();
        generate_random_disjunct(automat, source, sel, target, len, wconf.length.second, disjunct);
    }
}

config::selectivity::type generate_random_selectivity(const config::workload & wconf) {
    int nb = wconf.selectivity.constant + wconf.selectivity.linear + wconf.selectivity.quadratic;
    int n = uniform_random_generator(1, nb).next();
    int i = 0;
    
    if (wconf.selectivity.constant) {
        i++;
        if(i == n) {
            return config::selectivity::CONSTANT;
        }
    }
    
    if (wconf.selectivity.linear) {
        i++;
        if(i == n) {
            return config::selectivity::LINEAR;
        }
    }
    
    if (wconf.selectivity.quadratic) {
        i++;
        if(i == n) {
            return config::selectivity::QUADRATIC;
        }
    }
    return config::selectivity::LINEAR;
}

void generate_arity_more_than_2(workload::query & q, size_t arity, bool is_cycle) {
    size_t nb_conjs = q.bodies[0].conjuncts.size();
    if (!is_cycle) {
        if (arity > nb_conjs+1)
            arity = nb_conjs+1;
    }
    else {
        if (arity > nb_conjs)
            arity = nb_conjs;
    } 
    for (size_t i = 0; i < arity; i++) {
        while (true) {
            size_t i = uniform_random_generator(0, arity-1).next();
            if(std::find(q.variables.begin(), q.variables.end(), "?x" + to_string(i)) == q.variables.end()) {
		q.variables.push_back("?x" + to_string(i));
                break;
            }
        }
    }
}

void generate_chain(const config::config & conf, const config::workload & wconf, workload::query & q) {
    size_t nb_conjs = uniform_random_generator(wconf.conjuncts.first, wconf.conjuncts.second).next();

    if (nb_conjs < MIN_CONJUNCTS)
        MIN_CONJUNCTS = nb_conjs;
    if (nb_conjs > MAX_CONJUNCTS)
        MAX_CONJUNCTS = nb_conjs;

    q.bodies.emplace_back();
    auto & body = q.bodies[0];
    body.conjuncts.resize(nb_conjs);
    
    matrix distmat;
    distance_matrix_between_types(conf, distmat);
    graph g;
    compute_graph_from_matrix(distmat, wconf.length.second, g);
    matrix_of_paths pathmat;
    auto selectivity = generate_random_selectivity(wconf);
    q.info.selectivity = selectivity;
    
    number_of_paths(g, selectivity, nb_conjs, pathmat);
    vector<triple> path;
    generate_random_path(g, pathmat, -1, nb_conjs, wconf.multiplicity, path);
    
    for(size_t i = 0; i < nb_conjs; i++) {
        auto & conjunct = body.conjuncts[i];
        conjunct.source = "?x" + to_string(i);
        conjunct.target = "?x" + to_string(i+1);
        conjunct.star = path[i].star;
        generate_random_conjunct(conf, wconf, path[i].start, path[i].selectivity, path[i].end, conjunct);
    }
    
    size_t arity = uniform_random_generator(wconf.arity.first, wconf.arity.second).next();
    q.info.arity = arity;

    if (arity == 1) {
        q.variables.push_back("?x0");
    }
    else if (arity == 2) {
        q.variables.push_back("?x0");
        q.variables.push_back("?x" + to_string(nb_conjs));
    }
    else if (arity > 2) {
        generate_arity_more_than_2(q, arity, false);
    }

}

void generate_star(const config::config & conf, const config::workload & wconf, workload::query & q) {
    size_t nb_conjs = uniform_random_generator(wconf.conjuncts.first, wconf.conjuncts.second).next();

    if (nb_conjs < MIN_CONJUNCTS)
        MIN_CONJUNCTS = nb_conjs;
    if (nb_conjs > MAX_CONJUNCTS)
        MAX_CONJUNCTS = nb_conjs;

    q.bodies.emplace_back();
    auto & body = q.bodies[0];
    body.conjuncts.resize(nb_conjs);
    
    matrix distmat;
    distance_matrix_between_types(conf, distmat);
    graph g;
    compute_graph_from_matrix(distmat, wconf.length.second, g);
    
    auto selectivity = generate_random_selectivity(wconf);
    q.info.selectivity = selectivity;
    
    matrix_of_paths pathmat;
    number_of_paths(g, selectivity, 1, pathmat);
    
    vector<triple> path;
    generate_random_path(g, pathmat, -1, 1, wconf.multiplicity, path);
    
    auto & first_conjunct = body.conjuncts[0];
    first_conjunct.source = "?x0";
    first_conjunct.target = "?x1";
    first_conjunct.star = path[0].star;
       
    generate_random_conjunct(conf, wconf, path[0].start, path[0].selectivity, path[0].end, first_conjunct);
    
    size_t central_type = path[0].start;
    
    for(size_t i = 1; i < nb_conjs; i++) {
        auto & conjunct = body.conjuncts[i];
        generate_random_path(g, pathmat, central_type, 1, wconf.multiplicity, path);
        conjunct.source = "?x0";
        conjunct.target = "?x" + to_string(i+1);
        conjunct.star = path[0].star;
        generate_random_conjunct(conf, wconf, central_type, path[0].selectivity, path[0].end, conjunct);
    }
    
    size_t arity = uniform_random_generator(wconf.arity.first, wconf.arity.second).next();
    q.info.arity = arity;

    if (arity == 1) {
        q.variables.push_back("?x0");
    }
    else if (arity == 2) {
        q.variables.push_back("?x0");
        q.variables.push_back("?x1");
    }
    else if (arity > 2) {
        generate_arity_more_than_2(q, arity, false);
    }
}


void generate_cycle(const config::config & conf, const config::workload & wconf, workload::query & q) {
    size_t nb_conjs = uniform_random_generator(wconf.conjuncts.first, wconf.conjuncts.second).next();

    if (nb_conjs < MIN_CONJUNCTS)
        MIN_CONJUNCTS = nb_conjs;
    if (nb_conjs > MAX_CONJUNCTS)
        MAX_CONJUNCTS = nb_conjs;

    q.bodies.emplace_back();
    auto & body = q.bodies[0];
    body.conjuncts.resize(nb_conjs);
    
    size_t n1 = nb_conjs/2;
    size_t n2 = nb_conjs - n1;
    
    matrix distmat;
    distance_matrix_between_types(conf, distmat);
    graph g;
    compute_graph_from_matrix(distmat, wconf.length.second, g);
    matrix_of_paths pathmat;
    auto selectivity = generate_random_selectivity(wconf);
    q.info.selectivity = selectivity;
       

    number_of_paths(g, selectivity, n1, pathmat);
    vector<triple> path;
    generate_random_path(g, pathmat, -1, n1, wconf.multiplicity, path);
    
    for(size_t i = 0; i < n1; i++) {
        auto & conjunct = body.conjuncts[i];
        conjunct.source = "?x" + to_string(i);
        conjunct.target = "?x" + to_string(i+1);
        conjunct.star = path[i].star;
        generate_random_conjunct(conf, wconf, path[i].start, path[i].selectivity, path[i].end, conjunct);
    }

    matrix_of_paths pathmat2;
    number_of_paths(g, selectivity, n2, pathmat2);
    vector<triple> path2;
    generate_random_path(g, pathmat2, path[0].start, n2, wconf.multiplicity, path2);
    
    for(size_t i = 0; i < n2; i++) {
        auto & conjunct = body.conjuncts[n1+i];
        if (i == 0)
            conjunct.source = "?x0";
        else
            conjunct.source = "?x" + to_string(n1+i);
        
        if (i == n2-1)
            conjunct.target = "?x" + to_string(n1);
        else
            conjunct.target = "?x" + to_string(n1+i+1);
        
        conjunct.star = path2[i].star;
        generate_random_conjunct(conf, wconf, path2[i].start, path2[i].selectivity, path2[i].end, conjunct);
    }
    
    size_t arity = uniform_random_generator(wconf.arity.first, wconf.arity.second).next();
    q.info.arity = arity;
    if (arity == 1) {
        q.variables.push_back("?x0");
    }
    else if (arity == 2) {
        q.variables.push_back("?x0");
        q.variables.push_back("?x" + to_string(n1));
    }
    else if (arity > 2) {
        generate_arity_more_than_2(q, arity, true);
    }
}


void generate_starchain(const config::config & conf, const config::workload & wconf, workload::query & q) {
    size_t nb_conjs = uniform_random_generator(wconf.conjuncts.first, wconf.conjuncts.second).next();

    if (nb_conjs < MIN_CONJUNCTS)
        MIN_CONJUNCTS = nb_conjs;
    if (nb_conjs > MAX_CONJUNCTS)
        MAX_CONJUNCTS = nb_conjs;

    q.bodies.emplace_back();
    auto & body = q.bodies[0];
    body.conjuncts.resize(nb_conjs);
    
    size_t n1 = (size_t) ceil ((double)nb_conjs/3.0);
    size_t n2 = (nb_conjs - n1) / 2;
    size_t n3 = nb_conjs - n1 - n2;
    
    matrix distmat;
    distance_matrix_between_types(conf, distmat);
    graph g;
    compute_graph_from_matrix(distmat, wconf.length.second, g);
    matrix_of_paths pathmat;
    auto selectivity = generate_random_selectivity(wconf);
    q.info.selectivity = selectivity;
    
    number_of_paths(g, selectivity, n1, pathmat);
    vector<triple> path;
    generate_random_path(g, pathmat, -1, n1, wconf.multiplicity, path);
    
    for(size_t i = 0; i < n1; i++) {
        auto & conjunct = body.conjuncts[i];
        conjunct.source = "?x" + to_string(i);
        conjunct.target = "?x" + to_string(i+1);
        conjunct.star = path[i].star;
        generate_random_conjunct(conf, wconf, path[i].start, path[i].selectivity, path[i].end, conjunct);
    }
    
    
    matrix_of_paths pathmat2;
    number_of_paths(g, selectivity, 1, pathmat2);
    
    vector<triple> path2;
    
    for(size_t i = 0; i < n2; i++) {
        auto & conjunct = body.conjuncts[n1+i];
        generate_random_path(g, pathmat2, path[0].start, 1, wconf.multiplicity, path2);
        conjunct.source = "?x0";
        conjunct.target = "?x" + to_string(n1+i+1);
        conjunct.star = path[0].star;
        generate_random_conjunct(conf, wconf, path2[0].start, path2[0].selectivity, path2[0].end, conjunct);
    }
    
    for(size_t i = 0; i < n3; i++) {
        auto & conjunct = body.conjuncts[n1+n2+i];
        generate_random_path(g, pathmat2, path[n1-1].end, 1, wconf.multiplicity, path2);
        conjunct.source = "?x" + to_string(n1);
        conjunct.target = "?x" + to_string(n1+n2+i+1);
        conjunct.star = path[0].star;
        generate_random_conjunct(conf, wconf, path2[0].start, path2[0].selectivity, path2[0].end, conjunct);
    }
    
    
    
    size_t arity = uniform_random_generator(wconf.arity.first, wconf.arity.second).next();
    q.info.arity = arity;

    if (arity == 1) {
        q.variables.push_back("?x0");
    }
    else if (arity == 2) {
        q.variables.push_back("?x0");
        q.variables.push_back("?x" + to_string(n1));
    }
    else if (arity > 2) {
        generate_arity_more_than_2(q, arity, false);
    }
}


void generate_query(const config::config & conf, const config::workload & wconf, workload::query & q) {
    int sum = wconf.type.chain + wconf.type.star + wconf.type.cycle + wconf.type.starchain;
    int n = uniform_random_generator(1, sum).next();
    int i = 0;


    if (wconf.type.chain) {
        i++;
        if(i == n) {
            workload2::generate_chain(conf, wconf, q);
            NB_CHAINS++;
            return;
        }
    }
    
    if (wconf.type.star) {
        i++;
        if(i == n) {
            workload2::generate_star(conf, wconf, q);
            NB_STARS++;
            return;
        }
    }
    
    if (wconf.type.cycle) {
        i++;
        if(i == n) {
            workload2::generate_cycle(conf, wconf, q);
            NB_CYCLES++;
            return;
        }
    }
    
    if (wconf.type.starchain) {
        i++;
        if(i == n) {
            workload2::generate_starchain(conf, wconf, q);
            NB_STARCHAINS++;
            return;
        }
    }
}

void generate_workload(const config::config & conf, workload::workload & wl, report::workload_report & rep) {
    struct timeval tbegin,tend;
    gettimeofday(&tbegin,NULL);

    size_t size = 0;
    size_t c = 0;
    size_t nb_arity_2 = 0;
    size_t nb_constant = 0;
    size_t nb_linear = 0;
    size_t nb_quadratic = 0;
    
    for (const auto & wconf : conf.workloads) {
        size += wconf.size;
    }
    wl.queries.resize(size);
    
    for (const auto & wconf : conf.workloads) {
        for (unsigned int i = 0; i < wconf.size; i++) {
            int nb_stars = 0;
            size_t min_disjunct = 10000000000;
            size_t max_disjunct = 0;
            size_t min_length = 1000000000;
            size_t max_length = 0;
            workload::query & query = wl.queries[c];
            //cout << "generate query " << i << endl;
            for (int j = 0; j < 100; j++) {
                try {
                    workload2::generate_query(conf, wconf, query);
                }
                catch (const string & e) {
                    continue;
                }
                if (j == 99) {
                    cerr << "fail to generate a query" << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            }
            query.info.id = wconf.id;
            query.info.number = i;
            query.info.input = conf.input;
            //wl.queries[c].info.multiplicity = wconf.multiplicity;
            query.info.conjunct = query.bodies[0].conjuncts.size();
            for (workload::conjunct & conjunct : query.bodies[0].conjuncts) {
                if (conjunct.star)
                    nb_stars++;
                if(conjunct.disjuncts.size() < min_disjunct)
                    min_disjunct = conjunct.disjuncts.size();
                if(conjunct.disjuncts.size() > max_disjunct)
                    max_disjunct = conjunct.disjuncts.size();
                for (workload::disjunct & disjunct : conjunct.disjuncts) {
                    if(disjunct.symbols.size() < min_length)
                        min_length = disjunct.symbols.size();
                    if(disjunct.symbols.size() > max_length)
                        max_length = disjunct.symbols.size();
                }
            }
            //q.info.disjuncts = wconf.disjuncts;
            //q.info.length = wconf.length;
            query.info.multiplicity = ((double) nb_stars) / query.info.conjunct;
            query.info.disjuncts.first = min_disjunct;
            query.info.disjuncts.second = max_disjunct;
            query.info.length.first = min_length;
            query.info.length.second = max_length;

            if (query.info.arity == 2) {
                 nb_arity_2++;
                 if (query.info.selectivity == config::selectivity::CONSTANT)
                     nb_constant++;
                 else if (query.info.selectivity == config::selectivity::LINEAR)
                     nb_linear++;
                 else
                     nb_quadratic++;
            }

            c++;
        }
    }

    rep.min_conjuncts = MIN_CONJUNCTS;
    rep.max_conjuncts = MAX_CONJUNCTS;
    rep.min_disjuncts = MIN_DISJUNCTS;
    rep.max_disjuncts = MAX_DISJUNCTS;
    rep.min_length = MIN_LENGTH;
    rep.max_length = MAX_LENGTH;
    rep.nb_chains = NB_CHAINS;
    rep.nb_stars = NB_STARS;
    rep.nb_starchains =  NB_STARCHAINS;
    rep.nb_cycles = NB_CYCLES;
    rep.nb_arity_2 = nb_arity_2;
    rep.nb_constant = nb_constant;
    rep.nb_linear = nb_linear;
    rep.nb_quadratic = nb_quadratic;

    rep.percentage_of_stars = ((double) NUMBER_OF_STARS) / ((double) NUMBER_OF_DISJUNCTS);

    gettimeofday(&tend,NULL);
    rep.exec_time=((double)(1000*(tend.tv_sec-tbegin.tv_sec)+((tend.tv_usec-tbegin.tv_usec)/1000)))/1000.;

}



}
