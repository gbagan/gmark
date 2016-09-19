/*
 * analysisIncrDetGraph.h
 *
 *  Created on: Sep 19, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef ANALYSISINCRDETGRAPH_H_
#define ANALYSISINCRDETGRAPH_H_

#include <iostream>
#include "config.h"
#include "incrementalDeterministicGraph.h"

namespace std {

class analysisIncrDetGraph {
private:
	incrementalDeterministicGraph graph;
	config::config conf;

	void printToRfile(ofstream & rFile, bool outDistr, config::edge edge);
public:
	analysisIncrDetGraph(incrementalDeterministicGraph graph, config::config configuration);
	virtual ~analysisIncrDetGraph();

	void numberOfNodesAnalysis();
	void distributionAnalysis(int edgeTypeNumber, ofstream & rFile);
};

} /* namespace std */

#endif /* ANALYSISINCRDETGRAPH_H_ */
