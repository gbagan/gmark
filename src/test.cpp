#include <unistd.h>
#include <fstream>
#include <chrono>
#include <thread>

#include "config.h"
#include "gmark.h"
#include "configparser.h"
#include "workload.h"
#include "workload2.h"
#include "report.h"
#include "monStaGen/incrementalDeterministicGraphGenerator.h"
#include "monStaGen/processingEdgeTypes.h"
#include "randomgen.h"


void print_report(report::report & rep) {
    cout << "report:" << endl;
    cout << "number of nodes: " << rep.nb_nodes << endl;
    cout << "number of edges: " << rep.nb_edges << endl;
    cout << "execution time: " << rep.exec_time << endl;
}

void html_graph_report(config::config & conf, report::report & rep, ofstream & stream) {
    stream << "<html>\n";
    stream << "<head>\n";
    stream << "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>\n";
    stream << "<script type=\"text/javascript\">\n";
    stream << "google.charts.load('current', {packages:['corechart', 'bar']});\n";
    stream << "google.charts.setOnLoadCallback(drawChart);\n";


    stream << "function drawChart() {\n";

    stream << "var data = google.visualization.arrayToDataTable([\n";
    stream << "['Node type', 'Number of nodes'], \n";
    for (auto & type : conf.types) {
        stream << "['" << type.alias << "', " << type.size[0] << "],\n";
    }
    stream << "]);\n";
    stream << "var options = {\n";
    stream << "vAxis: { logScale: true, baseline: 1 },\n";
    stream << "title: 'Number of nodes by node type'\n";
    stream << "};\n";
    stream << "var chart = new google.visualization.ColumnChart(document.getElementById('hist11'));\n";
    stream << "chart.draw(data, options);\n";

    stream << "var data = google.visualization.arrayToDataTable([\n";
    stream << "['Node type', 'Number of nodes'], \n";
    for (auto & type : conf.types) {
        stream << "['" << type.alias << "', " << type.size[0] << "],\n";
    }
    stream << "]);\n";
    stream << "var options = {\n";
    stream << "vAxis: { logScale: true, baseline: 1 },\n";
    stream << "title: 'Number of nodes by node type'\n";
    stream << "};\n";
    stream << "var chart = new google.visualization.ColumnChart(document.getElementById('hist12'));\n";
    stream << "chart.draw(data, options);\n";


    stream << "var data = google.visualization.arrayToDataTable([\n";
    stream << "['Predicate type', 'Number of predicates'], \n";
    for (auto & predicate : conf.predicates) {
        size_t size = 0;
        if (conf.nb_edges.empty() || conf.nb_edges[0] > 0)
            size = predicate.size[0];
        else
            size = predicate.proportion * rep.nb_edges;
        stream << "['" << predicate.alias << "', " << size << "],\n";
    }
    stream << "]);\n";
    stream << "var options = {\n";
    stream << "vAxis: { logScale: true, baseline: 1 },\n";
    stream << "title: 'Number of predicates by type'\n";
    stream << "};\n";
    stream << "var chart = new google.visualization.ColumnChart(document.getElementById('hist21'));\n";
    stream << "chart.draw(data, options);\n";


    stream << "var data = google.visualization.arrayToDataTable([\n";
    stream << "['Predicate type', 'Number of predicates'], \n";
    for (auto & predicate : rep.predicates) {
        stream << "['" << predicate.alias << "', " << predicate.size << "],\n";
    }
    stream << "]);\n";
    stream << "var options = {\n";
    stream << "vAxis: { logScale: true, baseline: 1 },\n";
    stream << "title: 'Number of predicates by type'\n";
    stream << "};\n";
    stream << "var chart = new google.visualization.ColumnChart(document.getElementById('hist22'));\n";
    stream << "chart.draw(data, options);\n";

    stream << "}\n";


    stream << "</script></head>\n";
    stream << "<body>\n";
    stream << "<table border=\"1\">\n";
    stream << "<tr><td>Size</td><td width=\"600\">" << conf.nb_nodes[0] << " nodes</td>";
    stream << "<td width=\"600\">" << rep.nb_nodes << " nodes (" << rep.nb_edges << " edges)</td></tr>\n";
    stream << "<tr><td>Execution time</td><td></td><td>" << rep.exec_time << " seconds</td></tr>\n";
    stream << "<tr><td></td><td><div id=\"hist11\"/></td><td><div id=\"hist12\"/></td></tr>\n";
    stream << "<tr><td></td><td><div id=\"hist21\"/></td><td><div id=\"hist22\"/></td></tr>\n";
    stream << "</table>\n";
    stream << "</body></html>\n";
}

