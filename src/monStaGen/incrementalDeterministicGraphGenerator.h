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
#include "../config.h"
#include "cumulativeDistributionUtils.h"
#include "nodeGenerator.h"

namespace std {


class incrementalDeterministicGraphGenerator {
public:
	struct edge2 {
		int subjectIterationId;
		int objectIterationId;
		int subjectId;
		string predicate;
		int objectId;
		int createdInGraph;
	};

	pair< vector<graphNode>, vector<graphNode> > nodes;
	vector<edge2> edges;
private:
	int outDistrShift = 0;
	int inDistrShift = 0;
	int graphNumber = 0;

	config::config conf;
	cumulativeDistributionUtils cumDistrUtils;
	std::default_random_engine randomGenerator;
	nodeGenerator nodeGen;

	uniform_real_distribution<double> uniformDistr = uniform_real_distribution<double>(0.0,1.0);

	void incrementGraph(config::edge & edgeType);


	double getMeanICsPerNode(distribution & distr, int zipfMax);

	void addEdge(graphNode &sourceNode, graphNode &targetNode, int predicate);

	void updateInterfaceConnectionsForZipfianDistributions(distribution distr, bool outDistr);

	void performShiftForNonScalableNodes(config::edge & edgeType);


//	void updateShifts(config::edge & edgeType, config::config & previousConf);
	void fixSchemaInequality(config::edge & edgeType);

	void performSchemaIndicatedShift(config::edge & edgeType);
	void performFixingShiftForZipfian(config::edge & edgeType, vector<int> & subjectNodeIdVector, vector<int> & objectNodeIdVector);

	void performOutDistributionShift(config::edge & edgeType);
	void performInDistributionShift(config::edge & edgeType);

	void printRankZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes);
	void printRankNonZipf(vector<graphNode> nodes, int edgeTypeId, int nbNodes);

	void generateEdges(config::edge & edgeType, double prob);


	// Correlation
	vector< vector<int> > mapping;
	vector<edge2> basis;

	vector<edge2> generateCorrelatedEdgeSet(config::edge & edgeType);

	void generateCorrelatedEdges(config::edge & edgeType, double prob, vector<edge2> correlatedEdges);

	void randomMapping(vector<int> subjects, vector<int> objects);
	void addToMapping(int subject, int target);

	void icPreservingMapping(vector<int> subjects, vector<int> objects, int correlatedETid);
	int findEdgeTypeId(int subjectId, int predicate, int objectId);
	vector<int> sortOnDegree(vector<int> objectsIterationIds);
	vector<int> sortOnDegree(vector<int> subjectsIterationIds, int correlatedETid);

public:
	incrementalDeterministicGraphGenerator();
	virtual ~incrementalDeterministicGraphGenerator();

	void generateIncDetGraph(ofstream*  outputFile, int* seeds, int edgeTypeIdLow, int edgeTypeIdHigh);
	int processEdgeTypeSingleGraph(config::config configuration, config::edge & edgeType, ofstream & outputFile, int graphNumber, bool printNodeProperties);
};

} /* namespace std */

#endif /* INCREMENTALDETERMINISTICGRAPHGENERATOR_H_ */
