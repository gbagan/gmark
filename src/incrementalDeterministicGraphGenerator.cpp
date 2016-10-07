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

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator(config::config configuration) {
	this->conf = configuration;
}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
	// TODO Auto-generated destructor stub
}

// ####### Generate edges #######
graphNode *incrementalDeterministicGraphGenerator::findNodeIdFromCumulProbs(vector<float> & cumulProbs, bool findSourceNode) {
	uniform_real_distribution<double> distribution(0.0,1.0);
	double randomValue = distribution(randomGenerator);
	int i = cumDistrUtils.findPositionInCdf(cumulProbs, randomValue);
	if (findSourceNode) {
		return &nodes.first.at(i);
	} else {
		return &nodes.second.at(i);
	}
}

graphNode *incrementalDeterministicGraphGenerator::findSourceNode(config::edge & edgeType) {
	vector<float> cdf = cumDistrUtils.calculateCDF(nodes.first, &tempNode);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return &tempNode;
	} else {
		return findNodeIdFromCumulProbs(cdf, true);
	}
}

graphNode *incrementalDeterministicGraphGenerator::findTargetNode(config::edge & edgeType, graphNode *sourceNode) {
	vector<float> cdf = cumDistrUtils.calculateCDF(nodes.second, sourceNode);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return &tempNode;
	} else {
		return findNodeIdFromCumulProbs(cdf, false);
	}
}


void incrementalDeterministicGraphGenerator::addEdge(graphNode *sourceNode, graphNode *targetNode, int predicate, ofstream*  outputFile) {
	sourceNode->decrementOpenInterfaceConnections();
	targetNode->decrementOpenInterfaceConnections();

	sourceNode->setConnection(targetNode->iterationId, 1);
	*outputFile << sourceNode->type << "-" << sourceNode->iterationId << " " << predicate << " " << targetNode->type << "-" << targetNode->iterationId << endl;
}
// ####### Generate edges #######


// ####### Update interface-connections #######
int incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodesVec, distribution distr, int zipfianStartValue) {
//	cout << "New Zipfian case" << endl;
	int nmNodes = nodesVec->size();

	int openInterfaceConnections = 0;

	vector<float> zipfianCdf = cumDistrUtils.zipfCdf(distr, nmNodes, zipfianStartValue);
	int newInterfaceConnections = 0;
	int difference = 0;
	for (int i=0; i<nodesVec->size(); i++) {
		graphNode *node = &nodesVec->at(i);

//		cout << "Postion of node" << i << ": " << node->getPosition(outDistr) << endl;
//		cout << "Old NmOfInterfaceConnections: " << node->getNumberOfInterfaceConnections(outDistr) << endl;
//		cout << "Old NmOfOpenInterfaceConnections: " << node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr) << endl;
//		cout << "node.getPosition(edgeTypeId, outDistr): " << node.getPosition(edgeTypeId, outDistr) << endl;
//		cout << "zipfianCdf.size: " << zipfianCdf.size() << endl;
		newInterfaceConnections = cumDistrUtils.findPositionInCdf(zipfianCdf, node->getPosition());

//		cout << "newInterfaceConnections: " << newInterfaceConnections << endl;

		difference = newInterfaceConnections - node->getNumberOfInterfaceConnections();
		node->incrementOpenInterfaceConnectionsByN(difference);
		node->setNumberOfInterfaceConnections(newInterfaceConnections);

//		cout << "difference: " << difference << endl;
//		cout << "New NmOfInterfaceConnections: " << node->getNumberOfInterfaceConnections(outDistr) << endl;
//		cout << "New NmOfOpenInterfaceConnections: " << node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr) << endl;
		openInterfaceConnections += node->getNumberOfOpenInterfaceConnections();
	}
	return openInterfaceConnections;
}

pair<int,int> incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType) {
	pair<int,int> numberOfOpenInterfaceConnections;
	int nmOpenICsOutDistr = -1;
	int nmOpenIcsInDistr = -1;

	if(edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		nmOpenICsOutDistr = updateInterfaceConnectionsForZipfianDistributions(&nodes.first, edgeType.outgoing_distrib, zipfianStartValueOut);
	}
	if(edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		nmOpenIcsInDistr = updateInterfaceConnectionsForZipfianDistributions(&nodes.second, edgeType.incoming_distrib, zipfianStartValueIn);
	}
	numberOfOpenInterfaceConnections.first = nmOpenICsOutDistr;
	numberOfOpenInterfaceConnections.second = nmOpenIcsInDistr;
	return numberOfOpenInterfaceConnections;
}

