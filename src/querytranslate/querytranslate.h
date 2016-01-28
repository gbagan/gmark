#ifndef QUERYTRANSLATE_H
#define QUERYTRANSLATE_H

#include "pugixml.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

void qtranslate(const string &, const string &, const string &, const string &, const string &);
void qtranslate_sql(pugi::xml_node, ofstream &);
void qtranslate_sql_body(pugi::xml_node, pugi::xml_node, ofstream &);
void qtranslate_sql_select(pugi::xml_node, unordered_map < string, pair < vector <int>, vector <int> > > &, int, ostream &, const string &);
void qtranslate_sparql(pugi::xml_node, ofstream &);
void qtranslate_sparql_regex(pugi::xml_node, ofstream &);
void qtranslate_cypher(pugi::xml_node, ofstream &);
void qtranslate_cypher_body(pugi::xml_node, string, ofstream &);
void qtranslate_cypher_doc(pugi::xml_document &);
void qtranslate_cypher_doc_reconstructBody(pugi::xml_node &, pugi::xml_node &, vector < pugi::xml_node >);
void qtranslate_logicblox(pugi::xml_node, ofstream &);
void qtranslate_logicblox_body(pugi::xml_node, int, ofstream &, string);

#endif
