/*
 * analyseCorrelation.h
 *
 *  Created on: Dec 19, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef ANALYSECORRELATION_H_
#define ANALYSECORRELATION_H_

#include "incrementalDeterministicGraphGenerator.h"

namespace std {

class analyseCorrelation {
private:
	config::config conf;
	double calculateSimilarity(int n1, int n2, vector<incrementalDeterministicGraphGenerator::edge2> edges, int nbEdgeTypesInCorrelation);

	void addBasisEdges();
	vector<incrementalDeterministicGraphGenerator::edge2> addEdges(string fileName, vector<int> edgeTypeId);

	void printSimilarities(int nodes, vector<incrementalDeterministicGraphGenerator::edge2> edges, int nbEdgeTypesInCorrelation);

	int findEdgeTypeId(int subjectId, int predicate, int objectId);
public:
	analyseCorrelation(config::config & conf);
	virtual ~analyseCorrelation();

	void analyze(config::edge & edgeType, int graphNumber);

};

} /* namespace std */

#endif /* ANALYSECORRELATION_H_ */