void incrementalDeterministicGraphGenerator::updateICsForNonScalableType(vector<graphNode> nodes, int iterationNumber, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distr, int* zipfianStartValue) {
//	cout << "Updating ICs for non scalable type" << endl;
	if (nodes.size() == 0) {
		cout << "A fixed amount of nodes of one of the types is equal to 0" << endl;
		return;
	}

	if (distr.type == DISTRIBUTION::ZIPFIAN) {
//		double newMean = (meanNonUpdateDistr * nmNodesNonUpdate) / nmNodesUpdate;
		double newMean = (meanNonUpdateDistr * iterationNumber) / nodes.size();
		int increment = round(newMean - meanUpdateDistr - *zipfianStartValue);
		if (increment > 0) {
			*zipfianStartValue += increment;
//			cout << "zipfianStartValue: " << *zipfianStartValue << endl;
//			cout << "Increment Zipfian distr by " << increment << endl;
			for (graphNode node: nodes) {
				node.incrementOpenInterfaceConnectionsByN(increment);
				node.incrementInterfaceConnectionsByN(increment);
			}
		}
	} else {
		int increment = floor((((iterationNumber + 1) * meanNonUpdateDistr) / nodes.size()) - meanUpdateDistr);
		if (increment > 0) {
			if (distr.type == DISTRIBUTION::NORMAL) {
//				cout << "Before distr.arg1: " << distr.arg1 << endl;
				distr.arg1 += increment;
//				cout << "After distr.arg1: " << distr.arg1 << endl;
			} else if (distr.type == DISTRIBUTION::UNIFORM) {
//				cout << "Before distr.arg1: " << distr.arg1 << endl;
//				cout << "Before distr.arg2: " << distr.arg2 << endl;
				double diff = distr.arg2 - distr.arg1;
				double newMean = ((distr.arg2 + distr.arg1) / 2) + (double) increment;
				if (diff > 0.0) {
					distr.arg1 = max(round(newMean - (diff/2)), 0.0);
					distr.arg2 = round(newMean + (diff/2));
				} else {
					distr.arg1 = newMean;
					distr.arg2 = newMean;
				}
//				cout << "After distr.arg1: " << distr.arg1 << endl;
//				cout << "After distr.arg2: " << distr.arg2 << endl;
			}
//			cout << "Update nodes with " << increment << endl;
			for (graphNode node: nodes) {
				node.incrementOpenInterfaceConnectionsByN(increment);
				node.incrementInterfaceConnectionsByN(increment);

			}
		}
	}
}
// ####### Update interface-connections #######

double incrementalDeterministicGraphGenerator::getMeanICsPerNode(distribution distr, int zipfMax, int zipfianStartValue) {
	float meanEdgesPerNode;
	if (distr.type == DISTRIBUTION::NORMAL) {
		meanEdgesPerNode = distr.arg1;
	} else if (distr.type == DISTRIBUTION::UNIFORM) {
		meanEdgesPerNode = ((float)distr.arg2 + (float)distr.arg1) / 2.0;
	} else if (distr.type == DISTRIBUTION::ZIPFIAN) {
//		if (distr.arg2 > 2) {
//			meanEdgesPerNode = (1.0/(distr.arg2-2.0)) + zipfianStartValue;
//		} else {
			float temp = 0;
			for (int i=1; i<=zipfMax; i++) {
				temp += i*pow((i), -1*distr.arg2);
			}
			meanEdgesPerNode = temp;// + zipfianStartValue;
//		}
//		cout << "Zipfian mean=" << temp << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(distribution & distr, double meanICsPerNodeForOtherDistr, bool changeOutDistr, double subjectProbOrSize, double objectProbOrSize) {
	if (distr.type == DISTRIBUTION::NORMAL) {
		distr.arg1 = (objectProbOrSize * meanICsPerNodeForOtherDistr) / subjectProbOrSize;
//			cout << "new normal mean: " << edgeType.outgoing_distrib.arg1 << endl;
	} else if (distr.type == DISTRIBUTION::UNIFORM) {
		double diff = distr.arg2 - distr.arg1;
		double newMean = (objectProbOrSize * meanICsPerNodeForOtherDistr) / subjectProbOrSize;
		if (diff > 0.0) {
			distr.arg1 = max(round(newMean - (diff/2)), 0.0);
			distr.arg2 = round(newMean + (diff/2));
		} else {
			distr.arg1 = round(newMean);
			distr.arg2 = round(newMean);
		}
//		cout << "New uniform min: " << edgeType.outgoing_distrib.arg1 << endl;
//		cout << "New uniform max: " << edgeType.outgoing_distrib.arg2 << endl;
	}
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType) {
//	cout << "Changing edge-type " << edgeType.edge_type_id << endl;

	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		if (conf.types.at(edgeType.subject_type).scalable) {
			// In-distr is not scalable, so change the in-distribution
			double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000, zipfianStartValueOut);
			double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types.at(edgeType.object_type).size, zipfianStartValueIn);
			if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
				zipfianStartValueIn = max(1.0, round(meanICsPerNodeForOutDistr - meanICsPerNodeForInDistr));
			} else {
				changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, false, conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.object_type).size);
			}
		} else {
			// Out-distr is not scalable, so change the out-distribution
			double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000, zipfianStartValueIn);
			double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types.at(edgeType.subject_type).size, zipfianStartValueOut);
			if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
