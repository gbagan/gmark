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


#include "config.h"

namespace config {

edge::edge (size_t s_type, size_t pred, size_t o_type, char multipl,
                          const distribution & odistrib, const distribution & idistrib){
    subject_type = s_type;
    predicate = pred;
    object_type = o_type;
    multiplicity = multipl;
    outgoing_distrib = odistrib;
    incoming_distrib = idistrib;
}


void schem::add_edge(size_t subject_type,  size_t predicate, size_t object_type, char multiplicity,
                            const distribution & outgoing_dist, const distribution & incoming_dist) {
    edges.emplace_back(subject_type, predicate, object_type, multiplicity, outgoing_dist, incoming_dist);
}
    
void schem::add_edge(size_t subject_type, size_t predicate, size_t object_type, char multiplicity,
                            DISTRIBUTION::type type, double arg1, double arg2) {
    distribution outgoing_dist(type, arg1, arg2);
    distribution incoming_dist(DISTRIBUTION::UNDEFINED, 0, 0);
    add_edge(subject_type, predicate, object_type, multiplicity, outgoing_dist, incoming_dist);
}
    
void schem::add_edge(size_t subject_type, size_t predicate, size_t object_type, char multiplicity,
                            DISTRIBUTION::type outgoing_type, double outgoing_arg1, double outgoing_arg2,
                            DISTRIBUTION::type incoming_type, double incoming_arg1, double incoming_arg2) {
    distribution outgoing_dist(outgoing_type, outgoing_arg1, outgoing_arg2);
    distribution incoming_dist(incoming_type, incoming_arg1, incoming_arg2);
    add_edge(subject_type, predicate, object_type, multiplicity, outgoing_dist, incoming_dist);
}


void config::complete_config() {
    complete_types_config();
    complete_predicates_config();
}

void config::complete_types_config() {
    size_t nb_not_allocated_nodes = nb_nodes;
    size_t nb_not_allocated_types = 0;
    size_t counter = 0;
    for (auto & type : types) {
        if (type.size > 0) {
            nb_not_allocated_nodes -= type.size;
        }
        else {
            nb_not_allocated_types++;
        }
    }
    if (nb_not_allocated_types != 0) {
        size_t average_nb_nodes = nb_not_allocated_nodes / nb_not_allocated_types;
        if (node_distribution.type == DISTRIBUTION::UNDEFINED) {
            node_distribution.type = DISTRIBUTION::NORMAL;
        }
        random_generator * gen = make_generator_with_mean(node_distribution.type, average_nb_nodes);
        for (size_t i = 0; i < types.size()-1; i++) {
            auto & type = types[i];
            if (type.size == 0) {
                type.size = gen->next();
                counter += type.size;
            }
        }
        auto & type = types[types.size()-1];
        if (type.size == 0) {
            if (counter < nb_not_allocated_nodes) {
                type.size = nb_not_allocated_nodes - counter;
            }
            else {
                type.size = gen->next();
            }
        }
        delete gen;
    }
}

void config::complete_predicates_config() {
    
}

ostream & operator << (ostream& stream, selectivity::type type) {
    switch(type) {
        case selectivity::CONSTANT:
            stream << "constant";
            break;
        case selectivity::LINEAR:
            stream << "linear";
            break;
        case selectivity::QUADRATIC:
            stream << "quadratic";
            break;
        default:
            stream << (int) type;
    }
    return stream;
}    
    
    
    
    
}
