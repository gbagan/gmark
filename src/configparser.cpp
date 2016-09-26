#include "configparser.h"
#include "pugixml.hpp"
#include "config.h"

#include <iostream>

namespace configparser {

int parse_config(const string & filename, config::config & conf) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (! result) {
        cerr << "XML [" << filename << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]" << endl;
        cerr << "Error description: " << result.description() << endl;
        cerr << "Error offset: " << result.offset << endl;   
        return 0;
    }
    
    pugi::xml_node root = doc.child("generator");

    pugi::xml_node graph_node = root.child("graph");
    
    unsigned int nodes = graph_node.child("nodes").text().as_uint();
    unsigned int edges = graph_node.child("edges").text().as_uint();
    
    if(conf.nb_nodes == 0) {
        conf.nb_nodes = nodes;
    }

    conf.input = filename;
    conf.nb_edges = edges;
   
    pugi::xml_node predicates = root.child("predicates");
    if (!predicates.empty()) {
        parse_predicates(predicates, conf);
    }
    
    pugi::xml_node types = root.child("types");
    if (!types.empty()) {
        parse_types(types, conf);
    }
    
    pugi::xml_node schema = root.child("schema");
    if (!schema.empty()) {
        parse_schema(schema, conf);
    }
    
    parse_workloads(root, conf);
    
    return 1;
}

void parse_predicates(pugi::xml_node node, config::config & conf) {
    size_t size = node.child("size").text().as_uint();
    //cout << "predicates size: " << size << endl;
    conf.predicates.resize(size);
    
    conf.predicate_distribution = parse_distribution(node.child("distribution"));
    
    for (pugi::xml_node alias_node : node.children("alias")) {
        size_t id = alias_node.attribute("symbol").as_uint();//
        string name = alias_node.text().get();
        //cout << "alias " << id << ", " << name  << endl;
        if (id < 0 || id >= size) {
            cerr << "id " << id << " is out of range" << endl;
            continue;
        }

        conf.predicates[id].alias = name;
    }
    
    for (pugi::xml_node proportion_node : node.children("proportion")) {
        size_t id = proportion_node.attribute("symbol").as_uint();
        double proportion = proportion_node.text().as_double();
        if (id < 0 || id >= size) {
            cerr << "id " << id << " is out of range" << endl;
            continue;
        }
        conf.predicates[id].proportion = proportion;
        conf.predicates[id].size = (size_t) (proportion * conf.nb_edges);
    }
}

void parse_types(pugi::xml_node node, config::config & conf) {
    size_t size = node.child("size").text().as_uint();
    //cout << "type size: " << size << endl;
    conf.types.resize(size);
    
    for (pugi::xml_node alias_node : node.children("alias")) {
        size_t id = alias_node.attribute("type").as_uint();//
        string name = alias_node.text().get();
        //cout << "alias " << id << ", " << name  << endl;
        if (id < 0 || id >= size) {
            cerr << "id " << id << " is out of range" << endl;
            continue;
        }
        conf.types[id].alias = name;
    }
    
    for (pugi::xml_node proportion_node : node.children("proportion")) {
        size_t id = proportion_node.attribute("type").as_uint();
        double proportion = proportion_node.text().as_double();
        //cout << "proportion " << id << ", " << proportion  << endl;
        if (id < 0 || id >= size) {
            cerr << "id " << id << " is out of range" << endl;
            continue;
        }
        conf.types[id].size = (size_t) (proportion * conf.nb_nodes);
        conf.types[id].scalable = true;
        conf.types[id].proportion = proportion;
        
        if (proportion * conf.nb_nodes > 0 &&  conf.types[id].size == 0) {
            conf.types[id].size = 1;
        }
    }
    
    for (pugi::xml_node fixed_node : node.children("fixed")) {
        size_t id = fixed_node.attribute("type").as_uint();
        size_t size2 = fixed_node.text().as_uint();
        //cout << "fixed " << id << ", " << size  << endl;
        if (id < 0 || id >= size) {
            cerr << "id " << id << " is out of range" << endl;
            continue;
        }
        conf.types[id].size = size2;
        conf.types[id].scalable = false;
        conf.types[id].proportion = -1;
    }    
}

