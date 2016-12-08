/*
 * incrementalDeterministicGraphGenerator.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef INCREMENTALDETERMINISTICGRAPHGENERATOR_H_
#define INCREMENTALDETERMINISTICGRAPHGENERATOR_H_

#include <random>
#include <algorithm>
#include <fstream>
#include "config.h"
#include "cumulativeDistributionUtils.h"
#include "nodeGenerator.h"

namespace std {


class incrementalDeterministicGraphGenerator {
private:
//	int timeForIteration = 0;
	int outDistrShift = 0;
	int inDistrShift = 0;

	config::config conf;
	pair<vector<graphNode>, vector<graphNode>> nodes;
	struct edge2 {
		int subjectIterationId;
		int objectIterationId;
		string subjectId;
		string predicate;
		string objectId;
	};
	int graphNumber = 0;
	vector<edge2> edges;
	graphNode tempNode = graphNode();
	int outputBufferLines = 0;

	cumulativeDistributionUtils cumDistrUtils;


	std::default_random_engine randomGenerator;

	nodeGenerator nodeGen;

	uniform_real_distribution<double> uniformDistr = uniform_real_distribution<double>(0.0,1.0);

	void incrementGraph(config::edge & edgeType);


	double getMeanICsPerNode(distribution & distr, int zipfMax);

	void addEdge(graphNode &sourceNode, graphNode &targetNode, int predicate);

	void updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodes, distribution distr, bool outDistr);

	void performShiftForNonScalableNodes(config::edge & edgeType);


	void updateShifts(config::edge & edgeType, config::config & previousConf);
	void fixSchemaInequality(config::edge & edgeType);

	void performSchemaIndicatedShift(config::edge & edgeType);
	void performFixingShiftForZipfian(config::edge & edgeType, vector<int> & subjectNodeIdVector, vector<int> & objectNodeIdVector);

	void performOutDistributionShift(config::edge & edgeType);
	void performInDistributionShift(config::edge & edgeType);

	void printRankZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes);
	void printRankNonZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes);


	void calculateSimilarity(graphNode n1, graphNode n2);
public:
	incrementalDeterministicGraphGenerator();
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph(ofstream*  outputFile, int* seeds, int edgeTypeIdLow, int edgeTypeIdHigh);
	int processEdgeTypeSingleGraph(config::config configuration, config::config previousConf, config::edge & edgeType, ofstream & outputFile, int graphNumber);
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