//				cout << "zipfianStartValueOut" << endl;
				zipfianStartValueOut = max(1.0, round(meanICsPerNodeForInDistr - meanICsPerNodeForOutDistr));
			} else {
				changeDistributionParams(edgeType.outgoing_distrib, meanICsPerNodeForInDistr, true, conf.types.at(edgeType.subject_type).size, conf.types.at(edgeType.subject_type).size);
			}
		}


		if (edgeType.incoming_distrib.type == DISTRIBUTION::UNDEFINED) {
			edgeType.incoming_distrib = distribution(DISTRIBUTION::UNIFORM, 1, 1);
		}
		return;
	}

	double subjectProbOrSize;
	double objectProbOrSize;
	if (conf.types.at(edgeType.subject_type).scalable) {
		subjectProbOrSize = conf.types.at(edgeType.subject_type).proportion;
		objectProbOrSize = conf.types.at(edgeType.object_type).proportion;
	} else {
		subjectProbOrSize = conf.types.at(edgeType.subject_type).size;
		objectProbOrSize = conf.types.at(edgeType.object_type).size;
	}

	// In-distr is undefined, so change this in-distr
	if (edgeType.incoming_distrib.type == DISTRIBUTION::UNDEFINED) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000, zipfianStartValueOut);
		changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
	}

	// Out-distr is Zipfian, so change the params of the in-distr (only if )
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000, zipfianStartValueOut);
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 0, zipfianStartValueIn);

//		cout << "meanICsPerNodeForInDistr: " << meanICsPerNodeForInDistr << endl;
//		cout << "objectProbOrSize: " << objectProbOrSize << endl;
//		cout << "meanICsPerNodeForOutDistr: " << meanICsPerNodeForOutDistr << endl;
//		cout << "subjectProbOrSize: " << subjectProbOrSize << endl;
		if ((objectProbOrSize * meanICsPerNodeForInDistr) > (subjectProbOrSize * meanICsPerNodeForOutDistr)) {
			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
//			changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
			double newMean = (objectProbOrSize * meanICsPerNodeForInDistr) / subjectProbOrSize;
			zipfianStartValueOut = ceil(newMean - meanICsPerNodeForOutDistr);
//			cout << "Change the Zipfian start value to: " << zipfianStartValueOut << endl;
		}
		return;
	}
	// In-distr is Zipfian, so change the params of the out-distr
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000, zipfianStartValueIn);
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 0, zipfianStartValueOut);

		if ((subjectProbOrSize * meanICsPerNodeForOutDistr) > (objectProbOrSize * meanICsPerNodeForInDistr)) {
			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
//			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true, subjectProbOrSize, objectProbOrSize);
			double newMean = (subjectProbOrSize * meanICsPerNodeForOutDistr) / objectProbOrSize;
			zipfianStartValueIn = ceil(newMean - meanICsPerNodeForInDistr);
//			cout << "Change the Zipfian start value to: " << zipfianStartValueIn << endl;
		}
		return;
	}

	// Both are non-Zipfian
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 0, zipfianStartValueOut);
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 0, zipfianStartValueIn);

		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
			meanICsPerNodeForInDistr * objectProbOrSize) {
			// Change in-distr
			changeDistributionParams(edgeType.incoming_distrib, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
			return;
		} else {
			// Change out-distr
			changeDistributionParams(edgeType.outgoing_distrib, meanICsPerNodeForInDistr, true, subjectProbOrSize, objectProbOrSize);
			return;
		}
	}

	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000, zipfianStartValueOut);
		double meanICsPerNodeForInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000, zipfianStartValueIn);
		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
			meanICsPerNodeForInDistr * objectProbOrSize) {
			// Change in-distr
			double newMean = (subjectProbOrSize * meanICsPerNodeForOutDistr) / objectProbOrSize;
			zipfianStartValueIn = ceil(newMean - meanICsPerNodeForInDistr);
//			cout << "Change the ZipfianIn start value to: " << zipfianStartValueIn << endl;
			return;
		} else {
			// Change out-distr
			double newMean = (objectProbOrSize * meanICsPerNodeForInDistr) / subjectProbOrSize;
			zipfianStartValueOut = ceil(newMean - meanICsPerNodeForOutDistr);
//			cout << "Change the ZipfianOut start value to: " << zipfianStartValueOut << endl;
			return;
		}
	}
}

