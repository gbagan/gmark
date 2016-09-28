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





//int incrementalDeterministicGraphGenerator::getNumberOfEdgesPerIteration(config::edge & edgeType) {
//	double subjectProb;
//	double objectProb;
//	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
//		// One scalable and one not scalable
//		return 1;
//	} else if (!conf.types.at(edgeType.subject_type).scalable && !conf.types.at(edgeType.object_type).scalable) {
//		// both not scalable
//		subjectProb = conf.types.at(edgeType.subject_type).size;
//		objectProb = conf.types.at(edgeType.object_type).size;
//	} else {
//		// both scalable
//		subjectProb = conf.types.at(edgeType.subject_type).proportion;
//		objectProb = conf.types.at(edgeType.object_type).proportion;
//	}
//
//	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
//		// Both Zipf
//		return -1;
//	} else if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN || edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
//		// One Zipf
//		if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN) {
//			return max(getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0), 1.0);
//		} else {
//			return max(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0), 1.0);
//		}
//	} else {
//		// None Zipf
//		double numberOfSubjectNodesPerIteration = max( subjectProb / objectProb, 1.0);
//		double numberOfObjecteNodesPerIteration = max( objectProb / subjectProb, 1.0);
//
//		int meanNumberOfInterfaceConnectionsPerIterationForSource = floor(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0)) * numberOfSubjectNodesPerIteration;
//		int meanNumberOfInterfaceConnectionsPerIterationForTarget = floor(getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0)) * numberOfObjecteNodesPerIteration;
////		cout << "getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0):" << getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0) << endl;
////		cout << "numberOfSubjectNodesPerIteration:" << numberOfSubjectNodesPerIteration << endl;
////
////		cout << "meanNumberOfInterfaceConnectionsPerIterationForSource:" << meanNumberOfInterfaceConnectionsPerIterationForSource << endl;
////		cout << "meanNumberOfInterfaceConnectionsPerIterationForTarget:" << meanNumberOfInterfaceConnectionsPerIterationForTarget << endl;
//		int result = min(meanNumberOfInterfaceConnectionsPerIterationForSource, meanNumberOfInterfaceConnectionsPerIterationForTarget);
////		cout << "Number of edges per iteration: " << max(result,1) << endl;
//		return max(result, 1);
//	}
//}


// ####### Generate edges #######
graphNode incrementalDeterministicGraphGenerator::findNodeIdFromCumulProbs(vector<float> & cumulProbs, int nodeType) {
	uniform_real_distribution<double> distribution(0.0,1.0);
	double randomValue = distribution(randomGenerator);
	int i = cumDistrUtils.findPositionInCdf(cumulProbs, randomValue);
	return graph.nodes.at(nodeType).at(i);
}

vector<float> incrementalDeterministicGraphGenerator::getCdf(distribution distr, int nodeType, int edgeTypeNumber, int nmNodes, graphNode sourceNode, bool findSourceNode) {
//	cout << "Searching for a node in the distribution: " + to_string(distr.arg1) << ", " << to_string(distr.arg2) << "\n";

//	cout << "Source it=" << sourceNode.iterationId << " id=" << sourceNode.id << endl;
	return cumDistrUtils.calculateUnifGausCumulPercentagesForNnodes(graph.nodes.at(nodeType), edgeTypeNumber, nmNodes, sourceNode, findSourceNode);
}


graphNode incrementalDeterministicGraphGenerator::findSourceNode(config::edge & edgeType, int nmNodes) {
	distribution distr = edgeType.outgoing_distrib;
	int nodeType = edgeType.subject_type;
	int edgeTypeNumber = edgeType.edge_type_id;

	vector<float> cdf = getCdf(distr, nodeType, edgeTypeNumber, nmNodes, graphNode(), true);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return graphNode();
	} else {
		return findNodeIdFromCumulProbs(cdf, nodeType);
	}
}

graphNode incrementalDeterministicGraphGenerator::findTargetNode(config::edge & edgeType, int nmNodes, graphNode sourceNode) {
	distribution distr = edgeType.incoming_distrib;
	int nodeType = edgeType.object_type;
	int edgeTypeNumber = edgeType.edge_type_id;

	vector<float> cdf = getCdf(distr, nodeType, edgeTypeNumber, nmNodes, sourceNode, false);

	if(cdf.at(0) == -1) {
//		cout << "Cannot find a node\n";
		return graphNode();
	} else {
		return findNodeIdFromCumulProbs(cdf, nodeType);
	}
}


