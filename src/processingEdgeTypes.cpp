/*
 * processingEdgeTypes.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: wilcovanleeuwen
 */

#include "processingEdgeTypes.h"
#include "configparser.h"
#include "incrementalDeterministicGraphGenerator.h"

namespace std {

processingEdgeTypes::processingEdgeTypes(config::config conf_, vector<int> nb_nodes_per_graph_, string conf_file_) {
	this->conf = conf_;
	this->nb_nodes_per_graph = nb_nodes_per_graph_;
	this->conf_file = conf_file_;
}

processingEdgeTypes::~processingEdgeTypes() {
	// TODO Auto-generated destructor stub
}

void processingEdgeTypes::sequentialProcessing() {
	auto runningTime = 0;
	for (int i=0; i<conf.nb_graphs; i++) {
		string file = "outputGraph" + to_string(i+1) + ".txt";
		const char * fileChar = file.c_str();
		remove(fileChar);
	}
	for (int i=0; i<conf.schema.edges.size(); i++) {
//		cout << "Processing edge-type " << i << endl;

		incrementalDeterministicGraphGenerator graphGenerator = incrementalDeterministicGraphGenerator();
		for (int j=0; j<conf.nb_graphs; j++) {
			config::config conf2;
			if (nb_nodes_per_graph.size() > j) {
				conf2.nb_nodes = nb_nodes_per_graph[j];
			} else {
				conf2.nb_nodes = 0;
			}
			configparser::parse_config(conf_file, conf2, j);
			conf2.complete_config();

			ofstream outputFile;
			outputFile.open("outputGraph" + to_string(j+1) + ".txt", ios::app);
			if (j == 0) {
				runningTime += graphGenerator.processEdgeType(conf2, conf2.schema.edges.at(i), outputFile, true);
			} else {
				runningTime += graphGenerator.processEdgeType(conf2, conf2.schema.edges.at(i), outputFile, false);
			}
		}
	}

	cout << runningTime << ", ";
}

} /* namespace std */
