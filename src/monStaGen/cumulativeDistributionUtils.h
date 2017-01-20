/*
 * cumulDistributionUtils.h
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#ifndef CUMULATIVEDISTRIBUTIONUTILS_H_
#define CUMULATIVEDISTRIBUTIONUTILS_H_

#include <vector>
#include "graphNode.h"
#include "../config.h"

namespace std {

//struct posIdProbTripple {
//	float position;
//	int id;
//	float zipfProb;
//	bool operator<(posIdProbTripple other) const
//	{
//		return id < other.id;
//	}
//};

class cumulativeDistributionUtils {
private:
//	void printKeyValuesVector(vector<pair<float, int>> keyValues);
//	void printTripples(vector<posIdProbTripple> tripple);
public:
	cumulativeDistributionUtils();
	virtual ~cumulativeDistributionUtils();

	int calculateCDF(vector<graphNode*> & nodes, graphNode & sourceNode, double randomValue);
//	vector<float> calculateZipfCumulPercentagesForNnodes(vector<graphNode> & nodes, int currentEdgeTypeNumber, float alpha, int iterationNumber, bool findSource);
	vector<double> zipfCdf(double alpha, int iterationNumber);

	int findPositionInCdf(vector<double> & cdf, double randomValue);

	// For sorting
//	bool compare(const pair<pair<float, int>, float> & first, const pair<pair<float, int>, float> & second);
};

} /* namespace std */

#endif /* CUMULATIVEDISTRIBUTIONUTILS_H_ */