void incrementalDeterministicGraphGenerator::addEdge(graphEdge & e, config::edge & edgeType) {
//	if(edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL || edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
		e.source.decrementOpenInterfaceConnections(edgeType.edge_type_id, true);
//	}
//	if(edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL || edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
		e.target.decrementOpenInterfaceConnections(edgeType.edge_type_id, false);
//	}

//	cout << "before -> e.source.connections->at(e.target.iterationId): " << e.source.connections[e.target.iterationId] << endl;
//
	e.source.setConnection(e.target.iterationId, 1);
//	cout << "after -> e.source.connections->at(e.target.iterationId): " << e.source.connections[e.target.iterationId] << endl;
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
//		cout << "node.getPosition(edgeTypeId, outDistr): " << node.getPosition(edgeTypeId, outDistr) << endl;
//		cout << "zipfianCdf.size: " << zipfianCdf.size() << endl;
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
//	cout << "nmSubjectNodes: " << nmSubjectNodes << endl;
//	cout << "nmObjectNodes: " << nmObjectNodes << endl;

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

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType, double meanICsPerNodeForOtherDistr, bool changeOutDistr, double subjectProbOrSize, double objectProbOrSize) {
	if (changeOutDistr) {
		if (edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL) {
			edgeType.outgoing_distrib.arg1 = (objectProbOrSize * meanICsPerNodeForOtherDistr) / subjectProbOrSize;
			cout << "new normal mean: " << edgeType.outgoing_distrib.arg1 << endl;
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
			cout << "New uniform min: " << edgeType.outgoing_distrib.arg1 << endl;
			cout << "New uniform max: " << edgeType.outgoing_distrib.arg2 << endl;
		}
	} else {
		if (edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL) {
			edgeType.incoming_distrib.arg1 = (subjectProbOrSize * meanICsPerNodeForOtherDistr) / objectProbOrSize;
			cout << "new normal mean: " << edgeType.incoming_distrib.arg1 << endl;
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
			cout << "New uniform min: " << edgeType.incoming_distrib.arg1 << endl;
			cout << "New uniform max: " << edgeType.incoming_distrib.arg2 << endl;
		}
	}
}

void incrementalDeterministicGraphGenerator::changeDistributionParams(config::edge & edgeType) {
	cout << "Changing edge-type " << edgeType.edge_type_id << endl;

	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		cout << "No optimization due to one scalable and one non-scalable node-type" << endl;
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

	// Out-distr is Zipfian, so change the params of the in-distr (only if )
	if (edgeType.outgoing_distrib.type == DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type != DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 10000);
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0);

		if ((objectProbOrSize * meanICsPerNodeForInDistr) > (subjectProbOrSize * meanICsPerNodeForOutDistr)) {
			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
			changeDistributionParams(edgeType, meanICsPerNodeForOutDistr, false, subjectProbOrSize, objectProbOrSize);
		}
		return;
	}
	// In-distr is Zipfian, so change the params of the out-distr
	if (edgeType.outgoing_distrib.type != DISTRIBUTION::ZIPFIAN && edgeType.incoming_distrib.type == DISTRIBUTION::ZIPFIAN) {
		double meanICsPerNodeForInDistr = getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 10000);
		double meanICsPerNodeForOutDistr = getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0);

		if ((subjectProbOrSize * meanICsPerNodeForOutDistr) > (objectProbOrSize * meanICsPerNodeForInDistr)) {
			// Mean of ICsPerIteration for non-Zipfian distr is higher that the mean of the Zipfian distr
			changeDistributionParams(edgeType, meanICsPerNodeForInDistr, true, subjectProbOrSize, objectProbOrSize);
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
	// Both are Zipfian or undefined TODO
}

int incrementalDeterministicGraphGenerator::getNumberOfOpenICs(vector<graphNode> nodes, int numberOfNodes, int edgeTypeId, bool isOutDistr) {
	int openICs = 0;
	for(int i=0; i<nodes.size(); i++) {
		if (i >= numberOfNodes) {
			return openICs;
		}
		openICs += nodes.at(i).getNumberOfOpenInterfaceConnections(edgeTypeId, isOutDistr);
	}
	return openICs;
}

