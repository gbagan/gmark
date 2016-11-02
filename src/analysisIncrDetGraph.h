/*
 * analysisIncrDetGraph.h
 *
 *  Created on: Sep 19, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef ANALYSISINCRDETGRAPH_H_
#define ANALYSISINCRDETGRAPH_H_

#include <iostream>
#include <string>
#include "config.h"

namespace std {

class analysisIncrDetGraph {
private:
	string outpuFile;
	config::config conf;

	void printToRfile(ofstream & rFile, bool outDistr, config::edge edge);
public:
	analysisIncrDetGraph(string oututFile, config::config configuration);
	virtual ~analysisIncrDetGraph();

	void numberOfNodesAnalysis();
	void distributionAnalysis(config::edge edgeType, ofstream & rFile);
	void numberOfEdgesVsNode();
	void zipfianPosAnalysis(int egdeTypeId, bool outDistr);
	void zipfianPosAnalysis();
};

} /* namespace std */

#endif /* ANALYSISINCRDETGRAPH_H_ */
