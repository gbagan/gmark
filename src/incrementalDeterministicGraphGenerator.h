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
#include "incrementalDeterministicGraph.h"
#include "cumulativeDistributionUtils.h"
#include "nodeGenerator.h"

namespace std {


class incrementalDeterministicGraphGenerator {
private:
	config::config conf;
	incrementalDeterministicGraph graph;
	cumulativeDistributionUtils cumDistrUtils;

	std::default_random_engine randomGeneratorForSeeding;
	std::default_random_engine randomGenerator;

	nodeGenerator nodeGen;

	int zipfianStartValueOut = 1;
	int zipfianStartValueIn = 1;

	void initializeNodesAndEdges();
	void processEdgeType(config::edge & edgeType, ofstream*  outputFile);
	void processIteration(int iterationNumber, config::edge & edgeType, ofstream*  outputFile);

	// For each iteration
	int getNumberOfEdgesPerIteration(config::edge & edgeType, int iterationNumber);
	int getNumberOfOpenICs(vector<graphNode> nodes, bool isOutDistr);

	double getMeanEdgesPerNode(config::edge & edgeType, distribution distr, int zipfMax);
	int getNumberOfEdgesPerIteration(config::edge & edgeType);

	graphNode *findSourceNode(config::edge & edgeType);
	graphNode *findTargetNode(config::edge & edgeType, graphNode *sourceNode);
	graphNode *findNodeIdFromCumulProbs(vector<float> & cumulProbs, bool findSourceNode);

	vector<float> getCdf(distribution distr, int nodeType, graphNode *sourceNode, bool findSourceNode);
	void addEdge(graphNode *sourceNode, graphNode *targetNode, int predicate, ofstream*  outputFiles);

	int updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodes, distribution distr, bool outDistr);
	pair<int,int> updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType);
	void updateICsForNonScalableType(vector<graphNode> nodes, int iterationNumber, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distrToUpdate, int edgeTypeId, bool updateSubjects);
//	void shiftZipfianDistribution(vector<graphNode> & nodes, int nmNodes, int edgeTypeId, bool changeSubjectNodes);

	void changeDistributionParams(config::edge & edgeType);
	void changeDistributionParams(config::edge & edgeType, double meanICsPerNodeForOtherDistr, bool changeOutDistr, double subjectProbOrSize, double ofjectProbOrSize);
public:
	incrementalDeterministicGraphGenerator(config::config configuration);
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph(ofstream*  outputFile);
	incrementalDeterministicGraph getGraph() {
		return graph;
	}
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
