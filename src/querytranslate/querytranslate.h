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
