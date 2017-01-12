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
#include "incrementalDeterministicGraphGenerator.h"
#include "analysisIncrDetGraph.h"
#include "processingEdgeTypes.h"
#include "analyseCorrelation.h"


//void processEdgeTypesParallel(int low, int high, int* seeds, string* tempOutputFileNames, config::config conf) {
////	cout << "Parallel processing of edgeTypes " << low << ":" << high << endl;
//	int diff = high - low;
//	if (diff == 1) {
//		ofstream outputFile;
//		outputFile.open(tempOutputFileNames[low], ios::trunc);
//
//		incrementalDeterministicGraphGenerator graphGenerator;
//		graphGenerator.processEdgeType(conf, conf.schema.edges.at(low), outputFile);
//
//		outputFile.close();
//	} else {
//		int mid = floor(diff / 2);
//		thread first (processEdgeTypesParallel, low, low+mid, seeds, tempOutputFileNames, conf);
//		thread second (processEdgeTypesParallel, low+mid, high, seeds, tempOutputFileNames, conf);
//
//		first.join();
//		second.join();
//	}
//}

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
        if (conf.nb_edges[0] > 0)
            size = predicate.size[0];
        else;
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


void creatRankFileZipf(config::config conf, string graphFileName) {
	vector<int> outDistr;
//	vector<int> inDistr;

	// Initialize outDistr-vector
	for (int i=0; i<=conf.types[conf.schema.edges[0].subject_type].size[0]; i++) {
		outDistr.push_back(0);
	}

	// Initialize inDistr-vector
//	for (int i=0; i<=conf.types[conf.schema.edges[0].object_type].size; i++) {
//		inDistr.push_back(0);
//	}

	// Analyze all edges
	string line;
	ifstream myfile(graphFileName);
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp = line;
			string getPred = temp.erase(0, temp.find(" ")+1);
			string predicate = getPred.substr(0, getPred.find(" "));

			temp = line;
			string subjectType = temp.substr(0, temp.find("-"));
			string getSub = temp.erase(0, temp.find("-")+1);
			string subject = getSub.substr(0, getSub.find(" "));

//			getPred = getPred.erase(0, getPred.find(" ")+1);
//
//			string objectType = getPred.substr(0, getPred.find("-"));
//			string getObj = getPred.erase(0, getPred.find("-")+1);
//			string object = getObj.substr(0, getObj.length());

			outDistr.at(stoi(subject))++;
//			inDistr.at(stoi(object) - ...TODO...)++;

		}
		myfile.close();
	} else {
		cout << "Unable to open file";
	}

	int max = 0;
	for (int val: outDistr) {
		if (val > max) {
			max = val;
		}
	}
	cout << "Maxdegree = " << max << endl;
	ofstream rankFile, degreeFile;
	string fname = "rankFileET0n" + to_string(conf.nb_nodes[0]) + ".txt";
	string fname2 = "rankFileET0n" + to_string(conf.nb_nodes[0]) + "degree.txt";
	rankFile.open (fname);
	degreeFile.open (fname2);
	for (int val: outDistr) {
		rankFile << (double)val/(double)max << endl;
		degreeFile << val << "-" << max << endl;
	}
}




