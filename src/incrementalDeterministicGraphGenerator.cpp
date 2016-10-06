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
	this->nodeGen = nodeGenerator(&randomGenerator, &graph, &conf);
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
		return &graph.nodes.first.at(i);
	} else {
		return &graph.nodes.second.at(i);
	}
}

vector<float> incrementalDeterministicGraphGenerator::getCdf(distribution distr, int nodeType, graphNode *sourceNode, bool findSourceNode) {
//	cout << "Searching for a node in the distribution: " + to_string(distr.arg1) << ", " << to_string(distr.arg2) << "\n";

//	cout << "Source it=" << sourceNode.iterationId << " id=" << sourceNode.id << endl;
	vector<graphNode> nodes;
	if (findSourceNode) {
		nodes = graph.nodes.first;
	} else {
		nodes = graph.nodes.second;
	}

	return cumDistrUtils.calculateUnifGausCumulPercentagesForNnodes(nodes, sourceNode, findSourceNode);
}


graphNode *incrementalDeterministicGraphGenerator::findSourceNode(config::edge & edgeType) {
	distribution distr = edgeType.outgoing_distrib;
	int nodeType = edgeType.subject_type;

	vector<float> cdf = getCdf(distr, nodeType, &graph.temp, true);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return &graph.temp;
	} else {
		return findNodeIdFromCumulProbs(cdf, true);
	}
}

graphNode *incrementalDeterministicGraphGenerator::findTargetNode(config::edge & edgeType, graphNode *sourceNode) {
	distribution distr = edgeType.incoming_distrib;
	int nodeType = edgeType.object_type;

	vector<float> cdf = getCdf(distr, nodeType, sourceNode, false);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return &graph.temp;
	} else {
		return findNodeIdFromCumulProbs(cdf, false);
	}
}


void incrementalDeterministicGraphGenerator::addEdge(graphNode *sourceNode, graphNode *targetNode, int predicate, ofstream*  outputFile) {
	sourceNode->decrementOpenInterfaceConnections(true);
	targetNode->decrementOpenInterfaceConnections(false);
//	e.source.decrementOpenInterfaceConnections(true);
//	e.target.decrementOpenInterfaceConnections(false);

	sourceNode->setConnection(targetNode->iterationId, 1);
	*outputFile << sourceNode->type << "-" << sourceNode->iterationId << " " << predicate << " " << targetNode->type << "-" << targetNode->iterationId << endl;
//	graph.edges.at(edgeType.edge_type_id).push_back(e);
}
// ####### Generate edges #######


// ####### Update interface-connections #######
int incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> *nodes, distribution distr, bool outDistr) {
//	cout << "New Zipfian case" << endl;
	int nmNodes;
	if (outDistr) {
		nmNodes = graph.nodes.first.size();
	} else {
		nmNodes = graph.nodes.second.size();
	}

	int openInterfaceConnections = 0;

	int zipfianStartValue;
	if (outDistr) {
		zipfianStartValue = zipfianStartValueOut;
	} else {
		zipfianStartValue = zipfianStartValueIn;
	}
	vector<float> zipfianCdf = cumDistrUtils.zipfCdf(distr, nmNodes, zipfianStartValue);
	int newInterfaceConnections = 0;
	int difference = 0;
	for (int i=0; i<nodes->size(); i++) {
		if (i >= nmNodes) {
			break;
		}
		graphNode *node = &nodes->at(i);

//		cout << "Postion of node" << i << ": " << node->getPosition(outDistr) << endl;
//		cout << "Old NmOfInterfaceConnections: " << node->getNumberOfInterfaceConnections(outDistr) << endl;
//		cout << "Old NmOfOpenInterfaceConnections: " << node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr) << endl;
//		cout << "node.getPosition(edgeTypeId, outDistr): " << node.getPosition(edgeTypeId, outDistr) << endl;
//		cout << "zipfianCdf.size: " << zipfianCdf.size() << endl;
		newInterfaceConnections = cumDistrUtils.findPositionInCdf(zipfianCdf, node->getPosition(outDistr));

//		cout << "newInterfaceConnections: " << newInterfaceConnections << endl;

		difference = newInterfaceConnections - node->getNumberOfInterfaceConnections(outDistr);
		node->incrementOpenInterfaceConnectionsByN(difference, outDistr);
		node->setNumberOfInterfaceConnections(newInterfaceConnections, outDistr);

//		cout << "difference: " << difference << endl;
//		cout << "New NmOfInterfaceConnections: " << node->getNumberOfInterfaceConnections(outDistr) << endl;
//		cout << "New NmOfOpenInterfaceConnections: " << node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr) << endl;
		openInterfaceConnections += node->getNumberOfOpenInterfaceConnections(outDistr);
	}
	return openInterfaceConnections;
}

