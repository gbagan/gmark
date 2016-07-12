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


/*  converting queries in gMark's XML syntax to a HTML interface
 *  the query will be visualized with Graphviz
 */



#include "queryinterface.h"

void qinterface(const string & inputfilename, const string & translatedir)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(inputfilename.c_str()); 


    if (! result)
    {
        cout << endl << "XML file [" << inputfilename << "] parsed with errors"<< endl;
        cout << "Error description: " << result.description() << endl;
        cout << "Error offset: " << result.offset << endl << endl;
        return;
    }


		int total = 0;
    for (pugi::xml_node query : doc.child("queries").children("query"))
			total ++;


    int qcount = 0;
    for (pugi::xml_node query : doc.child("queries").children("query"))
    {
			string prefix_file = "interface-queries/query-" + to_string(qcount);

			// Generate Dot

			ofstream plot_file;
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
							plot_file << "\t" << var_text_src << " -> " << var_text_trg << " [label=\"";
		          qinterface_dot_regex(conjunct_node.first_child(), plot_file);
							plot_file << "\"];\n";
		      }
		  }

			plot_file << "}\n";
			plot_file.close();
			string query_file = prefix_file + ".png";
			string draw_command = "dot -Tpng -o " + prefix_file + ".png " + plot_file_name;
			system (draw_command.c_str());



			// Generate HTML


			string input = query.child("metadata").child("input").child_value();
			string arity = query.child("metadata").child("arity").child_value();
			string selectivity = query.child("metadata").child("selectivity").child_value();
			string multiplicity = query.child("metadata").child("multiplicity").child_value();
			string conjunct = query.child("metadata").child("conjunct").child_value();
			string disjuncts = query.child("metadata").child("disjuncts").child_value();
			string length = query.child("metadata").child("length").child_value();

			ofstream html_file;
			string html_file_name = prefix_file + ".html";
			html_file.open(html_file_name);
			html_file << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n";
			html_file << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
			html_file << "<head>\n";
			html_file << "\t<title>Visualize query workload</title>\n";
			html_file << "\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n";
			html_file << "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"../style.css\"/>\n";
			html_file << "\t<script src=\"../animation.js\"></script>\n";
			html_file << "</head>\n";
			html_file << "<body>\n\n";
			html_file << "<div id=\"top\">\n\n";
			html_file << "<div id=\"left\">\n";
			html_file << "<h3>Stats for Query "<< to_string(qcount) <<"</h3>\n";
			html_file << "<ul>\n";
			html_file << "<li><i>Dataset</i>: " << input << "</li>\n";
			html_file << "<li><i>Arity</i>: " << arity << "</li>\n";
			html_file << "<li><i>Selectivity</i>: " << selectivity << "</li>\n";
			html_file << "<li><i>Size</i>: C[" << conjunct << "-" << conjunct << "], ";
			html_file << "D["<< disjuncts << "], L[" << length << "]</li>\n";
			html_file << "<li><i>Recursion</i>: " << to_string(int(atof(multiplicity.c_str()) * 100)) << "%</li>\n";
			html_file << "</ul>\n";
			html_file << "<hr/>\n";
			html_file << "<h3>Generate concrete syntaxes</h3>\n";
			html_file << "<input type=\"checkbox\" id=\"sparql-checkbox\"/>SPARQL<br/>\n";
			html_file << "<input type=\"checkbox\" id=\"opencypher-checkbox\"/>openCypher<br/>\n";
			html_file << "<input type=\"checkbox\" id=\"sql-checkbox\"/>SQL<br/>\n";
			html_file << "<input type=\"checkbox\" id=\"datalog-checkbox\"/>Datalog<br/>\n";
			html_file << "<hr/>\n";
			html_file << "<a href=\"query-"<< to_string((qcount - 1 >= 0)? (qcount - 1) : (total - 1)) <<".html\">&lt;&lt;Previous query</a>\n";
			html_file << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			html_file << "<a href=\"query-"<< to_string((qcount + 1) % total) <<".html\">Next query&gt;&gt;</a>\n";
			html_file << "</div> <!-- end left -->\n\n";
			html_file << "<div id=\"right\">\n";
			html_file << "<img src=\"query-" << to_string(qcount) <<".png\"/>\n";
			html_file << "</div> <!-- end right -->\n\n";
			html_file << "</div> <!-- end top -->\n\n";
			html_file << "<div id=\"bottom\">\n";
			html_file << "<br/><br/>\n\n";
			html_file << "<table border=\"1\">\n";
			html_file << "<tr>\n";
			html_file << "<th id=\"sparql-header\">SPARQL</th>\n";
			html_file << "<th id=\"opencypher-header\">openCypher*</th>\n";
			html_file << "<th id=\"sql-header\">SQL</th>\n";
			html_file << "<th id=\"datalog-header\">Datalog</th>\n";
			html_file << "</tr>\n";
			html_file << "<tr>\n";

			string line;

			html_file << "<td valign=\"top\" id=\"sparql-syntax\">";
			ifstream sparqlcode;
			sparqlcode.open(translatedir + "/query-" + to_string(qcount) + ".sparql");
			while(getline(sparqlcode, line)) {
				line.replace(line.find("<"), 1, "&lt;");
				line.replace(line.find(">"), 1, "&gt;<br/>");
				html_file << line << "<br/>";
			}
			sparqlcode.close();
			html_file << "</td>\n";

			html_file << "<td valign=\"top\" id=\"opencypher-syntax\">";
			ifstream opencyphercode;
			opencyphercode.open(translatedir + "/query-" + to_string(qcount) + ".cypher");
			while(getline(opencyphercode, line))
				html_file << line << "<br/>";
			opencyphercode.close();

			html_file << "<br/><br/>*Note: This query may have a different semantics than the intended one because openCypher does not allow Kleene star above concatenation and/or inverses.</td>\n";

			html_file << "<td valign=\"top\" id=\"sql-syntax\">";
			ifstream sqlcode;
			sqlcode.open(translatedir + "/query-" + to_string(qcount) + ".sql");
			while(getline(sqlcode, line))
				html_file << line << "<br/>";
			sqlcode.close();
			html_file << "</td>\n";

			html_file << "<td valign=\"top\" id=\"datalog-syntax\">";
			ifstream lbcode;
			lbcode.open(translatedir + "/query-" + to_string(qcount) + ".lb");
			while(getline(lbcode, line))
				html_file << line << "<br/>";
			lbcode.close();

			html_file << "</td>\n";

			html_file << "</tr>\n";
			html_file << "</table>\n";
			html_file << "</div><!-- end bottom -->\n\n";
			html_file << "</body>\n";
			html_file << "</html>\n";
			html_file.close();


			// Increment qcount
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
