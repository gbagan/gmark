/*
 * incrementalDeterministicGraphGenerator.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef INCREMENTALDETERMINISTICGRAPHGENERATOR_H_
#define INCREMENTALDETERMINISTICGRAPHGENERATOR_H_

#include <random>
#include <fstream>
#include "config.h"
#include "cumulativeDistributionUtils.h"
#include "nodeGenerator.h"

namespace std {


class incrementalDeterministicGraphGenerator {
private:
	config::config conf;
	pair<vector<graphNode>, vector<graphNode>> nodes;
	graphNode tempNode = graphNode();

	cumulativeDistributionUtils cumDistrUtils;


	std::default_random_engine randomGenerator;

	nodeGenerator nodeGen;

	uniform_real_distribution<double> uniformDistr = uniform_real_distribution<double>(0.0,1.0);

//	void initializeNodesAndEdges();
	void processIteration(int iterationNumber, config::edge & edgeType, ofstream*  outputFile);

	// For each iteration
//	int getNumberOfEdgesPerIteration(config::edge & edgeType, int iterationNumber);
	int getNumberOfEdgesPerIteration(config::edge edgeType, pair<int, int> zipfOpenInterfaceConnections);
	int getNumberOfOpenICs(vector<graphNode> nodes);

	double getMeanICsPerNode(distribution & distr, int zipfMax);

	graphNode *findSourceNode(config::edge & edgeType);
	graphNode *findTargetNode(config::edge & edgeType, graphNode & sourceNode);
//	graphNode *findNodeIdFromCumulProbs(vector<float> & cumulProbs, bool findSourceNode);

	void addEdge(graphNode *sourceNode, graphNode *targetNode, int predicate, ofstream*  outputFiles);

	int updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodes, distribution distr);
	pair<int,int> updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType);
	void updateICsForNonScalableType(vector<graphNode> nodes, int iterationNumber, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distrToUpdate);
	void updateICsForNonScalableType(config::edge & edgeType, int iterationNumber);
//	void shiftZipfianDistribution(vector<graphNode> & nodes, int nmNodes, int edgeTypeId, bool changeSubjectNodes);

	void changeDistributionParams(config::edge & edgeType);
	void changeDistributionParams(distribution & distr, double meanICsPerNodeForOtherDistr, double probOrSizeOther, double probOrSize);
public:
	incrementalDeterministicGraphGenerator(config::config configuration);
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph(ofstream*  outputFile, int* seeds, int edgeTypeIdLow, int edgeTypeIdHigh);
	void processEdgeType(config::edge & edgeType, ofstream*  outputFile, int seed);
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
