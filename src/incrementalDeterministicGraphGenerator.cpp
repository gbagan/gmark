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
	this->nextNodeId = 0;
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

// ####### Generate nodes #######
void incrementalDeterministicGraphGenerator::addInterfaceConnectionsToNode(graphNode &n, distribution distr, int currentEdgeTypeNumber, bool findSourceNode) {
	if (distr.type == DISTRIBUTION::UNDEFINED) {
		// TODO
	}
	else if (distr.type == DISTRIBUTION::ZIPFIAN) {
		n.setPosition(currentEdgeTypeNumber, randomGenerator(), findSourceNode);
	} else {
		int numberOfConnections;
		if (distr.type == DISTRIBUTION::UNIFORM) {
			std::uniform_int_distribution<int> distribution(distr.arg1, distr.arg2);
			numberOfConnections = distribution(randomGenerator);
		} else { // distr.type == DISTRIBUTION::NORMAL
			std::normal_distribution<double> distribution(distr.arg1, distr.arg2);
			numberOfConnections = round(distribution(randomGenerator));
		}
		n.setNumberOfOpenInterfaceConnections(currentEdgeTypeNumber, numberOfConnections, findSourceNode);
	}
}

void incrementalDeterministicGraphGenerator::addNode(graphNode n) {
	graph.nodes.at(n.type).push_back(n);
	nextNodeId++;
}

void incrementalDeterministicGraphGenerator::findOrCreateNode(config::edge & edgeType, bool findSourceNode, int iterationNumber) {
	distribution distr;
	size_t type;
	if(findSourceNode) {
		distr = edgeType.outgoing_distrib;
		type = edgeType.subject_type;
	} else {
		distr = edgeType.incoming_distrib;
		type = edgeType.object_type;
	}

	graphNode* n;
	if(graph.nodes.at(type).size() > iterationNumber) {
		// Node already exists in the graph
		cout << "NodeType" << type << "n" << iterationNumber << " already exists in the graph\n";

		n = &graph.nodes.at(type).at(iterationNumber);
		addInterfaceConnectionsToNode(*n, distr, edgeType.edge_type_id, findSourceNode);
		if (iterationNumber <= conf.types.at(type).size) {
			n->is_virtual = false;
		}
	} else {
		// Node is not in the graph at this moment
		if(!conf.types.at(type).scalable && iterationNumber > conf.types.at(type).size-1) {
			// NodeType is not scalable and all nodes are already created and added to the graph
			// !configuration.types.at(type).scalable <- NodeType is not scalable
			// iterationNumber > configuration.types.at(type).size <- all nodes are already created and added to the graph
			return; // So: Do NOT add the node
		}

		bool isVirtual;
		if (iterationNumber > conf.types.at(type).size-1) {
			isVirtual = true;
		} else {
			isVirtual = false;
		}

		n = new graphNode(nextNodeId, iterationNumber, type, isVirtual, conf.schema.edges.size());
		addInterfaceConnectionsToNode(*n, distr, edgeType.edge_type_id, findSourceNode);

		cout << "Creating a node at iteration " << iterationNumber << " of type:" << type <<
				". Size of that type=" << conf.types.at(type).size << "\n";


		addNode(*n);
	}
}
// ####### Generate nodes #######



int incrementalDeterministicGraphGenerator::getNumberOfEdgesPerIteration() {
	// TODO: Remove magic number
	return 2;
}



// ####### Generate edges #######
graphNode incrementalDeterministicGraphGenerator::findNodeIdFromCumulProbs(vector<float> & cumulProbs, int nodeType) {
	int i = 0;
	uniform_real_distribution<double> distribution(0.0,1.0);
	double randomValue = distribution(randomGenerator);
	cout << "RandomValue(0.0, 1.0)=" << randomValue << "\n";
	for(float cumulProbValue: cumulProbs) {
		if(randomValue <= cumulProbValue) {
			break;
		}
		i++;
	}

	return graph.nodes.at(nodeType).at(i);
}

vector<float> incrementalDeterministicGraphGenerator::getCdf(distribution distr, int nodeType, int edgeTypeNumber, int iterationNumber, bool findSourceNode) {
	vector<float> cumulProbs;
	cout << "Searching for a node in the distribution: " + to_string(distr.arg1) << ", " << to_string(distr.arg2) << "\n";

	if(distr.type == DISTRIBUTION::ZIPFIAN) {
		cumulProbs = cumDistrUtils.calculateZipfCumulPercentagesForNnodes(graph.nodes.at(nodeType), edgeTypeNumber, distr.arg2, iterationNumber, findSourceNode);
	} else {
		cumulProbs = cumDistrUtils.calculateUnifGausCumulPercentagesForNnodes(graph.nodes.at(nodeType), edgeTypeNumber, iterationNumber, findSourceNode);
	}

	return cumulProbs;
}


