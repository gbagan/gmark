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
	this->id = -1;
	this->iterationId = -1;
	this->type = -1;

	this->numberOfOpenInterfaceConnections = 0;
	this->numberOfInterfaceConnections = 0;
	this->position = 0.0;
}
graphNode::graphNode(int globalId, int localId, int nodeType,
		int numberOfOpenICs, int numberOfICs, double position) {
	this->id = globalId;
	this->iterationId = localId;
	this->type = nodeType;

	this->numberOfOpenInterfaceConnections = numberOfICs;
	this->numberOfInterfaceConnections = numberOfOpenICs;
	this->position = position;
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
	this->numberOfOpenInterfaceConnections += number;
}
void graphNode::incrementInterfaceConnectionsByN(int number) {
	this->numberOfInterfaceConnections += number;

}

void graphNode::setPosition(double number) {
	this->position = number;
}
double graphNode::getPosition() {
	return this->position;
}

} /* namespace std */
