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
#include "../config.h"

namespace std {

class analysisIncrDetGraph {
private:
	string outpuFile;
	config::config conf;
	int id = 0;

	void printToRfile(ofstream & rFile, bool outDistr, config::edge edge);
	void printToRfile2(ofstream & rFile, bool outDistr, config::edge edge);
public:
	analysisIncrDetGraph(string oututFile, config::config configuration);
	virtual ~analysisIncrDetGraph();

	void numberOfNodesAnalysis();
	void distributionAnalysis(config::edge edgeType, ofstream & rFile, int graphNumber);
	void distributionAnalysis2(config::edge edgeType, ofstream & rFile, int graphNumber);
	void numberOfEdgesVsNode();
//	void zipfianPosAnalysis(int egdeTypeId, bool outDistr);
//	void zipfianPosAnalysis();
	void stability(int edgeTypeId);
};

} /* namespace std */

#endif /* ANALYSISINCRDETGRAPH_H_ */