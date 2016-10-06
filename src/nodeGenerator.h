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
	config::config* conf;

	nodeGenerator();
	nodeGenerator(default_random_engine* randomGenerator, incrementalDeterministicGraph* graph, config::config* conf);
	virtual ~nodeGenerator();

	void addNodes(config::edge & edgeType, int type1, int type2, bool subject);
	void addSubjectNodes(config::edge & edgeType);
	void addObjectNodes(config::edge & edgeType);
	void addNode(config::edge & edgeType, bool findSourceNode);

	void addInterfaceConnectionsToNode(graphNode &n, distribution distr);

	void initializeConnections(graphNode &n, int maxNumberOfConnections);
};

} /* namespace std */

#endif /* NODEGENERATOR_H_ */