void creatRankFileNonZipf(config::config conf, string graphFileName) {
	vector<int> outDistr;
//	vector<int> inDistr;

	// Initialize outDistr-vector
	for (int i=0; i<conf.types[conf.schema.edges[0].subject_type].size[0]; i++) {
		outDistr.push_back(0);
	}

	// Initialize inDistr-vector
//	for (int i=0; i<=conf.types[conf.schema.edges[0].object_type].size; i++) {
//		inDistr.push_back(0);
//	}

	// Analyze all edges
	string line;
	ifstream myfile(graphFileName);
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string temp = line;
//			string getPred = temp.erase(0, temp.find(" ")+1);
//			string predicate = getPred.substr(0, getPred.find(" "));

//			temp = line;
			string subjectType = temp.substr(0, temp.find("-"));
			string getSub = temp.erase(0, temp.find("-")+1);
			string subject = getSub.substr(0, getSub.find(" "));

//			getPred = getPred.erase(0, getPred.find(" ")+1);
//
//			string objectType = getPred.substr(0, getPred.find("-"));
//			string getObj = getPred.erase(0, getPred.find("-")+1);
//			string object = getObj.substr(0, getObj.length());

			outDistr.at(stoi(subject))++;
//			inDistr.at(stoi(object) - ...TODO...)++;

		}
		myfile.close();
	} else {
		cout << "Unable to open file";
	}

	ofstream rankFile;
	rankFile.open("rankFileET0n" + to_string(conf.nb_nodes[0]) + ".txt", ios::trunc);
	for (int currentNodeDegree: outDistr) {
		int nodesWithLowerDegree = 0;
		for (int compareNodeDegree: outDistr) {
			if (compareNodeDegree <= currentNodeDegree) {
				nodesWithLowerDegree++;
			}
		}
		double rank = (double)nodesWithLowerDegree / (double)outDistr.size();
		rankFile << to_string(rank) << endl;
	}


	rankFile.close();
}





int main(int argc, char ** argv) {
    string conf_file = "../use-cases/test2.xml";
    string graph_file;
    string workload_file;
    string report_directory = ".";
    int c;
//    bool selectivity = true;
//    long nb_nodes = -1;
    string nb_nodes_string = "";
    bool print_alias = false;    

    while ((c = getopt(argc, argv, "c:g:w:an:r:")) != -1) {
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
                nb_nodes_string = optarg;
                break;
        }
    }
    
    size_t pos = 0;
    std::string token;
    vector<unsigned int> nb_nodes_per_graph;
    while ((pos = nb_nodes_string.find("-")) != string::npos) {
    	token = nb_nodes_string.substr(0, pos);
//        cout << "Found token: " << token << endl;
        if (token.compare("") == 0) {
        	nb_nodes_per_graph.push_back(0);
        } else {
        	nb_nodes_per_graph.push_back(stoi(token));
        	nb_nodes_string.erase(0, pos + 1);
        }
    }

    if (nb_nodes_string.compare("") != 0) {
//    	cout << "Found token: " << nb_nodes_string << endl;
    	nb_nodes_per_graph.push_back(stoi(nb_nodes_string));
    }


    config::config conf;
    if (nb_nodes_per_graph.size() > 0) {
    	for (int nb: nb_nodes_per_graph) {
    		conf.nb_nodes.push_back(nb);
        }
        conf.nb_graphs = nb_nodes_per_graph.size();
    }
    else {
        conf.nb_nodes = {-1};
    }
    conf.print_alias = print_alias;
    
//    cout << "Parse config" << endl;
    configparser::parse_config(conf_file, conf);

//    cout << "complete config" << endl;
    conf.complete_config();
//    cout << "Number of graphs=" << conf.nb_graphs << endl;

//    cout << "Config file: " << conf_file << endl;

    // #### ALGORITHM ####
//    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();


    // TODO: get the seed from the XML instead of this magic number
//    std::default_random_engine randomGeneratorForSeeding;
//    randomGeneratorForSeeding.seed(222);
//
////	ofstream *outputFiles = new ofstream[conf.schema.edges.size()];
//	string *tempOutputFileNames = new string[conf.schema.edges.size()];
//	int *seeds = new int[conf.schema.edges.size()];
//
//	int nmOfEdgeTypes = conf.schema.edges.size();
//	for (int i=0; i<nmOfEdgeTypes; i++) {
//		int seed = randomGeneratorForSeeding();
////		cout << "Seed " << i << ": " << seed << endl;
//		seeds[i] = seed;
//
//		tempOutputFileNames[i] = "tempOutputFile" + to_string(i) + ".txt";
//	}

	// ##Parallel##