int incrementalDeterministicGraphGenerator::getNumberOfOpenICs(vector<graphNode> nodes) {
	int openICs = 0;
	for(int i=0; i<nodes.size(); i++) {
		openICs += nodes.at(i).getNumberOfOpenInterfaceConnections();
	}
	return openICs;
}

void incrementalDeterministicGraphGenerator::processIteration(int iterationNumber, config::edge & edgeType, ofstream*  outputFile) {
//	if (iterationNumber % 1000 == 0) {
//		cout << endl<< "---Process interationNumber " << to_string(iterationNumber) << " of edgeType " << to_string(edgeType.edge_type_id) << "---" << endl;
//	}
	nodeGen.addSubjectNodes(edgeType);
	nodeGen.addObjectNodes(edgeType);

//	int subjectNodes = graph.nodes.first.size();
//	int objectNodes = graph.nodes.second.size();

	double subjectProbOrSize = -1.0;
	double objectProbOrSize = -1.0;
	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		if (!conf.types.at(edgeType.subject_type).scalable) {
			// Subject is not scalable so update the ICs of all the subject nodes
			if (iterationNumber >= conf.types.at(edgeType.subject_type).size) {
				double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, conf.types.at(edgeType.subject_type).size, zipfianStartValueOut);
				double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, 10000, zipfianStartValueIn);
				updateICsForNonScalableType(nodes.first, iterationNumber, meanOutDistr, meanInDistr, edgeType.outgoing_distrib, &zipfianStartValueOut);
			}
		} else {
			// object is not scalable so update the ICs of all the object nodes
			if (iterationNumber >= conf.types.at(edgeType.object_type).size) {
				double meanOutDistr = getMeanICsPerNode(edgeType.outgoing_distrib, 10000, zipfianStartValueOut);
				double meanInDistr = getMeanICsPerNode(edgeType.incoming_distrib, conf.types.at(edgeType.object_type).size, zipfianStartValueIn);
				updateICsForNonScalableType(nodes.second, iterationNumber, meanInDistr, meanOutDistr, edgeType.incoming_distrib, &zipfianStartValueIn);
			}
		}
	} else {
		if (conf.types.at(edgeType.subject_type).scalable) {
			subjectProbOrSize = conf.types.at(edgeType.subject_type).proportion;
			objectProbOrSize = conf.types.at(edgeType.subject_type).proportion;
		} else {
			subjectProbOrSize = conf.types.at(edgeType.subject_type).size;
			objectProbOrSize = conf.types.at(edgeType.subject_type).size;
		}
	}

	pair<int, int> zipfOpenInterfaceConnections = updateInterfaceConnectionsForZipfianDistributions(edgeType);

	int numberOfEdgesPerIteration = 1;
	int c = 0;
	int sf = 2;
	if (zipfOpenInterfaceConnections.first != -1 && zipfOpenInterfaceConnections.second != -1) {
		// Zipf in- and out-distr case
		numberOfEdgesPerIteration = min(zipfOpenInterfaceConnections.first, zipfOpenInterfaceConnections.second);
		c = 0;
	} else if (zipfOpenInterfaceConnections.first != -1){
		// Zipf out-distr case
		int openICsForInDistr = getNumberOfOpenICs(nodes.second);
		numberOfEdgesPerIteration = min(zipfOpenInterfaceConnections.first, openICsForInDistr);
		c = round(getMeanICsPerNode(edgeType.incoming_distrib, 0, zipfianStartValueIn) * max(1.0, objectProbOrSize / subjectProbOrSize));
	} else if (zipfOpenInterfaceConnections.second != -1) {
		// Zipf in-distr case
		int openICsForOutDistr = getNumberOfOpenICs(nodes.first);
		numberOfEdgesPerIteration = min(openICsForOutDistr, zipfOpenInterfaceConnections.second);
		c = round(getMeanICsPerNode(edgeType.outgoing_distrib, 0, zipfianStartValueOut) * max(1.0, subjectProbOrSize / objectProbOrSize));
	} else {
		// Non Zipfian case
		int openICsForInDistr = getNumberOfOpenICs(nodes.second);
		int openICsForOutDistr = getNumberOfOpenICs(nodes.first);
		numberOfEdgesPerIteration = min(openICsForOutDistr, openICsForInDistr);

		double maxSubjectObjectRatio = max(subjectProbOrSize /  objectProbOrSize, objectProbOrSize / subjectProbOrSize);
		c = round(min(getMeanICsPerNode(edgeType.outgoing_distrib, 0, zipfianStartValueOut), getMeanICsPerNode(edgeType.incoming_distrib, 0, zipfianStartValueIn)) * maxSubjectObjectRatio);
	}
	numberOfEdgesPerIteration -= c*sf;