void html_workload_report(config::config & conf, report::workload_report & rep, ofstream & stream) {
    config::workload & wconf = conf.workloads[0];
    stream << "<html>\n";
    stream << "<body>\n";
    stream << "<table border=\"1\">\n";
    stream << "<tr><td>Number of queries</td><td width=\"600\">" << wconf.size << "</td><td width=\"600\">" << wconf.size << "</td></tr>\n";
    stream << "<tr><td>Arity</td><td>" << wconf.arity.first << "-" << wconf.arity.second;
    stream << "</td><td>" << rep.min_arity << "-" << rep.max_arity;
    stream << " (" << rep.nb_arity_2 << " queries are binary)</td></tr>\n";
    stream << "<tr><td>Number of conjuncts</td><td>" << wconf.conjuncts.first << "-" << wconf.conjuncts.second;
    stream << "</td><td>" << rep.min_conjuncts << "-" << rep.max_conjuncts << "</td></tr>\n";
    stream << "<tr><td>Number of disjuncts</td><td>" << wconf.disjuncts.first << "-" << wconf.disjuncts.second;
    stream << "</td><td>" << rep.min_disjuncts << "-" << rep.max_disjuncts << "</td></tr>\n";
    stream << "<tr><td>Chain length</td><td>" << wconf.length.first << "-" << wconf.length.second;
    stream << "</td><td>" << rep.min_length << "-" << rep.max_length << "</td></tr>\n";
    stream << "<tr><td>Percentage of stars</td><td>" << 100 * wconf.multiplicity << "%</td><td>" << 100 * rep.percentage_of_stars << "%</td></tr>\n";
    stream << "<tr><td>Execution time</td><td></td><td>" << rep.exec_time << "</td></tr>\n";

    stream << "<tr><td></td><td>";
    int typetotal = (wconf.type.chain ? 1 : 0) +  (wconf.type.star ? 1 : 0) + (wconf.type.starchain ? 1 : 0) + (wconf.type.cycle ? 1 : 0);
    stream << "chain: " << (wconf.type.chain * 100) / typetotal << "%<br/>";
    stream << "star: " << (wconf.type.star * 100) / typetotal << "%<br/>";
    stream << "starchain: " << (wconf.type.starchain * 100) / typetotal << "%<br/>";
    stream << "cycle: " << (wconf.type.cycle * 100) / typetotal << "%</td>\n";
    stream << "<td>";
    stream << "chain: " << (rep.nb_chains * 100) / wconf.size << "%<br/>";
    stream << "star: " << (rep.nb_stars * 100) / wconf.size << "%<br/>";
    stream << "starchain: " << (rep.nb_starchains * 100) / wconf.size << "%<br/>";
    stream << "cycle: " << (rep.nb_cycles * 100) / wconf.size << "%</td></tr>\n";

    stream << "<tr><td></td><td>";
    int seltotal = (wconf.selectivity.constant ? 1 : 0) +  (wconf.selectivity.linear ? 1 : 0) + (wconf.selectivity.quadratic ? 1 : 0);
    stream << "constant: " << (wconf.selectivity.constant * 100) / seltotal << "%<br/>";
    stream << "linear: " << (wconf.selectivity.linear * 100) / seltotal << "%<br/>";
    stream << "quadratic: " << (wconf.selectivity.quadratic * 100) / seltotal << "%</td><td>";
    stream << "constant: " << (rep.nb_arity_2 == 0 ? 0 : (rep.nb_constant * 100) / rep.nb_arity_2) << "%<br/>";
    stream << "linear: " << (rep.nb_arity_2 == 0 ? 0 : (rep.nb_linear * 100) / rep.nb_arity_2) << "%<br/>";
    stream << "quadratic: " << (rep.nb_arity_2 == 0 ? 0 : (rep.nb_quadratic * 100) / rep.nb_arity_2) << "%</td></tr>";
    stream << "</table>\n";
    stream << "</body></html>\n";

}



