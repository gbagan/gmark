/*
 * incrementalDeterministicGraphGenerator.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <vector>
#include "incrementalDeterministicGraphGenerator.h"
#include "graphNode.h"

namespace std {

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator() {
	randomGenerator.seed(chrono::system_clock::now().time_since_epoch().count());
}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
	// TODO Auto-generated destructor stub
}

// ####### Generate edges #######
void incrementalDeterministicGraphGenerator::addEdge(graphNode &sourceNode, graphNode &targetNode, int predicate) {
//	cout << "SourceNode" << sourceNode.iterationId << endl;
//	cout << "Before: subject openICs = " << sourceNode.getNumberOfOpenInterfaceConnections() << endl;
	sourceNode.decrementOpenInterfaceConnections();
//	cout << "After: subject openICs = " << sourceNode.getNumberOfOpenInterfaceConnections() << endl;
	targetNode.decrementOpenInterfaceConnections();

//	if (outputBufferLines < 10 && predicate == 1) {
//		cout << to_string(sourceNode.type) + "-" + to_string(sourceNode.iterationId) + " " + to_string(predicate) + " " + to_string(targetNode.type) + "-" + to_string(targetNode.iterationId) + "\n";
//	}

	edge2 newEdge;
	newEdge.subjectId = sourceNode.id;
	newEdge.predicate = to_string(predicate);
	newEdge.objectId = targetNode.id;

	edges.push_back(newEdge);

//	string outputBuffer = to_string(sourceNode.type) + "-" + to_string(sourceNode.iterationId) + " " + to_string(predicate) + " " + to_string(targetNode.type) + "-" + to_string(targetNode.iterationId);

//	outputBufferLines++;
//	if (outputBufferLines % 50 == 0 || lastEdge) {
		// Use endl to flush
//		*outputFile << outputBuffer << endl;
//	} else {
//		outputBuffer += outputBuffer + "\n";
//	}
//	*outputFile << sourceNode.type << "-" << sourceNode.iterationId << " " << predicate << " " << targetNode.type << "-" << targetNode.iterationId << endl;
}
// ####### Generate edges #######


// ####### Update interface-connections #######
void incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodesVec, distribution distr, bool outDistr) {
//	cout << "New Zipfian case" << endl;
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
		node.incrementOpenInterfaceConnectionsByN(difference);
		node.setNumberOfInterfaceConnections(newInterfaceConnections);
//		cout << "after openICs: " << node.getNumberOfOpenInterfaceConnections() << endl;
	}
}


//void incrementalDeterministicGraphGenerator::updateICsForNonScalableType(vector<graphNode> & nodes, int nmNodesOther, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distr) {
////	cout << "Updating ICs for non scalable type" << endl;
//	if (nodes.size() == 0) {
//		cout << "A fixed amount of nodes of one of the types is equal to 0" << endl;
//		return;
//	}
//
////	cout << "iterationNumber: " << iterationNumber << endl;
//	int increment = floor((((nmNodesOther + 1) * meanNonUpdateDistr) / nodes.size()) - meanUpdateDistr);
//	if (increment > 0) {
//		if (distr.type == DISTRIBUTION::NORMAL || distr.type == DISTRIBUTION::ZIPFIAN) {
////			cout << "Before distr.arg1: " << distr.arg1 << endl;
//			distr.arg1 += increment;
////			cout << "After distr.arg1: " << distr.arg1 << endl;
//		} else if (distr.type == DISTRIBUTION::UNIFORM) {
////			cout << "Before distr.arg1: " << distr.arg1 << endl;
////			cout << "Before distr.arg2: " << distr.arg2 << endl;
//			double diff = distr.arg2 - distr.arg1;
//			double newMean = ((distr.arg2 + distr.arg1) / 2) + (double) increment;
//			if (diff > 0.0) {
//				distr.arg1 = max(round(newMean - (diff/2)), 0.0);
//				distr.arg2 = round(newMean + (diff/2));
//			} else {
//				distr.arg1 = newMean;
//				distr.arg2 = newMean;
//			}
////				cout << "After distr.arg1: " << distr.arg1 << endl;
////				cout << "After distr.arg2: " << distr.arg2 << endl;
//		}
//
////		cout << "Update nodes with " << increment << endl;
//		for (graphNode & node: nodes) {
//			node.incrementOpenInterfaceConnectionsByN(increment);
//			node.incrementInterfaceConnectionsByN(increment);
//		}
//	}
//}

//void incrementalDeterministicGraphGenerator::updateICsForNonScalableType(config::edge & edgeType) {
//	if (!conf.types.at(edgeType.subject_type).scalable) {
//		// Subject is not scalable so update the ICs of all the subject nodes
////		if (conf.types.at(edgeType.object_type).size >= conf.types.at(edgeType.subject_type).size) {
//			double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, nodes.first.size());
//			double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, nodes.second.size());
//			updateICsForNonScalableType(nodes.first, conf.types.at(edgeType.object_type).size, meanOutDistr, meanInDistr, edgeType.outgoing_distrib);
////		}
//	} else {
//		// object is not scalable so update the ICs of all the object nodes
////		if (conf.types.at(edgeType.subject_type).size >= conf.types.at(edgeType.object_type).size) {
//			double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, nodes.first.size());
//			double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, nodes.second.size());
//			updateICsForNonScalableType(nodes.second, conf.types.at(edgeType.subject_type).size, meanInDistr, meanOutDistr, edgeType.incoming_distrib);
////		}
//	}
//}
// ####### Update interface-connections #######

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
//		cout << "Zipfian mean=" << meanEdgesPerNode << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
}

//int incrementalDeterministicGraphGenerator::shiftDistribution(distribution & distr, double newMean) {
//	int updateInt = 0;
//	if (distr.type == DISTRIBUTION::NORMAL) {
//		double update = newMean - distr.arg1;
//		updateInt = round(update);
//		if (updateInt > 0) {
//			distr.arg1 = newMean;
//		}
//
//	} else if (distr.type == DISTRIBUTION::UNIFORM) {
//		updateInt = round(newMean - distr.arg1);
//		if (updateInt > 0) {
//			distr.arg1 += updateInt;
//			distr.arg2 += updateInt;
//	//		cout << "New uniform min: " << distr.arg1 << endl;
//	//		cout << "New uniform max: " << distr.arg2 << endl;
//		}
//	} else if (distr.type == DISTRIBUTION::ZIPFIAN) {
//
//	}
//	return updateInt;
//}

void incrementalDeterministicGraphGenerator::updateDistributionShifts(config::edge & edgeType) {
	if ((edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN ^ edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN)) {
		// Do this after constructing the Zipfian ICs-vector
		return;
	}

	cout << "conf.types[edgeType.subject_type].size: " << conf.types[edgeType.subject_type].size << endl;
	cout << "conf.types[edgeType.object_type].size: " << conf.types[edgeType.object_type].size << endl;

	double evOutDistribution = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types[edgeType.subject_type].size) + outDistrShift;
	cout << "evOutDistribution: " << evOutDistribution << endl;

	double evInDistribution = getMeanICsPerNode(edgeType.incoming_distrib, conf.types[edgeType.object_type].size) + inDistrShift;
	cout << "evInDistribution: " << evInDistribution << endl;

	if ((conf.types[edgeType.subject_type].size * evOutDistribution) > (conf.types[edgeType.object_type].size * evInDistribution)) {
		double newMean = ((double) conf.types[edgeType.subject_type].size * evOutDistribution) / (double) conf.types[edgeType.object_type].size;
		int updateInt = floor(newMean - evInDistribution);

		cout << "newMean in: " << newMean << endl;
		if (updateInt > 0) {
			inDistrShift += updateInt;
			cout << "Update in distribution: " << updateInt << endl;
			cout << "inDistrShift: " << inDistrShift << endl;
			for (graphNode & n: nodes.second) {
				n.incrementInterfaceConnectionsByN(updateInt);
				n.incrementOpenInterfaceConnectionsByN(updateInt);
			}
		}
	} else {
		double newMean = ((double) conf.types[edgeType.object_type].size * evInDistribution) / (double) conf.types[edgeType.subject_type].size;
		int updateInt = floor(newMean - evOutDistribution);

		cout << "newMean out: " << newMean << endl;
		if (updateInt > 0) {
			outDistrShift += updateInt;
			cout << "Update out distribution: " << updateInt << endl;
			cout << "outDistrShift: " << outDistrShift << endl;
			for (graphNode & n: nodes.first) {
				n.incrementInterfaceConnectionsByN(updateInt);
				n.incrementOpenInterfaceConnectionsByN(updateInt);
			}
		}
	}

//	if (conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.subject_type).proportion == 0) {
//		conf.types.at(edgeType.subject_type).proportion = 1.0 / (double)conf.types.size();
//		conf.types.at(edgeType.subject_type).size = (1.0 / (double)conf.types.size()) * conf.nb_nodes;
//	}
//	if (conf.types.at(edgeType.object_type).scalable && conf.types.at(edgeType.object_type).proportion == 0) {
//		conf.types.at(edgeType.object_type).scalable = true;
//		conf.types.at(edgeType.object_type).proportion = 1.0 / (double)conf.types.size();
//		conf.types.at(edgeType.object_type).size = (1.0 / (double)conf.types.size()) * conf.nb_nodes;
//	}
//
//	// One scalable node-type
//	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
//		if (conf.types.at(edgeType.subject_type).scalable) {
//			// In-distr is not scalable, so change the in-distribution
//			double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000);
//			double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, conf.types.at(edgeType.object_type).size);
//			if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
//				edgeType.incoming_distrib.arg1 = max(1.0, round(meanICsPerNodeForOutDistr - meanICsPerNodeForInDistr));
//			} else {
//				changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.object_type).size);
//			}
//		} else {
//			// Out-distr is not scalable, so change the out-distribution
//			double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000);
//			double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types.at(edgeType.subject_type).size);
//			if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
//				edgeType.outgoing_distrib.arg1 = max(1.0, round(meanICsPerNodeForInDistr - meanICsPerNodeForOutDistr));
//			} else {
//				changeDistributionParams(edgeType.outgoing_distrib, meanICsPerNodeForInDistr, conf.types.at(edgeType.subject_type).size, conf.types.at(edgeType.subject_type).size);
//			}
//		}
//
//
//		if (edgeType.incoming_distrib.type == DISTRIBUTION::UNDEFINED) {
//			edgeType.incoming_distrib = distribution(DISTRIBUTION::UNIFORM, 1, 1);
//		}
//		return;
//	}
//
//	// Both node-types, of the current edge-type, are scalable or both are not
//	double subjectProbOrSize;
//	double objectProbOrSize;
//	if (conf.types.at(edgeType.subject_type).scalable) {
//		subjectProbOrSize = conf.types.at(edgeType.subject_type).proportion;
//		objectProbOrSize = conf.types.at(edgeType.object_type).proportion;
//	} else {
//		subjectProbOrSize = conf.types.at(edgeType.subject_type).size;
//		objectProbOrSize = conf.types.at(edgeType.object_type).size;
//	}
//
//	// Out-distr is Zipfian, so change the params of the in-distr (only if )
//	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
//		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000);
//		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 0);
//
//		if ((objectProbOrSize * meanICsPerNodeForInDistr) > (subjectProbOrSize * meanICsPerNodeForOutDistr)) {
//			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
////			changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
//			double newMean = (objectProbOrSize * meanICsPerNodeForInDistr) / subjectProbOrSize;
//			edgeType.outgoing_distrib.arg1 = ceil(newMean - meanICsPerNodeForOutDistr);
////			cout << "Change the Zipfian start value to: " << zipfianStartValueOut << endl;
//		}
//		return;
//	}
//	// In-distr is Zipfian, so change the params of the out-distr
//	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
//		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000);
//		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 0);
//
//		if ((subjectProbOrSize * meanICsPerNodeForOutDistr) > (objectProbOrSize * meanICsPerNodeForInDistr)) {
//			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
////			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true, subjectProbOrSize, objectProbOrSize);
//			double newMean = (subjectProbOrSize * meanICsPerNodeForOutDistr) / objectProbOrSize;
//			edgeType.incoming_distrib.arg1 = ceil(newMean - meanICsPerNodeForInDistr);
////			cout << "Change the Zipfian start value to: " << zipfianStartValueIn << endl;
//		}
//		return;
//	}
//
//	// Both are non-Zipfian
//	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
//		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 0);
//		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 0);
//
//		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
//			meanICsPerNodeForInDistr * objectProbOrSize) {
//			// Change in-distr
//			changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, subjectProbOrSize, objectProbOrSize);
//			return;
//		} else {
//			// Change out-distr
//			changeDistributionParams(edgeType.outgoing_distrib, meanICsPerNodeForInDistr, objectProbOrSize, subjectProbOrSize);
//			return;
//		}
//	}
//
//	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
//		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000);
//		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000);
//		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
//			meanICsPerNodeForInDistr * objectProbOrSize) {
//			// Change in-distr
//			double newMean = (subjectProbOrSize * meanICsPerNodeForOutDistr) / objectProbOrSize;
//			edgeType.incoming_distrib.arg1 = ceil(newMean - meanICsPerNodeForInDistr);
////			cout << "Change the ZipfianIn start value to: " << zipfianStartValueIn << endl;
//			return;
//		} else {
//			// Change out-distr
//			double newMean = (objectProbOrSize * meanICsPerNodeForInDistr) / subjectProbOrSize;
//			edgeType.outgoing_distrib.arg1 = ceil(newMean - meanICsPerNodeForOutDistr);
////			cout << "Change the ZipfianOut start value to: " << zipfianStartValueOut << endl;
//			return;
//		}
//	}
}


//int incrementalDeterministicGraphGenerator::getDistributionRandomnessTradeoff(config::edge edgeType, int iterationNumber) {
//	int scale = 5;
//	int c = 1;
//
//	if ((conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.object_type).scalable)
//			|| (!conf.types.at(edgeType.subject_type).scalable && !conf.types.at(edgeType.object_type).scalable)) {
//		int subjects;
//		if (conf.types.at(edgeType.subject_type).scalable) {
//			subjects = round(max(1.0, conf.types.at(edgeType.subject_type).proportion / conf.types.at(edgeType.object_type).proportion)) * iterationNumber;
//		} else {
//			subjects = conf.types.at(edgeType.subject_type).size;
//		}
//
//		int objects;
//		if (conf.types.at(edgeType.object_type).scalable) {
//			objects = round(max(1.0, conf.types.at(edgeType.object_type).proportion / conf.types.at(edgeType.subject_type).proportion)) * iterationNumber;
//		} else {
//			objects = conf.types.at(edgeType.object_type).size;
//		}
//
//		c = round(max(getMeanICsPerNode(edgeType.outgoing_distrib, subjects), getMeanICsPerNode(edgeType.incoming_distrib, objects)));
//	}
//
//	return max(scale, c*scale);
//}


vector<int> constructNodesVector(vector<graphNode> & nodes) {
	vector<int> nodesVector;
	for (graphNode & n: nodes) {
//		cout << "node" << n.iterationId << " has " << n.getNumberOfOpenInterfaceConnections() << " openICs" << endl;
		for (int i=0; i<n.getNumberOfOpenInterfaceConnections(); i++) {
			nodesVector.push_back(n.iterationId);
		}
	}
	return nodesVector;
}

void incrementalDeterministicGraphGenerator::performDistributionShift(config::edge & edgeType) {
	if (graphNumber > 0) {
		int sf = edgeType.scale_factor;
		if (conf.types[edgeType.subject_type].size > conf.types[edgeType.object_type].size) {
			// Increment all ICs and openICs with 1
			outDistrShift += sf;
			for (graphNode & node: nodes.first) {
				cout << "Update out-nodes in dbsh: " << sf << endl;
				node.incrementOpenInterfaceConnectionsByN(sf);
				node.incrementInterfaceConnectionsByN(sf);
			}

			// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
			double num = ((double) sf * (double)conf.types[edgeType.subject_type].size) / (double)conf.types[edgeType.object_type].size;
			int numIncrement = floor(num);
			double randomValue = uniformDistr(randomGenerator);
			if (randomValue < num - (double)floor(num)) {
				numIncrement++;
			}
			inDistrShift += numIncrement;
			for (graphNode & node: nodes.second) {
				cout << "Update in-nodes in dbsh: " << sf << endl;
				node.incrementOpenInterfaceConnectionsByN(numIncrement);
				node.incrementInterfaceConnectionsByN(numIncrement);
			}
		} else {
			// Increment all ICs and openICs with 1
			inDistrShift += sf;
			for (graphNode & node: nodes.second) {
				cout << "Update out-nodes in dbsh: " << sf << endl;
				node.incrementOpenInterfaceConnectionsByN(sf);
				node.incrementInterfaceConnectionsByN(sf);
			}

			// Increment all ICs and openICs with floor(num) or floor(num)+1 with probability num-floor(num)
			double num = ((double) sf * (double)conf.types[edgeType.object_type].size) / (double)conf.types[edgeType.subject_type].size;
			int numIncrement = floor(num);
			double randomValue = uniformDistr(randomGenerator);
			if (randomValue < num - (double)floor(num)) {
				numIncrement++;
			}
			outDistrShift += numIncrement;
			for (graphNode & node: nodes.first) {
				cout << "Update in-nodes in dbsh: " << sf << endl;
				node.incrementOpenInterfaceConnectionsByN(numIncrement);
				node.incrementInterfaceConnectionsByN(numIncrement);
			}
		}
	}
}


void incrementalDeterministicGraphGenerator::incrementGraph(config::edge & edgeType, int graphNumber) {
	// Perform the shifting of the distribution as indecated by the user in the schema
//	if (!(conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable)) {
//		performDistributionShift(edgeType);
//	}

	// Add subject and object nodes to the graph
	// Specify the current shift to get the wright amound of ICs
	nodeGen.addSubjectNodes(edgeType, outDistrShift);
	nodeGen.addObjectNodes(edgeType, inDistrShift);



	// Update the ICs for the Zipfian distribution to satisfy the property that influecer nodes will get more ICs when the graph grows
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(&nodes.first, edgeType.outgoing_distrib, true);
	}
	if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		updateInterfaceConnectionsForZipfianDistributions(&nodes.second, edgeType.incoming_distrib, false);
	}

	// TODO
//	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
//		updateICsForNonScalableType(edgeType);
//	}



	// Create vectors
	vector<int> subjectNodeIdVector;
	vector<int> objectNodeIdVector;

	// Out distr is Zipfian and in distr is not
	if (graphNumber == 0 && edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		// Construct the Zipfian vector
		subjectNodeIdVector = constructNodesVector(nodes.first);
		int totalOpenICsSubjetcs = subjectNodeIdVector.size();

		double mean = getMeanICsPerNode(edgeType.incoming_distrib, conf.types[edgeType.object_type].size);
		double newMean = (double) totalOpenICsSubjetcs / (double) conf.types[edgeType.object_type].size;

		// Shift the non-Zipfian distribution
		int shift = round(newMean - mean);
		if (shift > 0) {
			inDistrShift += shift;
			cout << "Final shift in-degree: " << inDistrShift << endl;
			for (graphNode & n: nodes.second) {
				n.incrementInterfaceConnectionsByN(shift);
				n.incrementOpenInterfaceConnectionsByN(shift);
			}
		}

		// Construct the non-Zipfian distr
		objectNodeIdVector = constructNodesVector(nodes.second);
	}
	else if (graphNumber == 0 && edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		// Construct the Zipfian vector
		objectNodeIdVector = constructNodesVector(nodes.second);
		int totalOpenICsObjetcs = objectNodeIdVector.size();

		double mean = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types[edgeType.subject_type].size);
		double newMean = (double) totalOpenICsObjetcs / (double) conf.types[edgeType.subject_type].size;

		// Shift the non-Zipfian distribution
		int shift = round(newMean - mean);
		if (shift > 0) {
			outDistrShift += shift;
			cout << "Final shift out-degree: " << outDistrShift << endl;
			for (graphNode & n: nodes.first) {
				n.incrementInterfaceConnectionsByN(shift);
				n.incrementOpenInterfaceConnectionsByN(shift);
			}
		}

		// Construct the non-Zipfian distr
		subjectNodeIdVector = constructNodesVector(nodes.first);
	} else {
		subjectNodeIdVector = constructNodesVector(nodes.first);
		objectNodeIdVector = constructNodesVector(nodes.second);
	}


	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN || edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		// Perform a shift in one of the distributions, when there is a large difference in the lengths of the vectors
		if (subjectNodeIdVector.size() < objectNodeIdVector.size()) {
			int diff = objectNodeIdVector.size() - subjectNodeIdVector.size();
			int incr = diff / conf.types[edgeType.subject_type].size;
			if (incr > 0) {
				outDistrShift += incr;
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
			int incr = diff / conf.types[edgeType.object_type].size;
			if (incr > 0) {
				inDistrShift += incr;
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


	// Shuffle vectors
	shuffle(objectNodeIdVector.begin(), objectNodeIdVector.end(), randomGenerator);
	shuffle(subjectNodeIdVector.begin(), subjectNodeIdVector.end(), randomGenerator);


	// Create edges
	for (int i=0; i<min(subjectNodeIdVector.size(), objectNodeIdVector.size()); i++) {
//		double randomValue = uniformDistr(randomGenerator);
//		if (randomValue > 0.03 || (!conf.types[edgeType.subject_type].scalable || !conf.types[edgeType.object_type].scalable)) {
//			cout << "Add edge: " << subjectNodeIdVector[i]->iterationId << " - " << objectNodeIdVector[i]->iterationId << endl;
			addEdge(nodes.first[subjectNodeIdVector[i]], nodes.second[objectNodeIdVector[i]], edgeType.predicate);
//		}
	}

}

void incrementalDeterministicGraphGenerator::printRank(vector<graphNode> nodes, int edgeTypeId, int nbNodes) {
	int maxDegree = 0;
	for (graphNode node: nodes) {
		int degree = node.numberOfInterfaceConnections - node.numberOfOpenInterfaceConnections;
		if (degree > maxDegree) {
			maxDegree = degree;
		}
	}

	cout << "Maxdegree=" << maxDegree << endl;
	ofstream rankFile, degreeFile;
	rankFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + ".txt", ios::trunc);
	degreeFile.open("rankFileET" + to_string(edgeTypeId) + "n" + to_string(nbNodes) + "degree.txt", ios::trunc);
	for (graphNode node: nodes) {
		double rank = ((double)node.numberOfInterfaceConnections - (double)node.numberOfOpenInterfaceConnections) / (double) maxDegree;
		rankFile << to_string(rank) << endl;
		degreeFile << to_string(node.numberOfInterfaceConnections - node.numberOfOpenInterfaceConnections) << "-" << to_string(maxDegree) << endl;
	}
	rankFile.close();
}

//void incrementalDeterministicGraphGenerator::updateShifts(config::edge & edgeType, config::config & previousConf) {
//	// Calc subject- and object-nodes-change
//	int subjectNodesNew = conf.types[edgeType.subject_type].size;
//	int objectNodesNew = conf.types[edgeType.object_type].size;
//
//	int subjectNodesOld = previousConf.types[edgeType.subject_type].size;
//	int objectNodesOld = previousConf.types[edgeType.object_type].size;
//
//	double subjectNodesChange = (double) subjectNodesNew - subjectNodesOld;
//	double objectNodesChange = (double) objectNodesNew - objectNodesOld;
//
//	// Calc mean out- and in-distr-change
//	double meanOutDistrOld = getMeanICsPerNode(edgeType.outgoing_distrib, subjectNodesOld);
//	double meanInDistrOld =	getMeanICsPerNode(edgeType.incoming_distrib, objectNodesOld);
//
//	double meanOutDistrNew = getMeanICsPerNode(edgeType.outgoing_distrib, subjectNodesNew);
//	double meanInDistrNew = getMeanICsPerNode(edgeType.incoming_distrib, objectNodesNew);
//
//	double meanOutDistrChange = meanOutDistrNew - meanOutDistrOld;
//	double meanInDistrChange = meanInDistrNew - meanInDistrOld;
//
//	// Calc the resulting changes
//	double subjectChange = subjectNodesChange * meanOutDistrChange;
//	double objectChange = objectNodesChange * meanInDistrChange;
//
//	if (subjectChange > objectChange) {
//		double diff = subjectChange - objectChange;
//		int incr = round(diff / conf.types[edgeType.object_type].size);
//		if (incr > 0) {
//			inDistrShift += incr;
//			for (graphNode & n: nodes.second) {
//				n.incrementInterfaceConnectionsByN(incr);
//				n.incrementOpenInterfaceConnectionsByN(incr);
//			}
//		}
//	} else {
//		double diff =  objectChange - subjectChange;
//		int incr = round(diff / conf.types[edgeType.subject_type].size);
//		if (incr > 0) {
//			outDistrShift += incr;
//			for (graphNode & n: nodes.first) {
//				n.incrementInterfaceConnectionsByN(incr);
//				n.incrementOpenInterfaceConnectionsByN(incr);
//			}
//		}
//	}
//}

int incrementalDeterministicGraphGenerator::processEdgeTypeSingleGraph(config::config configuration, config::config previousConf, config::edge & edgeType, ofstream & outputFile, int graphNumber_) {
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

	this->conf = configuration;
	this->graphNumber = graphNumber_;
	cout << "\n\n-----------GraphNumber: " << graphNumber << "--------------" << endl;
//	cout << "Number of nodes: " << conf.nb_nodes << endl;

	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && outDistrShift == 0) {
		outDistrShift = 1;
	}
	if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN && inDistrShift == 0) {
		inDistrShift = 1;
	}

	if (graphNumber == 0) {
		updateDistributionShifts(edgeType);
	}

//	if (graphNumber > 0) {
//		updateShifts(edgeType, previousConf);
//	}
	nodeGen = nodeGenerator(edgeType, nodes.first.size(), nodes.second.size(), &randomGenerator, &nodes, &conf);


	cout << "OutShif: " << outDistrShift << endl;
	cout << "InShif: " << inDistrShift << endl;
	incrementGraph(edgeType, graphNumber);

//	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
//	auto durationWitoutMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();

	// Materialize the edge
//	string outputBuffer = "";
//	cout << "Number of edges: " << edges.size() << endl;
	for (int i=0; i<edges.size(); i++) {
		edge2 e = edges[i];
		string edgeString = e.subjectId + " " + e.predicate + " " + e.objectId;

		if (i==edges.size()-1) {
			// Use endl to flush
//			outputBuffer += edgeString;
			outputFile << edgeString << endl;
//			outputBuffer = "";
		} else {
//			outputBuffer += edgeString + "\n";
			outputFile << edgeString << "\n";
		}
	}
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	auto durationWithMaterialize = chrono::duration_cast<chrono::milliseconds>( end - start ).count();


//	printRank(nodes.first, edgeType.edge_type_id, conf.nb_nodes);
//	printRank(nodes.second, edgeType.edge_type_id, conf.nb_nodes);


	return durationWithMaterialize;
}



} /* namespace std */
