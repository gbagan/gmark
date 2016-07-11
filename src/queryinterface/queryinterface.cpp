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


/*  converting queries in gMark's XML syntax to a HTML interface
 *  the query will be visualized with Graphviz
 */



#include "queryinterface.h"

void qinterface(const string & inputfilename)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(inputfilename.c_str()); 
    int qcount = 0;

    if (! result)
    {
        cout << endl << "XML file [" << inputfilename << "] parsed with errors"<< endl;
        cout << "Error description: " << result.description() << endl;
        cout << "Error offset: " << result.offset << endl << endl;
        return;
    }


    for (pugi::xml_node query : doc.child("queries").children("query"))
    {

// begin Radu

			ofstream plot_file;
			string prefix_file = "interface-queries/query-" + to_string(qcount);
			string plot_file_name = prefix_file + ".dot";
			plot_file.open(plot_file_name);
			set<string> nodes_already_drawn;

			plot_file << "digraph g {\n";
  	  for (pugi::xml_node var_node : query.child("head").children("var"))
  	  {
				string var_text = var_node.text().get();
				var_text.erase(var_text.begin());
        plot_file << "\t" << var_text; 
				plot_file << " [fillcolor=\"yellow\", style=\"filled,\" shape=circle, label=\"?"<< var_text <<"\"];\n";
				nodes_already_drawn.insert(var_text);
    	}

		  for (pugi::xml_node body_node : query.child("bodies").children("body"))
		  {
		      for (pugi::xml_node conjunct_node : body_node.children("conjunct"))
		      {
							string var_text_src = conjunct_node.attribute("src").value();	
							var_text_src.erase(var_text_src.begin());
							if (nodes_already_drawn.find(var_text_src) == nodes_already_drawn.end()) 
							{
		      			plot_file << "\t" << var_text_src << " [shape=circle, label=\"?"<< var_text_src <<"\"];\n";
								nodes_already_drawn.insert(var_text_src);
							}
							string var_text_trg = conjunct_node.attribute("trg").value();
							var_text_trg.erase(var_text_trg.begin());
							if (nodes_already_drawn.find(var_text_trg) == nodes_already_drawn.end())
							{
		      			plot_file << "\t" << var_text_trg << " [shape=circle, label=\"?"<< var_text_trg <<"\"];\n";
								nodes_already_drawn.insert(var_text_trg);
							}
							plot_file << var_text_src << " -> " << var_text_trg << "[label=\"";
		          qinterface_dot_regex(conjunct_node.first_child(), plot_file);
							plot_file << "\"]";
		      }
		  }

			plot_file << "}\n";
			plot_file.close();
			string query_file = prefix_file + ".png";
			string draw_command = "dot -Tpng -o " + prefix_file + ".png " + plot_file_name;
			system (draw_command.c_str());

// end Radu


        qcount++;
    }

    cout << endl << endl << qcount << " queries processed" << endl << endl;
}


void qinterface_dot_regex(pugi::xml_node query, ofstream & file)
{
    string name = query.name();
    if(name == "symbol")
    {
        file << "p" << query.text().get();
        if (query.attribute("inverse").as_bool())
        {
            file << "-";
        }
    }
    else if(name == "star")
    {
        file << "(";
        qinterface_dot_regex(query.first_child(), file);
        file << ")*";
    }
    else if (name == "concat")
    {
        bool first = true;
        file << "(";
        for (pugi::xml_node child : query.children())
        {
            if (!first)
                file << ".";
            qinterface_dot_regex(child, file);
            first = false;
        }
        file << ")";
    }
    else if (name == "disj")
    {
        bool first = true;
        file << "(";
        for (pugi::xml_node child : query.children())
        {
            if (!first)
                file << "+";
            qinterface_dot_regex(child, file);
            first = false;
        }
        file << ")";
    }

}
