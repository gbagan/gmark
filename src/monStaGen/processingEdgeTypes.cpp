/*
 * processingEdgeTypes.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: wilcovanleeuwen
 */

#include "processingEdgeTypes.h"
#include "../configparser.h"

namespace std {

processingEdgeTypes::processingEdgeTypes(config::config conf_, string conf_file_) {
	this->conf = conf_;
	this->conf_file = conf_file_;
}

processingEdgeTypes::~processingEdgeTypes() {
	// TODO Auto-generated destructor stub
}

void processingEdgeTypes::sequentialProcessing(bool printNodeProperties, string graphFile) {
	// Delete the output graphs
	for (unsigned int i=0; i<conf.nb_graphs; i++) {
		string file = graphFile + to_string(i) + ".txt";
		const char * fileChar = file.c_str();
		remove(fileChar);
	}


	for (config::edge edgeType: conf.schema.edges) {
//		cout << "Processing edge-type " << i << endl;
		processEdgeType(edgeType, printNodeProperties, graphFile);
	}
}

void processingEdgeTypes::processEdgeType(config::edge & edgeType, bool printNodeProperties, string graphFile) {
	incrementalDeterministicGraphGenerator graphGenerator = incrementalDeterministicGraphGenerator();

	// For all different graph sizes
	for (unsigned int j=0; j<conf.nb_graphs; j++) {
		// Define the output file
		ofstream outputFile;
		outputFile.open(graphFile + to_string(j) + ".txt", ios::app);

		graphGenerator.processEdgeTypeSingleGraph(conf, conf.schema.edges.at(edgeType.edge_type_id), outputFile, graphFile, j, printNodeProperties);

	}
}

} /* namespace std */
