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
#include <unistd.h>
#include <fstream>
#include "queryinterface.h"

int main(int argc, char ** argv) {

//	./test -w ../../xml/queries.xml -t ../querytranslate/translated -o interface-queries
	

//  qinterface("../../xml/queries.xml", "../querytranslate/translated");

	string inputfilename, translatedir, interfacedir;
	int c;

	while ((c = getopt(argc, argv, "w:t:o:")) != -1) {
	switch(c) {
		case 'w':
			inputfilename = optarg;
			break;
		case 't':
			translatedir = optarg;
			break;
		case 'o':
			interfacedir = optarg;
			break;
	}
	}

	qinterface (inputfilename.c_str(), translatedir.c_str(), interfacedir.c_str());

  exit(EXIT_SUCCESS);

}
