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
	this->birthIdSubject = 0;
	this->birthIdObject = 0;
}
nodeGenerator::~nodeGenerator() {
//	for (graphNode node: nodes->first) {
//		delete &node;
//	}
//	for (graphNode node: nodes->second) {
//		delete &node;
//	}
}

nodeGenerator::nodeGenerator(config::edge & edgeType, int birthIdSubj, int birthIdObj, default_random_engine* randomGenerator_, pair<vector<graphNode>,vector<graphNode>>* nodes_, config::config* conf_) {
	this->birthIdSubject = birthIdSubj;
	this->birthIdObject = birthIdObj;
	this->randomGenerator = randomGenerator_;
	this->nodes = nodes_;
	this->conf = conf_;

//	if (edgeType.outgoing_distrib.type == DISTRIBUTION::UNIFORM) {
//		this->uniformOutDistr = uniform_int_distribution<int>(edgeType.outgoing_distrib.arg1, edgeType.outgoing_distrib.arg2);
//	} else if (edgeType.outgoing_distrib.type == DISTRIBUTION::NORMAL) {
//		this->normalOutDistr = normal_distribution<double>(edgeType.outgoing_distrib.arg1, edgeType.outgoing_distrib.arg2);
//	}
//
//	if (edgeType.incoming_distrib.type == DISTRIBUTION::UNIFORM) {
//		this->uniformInDistr = uniform_int_distribution<int>(edgeType.incoming_distrib.arg1, edgeType.incoming_distrib.arg2);
//	} else if (edgeType.incoming_distrib.type == DISTRIBUTION::NORMAL) {
//		this->normalInDistr = normal_distribution<double>(edgeType.incoming_distrib.arg1, edgeType.incoming_distrib.arg2);
//	}
//
//	uniformZeroOneDistr = uniform_real_distribution<double>(0.0,1.0);
}

//void nodeGenerator::initializeConnections(graphNode &n, int maxNumberOfConnections) {
//	for (int i=0; i<maxNumberOfConnections; i++) {
//		n.setConnection(i, 0);
//	}
//}


int nodeGenerator::getNumberOfICs(distribution distr, bool addSourceNode) {
	int numberOfConnections;
	if (distr.type == DISTRIBUTION::UNIFORM) {
//		cout << "UNIFORM with " << distr.arg1 << " " << distr.arg2 << endl;

		numberOfConnections = uniform_int_distribution<int>(distr.arg1, distr.arg2)(*randomGenerator);

	} else if (distr.type == DISTRIBUTION::NORMAL) {
//		cout << "NORMAL with " << distr.arg1 << " " << distr.arg2 << endl;

		numberOfConnections = round(normal_distribution<double>(distr.arg1, distr.arg2)(*randomGenerator));

	} else { // distr.type == DISTRIBUTION::UNDEFINED
		numberOfConnections = 0;
	}

//	if (numberOfConnections < 0) {
//		numberOfConnections = 0;
//	}

//	cout << "Node get " << numberOfConnections << " interface-connections" << endl;
	return numberOfConnections;
}


void nodeGenerator::addNode(config::edge & edgeType, int distrShift, bool addSourceNode) {
	distribution distr;
	size_t type;
	int birthId;
	if(addSourceNode) {
		distr = edgeType.outgoing_distrib;
		type = edgeType.subject_type;
		birthId = birthIdSubject;
	} else {
		distr = edgeType.incoming_distrib;
		type = edgeType.object_type;
		birthId = birthIdObject;
	}


//	if(!conf->types.at(type).scalable && numberOfNodes > conf->types.at(type).size-1) {
//		// NodeType is not scalable and all nodes are already created and added to the graph
//		// !configuration.types.at(type).scalable <- NodeType is not scalable
//		// graph->nodes.at(type).size() > configuration.types.at(type).size <- all nodes are already created and added to the graph
//		return; // So: Do NOT add the node
//	}


	float pos = 0.0;
	int numberOfICs = 0;
	if (distr.type == DISTRIBUTION::ZIPFIAN) {
		pos = uniform_real_distribution<double>(0.0,1.0)(*randomGenerator);
	} else {
		numberOfICs = getNumberOfICs(distr, addSourceNode) + distrShift;
	}

	int nonNegativeICs = numberOfICs;
	if (numberOfICs < 0) {
		nonNegativeICs = 0;
	}
	graphNode *n = new graphNode(to_string(type) + "-" + to_string(birthId), birthId, type, conf->schema.edges.size(), nonNegativeICs, nonNegativeICs, pos, numberOfICs);

	if (addSourceNode) {
//		initializeConnections(*n, conf->types.at(otherType).size*2);
		nodes->first.push_back(*n);
		birthIdSubject++;
	} else {
		nodes->second.push_back(*n);
		birthIdObject++;
	}

//		cout << "Creating a node at iteration " << iterationNumber << " of type:" << type <<
//				". Size of that type=" << conf.types.at(type).size << "\n";



}

//void nodeGenerator::addNodes(config::edge & edgeType, int type1, int type2, bool isSubject) {
//	if ((conf->types.at(edgeType.subject_type).scalable && conf->types.at(edgeType.object_type).scalable)
//			|| (!conf->types.at(edgeType.subject_type).scalable && !conf->types.at(edgeType.object_type).scalable)) {
//		double prob1;
//		double prob2;
//		if (conf->types.at(edgeType.subject_type).scalable) {
//			prob1 = conf->types.at(type1).proportion;
//			prob2 = conf->types.at(type2).proportion;
//		} else {
//			prob1 = conf->types.at(type1).size;
//			prob2 = conf->types.at(type2).size;
//		}
//		if (prob1 > prob2) {
//			int nmOfNodesPerIteration = floor(prob1 / prob2);
//
//			double probNextNode = (prob1 / prob2) - (double)nmOfNodesPerIteration;
//			double randomValue = uniform_real_distribution<double>(0.0,1.0)(*randomGenerator);
//			if(randomValue < probNextNode) {
//				nmOfNodesPerIteration++;
//			}
//
////			cout << "Creating " << nmOfNodesPerIteration << " nodes for type " << type1 << endl;
//			for (int i=0; i<nmOfNodesPerIteration; i++) {
//				addNode(edgeType, isSubject);
//
//}

void nodeGenerator::addSubjectNodes(config::edge & edgeType, int distrShift) {
	int currentNumberOfSubjectNodes = nodes->first.size();
	int totalNumberSubjectOfNodes = conf->types[edgeType.subject_type].size;
	int subjectNodesToGenerate = totalNumberSubjectOfNodes - currentNumberOfSubjectNodes;
	for (int i=0; i<subjectNodesToGenerate; i++) {
		addNode(edgeType, distrShift, true);
	}
}
void nodeGenerator::addObjectNodes(config::edge & edgeType, int distrShift) {
	int currentNumberOfObjectNodes = nodes->second.size();
	int totalNumberObjectOfNodes = conf->types[edgeType.object_type].size;
	int objectNodesToGenerate = totalNumberObjectOfNodes - currentNumberOfObjectNodes;
	for (int i=0; i<objectNodesToGenerate; i++) {
		addNode(edgeType, distrShift, false);
	}
}

} /* namespace std */
