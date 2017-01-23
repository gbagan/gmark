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

nodeGenerator::nodeGenerator(config::edge & edgeType, int birthIdSubj, int birthIdObj, default_random_engine* randomGenerator_, pair< vector<graphNode>,vector<graphNode> >* nodes_, config::config* conf_) {
	this->birthIdSubject = birthIdSubj;
	this->birthIdObject = birthIdObj;
	this->randomGenerator = randomGenerator_;
	this->nodes = nodes_;
	this->conf = conf_;
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



	float pos = 0.0;
	int numberOfICs = 0;
	if (distr.type == DISTRIBUTION::ZIPFIAN) {
		pos = uniform_real_distribution<double>(0.0,1.0)(*randomGenerator);
	} else {
		numberOfICs = getNumberOfICs(distr, addSourceNode) + distrShift;
	}
//	cout << "Adding node" << endl;
//	cout << "Type: " << type << endl;
//	cout << "BirthId: " << birthId << endl;
//	cout << "Id: " << type + (birthId * conf->types.size()) << endl;
	graphNode *n = new graphNode(type + (birthId * conf->types.size()), birthId, type, conf->schema.edges.size(), numberOfICs, numberOfICs, pos);

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

void nodeGenerator::addSubjectNodes(config::edge & edgeType, int distrShift, int graphNumber) {
	int currentNumberOfSubjectNodes = nodes->first.size();
	int totalNumberSubjectOfNodes = conf->types[edgeType.subject_type].size[graphNumber];
	int subjectNodesToGenerate = totalNumberSubjectOfNodes - currentNumberOfSubjectNodes;
	for (int i=0; i<subjectNodesToGenerate; i++) {
		addNode(edgeType, distrShift, true);
	}
}
void nodeGenerator::addObjectNodes(config::edge & edgeType, int distrShift, int graphNumber) {
	int currentNumberOfObjectNodes = nodes->second.size();
	int totalNumberObjectOfNodes = conf->types[edgeType.object_type].size[graphNumber];
	int objectNodesToGenerate = totalNumberObjectOfNodes - currentNumberOfObjectNodes;
	for (int i=0; i<objectNodesToGenerate; i++) {
		addNode(edgeType, distrShift, false);
	}
}

} /* namespace std */