graphNode incrementalDeterministicGraphGenerator::findSourceNode(config::edge & edgeType, int iterationNumber) {
	distribution distr = edgeType.outgoing_distrib;
	int nodeType = edgeType.subject_type;
	int edgeTypeNumber = edgeType.edge_type_id;

	vector<float> cdf = getCdf(distr, nodeType, edgeTypeNumber, iterationNumber, true);

	if(cdf.at(0) == -1) {
		cout << "Cannot find a node\n";
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
		cout << "Cannot find a node\n";
		return graphNode();
	} else {
		return findNodeIdFromCumulProbs(cdf, nodeType);
	}
}


void incrementalDeterministicGraphGenerator::addEdge(graphEdge e, config::edge & edgeType) {
	if(edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL || edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
		e.source.decrementOpenInterfaceConnections(edgeType.edge_type_id, true);
	}
	if(edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL || edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
		e.target.decrementOpenInterfaceConnections(edgeType.edge_type_id, false);
	}
	graph.edges.at(edgeType.edge_type_id).push_back(e);
}
// ####### Generate edges #######







void incrementalDeterministicGraphGenerator::processIteration(int iterationNumber, config::edge & edgeType) {
	cout << endl<< "---Process interationNumber " << to_string(iterationNumber) << " of edgeType " << to_string(edgeType.edge_type_id) << "---" << endl;

	findOrCreateNode(edgeType, true, iterationNumber);
	findOrCreateNode(edgeType, false, iterationNumber);

	if(iterationNumber == 0) {
		// This will increase the graph connectivity, but slightly decrease the distributions
		// TODO: analysis with and without this part
		return;
	}

	int n = getNumberOfEdgesPerIteration();
	for (int i=0; i<n; i++) {
		graphNode sourceNode = findSourceNode(edgeType, iterationNumber);
		graphNode targetNode = findTargetNode(edgeType, iterationNumber);
		if(sourceNode.is_virtual || targetNode.is_virtual) {
			cout << "Edge is not added because source or target is virtual" << endl;
		} else {
			graphEdge edge(sourceNode, edgeType.predicate, targetNode);
			addEdge(edge, edgeType);
			cout << "Edge added:" << edge.toString() << endl;
		}
	}
}

void incrementalDeterministicGraphGenerator::processEdgeType(config::edge & edgeType) {
	cout << endl << endl << "-----Processing edge-type " << to_string(edgeType.edge_type_id) << "-----" << endl;
	int newSeed = randomGeneratorForSeeding();
	randomGenerator.seed(newSeed);

	int nmOfIterations = max(conf.types.at(edgeType.object_type).size, conf.types.at(edgeType.subject_type).size);
	cout << "Total number of iterations: " << nmOfIterations << endl;
	for(int i=0; i<nmOfIterations; i++) {
		processIteration(i, edgeType);
	}
}

void incrementalDeterministicGraphGenerator::generateIncDetGraph() {
	cout << "Generate a incremental deterministic graph (given a seed)" << endl;

	// TODO: get the seed from the XML instead of this magic number
	randomGeneratorForSeeding.seed(222);
	initializeNodesAndEdges();

	for (config::edge & edgeType : conf.schema.edges) {
		processEdgeType(edgeType);
	}

	// Print nodes:
	cout << "\n\n\n###NODES###" << endl;
	int i = 0;
	for(vector<graphNode> nodeVector: graph.nodes) {
		cout << "Expected number of nodes: " << to_string(conf.types.at(i).size) << endl;
		i++;
		for(graphNode node: nodeVector) {
			if (!node.is_virtual) {
				cout << conf.types.at(node.type).alias  << to_string(node.iterationId) << endl;
			}
		}
	}

	// Print edges:
	cout << "\n###EDGES###" << endl;
	for(vector<graphEdge> edgeVector: graph.edges) {
		for(graphEdge edge: edgeVector) {
			cout << conf.types.at(edge.source.type).alias << to_string(edge.source.iterationId) << " (id=" << to_string(edge.source.id) << ")" <<
					" - " << conf.predicates.at(edge.predicate).alias << " - " <<
					conf.types.at(edge.target.type).alias << to_string(edge.target.iterationId) << " (id=" << to_string(edge.target.id) << ")" << endl;
		}
		cout << endl;
	}
}

} /* namespace std */