void parseNodeSequence(vector<unsigned int>* nodeSequence, string nodeSequenceString) {
	// Define the sequence as: 10-20-30-40
	// Generates 4 graphs where the first graph has 10 nodes, the second 20 nodes, etc.
	int pos = 0;
	std::string token;
	while ((pos = nodeSequenceString.find("-")) != (int)string::npos) {
		token = nodeSequenceString.substr(0, pos);
//        cout << "Found token: " << token << endl;
		if (token.compare("") == 0) {
			nodeSequence->push_back(0);
		} else {
			nodeSequence->push_back(stoi(token));
			nodeSequenceString.erase(0, pos + 1);
		}
	}

	if (nodeSequenceString.compare("") != 0) {
//    	cout << "Found token: " << nb_nodes_string << endl;
		nodeSequence->push_back(stoi(nodeSequenceString));
	}
}


int main(int argc, char ** argv) {
//    cout << "Starting...." << endl;
	string conf_file = "../use-cases/test.xml";
    string graph_file;
    string workload_file;
    string report_directory = ".";
    int c;
    bool selectivity = true;
//    long nb_nodes = -1;
    string nb_nodes_string = "";
    bool print_alias = false;
    bool monStaGen = false;
    bool printNodeProperties = false;
    vector<unsigned int> graphSequence;

    while ((c = getopt(argc, argv, "c:g:w:an:r:mps:")) != -1) {
        switch(c) {
            case 'c':
                conf_file = optarg;
                break;
            case 'g':
                graph_file = optarg;
                break;
            case 'w':
                workload_file = optarg;
                break;
            case 'a':
                print_alias = true;
                break;
            case 'r':
                report_directory = optarg;
                break;
            case 'n':
//                nb_nodes = atol(optarg);
            	// optarg needs to be in the form
            	//		(int-)* int
            	// for example, a sequence with three graphs: 10000-20000-30000
            	// or, a single graph: 30000
                nb_nodes_string = optarg;
                parseNodeSequence(&graphSequence, nb_nodes_string);
                break;
            case 'm':
            	monStaGen = true;
            	break;
            case 'p':
            	printNodeProperties = true;
            	break;
            case 's':
                // Seed selection
                RANDOM_GEN.seed(atol(optarg));
                break;
        }
    }


    config::config conf;
    if (graphSequence.size() > 0) {
    	for (int nb: graphSequence) {
    		conf.nb_nodes.push_back(nb);
        }
        conf.nb_graphs = graphSequence.size();
    }
    else {
        conf.nb_nodes = {-1};
        conf.nb_graphs = 0;
    }
    conf.print_alias = print_alias;

//    cout << "Parse config" << endl;
    configparser::parse_config(conf_file, conf);

//    cout << "complete config" << endl;
    conf.complete_config();
//    cout << "Number of graphs=" << conf.nb_graphs << endl;

//    cout << "Config file: " << conf_file << endl;

    if (graph_file == "") {
    	graph_file = "ignore/outputGraph";
    }


    if (monStaGen) {
		processingEdgeTypes processETs(conf, conf_file);
		processETs.sequentialProcessing(printNodeProperties, graph_file);
    } else {
		for (size_t i=0; i<conf.nb_graphs; i++) {
	//	  	cout << "Processing graph " << i << endl;
			report::report rep;
			ofstream graph_stream;

			string fileName = graph_file + to_string(i) + ".txt";

			graph_stream.open(fileName);
			graph::ntriple_graph_writer writer(graph_stream);

			writer.build_graph(conf, rep, i);

			ofstream report_stream;
	        report_stream.open(report_directory + "/graph.html");
	        html_graph_report(conf, rep, report_stream);

	        graph_stream.close();
		}
    }


    if (workload_file != "") {
        report::workload_report rep;

        ofstream workload_stream;
        workload_stream.open(workload_file);
        workload::workload wl;
        if(selectivity) {
            workload2::generate_workload(conf, wl, rep);
        }
        else {
            workload::generate_workload(conf, wl);
        }
        workload::write_xml(wl, workload_stream, conf);
        workload_stream.close();

        ofstream report_stream;
        report_stream.open(report_directory + "/workload.html");
        html_workload_report(conf, rep, report_stream);

        /*
        workload2::matrix mat;
        workload2::distance_matrix_between_types(conf, mat);
        cout << mat << endl;
        workload2::graph graph;
        compute_graph_from_matrix(mat, 4, graph);
        workload2::matrix_of_paths mat2;
        number_of_paths(graph, config::selectivity::CONSTANT, 3, mat2);
        vector<workload2::triple> path;
        generate_random_path(graph, mat2, -1, 3, path);
        for (auto & triple : path) {
            cout << triple << endl;
        }
        */
    }
}