//	cout << "numberOfEdgesPerIteration: " << numberOfEdgesPerIteration << endl;

	for (int i=0; i<numberOfEdgesPerIteration; i++) {
		graphNode *sourceNode;
		graphNode *targetNode;


		sourceNode = findSourceNode(edgeType);
		if (sourceNode->iterationId == -1) {
//			cout << "Edge is not added because of no available ICs in the Subject nodes, so go to next iteration" << endl;
			break;
		} else {
			targetNode = findTargetNode(edgeType, sourceNode);
		}

		if(targetNode->iterationId == -1) {
//			cout << "Target iterationId = -1" << endl;
			break;
		} else {
			addEdge(sourceNode, targetNode, edgeType.predicate, outputFile);
//			cout << "Edge added: [" << sourceNode->iterationId << " - " << targetNode->iterationId << "]" << endl;
		}
	}
}

void incrementalDeterministicGraphGenerator::processEdgeType(config::edge & edgeType, ofstream*  outputFile, int seed) {
//	cout << endl << endl << "-----Processing edge-type " << to_string(edgeType.edge_type_id) << "-----" << endl;
	zipfianStartValueIn = 1;
	zipfianStartValueOut = 1;

	randomGenerator.seed(seed);

	changeDistributionParams(edgeType);

	nodeGen = nodeGenerator(edgeType, &randomGenerator, &nodes, &conf);

	int nmOfIterations;
	if ((conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.object_type).scalable)
			|| (!conf.types.at(edgeType.subject_type).scalable && !conf.types.at(edgeType.object_type).scalable)) {
		nmOfIterations = min(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	} else {
		nmOfIterations = max(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	}
//	cout << "Total number of iterations: " << nmOfIterations << endl;


//	int numberOfEdgesPerIteration = getNumberOfEdgesPerIteration(edgeType);
//	cout << "numberOfEdgesPerIteration: " << numberOfEdgesPerIteration << endl;

	for(int i=0; i<nmOfIterations; i++) {
//		cout << "Number of maxNodes: " << numberOfNodesOfMax << endl;
		processIteration(i, edgeType, outputFile);
	}
}

//void incrementalDeterministicGraphGenerator::generateIncDetGraph(ofstream*  outputFile, int* seeds, int edgeTypeIdLow, int edgeTypeIdHigh) {
////	cout << "Generate a incremental deterministic graph (given a seed)" << endl;
//
//
//	for (int i=edgeTypeIdLow; i<edgeTypeIdHigh; i++) {
//		processEdgeType(conf.schema.edges.at(i), outputFile, seeds[i]);
//	}
//}

} /* namespace std */
