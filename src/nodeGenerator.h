/*
 * nodeGenerator.h
 *
 *  Created on: Sep 26, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef NODEGENERATOR_H_
#define NODEGENERATOR_H_

#include <random>
#include "config.h"
#include "incrementalDeterministicGraph.h"

namespace std {

class nodeGenerator {

public:
	default_random_engine* randomGenerator;
	incrementalDeterministicGraph* graph;
	int nextNodeId;
	config::config* conf;

	nodeGenerator();
	nodeGenerator(default_random_engine* randomGenerator, incrementalDeterministicGraph* graph, config::config* conf);
	virtual ~nodeGenerator();

	int addOrUpdateNodes(config::edge & edgeType, int iterationNumber, int numberOfNodesOfMax, int type1, int type2, bool subject);
	int addOrUpdateSubjectNodes(config::edge & edgeType, int iterationNumber, int numberOfNodesOfMax);
	int addOrUpdateObjectNodes(config::edge & edgeType, int iterationNumber, int numberOfNodesOfMax);
	void findOrCreateNode(config::edge & edgeType, bool findSourceNode, int iterationNumber);

	void addInterfaceConnectionsToNode(graphNode &n, distribution distr, int edgeTypeNumber, bool findSourceNode);
	void addNode(graphNode n);
};

} /* namespace std */

#endif /* NODEGENERATOR_H_ */