void parse_schema(pugi::xml_node node, config::config & conf) {
    int currentEdgeTypeId = 0;
	for (pugi::xml_node source_node : node.children("source")) {
        size_t source_type = source_node.attribute("type").as_uint();
        for (pugi::xml_node target_node : source_node.children("target")) {
            char multiplicity = '*';
            size_t target_type = target_node.attribute("type").as_uint();
            size_t symbol = target_node.attribute("symbol").as_uint();
            string mult_string = target_node.attribute("multiplicity").value();
            size_t edgeTypeId = currentEdgeTypeId;
            currentEdgeTypeId++;
            if (mult_string.size() > 0 && (mult_string[0] == '?' || mult_string[0] == '+' || mult_string[0] == '1')) {
                multiplicity = mult_string[0];
            }
            pugi::xml_node outdistribution_node = target_node.child("outdistribution");
            distribution outdistribution = parse_distribution(outdistribution_node);
            pugi::xml_node indistribution_node = target_node.child("indistribution");
            distribution indistribution = parse_distribution(indistribution_node);
            
            if (multiplicity == '1') { // && outdistribution.type == DISTRIBUTION::UNDEFINED) {
                outdistribution = distribution(DISTRIBUTION::UNIFORM, 1, 1);
            }
            else if (multiplicity == '?') {// && outdistribution.type == DISTRIBUTION::UNDEFINED) {
                outdistribution = distribution(DISTRIBUTION::UNIFORM, 0, 1);
            }
            
            if(outdistribution.type == DISTRIBUTION::UNDEFINED) {
                outdistribution = distribution(DISTRIBUTION::ZIPFIAN, 0, 2.5);
            }
            
            
            
            conf.schema.add_edge(source_type, symbol, target_type, multiplicity, edgeTypeId, outdistribution, indistribution);
            //cout << "conf.add_edge "  << source_type << " " << symbol << " " << target_type << " " << multiplicity << " " << outdistribution << " " << indistribution <<endl;
            
        }
    }
}

distribution parse_distribution(pugi::xml_node node) {
    distribution dist;
    if (!node.empty()) {
        string type = node.attribute("type").value();
        if (type == "uniform") {
            size_t min = node.child("min").text().as_uint();
            size_t max = node.child("max").text().as_uint();
            dist.type = DISTRIBUTION::UNIFORM;
            dist.arg1 = min;
            dist.arg2 = max;
        } else if (type == "zipfian") {
            size_t n = node.child("n").text().as_uint();
            double alpha = node.child("alpha").text().as_double();
            dist.type = DISTRIBUTION::ZIPFIAN;
            dist.arg1 = n;
            dist.arg2 = alpha;
        } else if (type == "normal" or type == "gaussian") {
            double mean = node.child("mu").text().as_double();
            double stddev = node.child("sigma").text().as_double();
            dist.type = DISTRIBUTION::NORMAL;
            dist.arg1 = mean;
            dist.arg2 = stddev;
        }
    }
    return dist;
}


void parse_workloads(pugi::xml_node node, config::config & conf) {
    for (pugi::xml_node workload_node : node.children("workload")) {
        config::workload workload;

        workload.id = workload_node.attribute("id").as_uint();
        workload.size = workload_node.attribute("size").as_uint();
        
        pugi::xml_node arity_node = workload_node.child("arity");
        size_t arity_min = arity_node.attribute("min").as_uint();
        size_t arity_max = arity_node.attribute("max").as_uint();
        workload.arity = make_pair(arity_min, arity_max);
        
        workload.multiplicity = workload_node.child("multiplicity").attribute("star").as_double();
        
        pugi::xml_node size_node = workload_node.child("size");
        
        pugi::xml_node conjuncts_node = size_node.child("conjuncts");
        size_t conjuncts_min = conjuncts_node.attribute("min").as_uint();
        size_t conjuncts_max = conjuncts_node.attribute("max").as_uint();
        workload.conjuncts = make_pair(conjuncts_min, conjuncts_max);
        
        pugi::xml_node disjuncts_node = size_node.child("disjuncts");
        size_t disjuncts_min = disjuncts_node.attribute("min").as_uint();
        size_t disjuncts_max = disjuncts_node.attribute("max").as_uint();
        workload.disjuncts = make_pair(disjuncts_min, disjuncts_max);

        pugi::xml_node length_node = size_node.child("length");
        size_t length_min = length_node.attribute("min").as_uint();
        size_t length_max = length_node.attribute("max").as_uint();
        workload.length = make_pair(length_min, length_max);
        
        pugi::xml_node type_node = workload_node.child("type");
        workload.type.chain = type_node.attribute("chain").as_bool();
        workload.type.star = type_node.attribute("star").as_bool();
        workload.type.cycle = type_node.attribute("cycle").as_bool();
        workload.type.starchain = type_node.attribute("starchain").as_bool();
        
        pugi::xml_node output_node = workload_node.child("output");
        workload.output.sparql = output_node.attribute("sparql").as_bool();
        workload.output.postgres = output_node.attribute("postgres").as_bool();
        workload.output.cypher = output_node.attribute("cypher").as_bool();
        
        pugi::xml_node selectivity_node = workload_node.child("selectivity");
        workload.selectivity.constant = selectivity_node.attribute("constant").as_bool();
        workload.selectivity.linear = selectivity_node.attribute("linear").as_bool();
        workload.selectivity.quadratic = selectivity_node.attribute("quadratic").as_bool();        
        conf.workloads.push_back(workload);
    }
}

}
