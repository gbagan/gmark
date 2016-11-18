/*
 * graphNode.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: wilcovanleeuwen
 */

#include <iostream>
#include "graphNode.h"


namespace std {

graphNode::graphNode() {
	this->id = "None";
	this->iterationId = -1;
	this->type = -1;

	this->numberOfOpenInterfaceConnections = 0;
	this->numberOfInterfaceConnections = 0;
	this->position = 0.0;

//	this->connections = new int[1];
}
graphNode::graphNode(string globalId, int localId, int nodeType, int numberOfEdgeTypes, int numberOfOpenICs,
		int numberOfICs, float position, int nonNegativeICs) {
	this->id = globalId;
	this->iterationId = localId;
	this->type = nodeType;

	this->numberOfOpenInterfaceConnections = numberOfICs;
	this->numberOfInterfaceConnections = numberOfOpenICs;
	this->position = position;
	this->numberOfNonNegativeICs = nonNegativeICs;
//	this->connections = new int[maxNumberOfConnections];
}

graphNode::~graphNode() {
	// TODO Auto-generated destructor stub
}


int graphNode::getNumberOfOpenInterfaceConnections() {
	return numberOfOpenInterfaceConnections;
}
void graphNode::setNumberOfOpenInterfaceConnections(int number) {
	numberOfOpenInterfaceConnections = number;
}


int graphNode::getNumberOfInterfaceConnections() {
	return this->numberOfInterfaceConnections;
}
void graphNode::setNumberOfInterfaceConnections(int number) {
	this->numberOfInterfaceConnections = number;
}

void graphNode::decrementOpenInterfaceConnections() {
	this->numberOfOpenInterfaceConnections--;
}
void graphNode::incrementOpenInterfaceConnectionsByN(int number) {
//	cout << "numberOfNonNegativeICs" << this->numberOfNonNegativeICs << endl;
//	cout << "numberOfNonNegativeICs" << this->numberOfNonNegativeICs << endl;
//	cout << "numberOfOpenInterfaceConnections" << this->numberOfOpenInterfaceConnections << endl;
	if (this->numberOfNonNegativeICs >= 0) {
		this->numberOfOpenInterfaceConnections += number;
	}
//	cout << "numberOfOpenInterfaceConnections" << this->numberOfOpenInterfaceConnections << endl;
}
void graphNode::incrementInterfaceConnectionsByN(int number) {
//	cout
	this->numberOfNonNegativeICs += number;
	if (this->numberOfNonNegativeICs > 0) {
		this->numberOfInterfaceConnections = this->numberOfNonNegativeICs;
	}
}

void graphNode::setPosition(double number) {
	this->position = number;
}
float graphNode::getPosition() {
	return this->position;
}

//void graphNode::setConnection(int connectionIterationId, int value) {
//	this->connections[connectionIterationId] = value;
//}
//int graphNode::getConnection(int connectionIterationId) {
//	return this->connections[connectionIterationId];
//}

} /* namespace std */
