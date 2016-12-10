/*
 * processingEdgeTypes.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: wilcovanleeuwen
 */

#include "processingEdgeTypes.h"
#include "configparser.h"

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
	// Delete the output graphs
	auto runningTime = 0;
	for (int i=0; i<conf.nb_graphs; i++) {
		string file = "outputGraph" + to_string(i) + ".txt";
		const char * fileChar = file.c_str();
		remove(fileChar);
	}

//	// Keep track of the edge-types that are already processed due to the correlation
//	vector<int> allreadyProcessed;
//
//	vector<incrementalDeterministicGraphGenerator::edge2> generatedEdges;
//	for (config::correlationDef correlation: conf.correlations) {
//		for (int edgeTypeIdBasis: correlation.basis_for_correlation) {
//			vector<incrementalDeterministicGraphGenerator::edge2> edges = processEdgeType(edgeTypeIdBasis);
//		}
//	}

	// Generate the remaining edge-types
	for (int i=0; i<conf.schema.edges.size(); i++) {
//		cout << "Processing edge-type " << i << endl;
		processEdgeType(i);
	}

	cout << runningTime << ", ";
}

vector<incrementalDeterministicGraphGenerator::edge2> processingEdgeTypes::processEdgeType(int edgeTypeId) {
	incrementalDeterministicGraphGenerator graphGenerator = incrementalDeterministicGraphGenerator();
//	config::config previousConf = conf;
	for (int j=0; j<conf.nb_graphs; j++) {
		config::config conf2;
		if (nb_nodes_per_graph.size() > j) {
			conf2.nb_nodes = nb_nodes_per_graph[j];
		} else {
			conf2.nb_nodes = 0;
		}
		configparser::parse_config(conf_file, conf2, j);
		conf2.complete_config();


		for (config::correlationDef corr: conf2.correlations) {
			cout << "Correlation in conf2: " << endl;
			cout << " Basis: ";
			for(int i: corr.basis_for_correlation) {
				cout << i << ",";
			}
			cout << endl << " Target: ";
			for(int i: corr.target_for_correlation) {
				cout << i << ",";
			}
			cout << endl;
		}
		ofstream outputFile;
		outputFile.open("outputGraph" + to_string(j) + ".txt", ios::app);

		graphGenerator.processEdgeTypeSingleGraph(conf2, conf2.schema.edges.at(edgeTypeId), outputFile, j);

//		previousConf = conf2;
	}
	return graphGenerator.edges;
}

} /* namespace std */