int incrementalDeterministicGraphGenerator::processIteration(int iterationNumber, config::edge & edgeType, int numberOfNodesOfMax) {
//	if (iterationNumber % 1000 == 0) {
//		cout << endl<< "---Process interationNumber " << to_string(iterationNumber) << " of edgeType " << to_string(edgeType.edge_type_id) << "---" << endl;
//	}

	int nmSubjectNodes = nodeGen.addOrUpdateSubjectNodes(edgeType, iterationNumber, numberOfNodesOfMax, conf.types.at(edgeType.object_type).size);
	int nmObjectNodes = nodeGen.addOrUpdateObjectNodes(edgeType, iterationNumber, numberOfNodesOfMax, conf.types.at(edgeType.subject_type).size);
	int nmNodesMax = max(nmSubjectNodes, nmObjectNodes);
	nmNodesMax = max(nmNodesMax, iterationNumber+1);

	pair<int, int> zipfOpenInterfaceConnections;
	int subjectNodes;
	int objectNodes;
	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		subjectNodes = iterationNumber+1;
		objectNodes = iterationNumber+1;
	} else {
		if (conf.types.at(edgeType.subject_type).size > conf.types.at(edgeType.object_type).size) {
			subjectNodes = nmNodesMax;
			objectNodes = iterationNumber+1;
		} else {
			subjectNodes = iterationNumber+1;
			objectNodes = nmNodesMax;
		}
	}
	zipfOpenInterfaceConnections = updateInterfaceConnectionsForZipfianDistributions(edgeType, subjectNodes, objectNodes);


	int numberOfEdgesPerIteration = 1;
	int c = 0;
	int sf = 2;
	if (conf.types.at(edgeType.subject_type).scalable ^ conf.types.at(edgeType.object_type).scalable) {
		// One scalable and one not scalable
		numberOfEdgesPerIteration = 1;
		c = 0;
	} else if (zipfOpenInterfaceConnections.first != -1 && zipfOpenInterfaceConnections.second != -1) {
		// Zipf in- and out-distr case
		numberOfEdgesPerIteration = min(zipfOpenInterfaceConnections.first, zipfOpenInterfaceConnections.second);
		c = 0;
	} else if (zipfOpenInterfaceConnections.first != -1){
		// Zipf out-distr case
		int openICsForInDistr = getNumberOfOpenICs(graph.nodes.at(edgeType.object_type), objectNodes, edgeType.edge_type_id, false);
		numberOfEdgesPerIteration = min(zipfOpenInterfaceConnections.first, openICsForInDistr);
		c = round(getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0));
	} else if (zipfOpenInterfaceConnections.second != -1) {
		// Zipf in-distr case
		int openICsForOutDistr = getNumberOfOpenICs(graph.nodes.at(edgeType.subject_type), subjectNodes, edgeType.edge_type_id, true);
		numberOfEdgesPerIteration = min(openICsForOutDistr, zipfOpenInterfaceConnections.second);
		c = round(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0));
	} else {
		// Non Zipfian case
		int openICsForInDistr = getNumberOfOpenICs(graph.nodes.at(edgeType.object_type), objectNodes, edgeType.edge_type_id, false);
		int openICsForOutDistr = getNumberOfOpenICs(graph.nodes.at(edgeType.subject_type), subjectNodes, edgeType.edge_type_id, true);
		numberOfEdgesPerIteration = min(openICsForOutDistr, openICsForInDistr);
		c = round(min(getMeanEdgesPerNode(edgeType, edgeType.outgoing_distrib, 0), getMeanEdgesPerNode(edgeType, edgeType.incoming_distrib, 0)));
	}
	numberOfEdgesPerIteration -= c*sf;

//	cout << "numberOfEdgesPerIteration: " << numberOfEdgesPerIteration << endl;

	for (int i=0; i<numberOfEdgesPerIteration; i++) {
		graphNode sourceNode;
		graphNode targetNode;

		if (conf.types.at(edgeType.subject_type).proportion > conf.types.at(edgeType.object_type).proportion) {
//			cout << "Find source node from nodes 0:" << nmNodesMax-1 << endl;
			sourceNode = findSourceNode(edgeType, max(nmNodesMax-1,iterationNumber));
//			cout << "Find target node from nodes 0:" << iterationNumber << endl;
			targetNode = findTargetNode(edgeType, iterationNumber, sourceNode);
		} else {
			sourceNode = findSourceNode(edgeType, iterationNumber);
			targetNode = findTargetNode(edgeType, max(nmNodesMax-1,iterationNumber), sourceNode);
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
	if ((conf.types.at(edgeType.subject_type).scalable && conf.types.at(edgeType.object_type).scalable)
			|| (!conf.types.at(edgeType.subject_type).scalable && !conf.types.at(edgeType.object_type).scalable)) {
		nmOfIterations = min(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	} else {
		nmOfIterations = max(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	}
//	cout << "Total number of iterations: " << nmOfIterations << endl;


//	int numberOfEdgesPerIteration = getNumberOfEdgesPerIteration(edgeType);
//	cout << "numberOfEdgesPerIteration: " << numberOfEdgesPerIteration << endl;

	int numberOfNodesOfMax = 0;
	for(int i=0; i<nmOfIterations; i++) {
//		cout << "Number of maxNodes: " << numberOfNodesOfMax << endl;
		numberOfNodesOfMax = processIteration(i, edgeType, numberOfNodesOfMax);
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
