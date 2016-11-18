/*
 * processingEdgeTypes.h
 *
 *  Created on: Nov 18, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef PROCESSINGEDGETYPES_H_
#define PROCESSINGEDGETYPES_H_

#include "config.h"

namespace std {

class processingEdgeTypes {
	config::config conf;
	vector<int> nb_nodes_per_graph;
	string conf_file;
public:
	processingEdgeTypes(config::config conf, vector<int> nb_nodes_per_graph, string conf_file);
	virtual ~processingEdgeTypes();

	void sequentialProcessing();
};

} /* namespace std */

#endif /* PROCESSINGEDGETYPES_H_ */
