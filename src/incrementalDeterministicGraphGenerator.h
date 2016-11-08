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
	int timeForNodeGen = 0;
	int timeForUpdating = 0;
	int timeForShuffling = 0;
	int timeForAddingEdges = 0;

//	int timeForIteration = 0;



	config::config conf;
	pair<vector<graphNode>, vector<graphNode>> nodes;
	struct edge2 {
		string subjectId;
		string predicate;
		string objectId;
	};
	vector<edge2> edges;
	graphNode tempNode = graphNode();
	int outputBufferLines = 0;

	cumulativeDistributionUtils cumDistrUtils;


	std::default_random_engine randomGenerator;

	nodeGenerator nodeGen;

	uniform_real_distribution<double> uniformDistr = uniform_real_distribution<double>(0.0,1.0);

//	void initializeNodesAndEdges();
	void processIteration(config::edge & edgeType);

	// For each iteration
//	int getNumberOfEdgesPerIteration(config::edge & edgeType, int iterationNumber);
//	int getNumberOfEdgesPerIteration(config::edge edgeType, pair<int, int> zipfOpenInterfaceConnections);
//	int getNumberOfEdgesPerIteration(config::edge edgeType, vector<graphNode*> subjectNodesWithOpenICs, vector<graphNode*> objectNodesWithOpenICs, int iterationNumber);
//	int getNumberOfOpenICs(vector<graphNode*> nodes);

	double getMeanICsPerNode(distribution & distr, int zipfMax);

//	int findSourceNode(config::edge & edgeType, vector<graphNode*> &nodesWithOpenICs);
//	int findTargetNode(config::edge & edgeType, int sourceNodeLocalId, vector<graphNode*> &nodesWithOpenICs);
//	graphNode *findNodeIdFromCumulProbs(vector<float> & cumulProbs, bool findSourceNode);

	void addEdge(graphNode &sourceNode, graphNode &targetNode, int predicate);

	void updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodes, distribution distr);
//	pair<int,int> updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType);
	void updateICsForNonScalableType(vector<graphNode> & nodes, int nmNodesOther, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distrToUpdate);
	void updateICsForNonScalableType(config::edge & edgeType);
//	void shiftZipfianDistribution(vector<graphNode> & nodes, int nmNodes, int edgeTypeId, bool changeSubjectNodes);

	void changeDistributionParams(config::edge & edgeType);
	void changeDistributionParams(distribution & distr, double meanICsPerNodeForOtherDistr, double probOrSizeOther, double probOrSize);

	int getDistributionRandomnessTradeoff(config::edge edgeType, int iterationNumber);

//	vector<graphNode*> constructNodesVectorAndRemoveNodeWithZeroICs(vector<graphNode> &nodes_);

	void printRank(vector<graphNode> nodes, int edgeTypeId, int nbNodes);
public:
	incrementalDeterministicGraphGenerator();
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph(ofstream*  outputFile, int* seeds, int edgeTypeIdLow, int edgeTypeIdHigh);
	int processEdgeType(config::config configuration, config::edge & edgeType, ofstream & outputFile, bool printZipfPos);
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
