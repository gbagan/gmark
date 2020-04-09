/* gMark
 * Copyright (C) 2015-2016 Guillaume Bagan <guillaume.bagan@liris.cnrs.fr>
 * Copyright (C) 2015-2016 Angela Bonifati <angela.bonifati@univ-lyon1.fr>
 * Copyright (C) 2015-2016 Radu Ciucanu <ciucanu@isima.fr>
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

#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>

#include "argparse/argparse.hpp"

#include "queryinterface.h"

int main(const int argc, const char ** argv) {

//	./test -w ../../xml/queries.xml -t ../querytranslate/translated -o interface-queries

	string input_filename, translate_dir, interface_dir;

	argparse::ArgumentParser parser("gmark-translate");
	parser.add_argument("help", "-h", "--help", "Prints the help", argparse::FLAG);
	parser.add_argument("workload", "-w", "--workload", "Input workload file", argparse::STORE);
	parser.add_argument("translate", "-t", "--translate", "Input translated queries folder", argparse::STORE);
	parser.add_argument("interface", "-o", "--output", "Interface output folder", argparse::STORE);

	if(!parser.parse(argv, argc))
	{
		// Bad argument
		return EXIT_FAILURE;
	}

	if(parser.get("help").is_set())
	{
		parser.print_usage();
		return EXIT_FAILURE;
	}

	input_filename = parser.get("workload").get_value();
	translate_dir = parser.get("translate").get_value();
	interface_dir = parser.get("interface").get_value();

	qinterface (input_filename.c_str(), translate_dir.c_str(), interface_dir.c_str());

	return EXIT_SUCCESS;
}
