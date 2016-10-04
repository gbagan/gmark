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
	void processEdgeType(config::edge & edgeType);
	int processIteration(int iterationNumber, config::edge & edgeType, int numberOfNodesForMax);

	// For each iteration
	int getNumberOfEdgesPerIteration(config::edge & edgeType, int iterationNumber);
	int getNumberOfOpenICs(vector<graphNode> nodes, int numberOfNodes, int edgeTypeId, bool isOutDistr);

	double getMeanEdgesPerNode(config::edge & edgeType, distribution distr, int zipfMax);
	int getNumberOfEdgesPerIteration(config::edge & edgeType);

	graphNode findSourceNode(config::edge & edgeType, int iterationNumber);
	graphNode findTargetNode(config::edge & edgeType, int iterationNumber, graphNode sourceNode);
	graphNode findNodeIdFromCumulProbs(vector<float> & cumulProbs, int nodeType);

	vector<float> getCdf(distribution distr, int nodeType, int edgeTypeNumber, int iterationNumber, graphNode sourceNode, bool findSourceNode);
	void addEdge(graphEdge & e, config::edge & edgeType);

	int updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> nodes, int iterationNumber, int edgeTypeId, distribution distr, bool outDistr);
	pair<int,int> updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType, int nmNodesMax, int nmNodesMin);
	void updateICsForNonScalableType(vector<graphNode> nodes, int iterationNumber, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distrToUpdate, int edgeTypeId, bool updateSubjects);
//	void shiftZipfianDistribution(vector<graphNode> & nodes, int nmNodes, int edgeTypeId, bool changeSubjectNodes);

	void changeDistributionParams(config::edge & edgeType);
	void changeDistributionParams(config::edge & edgeType, double meanICsPerNodeForOtherDistr, bool changeOutDistr, double subjectProbOrSize, double ofjectProbOrSize);
public:
	incrementalDeterministicGraphGenerator(config::config configuration);
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph();
	incrementalDeterministicGraph getGraph() {
		return graph;
	}
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
