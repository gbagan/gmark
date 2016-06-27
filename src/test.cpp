#include <unistd.h>
#include <fstream>

#include "config.h"
#include "gmark.h"
#include "configparser.h"
#include "workload.h"
#include "workload2.h"
#include "report.h"

void print_report(report::report & rep) {
    cout << "report:" << endl;
    cout << "number of nodes: " << rep.nb_nodes << endl;
    cout << "number of edges: " << rep.nb_edges << endl;
    cout << "execution time: " << rep.exec_time << endl;
}

void html_report(config::config & conf, report::report & rep, ofstream & stream) {
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
        stream << "['" << type.alias << "', " << type.size << "],\n";
    }

    stream << "]);\n";
    stream << "var options = {\n";
    stream << "title: 'Number of nodes by node type'\n";
    stream << "};\n";
    stream << "var chart = new google.visualization.ColumnChart(document.getElementById('hist11'));\n";
    stream << "chart.draw(data, options);\n";
    stream << "}\n";

    stream << "</script></head>\n";
    stream << "<body>\n";
    stream << "<table border=\"1\">\n";
    stream << "<tr><td>Number of nodes</td><td width=\"600\">" << conf.nb_nodes << "</td><td width=\"600\">" << rep.nb_nodes << "</td></tr>\n";
    stream << "<tr><td>Number of edges</td><td>" << conf.nb_edges << "</td><td>" << rep.nb_edges << "</td></tr>\n";
    stream << "<tr><td>Execution time</td><td></td><td>" << rep.exec_time << "</td></tr>\n";
    stream << "<tr><td></td><td><div id=\"hist11\"/></td><td><div id=\"hist12\"/></td></tr>\n";
    stream << "</table>\n";
    stream << "</body></html>\n";
}

int main(int argc, char ** argv) {
    string conf_file = "../use-cases/test.xml";
    string graph_file;
    string workload_file;
    int c;
    bool selectivity = false;
    long nb_nodes = -1;
    bool print_alias = false;    

    while ((c = getopt(argc, argv, "c:g:w:san:")) != -1) {
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
            case 's':
                selectivity = true;
                break;
            case 'a':
                print_alias = true;
                break;
            case 'n':
                nb_nodes = atol(optarg);
        }
    }
    
    config::config conf;
    if (nb_nodes >= 0) {
        conf.nb_nodes = nb_nodes;
    }
    else {
        conf.nb_nodes = 0;
    }
    conf.print_alias = print_alias;
    
    configparser::parse_config(conf_file, conf);
    
    report::report rep;

    cout << "complete config" << endl;
    conf.complete_config();
    
    
    if(graph_file != "") {
        ofstream graph_stream;
        graph_stream.open(graph_file);
        cout << "graph generation" << endl;
        graph::ntriple_graph_writer writer(graph_stream);
        writer.build_graph(conf, rep);
        graph_stream.close();
    }
    
    if (workload_file != "") {
        ofstream workload_stream;
        workload_stream.open(workload_file);
        workload::workload wl;
        if(selectivity) {
            workload2::generate_workload(conf, wl);
        }
        else {
            workload::generate_workload(conf, wl);
        }
        workload::write_xml(wl, workload_stream);
        workload_stream.close();
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

    print_report(rep);

    ofstream report_stream;
    report_stream.open("report.html");
    html_report(conf, rep, report_stream);
}