//	processEdgeTypesParallel(0, nmOfEdgeTypes, seeds, tempOutputFileNames, conf);
//
//	// Merge the outputFiles
//	ofstream outputFile;
//	outputFile.open("output.txt", ios::trunc);
//	for (int i=0; i<nmOfEdgeTypes; i++) {
//		ifstream tempFile(tempOutputFileNames[i]);
////		outputFile << tempFile.rdbuf();
//		if (tempFile.is_open()) {
//			string line;
//			while (getline(tempFile, line)) {
//				outputFile << line << endl;
//			}
//		}
//	}
//	outputFile.close();
	// ##Parallel##

	// ##Sequential##
//    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
//
//    processingEdgeTypes processETs(conf, conf_file);
//    processETs.sequentialProcessing();
//
//    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
//    auto duration = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
//	cout << duration << ", ";
//	 ##Sequential##










//    if(graph_file != "") {





      chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
      for (int i=0; i<conf.nb_graphs; i++) {
//    	  cout << "i: " << i << endl;
//    	cout << "Processing graph " << i << endl;
    	config::config conf2;
//    	cout << "i: " << i << endl;
    	if (nb_nodes_per_graph.size() > i) {
    		conf2.nb_nodes.push_back(nb_nodes_per_graph[i]);
		} else {
			conf2.nb_nodes.push_back(-1);
		}
    	conf2.nb_graphs = conf.nb_graphs;

		configparser::parse_config(conf_file, conf2);
		conf2.complete_config();
    	report::report rep;

        ofstream graph_stream;
        string fileName = "outputGraph" + to_string(i) + ".txt";
        graph_stream.open(fileName);
//        cout << "graph generation" << endl;
        graph::ntriple_graph_writer writer(graph_stream);

        writer.build_graph(conf2, rep);
//        creatRankFileZipf(conf2, fileName);
//        creatRankFileNonZipf(conf2, fileName);
      }
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	auto durationWithMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();
	cout << durationWithMaterialize << ", ";


//	for (int i=0; i<=100000000; i+=100000000/10) {
//		cout << "./src/test -c use-cases/test.xml -n " << i/10 << "-" << 2*i/10 << "-" << 3*i/10 << "-" << 4*i/10 << "-" << 5*i/10 <<  "-" << 6*i/10 << "-" << 7*i/10 << "-" << 8*i/10 << "-" << 9*i/10 << "-" << 10*i/10 <<endl;
////		cout << "./src/test -c use-cases/test.xml -n " << i <<endl;
//	}


//
//        for (int i=0; i<conf.schema.edges.size(); i++) {
//			ifstream tempFile("tempOutputOld" + to_string(i) + ".txt");
//			if (tempFile.is_open()) {
//				string line;
//				while (getline(tempFile, line)) {
//					graph_stream << line << endl;
//				}
//			}
//		}
//        graph_stream.close();
//
//
//        ofstream report_stream;
//        report_stream.open(report_directory + "/graph.html");
//        html_graph_report(conf, rep, report_stream);
//    }
//
//    if (workload_file != "") {
//        report::workload_report rep;
//
//        ofstream workload_stream;
//        workload_stream.open("workloadTest.txt");
//        workload::workload wl;
//        if(true) { // selectivity
//            workload2::generate_workload(conf, wl, rep);
//        }
//        else {
//            workload::generate_workload(conf, wl);
//        }
//        workload::write_xml(wl, workload_stream, conf);
//        workload_stream.close();
//
//        ofstream report_stream;
//        report_stream.open(report_directory + "/workload.html");
//        html_workload_report(conf, rep, report_stream);
//
//        /*
//        workload2::matrix mat;
//        workload2::distance_matrix_between_types(conf, mat);
//        cout << mat << endl;
//        workload2::graph graph;
//        compute_graph_from_matrix(mat, 4, graph);
//        workload2::matrix_of_paths mat2;
//        number_of_paths(graph, config::selectivity::CONSTANT, 3, mat2);
//        vector<workload2::triple> path;
//        generate_random_path(graph, mat2, -1, 3, path);
//        for (auto & triple : path) {
//            cout << triple << endl;
//        }
//        */
//    }




