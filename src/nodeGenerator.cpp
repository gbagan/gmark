/*
 * nodeGenerator.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: wilcovanleeuwen
 */

#include "nodeGenerator.h"

namespace std {

nodeGenerator::nodeGenerator() {
	this->randomGenerator = 0;
	this->graph = 0;
	this->conf = 0;
	this->nextNodeId = 0;
}

nodeGenerator::nodeGenerator(default_random_engine* randomGenerator_, incrementalDeterministicGraph* graph_, config::config* conf_) {
	this->randomGenerator = randomGenerator_;
	this->graph = graph_;
	this->conf = conf_;
	this->nextNodeId = 0;
}
nodeGenerator::~nodeGenerator() {
	// TODO
}


// ####### Generate nodes #######


void nodeGenerator::addInterfaceConnectionsToNode(graphNode &n, distribution distr, int currentEdgeTypeNumber, bool findSourceNode) {
	int numberOfConnections;
	if (distr.type == DISTRIBUTION::UNIFORM) {
//		cout << "UNIFORM with " << distr.arg1 << " " << distr.arg2 << endl;
		std::uniform_int_distribution<int> distribution(distr.arg1, distr.arg2);
		numberOfConnections = distribution(*randomGenerator);
	} else if (distr.type == DISTRIBUTION::NORMAL) {
//		cout << "NORMAL with " << distr.arg1 << " " << distr.arg2 << endl;
		std::normal_distribution<double> distribution(distr.arg1, distr.arg2);
		numberOfConnections = round(distribution(*randomGenerator));
	} else if (distr.type == DISTRIBUTION::ZIPFIAN) {
		uniform_real_distribution<double> distribution(0.0,1.0);
		double randomValue = distribution(*randomGenerator);
		n.setPosition(currentEdgeTypeNumber, randomValue, findSourceNode);
		numberOfConnections = 0;
	} else { // distr.type == DISTRIBUTION::UNDEFINED
		numberOfConnections = 0;
	}

	if (numberOfConnections < 0) {
		numberOfConnections = 0;
	}
	n.setNumberOfOpenInterfaceConnections(currentEdgeTypeNumber, numberOfConnections, findSourceNode);
	n.setNumberOfInterfaceConnections(currentEdgeTypeNumber, numberOfConnections, findSourceNode);
//	cout << "Node at iteration " << n.iterationId << " get " << numberOfConnections << " interface-connections" << endl;
}

void nodeGenerator::addNode(graphNode n) {
	graph->nodes.at(n.type).push_back(n);
	nextNodeId++;
}

void nodeGenerator::findOrCreateNode(config::edge & edgeType, bool findSourceNode, int localNmNodes) {
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
	if(graph->nodes.at(type).size() > localNmNodes) {
		// Node already exists in the graph
//		cout << "NodeType" << type << "n" << iterationNumber << " already exists in the graph\n";

		n = &graph->nodes.at(type).at(localNmNodes);
		addInterfaceConnectionsToNode(*n, distr, edgeType.edge_type_id, findSourceNode);
//		if (iterationNumber <= conf.types.at(type).size) {
//			n->is_virtual = false;
//		}
	} else {
		// Node is not in the graph at this moment
		if(!conf->types.at(type).scalable && localNmNodes > conf->types.at(type).size-1) {
			// NodeType is not scalable and all nodes are already created and added to the graph
			// !configuration.types.at(type).scalable <- NodeType is not scalable
			// iterationNumber > configuration.types.at(type).size <- all nodes are already created and added to the graph
			return; // So: Do NOT add the node
		}

//		bool isVirtual;
//		if (iterationNumber > conf.types.at(type).size-1) {
//			isVirtual = true;
//		} else {
//			isVirtual = false;
//		}

		n = new graphNode(nextNodeId, localNmNodes, type, conf->schema.edges.size());
		addInterfaceConnectionsToNode(*n, distr, edgeType.edge_type_id, findSourceNode);

//		cout << "Creating a node at iteration " << iterationNumber << " of type:" << type <<
//				". Size of that type=" << conf.types.at(type).size << "\n";


		addNode(*n);
	}
}

int nodeGenerator::addOrUpdateNodes(config::edge & edgeType, int iterationNumber, int numberOfNodesOfMax, int type1, int type2, bool isSubject) {
	if ((conf->types.at(edgeType.subject_type).scalable && conf->types.at(edgeType.object_type).scalable)
			|| (!conf->types.at(edgeType.subject_type).scalable && !conf->types.at(edgeType.object_type).scalable)) {
		double prob1;
		double prob2;
		if (conf->types.at(edgeType.subject_type).scalable) {
			prob1 = conf->types.at(type1).proportion;
			prob2 = conf->types.at(type2).proportion;
		} else {
			prob1 = conf->types.at(type1).size;
			prob2 = conf->types.at(type2).size;
		}
		if (prob1 > prob2) {
			int nmOfNodesPerIteration = floor(prob1 / prob2);

			double probNextNode = (prob1 / prob2) - (double)nmOfNodesPerIteration;
			uniform_real_distribution<double> distribution(0.0,1.0);
			double randomValue = distribution(*randomGenerator);
			if(randomValue < probNextNode) {
				nmOfNodesPerIteration++;
			}

//			cout << "Creating " << nmOfNodesPerIteration << " nodes for type " << type1 << endl;
			for (int i=0; i<nmOfNodesPerIteration; i++) {
				findOrCreateNode(edgeType, isSubject, numberOfNodesOfMax);
				numberOfNodesOfMax++;
			}
		} else {
//			cout << "Creating " << 1 << " node for type " << type1 << endl;
			findOrCreateNode(edgeType, isSubject, iterationNumber);
		}
	} else {
		if (isSubject) {
			if (iterationNumber < conf->types.at(edgeType.subject_type).size) {
//				cout << "Creating " << 1 << " node for type " << type1 << endl;
				findOrCreateNode(edgeType, isSubject, iterationNumber);
			}
		} else {
			if (iterationNumber < conf->types.at(edgeType.object_type).size) {
//				cout << "Creating " << 1 << " node for type " << type1 << endl;
				findOrCreateNode(edgeType, isSubject, iterationNumber);
			}
		}
		numberOfNodesOfMax = iterationNumber;
	}
	return numberOfNodesOfMax;
}

int nodeGenerator::addOrUpdateSubjectNodes(config::edge & edgeType, int iterationNumber, int numberOfNodesOfMax) {
	return addOrUpdateNodes(edgeType, iterationNumber, numberOfNodesOfMax, edgeType.subject_type, edgeType.object_type, true);
}
int nodeGenerator::addOrUpdateObjectNodes(config::edge & edgeType, int iterationNumber, int numberOfNodesOfMax) {
	return addOrUpdateNodes(edgeType, iterationNumber, numberOfNodesOfMax, edgeType.object_type, edgeType.subject_type, false);
}
// ####### Generate nodes #######

} /* namespace std */
