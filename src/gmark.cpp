#include "gmark.h"
#include "config.h"

#include <cstdlib>
#include <vector>
#include <utility>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <ostream>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <chrono>
#include <sys/time.h>

namespace graph {

vector<size_t> generate_random_slots(pair<size_t,size_t> range, const distribution & distrib) {
    vector<size_t> vslots;

    random_generator * gen = make_generator(distrib);
    
    for (size_t node = range.first; node <= range.second; node++) {
        size_t nb_slots = gen->next();
        if (distrib.type == DISTRIBUTION::ZIPFIAN) {
        	nb_slots++;
        }
        for (size_t i = 0; i < nb_slots; i++) {
            vslots.push_back(node);
        }
    }
    
    delete gen;
    
    return vslots;
}

void abstract_graph_writer::add_random_edges(config::edge & c_edge) {
    if (c_edge.incoming_distrib.type == DISTRIBUTION::UNDEFINED) {
        add_random_edges1(c_edge);
    } else {
        add_random_edges2(c_edge);
    }
}

void abstract_graph_writer::add_random_edges1(config::edge & c_edge) {
    auto subject_node_range = node_ranges_per_type[c_edge.subject_type];
    auto object_node_range = node_ranges_per_type[c_edge.object_type];
    size_t nb_objects = 1 + object_node_range.second - object_node_range.first;
    
    if(c_edge.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && c_edge.outgoing_distrib.arg1 == 0) {
        c_edge.outgoing_distrib.arg1 = nb_objects;
    }
    
    random_generator * gen = make_generator(c_edge.outgoing_distrib);
    uniform_random_generator uniform_gen(0, nb_objects);
    
    for (size_t subject = subject_node_range.first; subject <= subject_node_range.second; subject++) {
        size_t nb_edges = gen->next();
        for (size_t i = 0; i < nb_edges; i++) {
            size_t rnd = uniform_gen.next();
            //size_t rnd = rand() % nb_objects; // todo
            size_t object = object_node_range.first + rnd;
            //if (! has_edge(subject, c_edge.predicate, object)) {
            add_edge(subject, c_edge.predicate, object);
                    //    break;
                //}    
        }
    }
    
    delete gen;
}

void abstract_graph_writer::add_random_edges2(config::edge & c_edge) {
    auto subject_node_range = node_ranges_per_type[c_edge.subject_type];
    auto object_node_range = node_ranges_per_type[c_edge.object_type];
    size_t nb_subjects = 1 + subject_node_range.second - subject_node_range.first;
    size_t nb_objects = 1 + object_node_range.second - object_node_range.first;
    
    if(c_edge.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && c_edge.outgoing_distrib.arg1 == 0) {
        c_edge.outgoing_distrib.arg1 = nb_objects;
    }

    if(c_edge.incoming_distrib.type == DISTRIBUTION::ZIPFIAN && c_edge.incoming_distrib.arg1 == 0) {
        c_edge.incoming_distrib.arg1 = nb_subjects;
    }
    
    
    vector<size_t> subject_slots = generate_random_slots(node_ranges_per_type[c_edge.subject_type], c_edge.outgoing_distrib);
    vector<size_t> object_slots = generate_random_slots(node_ranges_per_type[c_edge.object_type], c_edge.incoming_distrib);
    size_t n = subject_slots.size();
    size_t m = object_slots.size();
    
    if (n < m) {
        random_shuffle(object_slots.begin(), object_slots.end());
    } else {
        random_shuffle(subject_slots.begin(), subject_slots.end());
        n = m;
    }
    for (size_t i = 0; i < n; i++) {
        add_edge(subject_slots[i], c_edge.predicate, object_slots[i]);
    }
}


void abstract_graph_writer::add_remaining_edges(size_t predicate, size_t nb_edges) {
//    cout << "add_remaining_edges: " << predicate << " " << nb_edges << endl;
}

void abstract_graph_writer::build_graph (config::config & conf, report::report & rep) {
    struct timeval tbegin,tend;
    gettimeofday(&tbegin,NULL);

    nb_nodes = 0;
    nb_edges = 0;
    nb_edges_by_type.resize(conf.predicates.size());
    for (size_t i = 0; i < nb_edges_by_type.size(); i++) {
        nb_edges_by_type[i] = 0;
    }

    //size_t nb_vertices = conf.nb_vertices;
    //size_t node_index = 0;
    this->conf = &conf;
    size_t type = 0;
    for (auto & typeconfig : conf.types) {
        add_vertices(type, typeconfig.size[0]);
        type++;
    }
    
    rep.nb_nodes = nb_nodes;

    created_edges.clear();
    created_edges.resize(conf.predicates.size());
    
//    cout << "creating edges" << endl;
    for (config::edge & edge : conf.schema.edges) {
//        cout << "add random edges: " << edge.subject_type << " " << edge.predicate << " " << edge.object_type << " " << edge.multiplicity << " " << edge.outgoing_distrib << " " << edge.incoming_distrib <<endl;
        add_random_edges(edge);
    }
    
    for (size_t predicate = 0; predicate < conf.predicates.size(); predicate++) {
        if(created_edges[predicate] < conf.predicates[predicate].size[0]) {
            int nb_edges = conf.predicates[predicate].size[0] - created_edges[predicate];
            add_remaining_edges(predicate, nb_edges);
        }
    }

    rep.nb_edges = nb_edges; 
    rep.predicates.resize(nb_edges_by_type.size());
    for (size_t i = 0; i < nb_edges_by_type.size(); i++) {
        rep.predicates[i].alias = conf.predicates[i].alias;
        rep.predicates[i].size = nb_edges_by_type[i];
    }

    gettimeofday(&tend,NULL);
    rep.exec_time=((double)(1000*(tend.tv_sec-tbegin.tv_sec)+((tend.tv_usec-tbegin.tv_usec)/1000)))/1000.;
}

void abstract_graph_writer::add_vertices(size_t type, size_t size) {
    if (size == 0) {
        size = 1;
    }
//    cout << "add_vertices: " << type << " " << size << " " <<  nb_nodes << " " << nb_nodes+size-1 << endl;
    node_ranges_per_type.push_back(make_pair(nb_nodes, nb_nodes+size-1));
    nb_nodes += size;
}

void abstract_graph_writer::add_edge(size_t subject, size_t predicate, size_t object) {
    created_edges[predicate]++;
    nb_edges++;
    nb_edges_by_type[predicate]++;
    print_edge(subject, predicate, object);
}


ntriple_graph_writer::ntriple_graph_writer (ostream & s) {
    stream = &s;
}

void ntriple_graph_writer::print_edge(size_t subject, size_t predicate, size_t object) {
    *stream <<  "0-" << subject << " ";
    string alias = conf->predicates[predicate].alias;
    if (conf->print_alias && alias.size() > 0)
	*stream << alias;
    else
        *stream << predicate;
    *stream << " 1-" << object-conf->nb_nodes[0]/2 << "\n";
    stream->flush();
}
}