pair<int,int> incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType) {
	pair<int,int> numberOfOpenInterfaceConnections;
	int nmOpenICsOutDistr = -1;
	int nmOpenIcsInDistr = -1;

	if(edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		nmOpenICsOutDistr = updateInterfaceConnectionsForZipfianDistributions(&graph.nodes.first, edgeType.outgoing_distrib, true);
	}
	if(edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		nmOpenIcsInDistr = updateInterfaceConnectionsForZipfianDistributions(&graph.nodes.second, edgeType.incoming_distrib, false);
	}
	numberOfOpenInterfaceConnections.first = nmOpenICsOutDistr;
	numberOfOpenInterfaceConnections.second = nmOpenIcsInDistr;
	return numberOfOpenInterfaceConnections;
}

void incrementalDeterministicGraphGenerator::updateICsForNonScalableType(vector<graphNode> nodes, int iterationNumber, double meanUpdateDistr, double meanNonUpdateDistr, distribution & distr, int edgeTypeId, bool updateSubjects) {
//	cout << "Updating ICs for non scalable type" << endl;
	if (nodes.size() == 0) {
		cout << "A fixed amount of nodes of one of the types is equal to 0" << endl;
		return;
	}

	if (distr.type == DISTRIBUTION::ZIPFIAN) {
		int* zipfianStartValue;
		if (updateSubjects) {
			zipfianStartValue = &zipfianStartValueOut;
		} else {
			zipfianStartValue = &zipfianStartValueIn;
		}
//		double newMean = (meanNonUpdateDistr * nmNodesNonUpdate) / nmNodesUpdate;
		double newMean = (meanNonUpdateDistr * iterationNumber) / nodes.size();
		int increment = round(newMean - meanUpdateDistr - *zipfianStartValue);
		if (increment > 0) {
			*zipfianStartValue += increment;
//			cout << "zipfianStartValue: " << *zipfianStartValue << endl;
//			cout << "Increment Zipfian distr by " << increment << endl;
			for (graphNode node: nodes) {
				node.incrementOpenInterfaceConnectionsByN(increment, updateSubjects);
				node.incrementInterfaceConnectionsByN(increment, updateSubjects);
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
				node.incrementOpenInterfaceConnectionsByN(increment, updateSubjects);
				node.incrementInterfaceConnectionsByN(increment, updateSubjects);

			}
		}
	}
}
// ####### Update interface-connections #######

double incrementalDeterministicGraphGenerator::getMeanEdgesPerNode(config::edge & edgeType, distribution distr, int zipfMax) {
	float meanEdgesPerNode;
	if (distr.type == DISTRIBUTION::NORMAL) {
		meanEdgesPerNode = distr.arg1;
	} else if (distr.type == DISTRIBUTION::UNIFORM) {
		meanEdgesPerNode = ((float)distr.arg2 + (float)distr.arg1) / 2.0;
	} else if (distr.type == DISTRIBUTION::ZIPFIAN) {
		float temp = 0;
		for (int i=1; i<=zipfMax; i++) {
			temp += i*pow((i), -1*distr.arg2);
		}
		meanEdgesPerNode = temp;
//		cout << "Zipfian mean=" << temp << endl;
	} else {
		meanEdgesPerNode = 1;
	}
	return meanEdgesPerNode;
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType, double meanICsPerNodeForOtherDistr, bool changeOutDistr, double subjectProbOrSize, double objectProbOrSize) {
	if (changeOutDistr) {
		if (edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL) {
			edgeType.outgoing_distrib.arg1 = (objectProbOrSize * meanICsPerNodeForOtherDistr) / subjectProbOrSize;
//			cout << "new normal mean: " << edgeType.outgoing_distrib.arg1 << endl;
		} else if (edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
			double diff = edgeType.outgoing_distrib.arg2 - edgeType.outgoing_distrib.arg1;
			double newMean = (objectProbOrSize * meanICsPerNodeForOtherDistr) / subjectProbOrSize;
			if (diff > 0.0) {
				edgeType.outgoing_distrib.arg1 = max(round(newMean - (diff/2)), 0.0);
				edgeType.outgoing_distrib.arg2 = round(newMean + (diff/2));
			} else {
				edgeType.outgoing_distrib.arg1 = round(newMean);
				edgeType.outgoing_distrib.arg2 = round(newMean);
			}
//			cout << "New uniform min: " << edgeType.outgoing_distrib.arg1 << endl;
//			cout << "New uniform max: " << edgeType.outgoing_distrib.arg2 << endl;
		}
	} else {
		if (edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL) {
			edgeType.incoming_distrib.arg1 = (subjectProbOrSize * meanICsPerNodeForOtherDistr) / objectProbOrSize;
//			cout << "new normal mean: " << edgeType.incoming_distrib.arg1 << endl;
		} else if (edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
			double diff = edgeType.incoming_distrib.arg2 - edgeType.incoming_distrib.arg1;
			double newMean = (subjectProbOrSize * meanICsPerNodeForOtherDistr) / objectProbOrSize;
			if (diff > 0.0) {
				edgeType.incoming_distrib.arg1 = max(round(newMean - (diff/2)), 0.0);
				edgeType.incoming_distrib.arg2 = round(newMean + (diff/2));
			} else {
				edgeType.incoming_distrib.arg1 = round(newMean);
				edgeType.incoming_distrib.arg2 = round(newMean);
			}
//			cout << "New uniform min: " << edgeType.incoming_distrib.arg1 << endl;
//			cout << "New uniform max: " << edgeType.incoming_distrib.arg2 << endl;
		}
	}
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType) {
//	cout << "Changing edge-type " << edgeType.edge_type_id << endl;

	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		if (conf.types.at(edgeType.subject_type).scalable) {
			// In-distr is not scalable, so change the in-distribution
			double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
			double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, conf.types.at(edgeType.object_type).size);
			if (edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
				zipfianStartValueIn = max(1.0, round(meanICsPerNodeForOutDistr - meanICsPerNodeForInDistr));
			} else {
				changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.object_type).size);
			}
		} else {
			// Out-distr is not scalable, so change the out-distribution
			double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
			double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, conf.types.at(edgeType.subject_type).size);
			if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
