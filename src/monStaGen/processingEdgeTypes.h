/*
 * processingEdgeTypes.h
 *
 *  Created on: Nov 18, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef PROCESSINGEDGETYPES_H_
#define PROCESSINGEDGETYPES_H_

#include "../config.h"
#include "incrementalDeterministicGraphGenerator.h"

namespace std {

class processingEdgeTypes {
private:
	config::config conf;
//	vector<int> nb_nodes_per_graph;
	string conf_file;

	void processEdgeType(config::edge & edgeType, bool printNodeProperties, string graphFile);
public:
	processingEdgeTypes(config::config conf, string conf_file);
	virtual ~processingEdgeTypes();

	void sequentialProcessing(bool printNodeProperties, string graphFile);
};

} /* namespace std */

#endif /* PROCESSINGEDGETYPES_H_ */