//    analysisIncrDetGraph analyzeGraph("test.txt", conf);
//    analyzeGraph.stability(0);
  	// #### ANALYSIS ####
//    for (int g=0; g<1; g++) {
////    	cout << "g=" << g << endl;
//    	string outpName = "outputGraph" + to_string(g) + ".txt";
//    	analysisIncrDetGraph analyzeGraph(outpName, conf);
//
//       ofstream rFile, rFileHulp;
////       string rName = "distributionAnalysis" + to_string(g) + ".r";
//       string rNameHulp = "distributionAnalysisHulp" + to_string(g) + ".r";
////       rFile.open(rName, ios::trunc);
//       rFileHulp.open(rNameHulp, ios::trunc);
//       rFile << "# This R-file is generated by gMark" << endl;
//       int i =0;
//       for(config::edge e: conf.schema.edges) {
//			rFile << "# Analysis of the distributions of edge-type: " << i << endl;
//			cout << "# Analysis of the distributions of edge-type: " << i << endl;
//	//       	analyzeGraph.distributionAnalysis2(e, rFile);
//			analyzeGraph.distributionAnalysis(e, rFileHulp, g);
//
//			rFile << endl;
//
//			i++;
//       }

//	rFile << "meanMin = mean(c(min0, min1, min2, min3, min4, min5, min6, min7, min8, min9, min10, min11, min12, min13, min14, min15, min16, min17, min18, min19))" << endl;
//	rFile << "sdMin = sd(c(min0, min1, min2, min3, min4, min5, min6, min7, min8, min9, min10, min11, min12, min13, min14, min15, min16, min17, min18, min19))" << endl;
//	rFile << "meanMax = mean(c(max0, max1, max2, max3, max4, max5, max6, max7, max8, max9, max10, max11, max12, max13, max14, max15, max16, max17, max18, max19))" << endl;
//	rFile << "sdMax = sd(c(max0, max1, max2, max3, max4, max5, max6, max7, max8, max9, max10, max11, max12, max13, max14, max15, max16, max17, max18, max19))" << endl;
//	rFile << "meanMean = mean(c(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19))" << endl;
//	rFile << "sdMean = sd(c(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19))" << endl;

//	rFile << "meanMean = mean(c(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19))" << endl;
//	rFile << "sdMean = sd(c(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19))" << endl;
//	rFile << "meanSd = mean(c(sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7, sd8, sd9, sd10, sd11, sd12, sd13, sd14, sd15, sd16, sd17, sd18, sd19))" << endl;
//	rFile << "sdSd = sd(c(sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7, sd8, sd9, sd10, sd11, sd12, sd13, sd14, sd15, sd16, sd17, sd18, sd19))" << endl;


//    rFile << "meanMean = mean(c(alpha0, alpha1, alpha2, alpha3, alpha4, alpha5, alpha6, alpha7, alpha8, alpha9, alpha10, alpha11, alpha12, alpha13, alpha14, alpha15, alpha16, alpha17, alpha18, alpha19))" << endl;
//	rFile << "sdMean = sd(c(alpha0, alpha1, alpha2, alpha3, alpha4, alpha5, alpha6, alpha7, alpha8, alpha9, alpha10, alpha11, alpha12, alpha13, alpha14, alpha15, alpha16, alpha17, alpha18, alpha19))" << endl;

//       	rFile.close();
//

//    }
//    analyzeGraph.stability(0);
////      analyzeGraph.numberOfEdgesVsNode();

  	// #### ANALYSIS ####









}