//				cout << "zipfianStartValueOut" << endl;
				zipfianStartValueOut = max(1.0, round(meanICsPerNodeForInDistr - meanICsPerNodeForOutDistr));
			} else {
				changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true, conf.types.at(edgeType.subject_type).size, conf.types.at(edgeType.subject_type).size);
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
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
		changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
	}

	// Out-distr is Zipfian, so change the params of the in-distr (only if )
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0);

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
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0);

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
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0);
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0);

		if (meanICsPerNodeForOutDistr * subjectProbOrSize >
			meanICsPerNodeForInDistr * objectProbOrSize) {
			// Change in-distr
			changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
			return;
		} else {
			// Change out-distr
			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true, subjectProbOrSize, objectProbOrSize);
			return;
		}
	}

	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
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

int incrementalDeterministicGraphGenerator::getNumberOfOpenICs(vector<graphNode> nodes, bool isOutDistr) {
	int openICs = 0;
	for(int i=0; i<nodes.size(); i++) {
		openICs += nodes.at(i).getNumberOfOpenInterfaceConnections(isOutDistr);
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
				double meanOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, conf.types.at(edgeType.subject_type).size);
				double meanInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
				updateICsForNonScalableType(graph.nodes.first, iterationNumber, meanOutDistr, meanInDistr, edgeType.outgoing_distrib, edgeType.edge_type_id, true);
			}
		} else {
			// object is not scalable so update the ICs of all the object nodes
			if (iterationNumber >= conf.types.at(edgeType.object_type).size) {
				double meanOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
				double meanInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, conf.types.at(edgeType.object_type).size);
				updateICsForNonScalableType(graph.nodes.second, iterationNumber, meanInDistr, meanOutDistr, edgeType.incoming_distrib, edgeType.edge_type_id, false);
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
		int openICsForInDistr = getNumberOfOpenICs(graph.nodes.second, false);
		numberOfEdgesPerIteration = min(zipfOpenInterfaceConnections.first, openICsForInDistr);
		c = round(getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0) * max(1.0, objectProbOrSize / subjectProbOrSize));
	} else if (zipfOpenInterfaceConnections.second != -1) {
		// Zipf in-distr case
		int openICsForOutDistr = getNumberOfOpenICs(graph.nodes.first, true);
		numberOfEdgesPerIteration = min(openICsForOutDistr, zipfOpenInterfaceConnections.second);
		c = round(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0) * max(1.0, subjectProbOrSize / objectProbOrSize));
	} else {
		// Non Zipfian case
		int openICsForInDistr = getNumberOfOpenICs(graph.nodes.second, false);
		int openICsForOutDistr = getNumberOfOpenICs(graph.nodes.first, true);
		numberOfEdgesPerIteration = min(openICsForOutDistr, openICsForInDistr);

		double maxSubjectObjectRatio = max(subjectProbOrSize /  objectProbOrSize, objectProbOrSize / subjectProbOrSize);
		c = round(min(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0), getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0)) * maxSubjectObjectRatio);
	}
	numberOfEdgesPerIteration -= c*sf;

//	cout << "numberOfEdgesPerIteration: " << numberOfEdgesPerIteration << endl;

	for (int i=0; i<numberOfEdgesPerIteration; i++) {
		graphNode *sourceNode;
		graphNode *targetNode;


//		cout << "Find source node from nodes 0:" << nmNodesMax-1 << endl;
		sourceNode = findSourceNode(edgeType);
//		cout << "Find target node from nodes 0:" << iterationNumber << endl;
		if (sourceNode->iterationId == -1) {
//			cout << "Edge is not added because of no available ICs in either the Subject nodes or the target nodes, so go to next iteration" << endl;
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
	graph = incrementalDeterministicGraph();

	randomGenerator.seed(seed);

	changeDistributionParams(edgeType);

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

void incrementalDeterministicGraphGenerator::generateIncDetGraph(ofstream*  outputFile, int* seeds, int edgeTypeIdLow, int edgeTypeIdHigh) {
//	cout << "Generate a incremental deterministic graph (given a seed)" << endl;


	for (int i=edgeTypeIdLow; i<edgeTypeIdHigh; i++) {
		processEdgeType(conf.schema.edges.at(i), outputFile, seeds[i]);
	}
}

} /* namespace std */
