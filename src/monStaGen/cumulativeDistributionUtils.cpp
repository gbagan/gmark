/*
 * cumulDistributionUtils.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <iostream>
#include <cmath>
#include <utility>

#include "cumulativeDistributionUtils.h"

namespace std {

cumulativeDistributionUtils::cumulativeDistributionUtils() {
	// TODO Auto-generated constructor stub

}

cumulativeDistributionUtils::~cumulativeDistributionUtils() {
	// TODO Auto-generated destructor stub
}

int cumulativeDistributionUtils::calculateCDF(vector<graphNode*> & nodes, graphNode & sourceNode, double randomValue) {
	int sum = 0;
	vector<int> nonNormalizedResults;
	int i = 0;

	for(graphNode* & n: nodes) {
		int possibleConnections = n->getNumberOfOpenInterfaceConnections();
		possibleConnections = max(possibleConnections, 0);

		nonNormalizedResults.push_back(possibleConnections);
		sum += possibleConnections;
		i++;
	}

	if(sum == 0) {
//		cout << "Sum of the pdf is 0\n";
		return -1;
	}

	float cumulValue = 0;
//	vector<float> normalizedCumulResults;
//	cout << "normalizedCumulResults size: " << nonNormalizedResults.size() << ": ";
	for(unsigned int i=0; i<nonNormalizedResults.size(); i++) {
		cumulValue += (float) nonNormalizedResults.at(i) / sum;
		if (randomValue <= cumulValue) {
			return nodes[i]->iterationId;
		}
//		normalizedCumulResults.push_back(cumulValue);
//		cout << to_string(temp) << ", ";
	}
//	cout << "\n";

//	return normalizedCumulResults;
	return nodes.size()-1;
}




vector<double> cumulativeDistributionUtils::zipfCdf(double alpha, int n) {
//	cout << "Zipfian n=" << n << endl;
//	cout << "Zipfian alpha=" << alpha << endl;

	vector<double> zipfian;
	double sum = 0.0;
	double step;
	for(int i=1; i<=n; i++) {
		step = pow(i, -1.0*alpha);
		zipfian.push_back(step);
		sum += step;
	}


	vector<double> cdf;

	double tempSum = 0.0;
	for(int i=0; i<n; i++) {
		tempSum += zipfian.at(i) / sum;
		cdf.push_back(tempSum);
	}

	return cdf;
}


int binarySearch(int low, int high, vector<double> & cdf, double randomValue) {
	if (high-low <= 1) {
		if (randomValue > cdf[low]) {
			return high;
		} else {
			return low;
		}
	} else {
		int mid = (int) ceil((high+low)/2.0);
		if (randomValue < cdf[mid]) {
			return binarySearch(low, mid, cdf, randomValue);
		} else {
			return binarySearch(mid, high, cdf, randomValue);
		}
	}
}

int cumulativeDistributionUtils::findPositionInCdf(vector<double> & cdf, double randomValue) {
	int pos = 0;
	if (randomValue > cdf[0]) {
		pos = binarySearch(0, cdf.size(), cdf, randomValue);
		if (pos >= (int) cdf.size()) {
			pos = cdf.size()-1;
		}
	}
	return pos;
}

} /* namespace std */
