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


class cumulativeDistributionUtils {

public:
	cumulativeDistributionUtils();
	virtual ~cumulativeDistributionUtils();

	int calculateCDF(vector<graphNode*> & nodes, graphNode & sourceNode, double randomValue);
	vector<double> zipfCdf(double alpha, int iterationNumber);
	int findPositionInCdf(vector<double> & cdf, double randomValue);

};

} /* namespace std */

#endif /* CUMULATIVEDISTRIBUTIONUTILS_H_ */
