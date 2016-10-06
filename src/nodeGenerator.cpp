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
	this->nodes = 0;
	this->conf = 0;
}

nodeGenerator::nodeGenerator(default_random_engine* randomGenerator_, pair<vector<graphNode>,vector<graphNode>>* nodes_, config::config* conf_) {
	this->randomGenerator = randomGenerator_;
	this->nodes = nodes_;
	this->conf = conf_;
}
nodeGenerator::~nodeGenerator() {
	// TODO
}

void nodeGenerator::initializeConnections(graphNode &n, int maxNumberOfConnections) {
	for (int i=0; i<maxNumberOfConnections; i++) {
		n.setConnection(i, 0);
	}
}


void nodeGenerator::addInterfaceConnectionsToNode(graphNode &n, distribution distr) {
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
		n.setPosition(randomValue);
		numberOfConnections = 0;
	} else { // distr.type == DISTRIBUTION::UNDEFINED
		numberOfConnections = 0;
	}

	if (numberOfConnections < 0) {
		numberOfConnections = 0;
	}
//	cout << "Before: " << n.getNumberOfInterfaceConnections(findSourceNode) << endl;
	n.setNumberOfOpenInterfaceConnections(numberOfConnections);
	n.setNumberOfInterfaceConnections(numberOfConnections);
//	cout << "Node at iteration " << n.iterationId << " get " << numberOfConnections << " interface-connections" << endl;
//	cout << "After: " << n.getNumberOfInterfaceConnections(findSourceNode) << endl;
}


void nodeGenerator::addNode(config::edge & edgeType, bool addSourceNode) {
	distribution distr;
	size_t type;
	size_t otherType;
	int numberOfNodes;
	if(addSourceNode) {
		distr = edgeType.outgoing_distrib;
		type = edgeType.subject_type;
		otherType = edgeType.object_type;
		numberOfNodes = nodes->first.size();
	} else {
		distr = edgeType.incoming_distrib;
		type = edgeType.object_type;
		otherType = edgeType.subject_type;
		numberOfNodes = nodes->second.size();
	}


	if(!conf->types.at(type).scalable && numberOfNodes > conf->types.at(type).size-1) {
		// NodeType is not scalable and all nodes are already created and added to the graph
		// !configuration.types.at(type).scalable <- NodeType is not scalable
		// graph->nodes.at(type).size() > configuration.types.at(type).size <- all nodes are already created and added to the graph
		return; // So: Do NOT add the node
	}


	graphNode *n = new graphNode(to_string(type) + "-" + to_string(numberOfNodes), numberOfNodes, type, conf->schema.edges.size(), conf->types.at(otherType).size*2);
	addInterfaceConnectionsToNode(*n, distr);
	initializeConnections(*n, conf->types.at(otherType).size*2);
	if (addSourceNode) {
		nodes->first.push_back(*n);
	} else {
		nodes->second.push_back(*n);
	}

//		cout << "Creating a node at iteration " << iterationNumber << " of type:" << type <<
//				". Size of that type=" << conf.types.at(type).size << "\n";



}

void nodeGenerator::addNodes(config::edge & edgeType, int type1, int type2, bool isSubject) {
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
				addNode(edgeType, isSubject);
			}
		} else {
//			cout << "Creating " << 1 << " node for type " << type1 << endl;
			addNode(edgeType, isSubject);
		}
	} else {
		if (isSubject) {
			if (nodes->first.size() < conf->types.at(edgeType.subject_type).size) {
//				cout << "Creating " << 1 << " node for type " << type1 << endl;
				addNode(edgeType, isSubject);
			}
		} else {
			if (nodes->second.size() < conf->types.at(edgeType.object_type).size) {
//				cout << "Creating " << 1 << " node for type " << type1 << endl;
				addNode(edgeType, isSubject);
			}
		}
	}
}

void nodeGenerator::addSubjectNodes(config::edge & edgeType) {
	addNodes(edgeType, edgeType.subject_type, edgeType.object_type, true);
}
void nodeGenerator::addObjectNodes(config::edge & edgeType) {
	addNodes(edgeType, edgeType.object_type, edgeType.subject_type, false);
}

} /* namespace std */
