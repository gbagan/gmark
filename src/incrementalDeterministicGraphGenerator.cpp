/*
 * incrementalDeterministicGraphGenerator.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <vector>
#include "incrementalDeterministicGraphGenerator.h"
#include "graphNode.h"
#include "graphEdge.h"

namespace std {

incrementalDeterministicGraphGenerator::incrementalDeterministicGraphGenerator(config::config configuration) {
	this->conf = configuration;
	this->nodeGen = nodeGenerator(&randomGenerator, &graph, &conf);
}

incrementalDeterministicGraphGenerator::~incrementalDeterministicGraphGenerator() {
	// TODO Auto-generated destructor stub
}

void incrementalDeterministicGraphGenerator::initializeNodesAndEdges() {
	vector<vector<graphNode>> nodes(conf.types.size());
	vector<vector<graphEdge>> edges(conf.predicates.size());

	graph.nodes = nodes;
	graph.edges = edges;
}





//int incrementalDeterministicGraphGenerator::getNumberOfEdgesPerIteration(config::edge & edgeType, int iterationNumber) {
//	// Both not Zipfian or undefined
//	if((edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM || edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL)
//			&& (edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM || edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL)) {
//		int openConnectionsOfSubject = graph.nodes.at(edgeType.subject_type).at(iterationNumber).getNumberOfOpenInterfaceConnections(edgeType.edge_type_id, true);
//		int openConnectionsOfObject = graph.nodes.at(edgeType.object_type).at(iterationNumber).getNumberOfOpenInterfaceConnections(edgeType.edge_type_id, false);
//		return min(openConnectionsOfSubject, openConnectionsOfObject);
//	}
//
//	// Out-distribution is Zipfian or undefined
//	if(edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM || edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL) {
//		return graph.nodes.at(edgeType.object_type).at(iterationNumber).getNumberOfOpenInterfaceConnections(edgeType.edge_type_id, false);
//	}
//
//	// In-distribution is Zipfian or undefined
//	if(edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM || edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL) {
//		return graph.nodes.at(edgeType.subject_type).at(iterationNumber).getNumberOfOpenInterfaceConnections(edgeType.edge_type_id, true);
//	}
//
//	// In - and out-distribution are Zipfian or undefined
//	return 2;
//}

int incrementalDeterministicGraphGenerator::getNumberOfEdgesPerIteration(config::edge & edgeType) {
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		return -1;
	} else if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN || edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
			return max(getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0), 1.0);
		} else {
			return max(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0), 1.0);
		}
	} else {
		double subjectProb = conf.types.at(edgeType.subject_type).proportion;
		double objectProb = conf.types.at(edgeType.object_type).proportion;

	//	cout << "subjectProb " << subjectProb << endl;
	//	cout << "objectProb " << objectProb << endl;
		int numberOfSubjectNodesPerIteration = max( round(subjectProb / objectProb), 1.0);
		int numberOfObjecteNodesPerIteration = max( round(objectProb / subjectProb), 1.0);

		int meanNumberOfInterfaceConnectionsPerIterationForSource = floor(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0)) * numberOfSubjectNodesPerIteration;
		int meanNumberOfInterfaceConnectionsPerIterationForTarget = floor(getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0)) * numberOfObjecteNodesPerIteration;

		int result = min(meanNumberOfInterfaceConnectionsPerIterationForSource, meanNumberOfInterfaceConnectionsPerIterationForTarget);
	//	cout << "Number of edges per iteration: " << max(result,1) << endl;
		return max(result, 1);
	}
}


// ####### Generate edges #######
graphNode incrementalDeterministicGraphGenerator::findNodeIdFromCumulProbs(vector<float> & cumulProbs, int nodeType) {
	uniform_real_distribution<double> distribution(0.0,1.0);
	double randomValue = distribution(randomGenerator);
	int i = cumDistrUtils.findPositionInCdf(cumulProbs, randomValue);
	return graph.nodes.at(nodeType).at(i);
}

vector<float> incrementalDeterministicGraphGenerator::getCdf(distribution distr, int nodeType, int edgeTypeNumber, int nmNodes, bool findSourceNode) {
//	cout << "Searching for a node in the distribution: " + to_string(distr.arg1) << ", " << to_string(distr.arg2) << "\n";

	return cumDistrUtils.calculateUnifGausCumulPercentagesForNnodes(graph.nodes.at(nodeType), edgeTypeNumber, nmNodes, findSourceNode);
}


graphNode incrementalDeterministicGraphGenerator::findSourceNode(config::edge & edgeType, int nmNodes) {
	distribution distr = edgeType.outgoing_distrib;
	int nodeType = edgeType.subject_type;
	int edgeTypeNumber = edgeType.edge_type_id;

	vector<float> cdf = getCdf(distr, nodeType, edgeTypeNumber, nmNodes, true);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return graphNode();
	} else {
		return findNodeIdFromCumulProbs(cdf, nodeType);
	}
}

graphNode incrementalDeterministicGraphGenerator::findTargetNode(config::edge & edgeType, int iterationNumber) {
	distribution distr = edgeType.incoming_distrib;
	int nodeType = edgeType.object_type;
	int edgeTypeNumber = edgeType.edge_type_id;

	vector<float> cdf = getCdf(distr, nodeType, edgeTypeNumber, iterationNumber, false);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return graphNode();
	} else {
		return findNodeIdFromCumulProbs(cdf, nodeType);
	}
}


void incrementalDeterministicGraphGenerator::addEdge(graphEdge e, config::edge & edgeType) {
//	if(edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL || edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
		e.source.decrementOpenInterfaceConnections(edgeType.edge_type_id, true);
//	}
//	if(edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL || edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
		e.target.decrementOpenInterfaceConnections(edgeType.edge_type_id, false);
//	}
	graph.edges.at(edgeType.edge_type_id).push_back(e);
}
// ####### Generate edges #######


// ####### Update interface-connections of Zipfian distributions #######
int incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(vector<graphNode> nodes, int iterationNumber, int edgeTypeId, distribution distr, bool outDistr) {
//	cout << "New Zipfian case" << endl;
	int openInterfaceConnections = 0;

	vector<float> zipfianCdf = cumDistrUtils.zipfCdf(distr, iterationNumber);
	graphNode node;
	int newInterfaceConnections = 0;
	int difference = 0;
	for (int i=0; i<nodes.size(); i++) {
		if (i > (iterationNumber+1)) {
			break;
		}
		node = nodes.at(i);

//		cout << "Old NmOfInterfaceConnections: " << node.getNumberOfInterfaceConnections(edgeTypeId, outDistr) << endl;
//		cout << "Old NmOfOpenInterfaceConnections: " << node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr) << endl;

		newInterfaceConnections = cumDistrUtils.findPositionInCdf(zipfianCdf, node.getPosition(edgeTypeId, outDistr));

//		cout << "newInterfaceConnections: " << newInterfaceConnections << endl;

		difference = newInterfaceConnections - node.getNumberOfInterfaceConnections(edgeTypeId, outDistr);
		node.incrementOpenInterfaceConnectionsByN(edgeTypeId, difference, outDistr);
		node.setNumberOfInterfaceConnections(edgeTypeId, newInterfaceConnections, outDistr);


//		cout << "New NmOfInterfaceConnections: " << node.getNumberOfInterfaceConnections(edgeTypeId, outDistr) << endl;
//		cout << "New NmOfOpenInterfaceConnections: " << node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr) << endl;
		openInterfaceConnections += node.getNumberOfOpenInterfaceConnections(edgeTypeId, outDistr);
	}
	return openInterfaceConnections;
}

pair<int,int> incrementalDeterministicGraphGenerator::updateInterfaceConnectionsForZipfianDistributions(config::edge & edgeType, int nmSubjectNodes, int nmObjectNodes) {
	pair<int,int> numberOfOpenInterfaceConnections;
	numberOfOpenInterfaceConnections.first = -1;
	numberOfOpenInterfaceConnections.second = -1;

	if(edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
		numberOfOpenInterfaceConnections.first = updateInterfaceConnectionsForZipfianDistributions(graph.nodes.at(edgeType.subject_type), nmSubjectNodes, edgeType.edge_type_id, edgeType.outgoing_distrib, true);
	}
	if(edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		numberOfOpenInterfaceConnections.second = updateInterfaceConnectionsForZipfianDistributions(graph.nodes.at(edgeType.object_type), nmObjectNodes, edgeType.edge_type_id, edgeType.incoming_distrib, false);
	}
	return numberOfOpenInterfaceConnections;
}
// ####### Update interface-connections of Zipfian distributions #######

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

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType, double meanICsPerNodeForOtherDistr, bool changeOutDistr) {
	if (changeOutDistr) {
		if (edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL) {
			edgeType.outgoing_distrib.arg1 = round((conf.types.at(edgeType.object_type).proportion * meanICsPerNodeForOtherDistr) / conf.types.at(edgeType.subject_type).proportion);
			cout << "new normal mean: " << edgeType.outgoing_distrib.arg1 << endl;
		} else if (edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
			double diff = edgeType.outgoing_distrib.arg2 - edgeType.outgoing_distrib.arg1;
			double newMean = (conf.types.at(edgeType.object_type).proportion * meanICsPerNodeForOtherDistr) / conf.types.at(edgeType.subject_type).proportion;
			if (diff > 0.0) {
				edgeType.outgoing_distrib.arg1 = max(round(newMean - (diff/2)), 0.0);
				edgeType.outgoing_distrib.arg2 = round(newMean + (diff/2));
			} else {
				edgeType.outgoing_distrib.arg1 = round(newMean);
				edgeType.outgoing_distrib.arg2 = round(newMean);
			}
			cout << "New uniform min: " << edgeType.outgoing_distrib.arg1 << endl;
			cout << "New uniform max: " << edgeType.outgoing_distrib.arg2 << endl;
		}
	} else {
		if (edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL) {
			edgeType.incoming_distrib.arg1 = round((conf.types.at(edgeType.subject_type).proportion * meanICsPerNodeForOtherDistr) / conf.types.at(edgeType.object_type).proportion);
			cout << "new normal mean: " << edgeType.incoming_distrib.arg1 << endl;
		} else if (edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
			double diff = edgeType.incoming_distrib.arg2 - edgeType.incoming_distrib.arg1;
			double newMean = (conf.types.at(edgeType.subject_type).proportion * meanICsPerNodeForOtherDistr) / conf.types.at(edgeType.object_type).proportion;
			if (diff > 0.0) {
				edgeType.incoming_distrib.arg1 = max(round(newMean - (diff/2)), 0.0);
				edgeType.incoming_distrib.arg2 = round(newMean + (diff/2));
			} else {
				edgeType.incoming_distrib.arg1 = round(newMean);
				edgeType.incoming_distrib.arg2 = round(newMean);
			}
			cout << "New uniform min: " << edgeType.incoming_distrib.arg1 << endl;
			cout << "New uniform max: " << edgeType.incoming_distrib.arg2 << endl;
		}
	}
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType) {
	cout << "Changing edge-type " << edgeType.edge_type_id << endl;

	if (!conf.types.at(edgeType.subject_type).scalable || !conf.types.at(edgeType.object_type).scalable) {
		cout << "No optimization due to non-scalable nodeType" << endl;
		return;
	}
	// Out-distr is Zipfian, so change the params of the in-distr
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
		changeDistributionParams(edgeType, meanICsPerNodeForInDistr, false);
		return;
	}
	// In-distr is Zipfian, so change the params of the out-distr
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
		changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true);
		return;
	}
	// Both are non-Zipfian
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);

		if (meanICsPerNodeForOutDistr * conf.types.at(edgeType.subject_type).proportion >
			meanICsPerNodeForInDistr * conf.types.at(edgeType.object_type).proportion) {
			// Change in-distr
			double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, false);
			return;
		} else {
			// Change out-distr
			double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true);
			return;
		}
	}
	// Both are Zipfian or undefined TODO


	// And calc the meanICs per node for the other distr
	// Call above function :D





//	if(edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
//		// Only in-distr in Zipfian
//		double meanOutDistr = (conf.types.at(edgeType.object_type).proportion * )
//	}


//	if (edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL || edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM
//			|| edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL || edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
//		// TODO: find aprroximation instead of 10000 for Zipfian mean value
//		double meanEdgesPerSourceNode = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
//		double meanEdgesPerTargetNode = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
//
//
//		if (edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL || edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
//			// Alter params of in-distr
//			changeDistributionParams(edgeType.object_type, edgeType.incoming_distrib, meanNumberOfSubjectInterFaceConnections);
//		} else {
//			// Alter params of out-distr
//			changeDistributionParams(edgeType.subject_type, edgeType.outgoing_distrib, meanNumberOfObjectInterFaceConnections);
//		}
//	}
}


int incrementalDeterministicGraphGenerator::processIteration(int iterationNumber, config::edge & edgeType, int numberOfNodesOfMax, int numberOfEdgesPerIteration) {
//	if (iterationNumber % 1000 == 0) {
//		cout << endl<< "---Process interationNumber " << to_string(iterationNumber) << " of edgeType " << to_string(edgeType.edge_type_id) << "---" << endl;
//	}

	int nmSubjectNodes = nodeGen.addOrUpdateSubjectNodes(edgeType, iterationNumber, numberOfNodesOfMax);
	int nmObjectNodes = nodeGen.addOrUpdateObjectNodes(edgeType, iterationNumber, numberOfNodesOfMax);
	int nmNodesMax = max(nmSubjectNodes, nmObjectNodes);


	pair<int, int> zipfOpenInterfaceConnections;
	if (conf.types.at(edgeType.subject_type).proportion > conf.types.at(edgeType.object_type).proportion) {
		zipfOpenInterfaceConnections = updateInterfaceConnectionsForZipfianDistributions(edgeType, nmNodesMax, iterationNumber);
	} else {
		zipfOpenInterfaceConnections = updateInterfaceConnectionsForZipfianDistributions(edgeType, iterationNumber, nmNodesMax);
	}

	if (zipfOpenInterfaceConnections.first != -1 && zipfOpenInterfaceConnections.second != -1) {
		numberOfEdgesPerIteration = min(zipfOpenInterfaceConnections.first, zipfOpenInterfaceConnections.second);
	}


//	cout << "numberOfEdgesPerIteration: " << numberOfEdgesPerIteration << endl;

	for (int i=0; i<numberOfEdgesPerIteration; i++) {
		graphNode sourceNode;
		graphNode targetNode;

		if (conf.types.at(edgeType.subject_type).proportion > conf.types.at(edgeType.object_type).proportion) {
//			cout << "Find source node from nodes 0:" << nmNodesMax-1 << endl;
			sourceNode = findSourceNode(edgeType, max(nmNodesMax-1,0));
//			cout << "Find target node from nodes 0:" << iterationNumber << endl;
			targetNode = findTargetNode(edgeType, iterationNumber);
		} else {
			sourceNode = findSourceNode(edgeType, iterationNumber);
			targetNode = findTargetNode(edgeType, max(nmNodesMax-1,0));
		}

		if(sourceNode.id == -1 || targetNode.id == -1) {
//			cout << "Edge is not added because of no available ICs in either the Subject nodes or the target nodes, so go to next iteration" << endl;
			//return nmNodesMax;
			break;
		} else {
			graphEdge edge(sourceNode, edgeType.predicate, targetNode);
			addEdge(edge, edgeType);
//			cout << "Edge added:" << edge.toString() << endl;
		}
	}
	return nmNodesMax;
}

void incrementalDeterministicGraphGenerator::processEdgeType(config::edge & edgeType) {
//	cout << endl << endl << "-----Processing edge-type " << to_string(edgeType.edge_type_id) << "-----" << endl;
	int newSeed = randomGeneratorForSeeding();
	randomGenerator.seed(newSeed);

	changeDistributionParams(edgeType);

	int nmOfIterations;
	if (conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.object_type).scalable ) {
		nmOfIterations = min(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	} else {
		nmOfIterations = max(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	}
//	cout << "Total number of iterations: " << nmOfIterations << endl;


	int numberOfEdgesPerIteration = getNumberOfEdgesPerIteration(edgeType);

	int numberOfNodesOfMax = 0;
	for(int i=0; i<nmOfIterations; i++) {
//		cout << "Number of maxNodes: " << numberOfNodesOfMax << endl;
		numberOfNodesOfMax = processIteration(i, edgeType, numberOfNodesOfMax, numberOfEdgesPerIteration);
	}
}

void incrementalDeterministicGraphGenerator::generateIncDetGraph() {
//	cout << "Generate a incremental deterministic graph (given a seed)" << endl;

	// TODO: get the seed from the XML instead of this magic number
	randomGeneratorForSeeding.seed(222);
	initializeNodesAndEdges();

	for (config::edge & edgeType : conf.schema.edges) {
		processEdgeType(edgeType);
	}

//	// Print nodes:
//	cout << "\n\n\n###NODES###" << endl;
//	int i = 0;
//	for(vector<graphNode> nodeVector: graph.nodes) {
//		cout << "Expected number of nodes: " << to_string(conf.types.at(i).size) << endl;
//		for(graphNode node: nodeVector) {
////			if (!node.is_virtual) {
//				cout << conf.types.at(node.type).alias  << to_string(node.iterationId) << " ICs[0].first=" << node.getNumberOfInterfaceConnections(0, true) << " pos[0].first=" << node.getPosition(0, true) << " ICs[0].second=" << node.getNumberOfInterfaceConnections(0, false) << " pos[0].second=" << node.getPosition(0, false)<< endl;
////			}
//		}
//		i++;
//	}
//
//	// Print edges:
//	cout << "\n###EDGES###" << endl;
//	for(vector<graphEdge> edgeVector: graph.edges) {
//		for(graphEdge edge: edgeVector) {
//			cout << conf.types.at(edge.source.type).alias << to_string(edge.source.iterationId) << " (id=" << to_string(edge.source.id) << ")" <<
//					" - " << conf.predicates.at(edge.predicate).alias << " - " <<
//					conf.types.at(edge.target.type).alias << to_string(edge.target.iterationId) << " (id=" << to_string(edge.target.id) << ")" << endl;
//		}
//		cout << endl;
//	}
}

} /* namespace std */
