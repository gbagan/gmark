/*
 * incrementalDeterministicGraphGenerator.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef INCREMENTALDETERMINISTICGRAPHGENERATOR_H_
#define INCREMENTALDETERMINISTICGRAPHGENERATOR_H_

#include <random>
#include "config.h"
#include "incrementalDeterministicGraph.h"
#include "cumulativeDistributionUtils.h"

namespace std {

class incrementalDeterministicGraphGenerator {
private:
	config::config conf;
	incrementalDeterministicGraph graph;
	int nextNodeId;
	cumulativeDistributionUtils cumDistrUtils;

	std::default_random_engine randomGeneratorForSeeding;
	std::default_random_engine randomGenerator;

	void initializeNodesAndEdges();
	void processEdgeType(config::edge & edgeType);
	void processIteration(int iterationNumber, config::edge & edgeType);

	// For each iteration
	void findOrCreateNode(config::edge & edgeType, bool findSourceNode, int iterationNumber);
	int getNumberOfEdgesPerIteration(config::edge & edgeType, int iterationNumber);
	void addInterfaceConnectionsToNode(graphNode &n, distribution distr, int edgeTypeNumber, bool findSourceNode);
	void addNode(graphNode n);

	graphNode findSourceNode(config::edge & edgeType, int iterationNumber);
	graphNode findTargetNode(config::edge & edgeType, int iterationNumber);
	graphNode findNodeIdFromCumulProbs(vector<float> & cumulProbs, int nodeType);
	vector<float> getCdf(distribution distr, int nodeType, int edgeTypeNumber, int iterationNumber, bool findSourceNode);
	void addEdge(graphEdge e, config::edge & edgeType);
public:
	incrementalDeterministicGraphGenerator(config::config configuration);
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph();
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
