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
#include "graphNode.h"

namespace std {

class nodeGenerator {
private:
	void addNode(config::edge & edgeType, bool findSourceNode);
	void addNodes(config::edge & edgeType, int type1, int type2, bool subject);
	int getNumberOfICs(distribution distr, bool addSourceNode);
	void initializeConnections(graphNode &n, int maxNumberOfConnections);

//	uniform_int_distribution<int> uniformOutDistr;
//	normal_distribution<double> normalOutDistr;
//	uniform_int_distribution<int> uniformInDistr;
//	normal_distribution<double> normalInDistr;
//	uniform_real_distribution<double> uniformZeroOneDistr;
public:
	default_random_engine* randomGenerator;
	pair<vector<graphNode>, vector<graphNode>>* nodes;
	config::config* conf;
	int birthIdSubject;
	int birthIdObject;

	nodeGenerator();
	nodeGenerator(config::edge & edgeType, int birthIdSub, int birthIdOb, default_random_engine* randomGenerator, pair<vector<graphNode>, vector<graphNode>>* nodes, config::config* conf);
	virtual ~nodeGenerator();

	void addSubjectNodes(config::edge & edgeType);
	void addObjectNodes(config::edge & edgeType);

};

} /* namespace std */

#endif /* NODEGENERATOR_H_ */
