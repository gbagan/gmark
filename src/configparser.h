#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "pugixml/pugixml.hpp"
#include "config.h"

namespace configparser {

using namespace std;

int parse_config(const string & filename, config::config & conf);
void parse_predicates(pugi::xml_node node, config::config & conf);
void parse_types(pugi::xml_node node, config::config & conf);
void parse_schema(pugi::xml_node node, config::config & conf);
void parse_workloads(pugi::xml_node node, config::config & conf);

distribution parse_distribution(pugi::xml_node node);

}

#endif
