/*
 * shiftingDegreeDistribution.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: wilcovanleeuwen
 */

#include "incrementalDeterministicGraphGenerator.h"

namespace std {

void incrementalDeterministicGraphGenerator::performSchemaIndicatedShift(config::edge & edgeType) {
	int sf = edgeType.scale_factor;
	if (sf <= 0) {
		return;
	}

	if (conf.types[edgeType.subject_type].size > conf.types[edgeType.object_type].size) {
		// Increment all ICs and openICs with 1
		outDistrShift += sf;
		cout << "Perform schema-indicated shift: out-degree distribution " << sf << endl;
		for (graphNode & node: nodes.first) {
			node.incrementOpenInterfaceConnectionsByN(sf);
			node.incrementInterfaceConnectionsByN(sf);
		}

		// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
		double num = ((double) sf * (double)conf.types[edgeType.subject_type].size[graphNumber]) / (double)conf.types[edgeType.object_type].size[graphNumber];
		int numIncrement = floor(num);
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue < num - (double)floor(num)) {
			numIncrement++;
		}
		inDistrShift += numIncrement;
		cout << "Perform schema-indicated shift: in-degree distribution " << sf << "\n" << endl;
		for (graphNode & node: nodes.second) {
			node.incrementOpenInterfaceConnectionsByN(numIncrement);
			node.incrementInterfaceConnectionsByN(numIncrement);
		}
	} else {
		// Increment all ICs and openICs with 1
		inDistrShift += sf;
		for (graphNode & node: nodes.second) {
			cout << "Perform schema-indicated shift: in-degree distribution " << sf << endl;
			node.incrementOpenInterfaceConnectionsByN(sf);
			node.incrementInterfaceConnectionsByN(sf);
		}

		// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
		double num = ((double) sf * (double)conf.types[edgeType.object_type].size[graphNumber]) / (double)conf.types[edgeType.subject_type].size[graphNumber];
		int numIncrement = floor(num);
		double randomValue = uniformDistr(randomGenerator);
		if (randomValue < num - (double)floor(num)) {
			numIncrement++;
		}
		outDistrShift += numIncrement;
		for (graphNode & node: nodes.first) {
			cout << "Perform schema-indicated shift: out-degree distribution " << sf << "\n" <<  endl;
			node.incrementOpenInterfaceConnectionsByN(numIncrement);
			node.incrementInterfaceConnectionsByN(numIncrement);
		}
	}
}

void incrementalDeterministicGraphGenerator::performFixingShiftForZipfian(config::edge & edgeType, vector<int> & subjectNodeIdVector, vector<int> & objectNodeIdVector) {
	// Perform a shift in one of the distributions, when there is a large difference in the lengths of the vectors
	if (subjectNodeIdVector.size() < objectNodeIdVector.size()) {
		int diff = objectNodeIdVector.size() - subjectNodeIdVector.size();
		int incr = diff / conf.types[edgeType.subject_type].size[graphNumber];
		if (incr > 0) {
			cout << "Update schema requirements in the presence of a Zipfian distribution in graphNumber " << graphNumber << endl;
			cout << "Shift out-distr with " << incr << " values to the right" << endl;
			outDistrShift += incr;
			cout << "Total outDistrShift: " << outDistrShift << endl << endl;
			for (graphNode & n: nodes.first) {
				n.incrementInterfaceConnectionsByN(incr);
				n.incrementOpenInterfaceConnectionsByN(incr);
				for (int i=0; i<incr; i++) {
					subjectNodeIdVector.push_back(n.iterationId);
				}
			}
		}
	} else {
		int diff = subjectNodeIdVector.size() - objectNodeIdVector.size();
		int incr = diff / conf.types[edgeType.object_type].size[graphNumber];
		if (incr > 0) {
			cout << "Update schema requirements in the presence of a Zipfian distribution in graphNumber " << graphNumber << endl;
			cout << "Shift in-distr with " << incr << " values to the right" << endl;
			inDistrShift += incr;
			cout << "Total inDistrShift: " << inDistrShift << endl << endl;
			for (graphNode & n: nodes.second) {
				n.incrementInterfaceConnectionsByN(incr);
				n.incrementOpenInterfaceConnectionsByN(incr);
				for (int i=0; i<incr; i++) {
					objectNodeIdVector.push_back(n.iterationId);
				}
			}
		}
	}
}



void incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(distribution distr, bool outDistr) {
//	cout << "New Zipfian case" << endl;
	vector<graphNode> *nodesVec;
	if (outDistr) {
		nodesVec = &nodes.first;
	} else {
		nodesVec = &nodes.second;
	}
	int nmNodes = nodesVec->size();

	vector<double> zipfianCdf = cumDistrUtils.zipfCdf(distr.arg2, nmNodes);

	int shift = 0;
	if (outDistr) {
		shift = outDistrShift;
	} else {
		shift = inDistrShift;
	}

	int newInterfaceConnections = 0;
	int difference = 0;
	for (graphNode & node: *nodesVec) {
		newInterfaceConnections = cumDistrUtils.findPositionInCdf(zipfianCdf, node.getPosition()) + shift;

//		cout << "newInterfaceConnections: " << newInterfaceConnections << endl;
		difference = newInterfaceConnections - node.getNumberOfInterfaceConnections();
		if (difference > 0) {
			node.incrementOpenInterfaceConnectionsByN(difference);
			node.setNumberOfInterfaceConnections(newInterfaceConnections);
		}
//		cout << "after openICs: " << node.getNumberOfOpenInterfaceConnections() << endl;
	}
}

void incrementalDeterministicGraphGenerator::performOutDistributionShift(config::edge & edgeType) {
	double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, -1) + outDistrShift;
	double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, -1) + inDistrShift;
	int newMeanOutDistr = ((double) conf.types.at(edgeType.object_type).size[graphNumber] * meanInDistr) / (double) conf.types.at(edgeType.subject_type).size[graphNumber];
	int incr = floor(newMeanOutDistr - meanOutDistr);

	if (incr > 0) {
		cout << "Conflicting schema requirements identified. The mean of the out-degree distribution will be set to " << newMeanOutDistr << endl;
		cout << "This is done by a shift of " << incr << " value to the right in the out-degree distribution\n" << endl;
		outDistrShift += incr;
		for (graphNode & n: nodes.first) {
			n.incrementInterfaceConnectionsByN(incr);
			n.incrementOpenInterfaceConnectionsByN(incr);
		}
	}
}

void incrementalDeterministicGraphGenerator::performInDistributionShift(config::edge & edgeType) {
	double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, -1) + outDistrShift;
	double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, -1) + inDistrShift;
	double newMeanInDistr = ((double) conf.types.at(edgeType.subject_type).size[graphNumber] * meanOutDistr) / (double) conf.types.at(edgeType.object_type).size[graphNumber];
	int incr = floor(newMeanInDistr - meanInDistr);

	if (incr > 0) {
		cout << "Conflicting schema requirements identified. The mean of the in-degree distribution will be set to " << newMeanInDistr << endl;
		cout << "This is done by a shift of " << incr << " value to the right in the in-degree distribution\n" << endl;
		inDistrShift += incr;
		for (graphNode & n: nodes.second) {
			n.incrementInterfaceConnectionsByN(incr);
			n.incrementOpenInterfaceConnectionsByN(incr);
		}
	}
}

void incrementalDeterministicGraphGenerator::performShiftForNonScalableNodes(config::edge & edgeType) {
	// Both distribution are non-Zipfian

	if (!conf.types.at(edgeType.subject_type).scalable) {
		performOutDistributionShift(edgeType);
	} else {
		performInDistributionShift(edgeType);
	}
}



double incrementalDeterministicGraphGenerator::getMeanICsPerNode(distribution & distr, int zipfMax) {
	double meanEdgesPerNode;
	if (distr.type == DISTRIBUTION::NORMAL) {
		meanEdgesPerNode = distr.arg1;
	} else if (distr.type == DISTRIBUTION::UNIFORM) {
		meanEdgesPerNode = (distr.arg2 + distr.arg1) / 2.0;
	} else if (distr.type == DISTRIBUTION::ZIPFIAN) {
		double temp = 0.0;
		for (int i=1; i<=zipfMax; i++) {
			temp += (i-1)*pow((i), -1*distr.arg2);
		}
		meanEdgesPerNode = temp;
		cout << "Zipfian mean=" << meanEdgesPerNode << endl;
		cout << "####----###### SHOULD NEVER HAPPEN!!!! ####----######" << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
}


} /* namespace std */
